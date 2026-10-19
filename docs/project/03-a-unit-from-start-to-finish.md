# Project 3. A unit from start to finish

A real example: game unit `0x800B1EDC`, class `Entity_GrowJuiceHeartVine`, done in one
session (commits `061f4a2` and `6be2452`). The source is
[`src/game/unit_800B1EDC.cpp`](../../src/game/unit_800B1EDC.cpp). Original assembly is
not reproduced here (project rule: no extracted code in git); run the build and read
`build/RM7P4F/asm/` to follow along.

## 1. Choosing the unit

`python tools/unit_candidates.py` lists the unsplit regions, smallest first. This one
was 0x4A8 bytes, 8 functions, few calls, and already listed in `TASKS.md` as a
candidate. Before touching anything: `python tools/report_diff.py --save` (baseline).

## 2. Bounds

- The region ends with a 0x20-byte function that stores `(0, 1, 0)` into a `.bss`
  vector and has a `.ctors` entry: the unit's static initializer. Its end is the end
  of the unit.
- The function just before the start is the previous unit's static initializer, so
  the start is right after it.

## 3. Data sections

For every symbol the code uses, find who else uses it (`grep -l "lbl_<addr>\b"` over
`build/RM7P4F/asm`):

- the vtable (`.data`), referenced only by the constructor; next to it the class name
  string `"Entity_GrowJuiceHeartVine"` (the RTTI name, so the class name is
  **evidence**, not a guess) and the RTTI base-class list;
- the RTTI object in `.sdata`;
- the shared `(0, 1, 0)` vector in `.bss`;
- float constants in `.sdata2`: 0.0, 0.0001, 0.5, 3.0, 1.0.

That gives the split, added to `config/RM7P4F/splits.txt` in address order:

```
game/unit_800B1EDC.cpp:
	.text       start:0x800B1EDC end:0x800B2384
	.ctors      start:0x80345D24 end:0x80345D28
	.data       start:0x8035E038 end:0x8035E138
	.bss        start:0x803C6D08 end:0x803C6D18
	.sdata      start:0x805F8BC8 end:0x805F8BD0
	.sdata2     start:0x805FE0E8 end:0x805FE108
```

plus `Object(NonMatching, "game/unit_800B1EDC.cpp")` in `configure.py`, and the
symbols renamed in `symbols.txt` to the names our object will emit
(`__ct__25Entity_GrowJuiceHeartVineFiP10EntityInfoi`, `__vt__...`, `__RTTI__...`,
`__sinit_\unit_800B1EDC_cpp`...).

## 4. Reading the functions

The vtable is the one of `Entity_Prop` with two slots replaced (the destructor and
`vf20`), so the class derives from `Entity_Prop` and adds no virtual functions. The
eight functions: constructor (clears a field at 0x264), destructor, two setters of
that field, `vf20` (two loops over the entity's attached nodes around the base
`vf20`), a function returning a hash, a sphere-sweep test, and the static
initializer. An almost identical unit (`unit_800B7A08`) gave the shape of `vf20` for
free.

## 5. First version and iteration

The first version compiled and the six small functions matched at once. The sweep test
(0x2C0 bytes, float-heavy) was at **80 %**. The steps that followed, each one
rebuild + compare:

| Change | Score | Why it helped |
|---|---|---|
| first natural version | 80.2 % | — |
| the overlap test as an inline function taking two vectors | 81.9 % | the original stores the two vectors to the stack, a sign of temporaries passed to an inline |
| inline as **sphere against sphere**, the static one having velocity `UnkVec3Zero()` | 82.1 % | the zero vector temporary got its slot right after the first local, as in the original |
| list accessor written `i >= count ? 0 : data[i]` | 86.7 % | branch layout of the accessor |
| `f32 t = 0.0f;` in the loop before the call | 98.7 % | the original loads 0.0 **inside** the loop and shares its register with `t`: `t` had an initial value |

## 6. The trap: a high score that is not a match

At 98.7 % the only lines `asm_compare.py` marked were display artefacts. Switching the
unit to `Matching` broke the build: `main.dol: FAILED`. The objdiff score ignores
register numbers; the bytes still differed.

A small script compared the rebuilt DOL with the original word by word and printed the
two disassemblies side by side. Then variants were scored by **differing words**
instead of percent:

| Change | Differing words |
|---|---|
| state at 98.7 % | 35 |
| moving sphere passed first to the inline (arguments are evaluated right to left, so the two radius loads swapped) | 32 |
| the list element stored in a `void*` local, tested, then cast | 11 (all integer registers right) |
| `disc = b * b; disc -= a * c;` instead of `disc = b * b - a * c;` | **0** |

Each variant was a text replacement applied by a script, followed by `ninja` and the
word count, so dozens of attempts took minutes. Several families gave nothing (loop
structure, `int` versus `u32`, other accessor shapes, other `sqrt` shapes,
declaration orders): they were dropped without regret.

## 7. Data and finish

- `.sdata2` was at 93 %: the `1.0f` constant's symbol was 8 bytes long (value plus
  padding). Giving it its real size of 4 bytes brought the section to 100 %.
- `.bss` at 50 % and `.data` at 84 % are the same in every matched unit: an
  artefact of the report, the linked DOL is identical.
- Gate (see [Daily workflow](05-daily-workflow.md)): `ninja` with
  `main.dol: OK`, `report_diff.py` with no regression (+8 matched functions, +1192
  bytes), docs updated, two commits (the unit, then the new techniques in
  `notes/MATCHING.md`), push, CI green.

## What to remember

- Start natural, then let each difference suggest the next change.
- Similar code elsewhere (here `fn_800B1AD0` of the previous unit is identical) means a
  shared inline function.
- Trust the linked checksum, not the score.
- Script the variants and measure them with the exact metric.

Next: [4. Problems met](04-problems-met.md).
