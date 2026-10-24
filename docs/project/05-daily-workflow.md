# Project 5. Daily workflow

How a work session runs in this project, step by step, with the commands. The same
steps are written as Claude Code skills in `.claude/skills/`; a human follows exactly
the same path.

| Phase | Human steps | Skill |
|---|---|---|
| plan | read `TASKS.md`, check `git status`, pick the work, save the baseline | `decomp-manager` |
| do | split, write, compare, iterate | `decomp-unit` |
| close | gate, docs, commit, push, CI | `unit-readiness` |

## 1. Plan

```
git status --short
git log --oneline -5
python tools/unit_candidates.py          # if starting a new unit
python tools/report_diff.py --save       # baseline before any change
```

Read [`TASKS.md`](../../TASKS.md): a NonMatching unit is worth retrying when a
technique from [`notes/MATCHING.md`](../../notes/MATCHING.md) fits its written gap;
otherwise take a new small unit, avoiding the "Set aside" list.

## 2. Do

1. Bounds and data sections (see [the worked example](03-a-unit-from-start-to-finish.md)).
2. Edit `config/RM7P4F/splits.txt`, `configure.py` (`Object(NonMatching, ...)`) and
   `symbols.txt`; `python configure.py`; `ninja`.
3. Write `src/game/unit_<start>.cpp`: a header comment with the range and how the
   bounds were found; shared headers from `include/game/`; functions not decompiled
   yet declared `extern "C"`.
4. Compare: `python tools/asm_compare.py unit_<start> <symbol>`; scores in
   `build/RM7P4F/report.json`.
5. Iterate; script variants in a scratch directory outside the repository.
6. When everything is at 100 %, switch to `Matching` and rebuild: the linked DOL is
   the real test.

## 3. Close (the gate)

All of these must pass before a commit:

```
python configure.py
ninja                                    # must end with: build/RM7P4F/main.dol: OK
python tools/report_diff.py              # must end with: OK: no regression
git status --short
git diff --stat
```

- A unit is `Matching` only at 100 % code and data and with the DOL still OK;
  otherwise `NonMatching`, with the gap written above the function and in `TASKS.md`.
- Docs in sync: the unit's row in `notes/SPLITS.md`, `TASKS.md`, new techniques in
  `notes/MATCHING.md`, reused code in `notes/PROVENANCE.md`.
- Nothing extracted from the game in the diff (`orig/`, `build/`, assembly).
- One subject per commit, English message, every `symbols.txt`/`splits.txt` change
  explained in it.

```
git push origin main
gh run list -R YggdrasziI/mlab-decomp --limit 1
```

A failed CI run is reported and fixed, never worked around.

## 4. Milestones

At a milestone, add a dated line to [`notes/PROGRESS.md`](../../notes/PROGRESS.md),
then `python tools/progress_badges.py --version <ID> --record` for each enabled version
and commit `progress/` with it.

## Habits

- Never call something matching from a score; only `main.dol: OK` proves it.
- Every name and fact has evidence (RTTI string, assert text, reference project,
  build result); otherwise it is a hypothesis and said so.
- Small units first; one unit at a time; time-box stubborn functions.

Back to the [guide index](../README.md).
