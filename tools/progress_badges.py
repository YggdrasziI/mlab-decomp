#!/usr/bin/env python3
"""Draw the README progress badges from the objdiff report, without decomp.dev.

Outputs, one folder per game version (shield-style SVG, one per measure, all code of
the DOL):
  progress/<version>/fuzzy.svg    fuzzy match percentage    (fuzzy_match_percent)
  progress/<version>/perfect.svg  perfectly matched code    (matched_code_percent)
  progress/<version>/linked.svg   code linked into main.dol (complete_code_percent)
  progress/<version>/history.csv  one line per --record

Usage:
  python tools/progress_badges.py                      # draw from build/RM7P4F/report.json
  python tools/progress_badges.py --record             # also append the current state to history.csv
  python tools/progress_badges.py --version RM7E4F     # another game version
"""

import argparse
import csv
import datetime
import json
import os
import subprocess
from xml.sax.saxutils import escape

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

# (file name, label, report measure, color)
BADGES = [
    ("fuzzy", "Fuzzy", "fuzzy_match_percent", "#a28c64"),
    ("perfect", "Perfect", "matched_code_percent", "#3584e4"),
    ("linked", "Linked", "complete_code_percent", "#26a269"),
]

HISTORY_FIELDS = [
    "date", "commit", "code_fuzzy_pct", "code_matched_pct", "code_linked_pct",
    "functions_matched", "functions_total",
]


def pct(measures, key):
    return float(measures.get(key, 0.0) or 0.0)


def git(*args):
    try:
        return subprocess.run(["git", *args], cwd=ROOT, capture_output=True, text=True).stdout.strip()
    except OSError:
        return ""


def text_width(text):
    """Rough width of 11px Verdana text, enough to size a badge."""
    narrow = sum(1 for c in text if c in "ijlt.,: %1")
    return 7 * (len(text) - narrow) + 4 * narrow


def badge(label, value, color):
    """A flat shield: grey label on the left, colored value on the right."""
    lw = text_width(label) + 12
    vw = text_width(value) + 12
    w = lw + vw
    return f"""<svg xmlns="http://www.w3.org/2000/svg" width="{w}" height="20" role="img" aria-label="{escape(label)}: {escape(value)}">
<title>{escape(label)}: {escape(value)}</title>
<linearGradient id="s" x2="0" y2="100%"><stop offset="0" stop-color="#bbb" stop-opacity=".1"/><stop offset="1" stop-opacity=".1"/></linearGradient>
<clipPath id="r"><rect width="{w}" height="20" rx="3" fill="#fff"/></clipPath>
<g clip-path="url(#r)"><rect width="{lw}" height="20" fill="#555"/><rect x="{lw}" width="{vw}" height="20" fill="{color}"/><rect width="{w}" height="20" fill="url(#s)"/></g>
<g fill="#fff" text-anchor="middle" font-family="Verdana,Geneva,DejaVu Sans,sans-serif" font-size="11">
<text x="{lw / 2}" y="15" fill="#010101" fill-opacity=".3">{escape(label)}</text><text x="{lw / 2}" y="14">{escape(label)}</text>
<text x="{lw + vw / 2}" y="15" fill="#010101" fill-opacity=".3">{escape(value)}</text><text x="{lw + vw / 2}" y="14">{escape(value)}</text>
</g>
</svg>
"""


def draw_badges(report, out_dir):
    m = report["measures"]
    for name, label, key, color in BADGES:
        with open(os.path.join(out_dir, name + ".svg"), "w", encoding="utf-8", newline="\n") as f:
            f.write(badge(label, "{:.2f}%".format(pct(m, key)), color))


def read_history(path):
    if not os.path.exists(path):
        return []
    with open(path, encoding="utf-8", newline="") as f:
        return list(csv.DictReader(f))


def record(report, history_path):
    rows = read_history(history_path)
    commit = git("rev-parse", "--short=7", "HEAD")
    if rows and rows[-1].get("commit") == commit:
        rows.pop()
    m = report["measures"]
    rows.append({
        "date": git("log", "-1", "--format=%cs") or datetime.date.today().isoformat(),
        "commit": commit,
        "code_fuzzy_pct": "{:.2f}".format(pct(m, "fuzzy_match_percent")),
        "code_matched_pct": "{:.2f}".format(pct(m, "matched_code_percent")),
        "code_linked_pct": "{:.2f}".format(pct(m, "complete_code_percent")),
        "functions_matched": m.get("matched_functions", 0),
        "functions_total": m.get("total_functions", 0),
    })
    # Keep the columns of older rows (game/SDK split) so no recorded value is lost.
    fields = list(HISTORY_FIELDS)
    for row in rows:
        fields += [k for k in row if k not in fields]
    with open(history_path, "w", encoding="utf-8", newline="\n") as f:
        w = csv.DictWriter(f, fieldnames=fields, lineterminator="\n")
        w.writeheader()
        for row in rows:
            w.writerow({k: row.get(k, "") for k in fields})


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--version", default="RM7P4F", type=str.upper,
                        help="game version (default: %(default)s)")
    parser.add_argument("--report", help="objdiff report (default: build/<version>/report.json)")
    parser.add_argument("--out", help="badge folder (default: progress/<version>)")
    parser.add_argument("--history", help="history file (default: progress/<version>/history.csv)")
    parser.add_argument("--record", action="store_true", help="append the current state to history.csv")
    args = parser.parse_args()
    if args.report is None:
        args.report = os.path.join(ROOT, "build", args.version, "report.json")
    if args.out is None:
        args.out = os.path.join(ROOT, "progress", args.version)
    if args.history is None:
        args.history = os.path.join(ROOT, "progress", args.version, "history.csv")

    os.makedirs(args.out, exist_ok=True)
    with open(args.report, encoding="utf-8") as f:
        report = json.load(f)
    if args.record:
        record(report, args.history)
    draw_badges(report, args.out)
    print("wrote fuzzy.svg, perfect.svg and linked.svg to", args.out)


if __name__ == "__main__":
    main()
