# Project 4. Problems met

Real obstacles of this project, what was understood and what is still open. Details
live in [`TASKS.md`](../../TASKS.md), [`notes/SPLITS.md`](../../notes/SPLITS.md) and
[`notes/MATCHING.md`](../../notes/MATCHING.md).

## Toolchain

| Problem | Status |
|---|---|
| Exact compiler version unknown: GC 3.0a5.2 and 3.0a3...3.0a5 give the same game code in every test | open; 3.0a5.2 used as a hypothesis, a function that tells them apart is watched for |
| The C library needs two compiler versions (six files GC/3.0a5, `printf.c` GC/3.0a5.2) | solved: per-object compiler version in `configure.py` |
| The 3.0a5.2 linker complained about the exception runtime files | solved: `__init_cpp_exceptions.cpp` split with the "GC 2.7+ and Wii" layout |
| Analysis took a few constants and a character table for pointers | solved: `block_relocations` in `config.yml`, each one commented |

## Splitting and linking

| Problem | Status |
|---|---|
| A split that builds proves nothing about file boundaries | rule: a boundary counts only once the unit matches |
| Units with only a static initializer are not referenced, the linker drops them | solved: `FORCEFILES` in `config/RM7P4F/ldscript.tpl` |
| **Link-order cycle**: `unit_80014230` has `.rodata` after the tables of later units although its `.text` comes first, so their `.rodata` cannot be split (`unit_8009F66C`, `unit_800B1498`) | worked around: the tables stay in the unsplit `.rodata` and the source declares them `extern`; `unit_800B1498` matches and links that way |
| **`.bss` order versus initialization order**: in several units the original places the shared vector last in `.bss` but initializes it first; every compiler tried lays `.bss` out in definition order | open, blocks linking of `unit_8013CA74` and `unit_8009F66C` |
| Weak (inline) functions and RTTI emitted in one unit in the original, another in ours (`unit_800C42B8` emits RTTI data that the DOL has in `unit_8009F66C`) | waits for the other unit |
| objdiff pairs symbols by name: a wrong `.bss` order can show 100 % and still break the link | always check the linked `main.dol`, not only the report |

## Matching

| Problem | Status |
|---|---|
| Register-numbering near-misses (99 %+ functions with one or two registers swapped) | several solved with declaration order, extra locals, `void*` locals; 35 game units still NonMatching, each with its exact gap in `TASKS.md` |
| Emission order of generated destructors and inline functions | solved per unit with `-ipa file` or `-inline auto,deferred`; never for all game code, because it breaks other units |
| GameSpy library: empty debug branches keep a jump and a dead compare in the original | open; no flag or source shape reproduces it |
| Search loops that reload their count every iteration in the original (no `ctr` loop) | open (`fn_800D8724`, `ExWorldWireTile::vfBC`) |
| Float-heavy functions: register allocation of floats | case by case; several techniques in `notes/MATCHING.md` |
| SDK regions newer than every reference project (DWC, AXFX, AX voice manager, debugger driver) | not split yet |

## Process

| Problem | Lesson |
|---|---|
| Fuzzy score 98.7 % but the DOL differs | compare the linked bytes; score variants by differing words |
| A DOL byte diff showed "0 words differ" for a NonMatching unit | its assembly is what gets linked: use the objdiff report until the unit is switched to `Matching` |
| Long sessions of guessing | time-box, record the gap, move on; the next unit often teaches the answer |
| Names that "look right" from a fan wiki | the wiki is a source of ideas, never evidence; unknown names stay address-based |
| Reused code with unknown license | ask before reusing; log every reuse in `notes/PROVENANCE.md` |

Next: [5. Daily workflow](05-daily-workflow.md).
