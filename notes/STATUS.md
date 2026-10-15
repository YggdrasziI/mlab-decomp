# Project status

## Done

- Template set up for `RM7P4F`; the build rebuilds an identical `main.dol`
  (`build.sha1` OK).
- `splits.txt` / `symbols.txt`: the second `.sbss` section renamed to `.sbss2`;
  `fn_802E5A08` merged into `fn_802E50F0` (the `case` blocks of its switch, table at
  0x803AD338).
- Private GitHub repository created (`origin`); the template is kept as `upstream`.
- Game information filled in `CLAUDE.md` (evidence in `notes/COMPILER.md`).
- Step 2 (partial): game code is compiled with a **GC 3.0** mwcc (3.0a3 to 3.0a5.2),
  `-fp_contract off` proven; Wii 1.x and GC 2.x ruled out (details in
  `notes/COMPILER.md`).
- Toolchain set to **GC/3.0a5.2** (working hypothesis), `-fp_contract off`,
  `mw_comment_version: 15`; `__init_cpp_exceptions.cpp` split with the GC 2.7+/Wii
  layout; build still matches.
- Step 3 (first pass): region map of `.text` and all data sections in
  `notes/SPLITS.md` (game / SDK / runtime / MSL / TRK, plus two unknown blocks).

- Step 4: GitHub Actions CI works: every push splits, links and checks `main.dol`
  against `build.sha1` (`main.dol: OK`), using the private build image
  `ghcr.io/yggdraszii/mlab-decomp-build:main` (repository `YggdrasziI/mlab-decomp-build`,
  holds `orig/RM7P4F/sys/main.dol`; Actions access: build repo Write, main repo Read).

- Step 5 (started): **Runtime.PPCEABI.H matches** (9/9 units, 100 % code and data,
  linked), from mscharged-decomp / TP code (CC0), provenance in `notes/PROVENANCE.md`.
- Step 5: **MSL_C 59/59 units match** and are linked (mscharged-decomp code, CC0;
  `realloc` and `fn_80330B20` in `alloc.c` decompiled from this DOL); see
  `notes/PROVENANCE.md`.

- Step 5: **MetroTRK matches** (29/29 units, mscharged-decomp code, CC0, GC/2.7).

- Step 5: **RVL_SDK 172/179 units match** (mscharged-decomp code, CC0); data
  sections placed by relocation pairing and verified byte by byte, then the 22
  byte-identical units that failed to link were fixed one by one (see
  `notes/PROVENANCE.md`, "Unit-by-unit data review").

## In progress

- Step 6 (game code): 40 game units split, 36 linked (unit table in `notes/SPLITS.md`).
  Unit bounds come from the static initializers (inline functions of a unit may follow
  its `__sinit`); compiled with `cflags_game` (base flags with `-RTTI on`). Shared
  headers: `include/game/unk_vec3.h` (the `(0, 1, 0)` vector), `include/game/externs.h`
  (not yet decompiled functions) and `include/game/Entity.h`: the Entity class family
  (Entity, Entity_Game, Entity_Static, Entity_Dynamic, Entity_Sensor, Entity_Prop,
  ExWorldTile...), with virtual functions named after their vtable offsets; callback
  headers (`ActionSelectionCallback.h`, `GestureCallback.h`) and the Wii Remote motion
  recognizers (`MotionRecognizer.h`). RTTI also names a `RingBuffer<Vector3>` class, so
  a `Vector3` type exists (probably the shared vector, not renamed yet).
  Not linked: `game/unit_8006CFA4.cpp` (Entity_Challenge_Projector, 5/6; one register
  choice in `vf20`), `game/unit_80086780.cpp` (Entity_BrainTossHand, 7/8; two stack
  slots swapped in `fn_800869A0`), `game/unit_80020480.cpp` (29/31; the two `AnyButtons*::vf0C` loops differ
  only by the order of three loop increments) and `game/unit_80042C18.cpp` (static
  initializer only: our linker drops an object nothing references, and FORCEACTIVE
  cannot name its local `__sinit`; FORCEFILES through a custom `ldscript_template` is a
  possible fix).
  Next: the remaining Entity_Prop-derived units (0x800B1EDC Entity_GrowJuiceHeartVine
  has a ray/sphere test over nested collision lists; 0x80098664; larger ones listed by
  calls to `__ct__11Entity_Prop`). Set aside for now: 0x8004B498 (multiple
  inheritance, base in the next unit),
  0x8010A678 and 0x800DF570 (vector types with inline operators), 0x80006B40 (static
  initializer only, same link issue as 0x80042C18).

- Step 3: TU-level splits. Done: Runtime.PPCEABI.H (9 units), MetroTRK (29 units), MSL_C
  (59 units, 3 small gaps) and RVL_SDK (177 units, about 52 % of SDK code);
  hypotheses from TP / Petari / mscharged-decomp checked against the DOL. Remaining:
  the unanchored SDK regions listed in `notes/SPLITS.md`, then the game code.
- Watch for a game function that matches with only {3.0a5.2} or only {3.0a3…3.0a5};
  it would settle the exact compiler version.

## Blocked

- Nothing.

## To do

- Step 5: RVL_SDK NonMatching units (all split): gt2Message, NATify, natneg, qr2
  (the "unthreaded branch / leftover compare" trait: in the DOL, empty debug branches
  keep a jump and a dead compare that our compile removes; no flag or spelling tried
  reproduces it), and gsPlatformSocket / gsPlatformUtil / gsMemory (this build's
  gsMemory keeps its callback table in .bss at 0x805C6D08 and the platform files call
  through it; no reference has that version).
  Unsplit SDK regions (see `notes/SPLITS.md`): the DWC block 0x802B3E88-0x802E5EEC
  and 0x802E6298-0x802FB3A4 (newer DWC than mscharged-decomp; mkw has the same sizes
  in places but unnamed), AXFX 0x8025D2E0-0x8025F934 (newer Delay-type effects, no
  names in any reference), 0x80263F18-0x80264F90 (AX voice manager used by the game,
  no reference), and the debugger driver block (0x80344FD4-0x80345AD8).

- Step 3: continue TU splits with the same method (reference projects for names and
  order, verified on this DOL by sizes and referenced data). The build alone cannot
  prove TU boundaries (see `notes/SPLITS.md`).
- Step 4: decomp.dev postponed by choice: everything stays private until the project is
  mature enough (owner's decision). Progress is logged privately in `notes/PROGRESS.md`. objdiff GUI to be
  installed locally by the owner (objdiff.json is generated by `configure.py`).
