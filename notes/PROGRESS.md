# Progress log

Private progress history (the project is not published and not on decomp.dev yet).
One line per milestone, figures taken from `build/RM7P4F/report.json` (objdiff report,
also uploaded as the `RM7P4F_report` artifact by CI; artifacts expire, this log does not).

| Date | Commit | Matched code | Matched functions | Units (split / total) | Milestone |
|---|---|---|---|---|---|
| 2026-10-12 | 053a96b | 0 / 3412916 bytes (0 %) | 0 / 12580 | 274 library units / 983 | Steps 1-4 done: setup, compiler (GC/3.0a5.2 working hypothesis), library splits (Runtime 9, MSL_C 59, MetroTRK 29, RVL_SDK 177), CI checking `main.dol` |
| 2026-10-12 | e94a2ee | 4416 / 3412916 bytes (0.13 %) | 34 / 12580 | 9 matching / 983 | Step 5 pilot: Runtime.PPCEABI.H fully matching and linked (code reused from mscharged-decomp and TP, CC0; see `notes/PROVENANCE.md`) |

Notes:

- "Units" counts units in `objdiff.json`; library units are the ones split by hand in
  `config/RM7P4F/splits.txt` (progress category `sdk`, 634340 bytes of code); the rest
  are dtk `auto_*` objects.
- Game code has no progress category content yet (no game unit split).
