# Monster Lab decompilation — specification

## Goal

Rebuild Monster Lab (Wii) from C/C++ source written from scratch so that, compiled
with the original compiler and flags, it produces a `main.dol` identical byte for
byte to the original.

Out of scope: a PC port (a possible later, separate project), game assets, any
redistribution of game files.

## Target binary

| Item | Value | Evidence |
|---|---|---|
| Game ID | `RM7P4F` (PAL Europe, "Europe (En,Fr,De,Es,It)") | `disc/header.bin` bytes 0-5 |
| Revision | 0 | `disc/header.bin` byte 7 = `0x00` |
| `main.dol` SHA-1 | `50f6e9b94dbe4df6f663541493106d19e0a40c72` | `config/RM7P4F/build.sha1` |
| RELs | none | no `.rel` in `files/` |
| `.map` / `.elf` | none | no `.map`, `.elf` or `.sym` on the disc |
| Build date | "Aug 28 2008 (14:30)", builds 519 / 142835 | `game/unit_80014230.cpp` |

## Toolchain

Evidence and the full candidate analysis are in `notes/COMPILER.md`.

- mwcc GC 3.0 family proven (3.0a3 to 3.0a5.2, exact version unknown). Working
  choice: **GC/3.0a5.2** (hypothesis, from the SDK tag). `-fp_contract off` proven.
  In MSL_C some files need 3.0a5 and `printf.c` needs 3.0a5.2.
- Wii SDK: RVL_SDK libraries built August 2007 to March 2008, tag `0x4199_60831`
  (HBM and NHTTP: `0x4199_60726`); RevoEX `REX-PPC 2.1.255.1 (RevoEX-2.1plus) REL
  080313085908`. Commercial SDK version name unknown.
- Game code: `cflags_game` in `configure.py` (base flags with `-RTTI on`). Per-unit
  exceptions, each justified in the unit's header comment: `-inline auto,deferred`
  (`game/unit_8011EA20.cpp`), `-ipa file` (`game/unit_801F59F0.cpp`).
- Libraries keep their own flags (MetroTRK on GC/2.7, MSL_C per file).

## Acceptance criteria

- Every build ends with `build/RM7P4F/main.dol: OK` (`ninja`, checked against
  `config/RM7P4F/build.sha1`), locally and in CI.
- A unit is `Matching` in `configure.py` only when objdiff reports 100 % for all its
  functions and data and the linked DOL still matches. Anything else stays
  `NonMatching`, with the remaining gap written in `TASKS.md`.
- A split that builds proves the bytes, not the translation unit: a TU boundary is
  proven only once the unit is decompiled and matches (`notes/SPLITS.md`).
- Every name, compiler fact or structure layout rests on evidence (RTTI, assertion
  or debug strings with their offset, symbols from another project on the same
  library, an objdiff diff, a build result). Without evidence it is labelled a
  hypothesis.
- Reused code from another project is logged with its commit and license in
  `notes/PROVENANCE.md`.

## Naming conventions

- Game units: `src/game/unit_<start address>.cpp` (real file names unknown); the unit
  table with bounds and contents is in `notes/SPLITS.md`.
- Unknown classes and structs: `Unk<Name>` or `Unk<address>`; unknown methods keep
  the address: `fn_<address>` (mangled as a member, e.g. `fn_801F59F0__12UnkMsgBufferFv`);
  virtual functions are `vfXX` after their vtable offset.
- Real names only from evidence: RTTI class names, strings, symbols of other projects
  on the same library. `wiki/monster-lab-wiki` gives candidate names, never evidence.
- Code, comments, notes and commit messages are in English.
- Game units that build the shared vector start with `#include "game/unk_vec3.mch"`
  (or `unk_vec3_rvl.mch` with Wii SDK headers): the game used a precompiled header,
  which puts that vector last in `.bss` (`notes/MATCHING.md`).
- Shared game headers in `include/game/`: `unk_vec3.h` (the shared `(0, 1, 0)` vector
  each including unit builds), `externs.h` (functions not decompiled yet), `Entity.h`
  (Entity family: Entity, Entity_Game, Entity_Static, Entity_Dynamic, Entity_Sensor,
  Entity_Prop, ExWorldTile...), callbacks (`ActionSelectionCallback.h`,
  `GestureCallback.h`, `CombatCallback.h`), `CameraInfluence.h`,
  `MotionRecognizer.h`, `InventoryBook.h` and `book.h`.

## Repository layout

```
configure.py              project configuration and build.ninja generator
config/RM7P4F/            dtk config: config.yml, symbols.txt, splits.txt, build.sha1,
                          ldscript.tpl (FORCEFILES for sinit-only units)
orig/RM7P4F/              original game files (git-ignored, never committed)
build/                    build output (git-ignored)
src/  include/  libs/     sources and headers (game, SDK, runtime, MSL, TRK)
tools/                    shared scripts (progress charts, report and asm helpers)
progress/                 progress charts and history (README images)
notes/                    reference documentation (see CLAUDE.md routing table)
docs/                     learning guide: general concepts and how this project works
wiki/monster-lab-wiki/    game knowledge for naming (not evidence)
.claude/skills/           project workflows (decomp-manager, decomp-unit,
                          unit-readiness)
.github/workflows/        CI: split, link and check main.dol in a private image
```

## Build

```
python configure.py       # generate build.ninja (options: --version RM7P4F, --map)
ninja                     # split, compile, link, check main.dol, write the report
```

Dependencies: Python and ninja (see `README.md`); on Windows use native tools, not
WSL. Outputs: `build/RM7P4F/report.json` (objdiff report), `objdiff.json` for the
objdiff GUI, disassembly of every unit under `build/RM7P4F/asm/`.
