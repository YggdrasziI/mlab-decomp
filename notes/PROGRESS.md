# Progress log

Private progress history (the project is not published and not on decomp.dev yet).
One line per milestone, figures taken from `build/RM7P4F/report.json` (objdiff report,
also uploaded as the `RM7P4F_report` artifact by CI; artifacts expire, this log does not).

| Date | Commit | Matched code | Matched functions | Units (split / total) | Milestone |
|---|---|---|---|---|---|
| 2026-10-12 | 0a8441e | 0 / 3412916 bytes (0 %) | 0 / 12580 | 274 library units / 983 | Steps 1-4 done: setup, compiler (GC/3.0a5.2 working hypothesis), library splits (Runtime 9, MSL_C 59, MetroTRK 29, RVL_SDK 177), CI checking `main.dol` |
| 2026-10-12 | a6bbae9 | 4416 / 3412916 bytes (0.13 %) | 34 / 12580 | 9 matching / 983 | Step 5 pilot: Runtime.PPCEABI.H fully matching and linked (code reused from mscharged-decomp and TP, CC0; see `notes/PROVENANCE.md`) |
| 2026-10-12 | 32074d7 | 48304 / 3412916 bytes (1.42 %) | 159 / 12580 | 38 matching / 983 | MSL_C: 29 of 59 units matching and linked (mscharged-decomp code, CC0) |
| 2026-10-12 | d3d2e64 | 65008 / 3412916 bytes (1.90 %) | 182 / 12580 | 65 matching / 982 | MSL_C: 56 of 59 units matching and linked; MSL files compiled with both GC/3.0a5 and GC/3.0a5.2 |
| 2026-10-12 | e7b97cc | 96644 / 3412916 bytes (2.83 %) | 341 / 12580 | 96 matching / 982 | Runtime 9/9, MSL_C 58/59, MetroTRK 29/29 matching and linked |
| 2026-10-12 | 8e34901 | 494344 / 3412920 bytes (14.48 %) | 2048 / 12580 | 182 matching / 987 | RVL_SDK: 86 units matching and linked; 7.80 % of code linked (ninja progress) |
| 2026-10-12 | e4fec51 | 494344 / 3412920 bytes (14.48 %) | 2048 / 12580 | 195 linked / 986 | RVL_SDK: 99 units linked; 8.48 % of code linked |
| 2026-10-12 | 972496c | 497736 / 3412920 bytes (14.58 %) | 2060 / 12580 | 217 linked / 990 | RVL_SDK: 121 units linked after the unit-by-unit data review; 10.16 % of code linked |
| 2026-10-13 | 0c11f92 | 509368 / 3412920 bytes (14.92 %) | 2104 / 12580 | 242 linked / 988 | RVL_SDK: 146 units linked (functions from TP/Petari/rb3 where they match); 12.22 % of code linked |
| 2026-10-13 | b4a9fff | 510768 / 3412920 bytes (14.97 %) | 2106 / 12580 | 243 linked / 988 | MSL_C 59/59 linked (`alloc.c` done: `realloc`, `fn_80330B20` decompiled from the DOL); 12.37 % of code linked |
| 2026-10-13 | 12f0315 | 525220 / 3412920 bytes (15.39 %) | 2164 / 12580 | 254 linked / 990 | RVL_SDK third pass, smallest units first: AXVPB (split redone), AXProf, __ppc_eabi_init (.text and .init), dvdDeviceError, scapi, NANDLogging, gpiUtility, WUDHidHost, OSPlayRecord, OSPlayTime; 12.82 % of code linked |
| 2026-10-13 | 3997951 | 539400 / 3412920 bytes (15.80 %) | 2210 / 12580 | 262 linked / 985 | RVL_SDK third pass continued: OSReset, dvderror, sb_queryengine, GXFifo, NANDCore, usb, OS, ipcclt (165/179 SDK units); NATify 99.8 %, natneg 98 %; 13.72 % of code linked |
| 2026-10-13 | 88fd0ea | 543204 / 3412920 bytes (15.92 %) | 2217 / 12580 | 263 linked / 983 | RVL_SDK: `scsystem` linked (166/179 SDK units), `gti2HandleMessage` decompiled (gt2Message 1 function left); 13.91 % of code linked |
| 2026-10-13 | da2b39b | 568928 / 3412920 bytes (16.67 %) | 2254 / 12580 | 265 linked / 980 | RVL_SDK: WUD (from Petari) and WPAD (newer behaviour after Petari) linked, 35+17+8 SDK functions named (168/179 SDK units); 15.37 % of code linked |
| 2026-10-13 | 42d5c0d | 604072 / 3412920 bytes (17.70 %) | 2307 / 12580 | 269 linked / 979 | RVL_SDK from Petari (CC0): WPADHIDParser, uusb_ppc, dvd_broadway, vi linked (172/179 SDK units); 16.67 % of code linked |
| 2026-10-14 | 3aa2574 | 616096 / 3412920 bytes (18.05 %) | 2322 / 12580 | 271 linked / 979 | RVL_SDK: gpiInfo (newer `GPIProfile` / `GPIPeer` layouts from mkw, CC0) and dvd (newer functions from Petari, CC0) linked (174/179 SDK units); 17.51 % of code linked |
| 2026-10-14 | 7b62bb3 | 620860 / 3412920 bytes (18.19 %) | 2327 / 12580 | 273 linked / 977 | RVL_SDK: OSExec (Petari's newer build) and mem_list (split shrunk to the unit; the rest is an unsplit MIX region) linked (176/179 SDK units); 17.68 % of code linked |
| 2026-10-14 | 1d7e3b7 | 637472 / 3412920 bytes (18.68 %) | 2361 / 12580 | 277 linked / 981 | RVL_SDK: MIX (`mix`, `remote`, newer than mscharged-decomp's) and DSP (`dsp`, `dsp_debug`, `dsp_task` from `__DSPHandler`) split and linked; GameSpy gt2Message / NATify / natneg still not matching; 18.14 % of code linked |
| 2026-10-14 | a7fcbf7 | 671420 / 3412920 bytes (19.67 %) | 2530 / 12580 | 296 linked / 973 | RVL_SDK unsplit regions found by a size-sequence search over the reference clones and linked: esp, KPAD, OSStateTM, OSLaunch, mtx/mtxvec/mtx44, mem_expHeap/frameHeap/allocator, ai, AX, AXAlloc, AXAux, AXOut, AXSPB, OSSemaphore, GXFrameBuf, GXLight, scapi_prdinfo extended; 19.14 % of code linked |
| 2026-10-14 | 98e0c5c | 745680 / 3412920 bytes (21.85 %) | 2755 / 12580 | 318 linked / 993 | Smallest gaps first: time.dolphin, WPAD/WUD debug_msg, wenc, AXFXReverbHiExp(+Dpl2); then GameSpy from mkw (GP x13, gsUdpEngine, qr2regkeys, md5c, gsAvailable linked; qr2, gsPlatformSocket, gsPlatformUtil, gsMemory split NonMatching); 20.97 % of code linked |
| 2026-10-15 | f9d4e3b | 752116 / 3412920 bytes (22.04 %) | 2809 / 12580 | 325 linked / 1001 | Game code: 8 units split (7 match, 6 linked): CAbstractInputEventStage base and sequences, input sequence set, build information (date "Aug 28 2008 (14:30)", build 519 / 142835), two small helpers; 21.04 % of code linked |
| 2026-10-15 | 9c1271c | 758544 / 3412920 bytes (22.23 %) | 2912 / 12580 | 346 linked / 1044 | Game code: 30 units split, 28 linked: the Entity class family (Entity_Game, Entity_Static, Entity_Dynamic, Entity_Trigger_Script, Entity_ArmWrestleBolt, ExWorldTrackItem, Entity_AstralTile, Entity_BrazierBowl, Entity_SpiritToken, Entity_Hazard_Prop), callbacks, `book` / `section`, small helpers; 21.23 % of code linked |
| 2026-10-15 | 223e3bf | 758980 / 3412920 bytes (22.24 %) | 2919 / 12580 | 347 linked / 1045 | Game code: 32 units split, 29 linked (Entity_Challenge_Projector 5/6 not linked; hash index table linked) |
| 2026-10-15 | 9689ba7 | 762404 / 3412920 bytes (22.34 %) | 2958 / 12580 | 350 linked / 1053 | Game code: 36 units split, 32 linked (added Entity_MysticBrewVortex, Entity_BrainTossTarget, Entity_ArmWrestleHand; Entity_BrainTossHand 7/8 not linked) |
| 2026-10-15 | 235558f | 764616 / 3412920 bytes (22.40 %) | 2978 / 12580 | 354 linked / 1062 | Callback family (NetworkInput / DebugMenu action selection, PlayerInput gesture), Entity_ReactorLeakHammer; motion recognizer classes identified from RTTI |
| 2026-10-15 | 2804227 | 768500 / 3412920 bytes (22.52 %) | 3019 / 12580 | 362 linked / 1071 | Camera influences: CIStitching, CIFixedTrackPlayer, CIDist, CIPitch, CIFixed_DJ1, CIFixedPlayerTracking_DJ1, CIFixedEyeLook, CIWaypointPath registration linked; CITrackTarget 5/6; 44 of 49 game units linked |
| 2026-10-15 | 713ccd7 | 769412 / 3412920 bytes (22.54 %) | 3024 / 12580 | 363 linked / 1070 | CIDynamic (base of the camera influences) linked, 21.52 % of code linked; its member type is `SphereGraph` (RTTI); 45 of 50 game units linked |
| 2026-10-15 | bd17980 | 770524 / 3412920 bytes (22.58 %) | 3037 / 12580 | 363 linked / 1062 | Camera units split: SphereGraph 4/5, CIDirectionGuide 4/5 (98.4 % vf0C), CIFree (vf24), CITrackTargetDan (small functions); 54 game units split, 45 linked |
| 2026-10-15 | 230d2ce | 783364 / 3412920 bytes (22.95 %) | 3233 / 12580 | 377 linked / 1087 | Game code: 75 units split, 59 linked; smallest units first: register set accessors (35 functions), state stack, text entry, door registry, manager wrappers, InventoryBook, game mode helpers; 6 more NonMatching units |
| 2026-10-16 | fd59fc1 | 805140 / 3412920 bytes (23.59 %) | 3477 / 12580 | 388 linked / 1102 | Game code: 97 units split, 70 linked (SewerItem, delayed event callbacks, CIPlayerOffset, online errors, button sequence, script helpers, tree destroy, QuestLog, two sinit-only units kept by FORCEFILES); 22.14 % of code linked |

Notes:

- "Units" counts units in `objdiff.json`; library units are the ones split by hand in
  `config/RM7P4F/splits.txt` (progress category `sdk`, 634340 bytes of code); the rest
  are dtk `auto_*` objects.
- Game code: see the unit table in `notes/SPLITS.md` and the NonMatching list in
  `notes/STATUS.md`.
