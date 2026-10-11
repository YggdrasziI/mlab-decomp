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

## In progress

- Step 2: tell apart the GC 3.0 versions. Fuzzing reduced them to two classes,
  {3.0a5.2} vs {3.0a3…3.0a5}; the only known differing pattern (dead int→float
  conversions) does not occur in the game. Undecidable so far.

## Blocked

- Choice of the exact compiler version: pending decision (see `notes/COMPILER.md`).

## To do

- Set `mw_version`, `linker_version` and `mw_comment_version` (14 for 3.0a3, 15 for
  3.0a5+) and switch to `-fp_contract off` in `configure.py`.
- Step 3: split SDK, runtime and game code in `splits.txt`.
- Step 4: objdiff, GitHub Actions CI, decomp.dev.
