#!/usr/bin/env python3
"""Draw progress charts from the objdiff report, without decomp.dev.

Outputs (SVG, readable on GitHub in light and dark themes):
  progress/treemap.svg   one rectangle per unit, sized by code, colored by fuzzy match
                         (same idea as the decomp.dev overview image)
  progress/progress.svg  current state: bars per category and a map of every unit
  progress/history.svg   matched and linked code over time (progress/history.csv)

Usage:
  python tools/progress_chart.py                # draw from build/RM7P4F/report.json
  python tools/progress_chart.py --record       # also append the current state to history.csv
  python tools/progress_chart.py --backfill     # seed history.csv from notes/PROGRESS.md
"""

import argparse
import csv
import datetime
import json
import os
import re
import subprocess
from xml.sax.saxutils import escape

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

# Colors chosen to stay readable on white and dark backgrounds.
COLOR_LINKED = "#2da44e"     # unit or code linked into the matching DOL
COLOR_MATCHED = "#a6d96a"    # matches but not linked yet
COLOR_PARTIAL = "#f0b429"    # source exists, not fully matching
COLOR_EMPTY = "#8c959f"      # no source yet
COLOR_TEXT = "#57606a"
COLOR_BG = "#ffffff"

HISTORY_FIELDS = [
    "date", "commit", "code_matched_pct", "code_linked_pct",
    "game_matched_pct", "game_linked_pct", "sdk_matched_pct", "sdk_linked_pct",
    "functions_matched", "functions_total",
]


def pct(measures, key):
    return float(measures.get(key, 0.0) or 0.0)


def git(*args):
    try:
        return subprocess.run(["git", *args], cwd=ROOT, capture_output=True, text=True).stdout.strip()
    except OSError:
        return ""


def category_rows(report):
    """Return (id, label, measures) for all code, the game and the SDK.

    objdiff's "game" category only counts units that already have a split, so
    the game figures are derived as everything that is not SDK code.
    """
    total = report["measures"]
    rows = [("all", "All code", total)]
    sdk = next((c for c in report.get("categories", []) if c["id"] == "sdk"), None)
    if sdk is None:
        return rows
    sm = sdk["measures"]
    game = {}
    for key in ("total_code", "matched_code", "complete_code", "total_functions", "matched_functions", "total_units", "complete_units"):
        game[key] = int(total.get(key, 0) or 0) - int(sm.get(key, 0) or 0)
    for key in ("matched_code", "complete_code"):
        game[key + "_percent"] = 100.0 * game[key] / game["total_code"] if game["total_code"] else 0.0
    rows.append(("game", "Game code (everything but the SDK)", game))
    rows.append(("sdk", sdk.get("name", "SDK Code"), sm))
    return rows


def bar(x, y, width, height, matched, linked, label, detail):
    """One horizontal bar: linked part, then matched part, on a grey track."""
    out = [
        f'<text x="{x}" y="{y - 6}" font-size="13" fill="{COLOR_TEXT}">{escape(label)}</text>',
        f'<text x="{x + width}" y="{y - 6}" font-size="12" fill="{COLOR_TEXT}" text-anchor="end">{escape(detail)}</text>',
        f'<rect x="{x}" y="{y}" width="{width}" height="{height}" rx="3" fill="{COLOR_EMPTY}" fill-opacity="0.35"/>',
    ]
    matched_w = width * max(matched, linked) / 100.0
    linked_w = width * linked / 100.0
    if matched_w > 0:
        out.append(f'<rect x="{x}" y="{y}" width="{matched_w:.1f}" height="{height}" rx="3" fill="{COLOR_MATCHED}"/>')
    if linked_w > 0:
        out.append(f'<rect x="{x}" y="{y}" width="{linked_w:.1f}" height="{height}" rx="3" fill="{COLOR_LINKED}"/>')
    return out


def unit_state(unit):
    m = unit.get("measures", {})
    meta = unit.get("metadata", {})
    if m.get("complete_units"):
        return COLOR_LINKED
    if meta.get("auto_generated"):
        return COLOR_EMPTY
    if pct(m, "matched_code_percent") >= 100.0 and pct(m, "fuzzy_match_percent") >= 100.0:
        return COLOR_MATCHED
    return COLOR_PARTIAL


