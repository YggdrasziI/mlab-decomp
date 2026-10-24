#!/usr/bin/env python3
"""List game units not split yet, smallest first.

Unit ends come from the static initializers in .ctors (each game unit that includes
the shared vector header has one), so bounds are approximate: inline functions emitted
after a unit's __sinit belong to that unit, not to the next one. Check the bounds as
described in the decomp-unit skill before splitting.

Usage:
  python tools/unit_candidates.py           # 40 smallest candidates
  python tools/unit_candidates.py -n 100    # more
  python tools/unit_candidates.py --version RM7E4F   # another game version (default RM7P4F)
"""

import argparse
import glob
import os
import re
import struct

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
DEFAULT_VERSION = "RM7P4F"
# End of game code per version: the first SDK function (notes/SPLITS.md). RM7E4F has
# the same code layout as RM7P4F (SPEC.md, "Other versions").
GAME_END = {
    "RM7P4F": 0x80232AF8,
    "RM7E4F": 0x80232AF8,
}
FLOAT_PREFIXES = ("f", "lfs", "lfd", "stfs", "stfd", "psq")


def read_dol(path):
    data = open(path, "rb").read()
    offsets = struct.unpack(">18I", data[0:72])
    addrs = struct.unpack(">18I", data[72:144])
    sizes = struct.unpack(">18I", data[144:216])

    def read_word(addr):
        for off, start, size in zip(offsets, addrs, sizes):
            if start <= addr < start + size:
                return struct.unpack(">I", data[off + addr - start:off + addr - start + 4])[0]
        return None

    return read_word


def main():
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("-n", type=int, default=40, help="number of candidates to show")
    parser.add_argument("--version", default=DEFAULT_VERSION, type=str.upper,
                        choices=sorted(GAME_END), help="game version (default: %(default)s)")
    args = parser.parse_args()
    game_end = GAME_END[args.version]

    config = os.path.join(ROOT, "config", args.version)
    symbols = open(os.path.join(config, "symbols.txt"), encoding="utf-8").read()
    splits = open(os.path.join(config, "splits.txt"), encoding="utf-8").read()
    read_word = read_dol(os.path.join(ROOT, "orig", args.version, "sys", "main.dol"))

    funcs = {}
    for m in re.finditer(r"^(\S+) = \.text:(0x[0-9A-F]+); // type:function size:(0x[0-9A-F]+)",
                         symbols, re.M):
        funcs[int(m.group(2), 16)] = (m.group(1), int(m.group(3), 16))

    # Walk .ctors from a known entry in both directions.
    ctors_starts = [int(x, 16) for x in re.findall(r"\.ctors\s+start:(0x[0-9A-F]+)", splits)]
    addr = min(ctors_starts)
    while True:
        prev = read_word(addr - 4)
        if prev and 0x80004000 < prev < game_end:
            addr -= 4
        else:
            break
    entries = []
    while True:
        value = read_word(addr)
        if not value:
            break
        entries.append(value)
        addr += 4

    split_starts = {int(x, 16) for x in re.findall(r"^game/unit_([0-9A-F]+)", splits, re.M)}
    split_ranges = [(int(a, 16), int(b, 16)) for a, b in
                    re.findall(r"^game/unit_[0-9A-F]+\.cpp:\n\t\.text\s+start:(0x[0-9A-F]+) end:(0x[0-9A-F]+)",
                               splits, re.M)]

    opcodes = {}
    for path in glob.glob(os.path.join(ROOT, "build", args.version, "asm", "**", "*.s"), recursive=True):
        for m in re.finditer(r"^/\* ([0-9A-F]{8}) [0-9A-F]+  [0-9A-F ]+ \*/\t(\S+)",
                             open(path, encoding="utf-8").read(), re.M):
            opcodes[int(m.group(1), 16)] = m.group(2)

    ends = sorted(v + funcs.get(v, ("", 0))[1] for v in entries if v < game_end)
    rows = []
    start = 0x80006B40
    for end in ends:
        unit = (start, end)
        start = end
        if unit[0] in split_starts or any(a < unit[1] and unit[0] < b for a, b in split_ranges):
            continue
        ops = [opcodes.get(a, "") for a in range(unit[0], unit[1], 4)]
        count = max(len(ops), 1)
        floats = sum(1 for op in ops if op.startswith(FLOAT_PREFIXES))
        calls = sum(1 for op in ops if op == "bl")
        nfuncs = sum(1 for a in funcs if unit[0] <= a < unit[1])
        rows.append((unit[1] - unit[0], unit[0], nfuncs, 100 * floats // count, calls))

    rows.sort()
    print("start       size    funcs  float%  calls")
    for size, begin, nfuncs, fpct, calls in rows[:args.n]:
        print(f"0x{begin:08X}  0x{size:05X}  {nfuncs:5d}  {fpct:5d}%  {calls:5d}")
    print(f"{len(rows)} unsplit game units")


if __name__ == "__main__":
    main()
