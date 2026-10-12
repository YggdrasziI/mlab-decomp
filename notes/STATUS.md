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

## In progress

- Step 3: TU-level splits. Done: Runtime.PPCEABI.H (9 units, hypotheses from Petari /
  mscharged-decomp checked against the DOL). Next: MetroTRK, MSL_C, then the SDK.
- Watch for a game function that matches with only {3.0a5.2} or only {3.0a3…3.0a5};
  it would settle the exact compiler version.

## Blocked

- Nothing.

## To do

- Step 3: continue TU splits with the same method (reference projects for names and
  order, verified on this DOL by sizes and referenced data). The build alone cannot
  prove TU boundaries (see `notes/SPLITS.md`).
- Step 4: objdiff, GitHub Actions CI, decomp.dev.
