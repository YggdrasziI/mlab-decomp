#!/usr/bin/env python3
"""Show one function of a compiled game unit next to the original disassembly.

Lines whose opcode differs are marked with '*'. Operand formats differ between the
two sides (decimal vs hex, addi vs subi, rlwinm vs clrlslwi/extrwi), so compare
operands by eye; the objdiff report gives the real score.

Usage:
  python tools/asm_compare.py unit_800445E4 fn_800446BC
  python tools/asm_compare.py unit_8000C478 <our symbol> [<original symbol>]
  python tools/asm_compare.py --version RM7E4F unit_800445E4 fn_800446BC
"""

import os
import re
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
VERSION = "RM7P4F"  # default; --version selects another game version
OBJDUMP = os.path.join(ROOT, "build", "binutils", "powerpc-eabi-objdump.exe")


def ours(unit, name):
    obj = os.path.join(ROOT, "build", VERSION, "src", "game", unit + ".o")
    out = subprocess.run([OBJDUMP, "-d", "--no-show-raw-insn", "-r", obj],
                         capture_output=True, text=True, check=True).stdout
    m = re.search(r"<" + re.escape(name) + r">:\n(.*?)(\n\n|\Z)", out, re.S)
    if not m:
        sys.exit(f"{name} not found in {obj}")
    lines = []
    for line in m.group(1).split("\n"):
        if "R_PPC" in line:
            if lines:
                lines[-1] += "  <" + line.split()[-1] + ">"
            continue
        lines.append(re.sub(r"\s+", " ", line.split("\t", 1)[-1].strip()))
    return lines


def original(unit, name):
    path = os.path.join(ROOT, "build", VERSION, "asm", "game", unit + ".s")
    text = open(path, encoding="utf-8").read()
    m = re.search(r"\.fn " + re.escape(name) + r",.*?\n(.*?)\.endfn", text, re.S)
    if not m:
        sys.exit(f"{name} not found in {path}")
    return [re.sub(r"\s+", " ", line.split("*/", 1)[1].strip())
            for line in m.group(1).split("\n") if line.startswith("/*")]


def main():
    global VERSION
    argv = sys.argv[1:]
    if len(argv) >= 2 and argv[0] == "--version":
        VERSION = argv[1].upper()
        argv = argv[2:]
    if len(argv) < 2:
        sys.exit(__doc__)
    unit, name = argv[0], argv[1]
    orig_name = argv[2] if len(argv) > 2 else name
    a, b = ours(unit, name), original(unit, orig_name)
    for i in range(max(len(a), len(b))):
        x = a[i] if i < len(a) else ""
        y = b[i] if i < len(b) else ""
        mark = " " if x.split(" ")[0] == y.split(" ")[0] else "*"
        print(f"{mark} {x[:50]:<50} | {y}")


if __name__ == "__main__":
    main()
