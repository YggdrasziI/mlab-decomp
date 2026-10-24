# References

## Tools

- [encounter/dtk-template](https://github.com/encounter/dtk-template): project
  skeleton (`configure.py`, ninja); its docs cover dependencies, getting started,
  symbols, splits, GitHub Actions, common BSS and the `.comment` section.
- [encounter/decomp-toolkit](https://github.com/encounter/decomp-toolkit) (dtk):
  DOL analysis, `symbols.txt` / `splits.txt`, final match check.
- [encounter/objdiff](https://github.com/encounter/objdiff): local diff (reads
  `objdiff.json`).
- [decomp.me](https://decomp.me): shared scratches and compiler tests.
- [decomp.dev](https://decomp.dev): progress tracking (not used: the project stays
  private).
- [decompals/wibo](https://github.com/decompals/wibo) (runs mwcc on Linux),
  [encounter/sjiswrap](https://github.com/encounter/sjiswrap) (UTF-8 to Shift-JIS).
- Ghidra (Gekko/Broadway module) and the Dolphin debugger to understand behaviour;
  Dolphin to extract the disc.
- Discord "GC/Wii Decompilation" (`#dtk` channel).

## Reference projects (local clones)

Search the local clones (Grep/Glob) before GitHub. A clone may be behind GitHub: ask
the owner before `git pull`. When reusing code, record the clone's commit
(`git -C <path> rev-parse HEAD`) and the license in `notes/PROVENANCE.md`.

Wii:

| Folder | Project | State |
|---|---|---|
| `mscharged-decomp` | [Mario Strikers Charged](https://github.com/yannicksuter/mscharged-decomp) (CC0) | 100 % |
| `Petari` | [Super Mario Galaxy](https://github.com/SMGCommunity/Petari) (CC0) | 100 % |
| `oot-vc` | [N64 Emulator for Wii](https://github.com/zeldaret/oot-vc) | partial |
| `pik2wii` | [New Play Control! Pikmin 2](https://github.com/projectPiki/pik2wii) | partial |
| `rb3` | [Rock Band 3](https://github.com/DarkRTA/rb3) | partial |
| `pik1wii` | [New Play Control! Pikmin](https://github.com/projectPiki/pik1wii) | partial |
| `ss` | [Skyward Sword](https://github.com/zeldaret/ss) | partial |
| `Garigari` | [Super Mario Galaxy 2](https://github.com/SMGCommunity/Garigari) | partial |
| `NSMBW-Decomp` | [New Super Mario Bros. Wii](https://github.com/NSMBW-Community/NSMBW-Decomp) | partial |
| `mkw` | [Mario Kart Wii](https://github.com/doldecomp/mkw) | partial |
| `Castaway` | [The Sims 2: Castaway](https://github.com/HaydnTrigg/Castaway) | partial |

GameCube:

| Folder | Project | State |
|---|---|---|
| `tp` | [Twilight Princess](https://github.com/zeldaret/tp) | 100 % |
| `melee` | [Super Smash Bros. Melee](https://github.com/doldecomp/melee) | 100 % |
| `SFA-Decomp` | [Star Fox Adventures](https://github.com/zcanann/SFA-Decomp) | 100 % |
| `pikmin` | [Pikmin](https://github.com/projectPiki/pikmin) | 100 % |
| `smstrikers-decomp` | [Super Mario Strikers](https://github.com/yannicksuter/smstrikers-decomp) | 100 % |
| `ac-decomp` | [Animal Crossing](https://github.com/ACreTeam/ac-decomp) | 100 % |
| `marioparty4` | [Mario Party 4](https://github.com/mariopartyrd/marioparty4) | 100 % |
| `oot-gc` | [N64 Emulator for GameCube](https://github.com/zeldaret/oot-gc) | 100 % |

The owner's own projects:

| Folder | Project | State |
|---|---|---|
| `spyro-dotd-decomp` | The Legend of Spyro: Dawn of the Dragon (Wii, `RO8P7D`), private repository `YggdrasziI/spyro-dotd-decomp` (CC0) | partial |

Same toolchain and layout as this project (dtk-template, multi-version model, progress
card). Part of its RVL_SDK, MSL_C and Runtime units come from this project: a source
shared with it is not independent evidence.

The `doldecomp` organization hosts reusable libraries (SDK, JSystem...); check the
version of each library against this DOL.

## Reuse permissions

- CC0 references (mscharged-decomp, Petari, TP) come first.
- `spyro-dotd-decomp` (the owner's own project, CC0): reuse allowed by the owner on
  2026-10-25.
- `DarkRTA/rb3` (no license file): reuse allowed by the owner on 2026-10-12 when the
  CC0 references lack a source.
- `zeldaret/oot-vc` (no license file): reuse allowed by the owner on 2026-10-14 (close
  RVL SDK build: MTX, time.dolphin, OS...), same conditions.
- `yannicksuter/smstrikers-decomp` (GC folder; its MSL C++ headers are marked as
  reconstructed proprietary runtime material, not CC0): reuse of
  `include/PowerPC_EABI_Support/MSL_C++/MSL_Common/msl_tree.h` allowed by the owner on
  2026-10-18 (MSL red-black tree for the game maps).
- Any other project without a known license needs the owner's approval first.
- Every reuse is logged in `notes/PROVENANCE.md` (file, commit, changes, license or
  its absence).

## Game knowledge

`wiki/monster-lab-wiki/` was written without binary access: use it for candidate
names and subsystem prefixes only, then require binary evidence before renaming.
Accepted names go to its `conventions/evidence-log.md`; keep its known-unknowns page in
sync with `notes/`.
