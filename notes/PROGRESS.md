# Progress log

Private progress history (the project is not published and not on decomp.dev yet).
One line per milestone, figures taken from `build/RM7P4F/report.json` (objdiff report,
also uploaded as the `RM7P4F_report` artifact by CI; artifacts expire, this log does not).

| Date | Commit | Matched code | Matched functions | Units (split / total) | Milestone |
|---|---|---|---|---|---|
| 2026-10-12 | 053a96b | 0 / 3412916 bytes (0 %) | 0 / 12580 | 274 library units / 983 | Steps 1-4 done: setup, compiler (GC/3.0a5.2 working hypothesis), library splits (Runtime 9, MSL_C 59, MetroTRK 29, RVL_SDK 177), CI checking `main.dol` |
| 2026-10-12 | e94a2ee | 4416 / 3412916 bytes (0.13 %) | 34 / 12580 | 9 matching / 983 | Step 5 pilot: Runtime.PPCEABI.H fully matching and linked (code reused from mscharged-decomp and TP, CC0; see `notes/PROVENANCE.md`) |
| 2026-10-12 | 4487afe | 48304 / 3412916 bytes (1.42 %) | 159 / 12580 | 38 matching / 983 | MSL_C: 29 of 59 units matching and linked (mscharged-decomp code, CC0) |
| 2026-10-12 | a984800 | 65008 / 3412916 bytes (1.90 %) | 182 / 12580 | 65 matching / 982 | MSL_C: 56 of 59 units matching and linked; MSL files compiled with both GC/3.0a5 and GC/3.0a5.2 |
| 2026-10-12 | 25a3e7a | 96644 / 3412916 bytes (2.83 %) | 341 / 12580 | 96 matching / 982 | Runtime 9/9, MSL_C 58/59, MetroTRK 29/29 matching and linked |
| 2026-10-12 | 62e0071 | 494344 / 3412920 bytes (14.48 %) | 2048 / 12580 | 182 matching / 987 | RVL_SDK: 86 units matching and linked; 7.80 % of code linked (ninja progress) |
| 2026-10-12 | (this commit) | 494344 / 3412920 bytes (14.48 %) | 2048 / 12580 | 195 linked / 986 | RVL_SDK: 99 units linked; 8.48 % of code linked |

Notes:

- "Units" counts units in `objdiff.json`; library units are the ones split by hand in
  `config/RM7P4F/splits.txt` (progress category `sdk`, 634340 bytes of code); the rest
  are dtk `auto_*` objects.
- Game code has no progress category content yet (no game unit split).
