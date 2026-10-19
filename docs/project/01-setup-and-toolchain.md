# Project 1. Setup and toolchain

What this project actually uses, and how a human would set up the same thing. The
target is `main.dol` of Monster Lab (Wii, `RM7P4F`, PAL, revision 0); details in
[`SPEC.md`](../../SPEC.md).

## The tools in use

| Tool | What it does here | Where |
|---|---|---|
| [dtk-template](https://github.com/encounter/dtk-template) | project skeleton: `configure.py` generates the ninja build; the template's own history is the first commits of the repository | `configure.py`, `tools/project.py` |
| decomp-toolkit (**dtk**) | analyses `main.dol` (functions, data, references), splits it into per-unit assembly, reads `symbols.txt` / `splits.txt`, rebuilds the DOL from the linked ELF and checks its SHA-1 | downloaded to `build/tools/dtk.exe` |
| **mwcc** (Metrowerks CodeWarrior compiler) | compiles the C/C++; game code uses GC/3.0a5.2 (working hypothesis, see below); some MSL_C files GC/3.0a5 | downloaded to `build/compilers/` |
| **ninja** | runs the build: split, compile, link, DOL check, objdiff report | `ninja` |
| **objdiff** (`objdiff-cli`) | compares each compiled unit with the original, function by function, and writes `build/RM7P4F/report.json` (fuzzy % per function and section); the GUI reads `objdiff.json` | `build/tools/objdiff-cli.exe` |
| binutils `powerpc-eabi-objdump` | disassembles our compiled objects and dumps their symbols (`-t`) to check names, sizes and sections | `build/binutils/` |
| Python helper scripts | see below | `tools/` |
| git, GitHub Actions | one subject per commit; CI splits, links and checks `main.dol` on every push in a private image | `.github/workflows/build.yml` |

Project scripts in `tools/`:

| Script | Use |
|---|---|
| `asm_compare.py <unit> <symbol>` | our compiled function next to the original disassembly, differing lines marked `*` |
| `report_diff.py --save` / `report_diff.py` | save the objdiff report as a baseline before a change; afterwards list what improved or regressed |
| `unit_candidates.py` | list game regions not split yet, smallest first, with float and call counts |
| `progress_chart.py --record` | draw the progress charts of the README from the report |
| `decompctx.py` | build a context file for decomp.me (from the template; not used so far) |

**Not used so far**, although they are common in other projects and listed in
[`notes/REFERENCES.md`](../../notes/REFERENCES.md): Ghidra, the Dolphin debugger,
decomp.me, a permuter, an automatic one-function decompiler. Everything here was done
by reading the dtk disassembly (`build/RM7P4F/asm/`), the reference projects and the
diffs. Ghidra would be useful for browsing large unknown code and its
cross-references; its pseudo-C would only be a draft.

On Windows the project uses native tools (Python, ninja), not WSL.

## Setting it up from nothing

1. Put the disc image in `orig/RM7P4F/` (never committed; `orig/` and `build/` are
   git-ignored).
2. Fill `config/RM7P4F/config.yml`: the DOL path, its SHA-1 (`build.sha1`), the
   symbol and split files. dtk analyses the DOL and writes the first
   `symbols.txt` (names like `fn_<address>` and `lbl_<address>`).
3. `python configure.py`, then `ninja`. With no source file at all, everything is
   assembly and the build must end with `build/RM7P4F/main.dol: OK`.
4. Fix what the automatic analysis got wrong. Here the first fixes were a second
   `.sbss` section that had to be named `.sbss2`, and a function `fn_802E5A08` that
   was really the case blocks of the switch in `fn_802E50F0` (merged). Later, a few
   values that dtk took for pointers got a `block_relocations` entry in
   `config.yml`, each with a comment.

## Identifying the compiler (how it was done here)

Evidence is collected in [`notes/COMPILER.md`](../../notes/COMPILER.md). In short:

- **Strings**: `<< RVL_SDK - ... >>` banners give the SDK libraries and their build
  dates; `Metrowerks Target Resident Kernel` and the CodeWarrior runtime strings name
  the toolchain family; the C++ exception tables point to a GC 2.7+/Wii linker
  layout.
- **Probes**: six game functions were rewritten in C and compiled with every
  available mwcc version. GC 2.x failed (pointer compares and merged `mr.`), the Wii
  1.x compilers failed (they build a jump table only from 8 `case` labels, the game
  has 7-label tables), the seven GC 3.0 versions all matched.
- **Flags**: `-fp_contract off` is proven (with `on`, fused multiply-add
  instructions appear that the DOL does not have).
- **Still open**: 3.0a5.2 versus 3.0a3...3.0a5. Fuzzing 240 random functions showed
  only two behaviour classes, and no game function tested so far tells them apart.
  3.0a5.2 is used because its build number matches the SDK tag; it is written down
  as a hypothesis. MSL_C, on the other hand, proved to need both: four math files and
  two others match only with GC/3.0a5, `printf.c` only with GC/3.0a5.2.
- `mw_comment_version: 15` in `config.yml` reproduces the `.comment` section that
  3.0a5.2 writes.

## Compiler flags

`cflags_base` in `configure.py` holds the flags found by the probes (`-O4,p`,
`-inline auto`, `-fp_contract off`...). Game code adds `-RTTI on` (`cflags_game`)
because the game has RTTI type names. A few units need their own flag, always
justified in the unit's header comment and in
[`notes/MATCHING.md`](../../notes/MATCHING.md): `-inline auto,deferred`
(`game/unit_8011EA20.cpp`), `-ipa file` (`game/unit_801F59F0.cpp`,
`game/unit_8009F66C.cpp`).

Next: [2. Steps taken](02-steps-taken.md).
