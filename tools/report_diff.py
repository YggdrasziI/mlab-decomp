#!/usr/bin/env python3
"""Compare the objdiff report with a saved baseline and list what changed.

Used as the no-regression gate before a commit (see the unit-readiness skill).

Usage:
  python tools/report_diff.py --save            # save build/RM7P4F/report.json as the baseline
  python tools/report_diff.py                   # compare the current report with the baseline
  python tools/report_diff.py --base other.json # compare with another report

Exit status 1 when a function present in both reports lost match percentage, or a
fully matched function or data section disappeared without a 100 % replacement.
"""

import argparse
import json
import os
import shutil
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
REPORT = os.path.join(ROOT, "build", "RM7P4F", "report.json")
BASELINE = os.path.join(ROOT, "build", "RM7P4F", "report_base.json")


def load(path):
    with open(path, encoding="utf-8") as f:
        report = json.load(f)
    items = {}
    for unit in report["units"]:
        name = unit["name"]
        for func in unit.get("functions", []):
            items[(name, "fn", func["name"])] = func.get("fuzzy_match_percent", 0.0)
        for section in unit.get("sections", []):
            if section["name"].startswith(".text"):
                continue
            items[(name, "data", section["name"])] = section.get("fuzzy_match_percent", 0.0)
    return report["measures"], items


def main():
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("--save", action="store_true", help="save the current report as the baseline")
    parser.add_argument("--base", default=BASELINE, help="baseline report (default: %(default)s)")
    args = parser.parse_args()

    if args.save:
        shutil.copyfile(REPORT, args.base)
        print(f"Saved baseline to {args.base}")
        return 0
    if not os.path.exists(args.base):
        print(f"No baseline at {args.base}; run with --save first", file=sys.stderr)
        return 2

    base_measures, base = load(args.base)
    cur_measures, cur = load(REPORT)

    regressions, improvements, removed, added = [], [], [], []
    for key in sorted(set(base) | set(cur)):
        old, new = base.get(key), cur.get(key)
        if old is None:
            added.append((key, new))
        elif new is None:
            removed.append((key, old))
        elif new < old:
            regressions.append((key, old, new))
        elif new > old:
            improvements.append((key, old, new))

    # A matched item that only changed name (symbol renamed) is not a regression.
    added_full = sum(1 for _, pct in added if pct == 100.0)
    removed_full = [(key, pct) for key, pct in removed if pct == 100.0]
    lost = removed_full[added_full:] if len(removed_full) > added_full else []

    def fmt(key):
        unit, kind, name = key
        return f"{unit.replace('main/', '')} {kind} {name}"

    for title, rows in (("Regressions", regressions), ("Improvements", improvements)):
        if rows:
            print(f"{title}:")
            for key, old, new in rows:
                print(f"  {fmt(key)}: {old:.2f} -> {new:.2f}")
    if removed or added:
        print(f"Removed: {len(removed)} ({len(removed_full)} at 100 %), "
              f"added: {len(added)} ({added_full} at 100 %)")
        for key, pct in removed:
            print(f"  - {fmt(key)} ({pct:.2f})")
        for key, pct in added:
            print(f"  + {fmt(key)} ({pct:.2f})")

    for field in ("matched_code", "complete_code", "matched_data", "matched_functions"):
        old, new = int(base_measures.get(field, 0)), int(cur_measures.get(field, 0))
        if old != new:
            print(f"{field}: {old} -> {new} ({new - old:+d})")

    if regressions or lost:
        print("FAIL: regressions found")
        return 1
    print("OK: no regression")
    return 0


if __name__ == "__main__":
    sys.exit(main())
