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
| 2026-10-12 | eae294f | 494344 / 3412920 bytes (14.48 %) | 2048 / 12580 | 195 linked / 986 | RVL_SDK: 99 units linked; 8.48 % of code linked |
| 2026-10-12 | 48855ac | 497736 / 3412920 bytes (14.58 %) | 2060 / 12580 | 217 linked / 990 | RVL_SDK: 121 units linked after the unit-by-unit data review; 10.16 % of code linked |
| 2026-10-13 | 90cb988 | 509368 / 3412920 bytes (14.92 %) | 2104 / 12580 | 242 linked / 988 | RVL_SDK: 146 units linked (functions from TP/Petari/rb3 where they match); 12.22 % of code linked |
| 2026-10-13 | 906585f | 510768 / 3412920 bytes (14.97 %) | 2106 / 12580 | 243 linked / 988 | MSL_C 59/59 linked (`alloc.c` done: `realloc`, `fn_80330B20` decompiled from the DOL); 12.37 % of code linked |
| 2026-10-13 | 1d905cd | 525220 / 3412920 bytes (15.39 %) | 2164 / 12580 | 254 linked / 990 | RVL_SDK third pass, smallest units first: AXVPB (split redone), AXProf, __ppc_eabi_init (.text and .init), dvdDeviceError, scapi, NANDLogging, gpiUtility, WUDHidHost, OSPlayRecord, OSPlayTime; 12.82 % of code linked |
| 2026-10-13 | eb8fbb9 | 539400 / 3412920 bytes (15.80 %) | 2210 / 12580 | 262 linked / 985 | RVL_SDK third pass continued: OSReset, dvderror, sb_queryengine, GXFifo, NANDCore, usb, OS, ipcclt (165/179 SDK units); NATify 99.8 %, natneg 98 %; 13.72 % of code linked |
| 2026-10-13 | d9a8d84 | 543204 / 3412920 bytes (15.92 %) | 2217 / 12580 | 263 linked / 983 | RVL_SDK: `scsystem` linked (166/179 SDK units), `gti2HandleMessage` decompiled (gt2Message 1 function left); 13.91 % of code linked |
| 2026-10-13 | 7f45f31 | 568928 / 3412920 bytes (16.67 %) | 2254 / 12580 | 265 linked / 980 | RVL_SDK: WUD (from Petari) and WPAD (newer behaviour after Petari) linked, 35+17+8 SDK functions named (168/179 SDK units); 15.37 % of code linked |
| 2026-10-13 | 31528e9 | 604072 / 3412920 bytes (17.70 %) | 2307 / 12580 | 269 linked / 979 | RVL_SDK from Petari (CC0): WPADHIDParser, uusb_ppc, dvd_broadway, vi linked (172/179 SDK units); 16.67 % of code linked |
| 2026-10-14 | d7dc2b0 | 616096 / 3412920 bytes (18.05 %) | 2322 / 12580 | 271 linked / 979 | RVL_SDK: gpiInfo (newer `GPIProfile` / `GPIPeer` layouts from mkw, CC0) and dvd (newer functions from Petari, CC0) linked (174/179 SDK units); 17.51 % of code linked |
| 2026-10-14 | eddfdb2 | 620860 / 3412920 bytes (18.19 %) | 2327 / 12580 | 273 linked / 977 | RVL_SDK: OSExec (Petari's newer build) and mem_list (split shrunk to the unit; the rest is an unsplit MIX region) linked (176/179 SDK units); 17.68 % of code linked |
| 2026-10-14 | a208594 | 637472 / 3412920 bytes (18.68 %) | 2361 / 12580 | 277 linked / 981 | RVL_SDK: MIX (`mix`, `remote`, newer than mscharged-decomp's) and DSP (`dsp`, `dsp_debug`, `dsp_task` from `__DSPHandler`) split and linked; GameSpy gt2Message / NATify / natneg still not matching; 18.14 % of code linked |
| 2026-10-14 | fd99a1a | 671420 / 3412920 bytes (19.67 %) | 2530 / 12580 | 296 linked / 973 | RVL_SDK unsplit regions found by a size-sequence search over the reference clones and linked: esp, KPAD, OSStateTM, OSLaunch, mtx/mtxvec/mtx44, mem_expHeap/frameHeap/allocator, ai, AX, AXAlloc, AXAux, AXOut, AXSPB, OSSemaphore, GXFrameBuf, GXLight, scapi_prdinfo extended; 19.14 % of code linked |
| 2026-10-14 | 49c4b6a | 745680 / 3412920 bytes (21.85 %) | 2755 / 12580 | 318 linked / 993 | Smallest gaps first: time.dolphin, WPAD/WUD debug_msg, wenc, AXFXReverbHiExp(+Dpl2); then GameSpy from mkw (GP x13, gsUdpEngine, qr2regkeys, md5c, gsAvailable linked; qr2, gsPlatformSocket, gsPlatformUtil, gsMemory split NonMatching); 20.97 % of code linked |

Notes:

- "Units" counts units in `objdiff.json`; library units are the ones split by hand in
  `config/RM7P4F/splits.txt` (progress category `sdk`, 634340 bytes of code); the rest
  are dtk `auto_*` objects.
- Game code: `game/unit_8001FCA8.cpp` is linked (first game unit); `game/unit_80020480.cpp`
  is split, 29/31 functions, not linked yet.