def draw_progress(report, path):
    width = 760
    x0 = 20
    bar_w = width - 2 * x0
    rows = [(label, m) for _, label, m in category_rows(report)]

    out = []
    y = 46
    for name, m in rows:
        detail = "{:.2f} % matched, {:.2f} % linked ({} / {} functions)".format(
            pct(m, "matched_code_percent"), pct(m, "complete_code_percent"),
            m.get("matched_functions", 0), m.get("total_functions", 0))
        out += bar(x0, y, bar_w, 14, pct(m, "matched_code_percent"), pct(m, "complete_code_percent"), name, detail)
        y += 42
    data = report["measures"]
    out += bar(x0, y, bar_w, 10, pct(data, "matched_data_percent"), pct(data, "complete_data_percent"),
               "All data", "{:.2f} % matched, {:.2f} % linked".format(
                   pct(data, "matched_data_percent"), pct(data, "complete_data_percent")))
    y += 34

    # Unit map: one square per unit, in link order.
    units = report.get("units", [])
    out.append(f'<text x="{x0}" y="{y}" font-size="13" fill="{COLOR_TEXT}">Units ({len(units)}), in link order</text>')
    y += 8
    cell = 9
    gap = 2
    cols = bar_w // (cell + gap)
    for i, unit in enumerate(units):
        cx = x0 + (i % cols) * (cell + gap)
        cy = y + (i // cols) * (cell + gap)
        title = escape(unit.get("name", ""))
        out.append(f'<rect x="{cx}" y="{cy}" width="{cell}" height="{cell}" rx="1.5" fill="{unit_state(unit)}">'
                   f'<title>{title}</title></rect>')
    y += ((len(units) + cols - 1) // cols) * (cell + gap) + 18

    legend = [(COLOR_LINKED, "linked"), (COLOR_MATCHED, "matching, not linked"),
              (COLOR_PARTIAL, "work in progress"), (COLOR_EMPTY, "not started")]
    lx = x0
    for color, label in legend:
        out.append(f'<rect x="{lx}" y="{y - 9}" width="10" height="10" rx="1.5" fill="{color}"/>')
        out.append(f'<text x="{lx + 14}" y="{y}" font-size="12" fill="{COLOR_TEXT}">{label}</text>')
        lx += 14 + 7 * len(label) + 24
    y += 12

    head = (f'<svg xmlns="http://www.w3.org/2000/svg" width="{width}" height="{y}" '
            f'viewBox="0 0 {width} {y}" font-family="-apple-system, Segoe UI, Helvetica, Arial, sans-serif">')
    title = f'<text x="{x0}" y="22" font-size="15" font-weight="600" fill="{COLOR_TEXT}">Monster Lab (RM7P4F) decompilation progress</text>'
    bg = f'<rect width="{width}" height="{y}" rx="6" fill="{COLOR_BG}" fill-opacity="0"/>'
    with open(path, "w", encoding="utf-8", newline="\n") as f:
        f.write("\n".join([head, bg, title] + out + ["</svg>"]) + "\n")


def squarify(items, x, y, w, h):
    """Squarified treemap layout: items are (size, payload), sorted by size descending."""
    out = []
    items = [it for it in items if it[0] > 0]
    total = float(sum(s for s, _ in items))
    if not items or w <= 0 or h <= 0:
        return out
    scale = w * h / total
    areas = [(s * scale, p) for s, p in items]

    def worst(row, side):
        s = sum(a for a, _ in row)
        big = max(a for a, _ in row)
        small = min(a for a, _ in row)
        return max(side * side * big / (s * s), (s * s) / (side * side * small))

    i = 0
    while i < len(areas):
        side = min(w, h)
        row = [areas[i]]
        i += 1
        while i < len(areas) and worst(row + [areas[i]], side) <= worst(row, side):
            row.append(areas[i])
            i += 1
        s = sum(a for a, _ in row)
        if w >= h:
            rw = s / h
            cy = y
            for a, p in row:
                out.append((x, cy, rw, a / rw, p))
                cy += a / rw
            x += rw
            w -= rw
        else:
            rh = s / w
            cx = x
            for a, p in row:
                out.append((cx, y, a / rh, rh, p))
                cx += a / rh
            y += rh
            h -= rh
    return out


def match_color(percent):
    """Red (0 %) to yellow to green (100 %); a full match is pure green."""
    hue = 120.0 * max(0.0, min(percent, 100.0)) / 100.0
    light = 38 if percent >= 100.0 else 45
    return f"hsl({hue:.0f}, 100%, {light}%)"


def draw_treemap(report, path):
    width, height = 840, 420
    units = []
    for unit in report.get("units", []):
        size = int(unit.get("measures", {}).get("total_code", 0) or 0)
        if size:
            units.append((size, unit))
    units.sort(key=lambda it: it[0], reverse=True)
    out = [
        f'<svg xmlns="http://www.w3.org/2000/svg" width="{width}" height="{height}" viewBox="0 0 {width} {height}">',
        '<defs><linearGradient id="shade" x1="0" y1="0" x2="1" y2="1">'
        '<stop offset="0" stop-color="#fff" stop-opacity="0.12"/>'
        '<stop offset="1" stop-color="#000" stop-opacity="0.28"/></linearGradient></defs>',
    ]
    for x, y, w, h, unit in squarify(units, 0, 0, width, height):
        m = unit["measures"]
        fuzzy = pct(m, "fuzzy_match_percent")
        name = unit.get("name", "")
        name = name[len("main/"):] if name.startswith("main/") else name
        title = escape("{}: {:.2f} % ({} bytes)".format(name, fuzzy, m.get("total_code", 0)))
        rect = f'x="{x:.2f}" y="{y:.2f}" width="{w:.2f}" height="{h:.2f}"'
        out.append(f'<g><title>{title}</title><rect {rect} fill="{match_color(fuzzy)}"/>'
                   f'<rect {rect} fill="url(#shade)" stroke="#000" stroke-opacity="0.45" stroke-width="0.6"/></g>')
    out.append("</svg>")
    with open(path, "w", encoding="utf-8", newline="\n") as f:
        f.write("\n".join(out) + "\n")


def read_history(path):
    if not os.path.exists(path):
        return []
    with open(path, encoding="utf-8", newline="") as f:
        return list(csv.DictReader(f))


def write_history(path, rows):
    with open(path, "w", encoding="utf-8", newline="\n") as f:
        w = csv.DictWriter(f, fieldnames=HISTORY_FIELDS, lineterminator="\n")
        w.writeheader()
        for row in rows:
            w.writerow({k: row.get(k, "") for k in HISTORY_FIELDS})


def backfill(history_path):
    """Seed the history from the hand-written table in notes/PROGRESS.md."""
    rows = []
    table = os.path.join(ROOT, "notes", "PROGRESS.md")
    pattern = re.compile(r"^\| (\d{4}-\d{2}-\d{2}) \| ([0-9a-f]{7}) \| \d+ / \d+ bytes \(([\d.]+) %\) \| (\d+) / (\d+) \|(.*)$")
    for line in open(table, encoding="utf-8"):
        m = pattern.match(line.strip())
        if not m:
            continue
        linked = re.search(r"([\d.]+) % of code linked", m.group(6))
        rows.append({
            "date": m.group(1), "commit": m.group(2), "code_matched_pct": m.group(3),
            "code_linked_pct": linked.group(1) if linked else "",
            "functions_matched": m.group(4), "functions_total": m.group(5),
        })
    write_history(history_path, rows)
    return rows


def record(report, history_path):
    rows = read_history(history_path)
    commit = git("rev-parse", "--short=7", "HEAD")
    if rows and rows[-1].get("commit") == commit:
        rows.pop()
    date = git("log", "-1", "--format=%cs") or datetime.date.today().isoformat()
    m = report["measures"]
    row = {
        "date": date, "commit": commit,
        "code_matched_pct": "{:.2f}".format(pct(m, "matched_code_percent")),
        "code_linked_pct": "{:.2f}".format(pct(m, "complete_code_percent")),
        "functions_matched": m.get("matched_functions", 0),
        "functions_total": m.get("total_functions", 0),
    }
    for cid, _, cm in category_rows(report)[1:]:
        row[cid + "_matched_pct"] = "{:.2f}".format(pct(cm, "matched_code_percent"))
        row[cid + "_linked_pct"] = "{:.2f}".format(pct(cm, "complete_code_percent"))
    rows.append(row)
    write_history(history_path, rows)
    return rows


def draw_history(rows, path):
    width, height = 760, 300
    left, right, top, bottom = 48, 20, 36, 46
    plot_w = width - left - right
    plot_h = height - top - bottom
    out = [
        f'<svg xmlns="http://www.w3.org/2000/svg" width="{width}" height="{height}" '
        f'viewBox="0 0 {width} {height}" font-family="-apple-system, Segoe UI, Helvetica, Arial, sans-serif">',
        f'<text x="{left}" y="22" font-size="15" font-weight="600" fill="{COLOR_TEXT}">Code progress over time (all code)</text>',
    ]
    values = [r for r in rows if r.get("code_matched_pct")]
    top_pct = 100.0
    if values:
        peak = max(float(r["code_matched_pct"]) for r in values)
        top_pct = max(10.0, min(100.0, (int(peak / 10) + 1) * 10))
    for i in range(0, int(top_pct) + 1, 5 if top_pct <= 40 else 10):
        y = top + plot_h - plot_h * i / top_pct
        out.append(f'<line x1="{left}" y1="{y:.1f}" x2="{left + plot_w}" y2="{y:.1f}" stroke="{COLOR_EMPTY}" stroke-opacity="0.3"/>')
        out.append(f'<text x="{left - 6}" y="{y + 4:.1f}" font-size="11" fill="{COLOR_TEXT}" text-anchor="end">{i} %</text>')

    def series(key, color, label, lx):
        pts = [(i, float(r[key])) for i, r in enumerate(rows) if r.get(key)]
        if not pts:
            return
        n = max(len(rows) - 1, 1)
        coords = " ".join("{:.1f},{:.1f}".format(left + plot_w * i / n, top + plot_h - plot_h * v / top_pct) for i, v in pts)
        out.append(f'<polyline points="{coords}" fill="none" stroke="{color}" stroke-width="2.2"/>')
        for i, v in pts:
            out.append('<circle cx="{:.1f}" cy="{:.1f}" r="2.6" fill="{}"><title>{} {}: {:.2f} %</title></circle>'.format(
                left + plot_w * i / n, top + plot_h - plot_h * v / top_pct, color, rows[i]["date"], rows[i]["commit"], v))
        out.append(f'<rect x="{lx}" y="{height - 16}" width="10" height="10" rx="1.5" fill="{color}"/>')
        out.append(f'<text x="{lx + 14}" y="{height - 7}" font-size="12" fill="{COLOR_TEXT}">{label}</text>')

    series("code_matched_pct", COLOR_MATCHED, "matched", left)
    series("code_linked_pct", COLOR_LINKED, "linked", left + 90)
    if rows:
        out.append(f'<text x="{left}" y="{top + plot_h + 16}" font-size="11" fill="{COLOR_TEXT}">{rows[0]["date"]}</text>')
        out.append(f'<text x="{left + plot_w}" y="{top + plot_h + 16}" font-size="11" fill="{COLOR_TEXT}" text-anchor="end">{rows[-1]["date"]}</text>')
    out.append("</svg>")
    with open(path, "w", encoding="utf-8", newline="\n") as f:
        f.write("\n".join(out) + "\n")


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--report", default=os.path.join(ROOT, "build", "RM7P4F", "report.json"))
    parser.add_argument("--out", default=os.path.join(ROOT, "progress"))
    parser.add_argument("--history", default=os.path.join(ROOT, "progress", "history.csv"))
    parser.add_argument("--record", action="store_true", help="append the current state to history.csv")
    parser.add_argument("--backfill", action="store_true", help="rebuild history.csv from notes/PROGRESS.md first")
    args = parser.parse_args()

    os.makedirs(args.out, exist_ok=True)
    history_path = args.history
    with open(args.report, encoding="utf-8") as f:
        report = json.load(f)

    if args.backfill:
        backfill(history_path)
    rows = record(report, history_path) if args.record else read_history(history_path)

    draw_treemap(report, os.path.join(args.out, "treemap.svg"))
    draw_progress(report, os.path.join(args.out, "progress.svg"))
    draw_history(rows, os.path.join(args.out, "history.svg"))
    print("wrote treemap.svg, progress.svg and history.svg to", args.out)


if __name__ == "__main__":
    main()
