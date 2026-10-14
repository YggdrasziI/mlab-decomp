# Code and data regions

Region map of `main.dol` (RM7P4F), used to split SDK, runtime/libc and game code.
Every boundary below comes from `config/RM7P4F/symbols.txt` (names recognized by dtk),
`<< RVL_SDK - … >>` version strings, or cross-references computed from the
disassembly in `build/RM7P4F/asm`.

## Important: what the build does and does not prove

dtk splits the original bytes and the linker puts them back together, so
`main.dol: OK` holds for **any** consistent set of split boundaries. A build that
matches proves the bytes; it does **not** prove that a split matches a real translation
unit (TU). A TU boundary is only proven once the TU is decompiled and matches.

## `.text` regions

| Region | Start | End (excl.) | Evidence |
|---|---|---|---|
| `.init` (startup, `memcpy`, `memset`) | 0x80004000 | — | `__check_pad3`, `__start`, `__init_registers`, `memcpy` 0x80004338, `memset` 0x8000443C |
| Game (+ possible middleware) | `.text` start | 0x80232AF8 | see "Game / SDK boundary" |
| Wii SDK (RVL) and network libraries | 0x80232AF8 | 0x8032EEE4 | `PPCMfmsr` 0x80232AF8; version strings registered by `EXIInit`, `SIInit`, `GXInit`, `DVDInit`, `SCInit`, … up to SSL (`fn_8032E18C`) |
| Runtime.PPCEABI.H | 0x8032EEE4 | 0x8032FEF0 | see "Runtime.PPCEABI.H units" below |
| MSL_C (including its math library) | 0x8032FEF0 | 0x8033F460 | see "MSL_C units" below; the 43 unnamed functions after `fwide` are MSL math (`e_pow.c` … `w_sqrt.c`) |
| MetroTRK | 0x8033F460 | 0x80344FD4 | see "MetroTRK units" below |
| Debugger communication driver (EXI2) | 0x80344FD4 | 0x80345AD8 | 31 functions; MetroTRK calls `DBInitComm`, `DBInitInterrupts`, `DBQueryData`, `DBRead`, `DBWrite`, `DBOpen`, `DBClose` here (0x80345110-0x80345388), the `DebuggerDriver.c` API. Function sizes match neither Petari's `NDEV` nor TP's `NdevExi2A`; exact library variant unknown, not split yet |

`strlen` (0x8032EEE4, weak) belongs to the runtime's `__mem.c` (see below).

### Game / SDK boundary (0x80232AF8)

- 0x80232AF8 is `PPCMfmsr`, the first SDK function recognized by dtk; the previous
  function `fn_802326C0` is unnamed.
- Calls from code below 0x80232AF8 into the SDK region: 2368.
  Calls from the SDK region into code below 0x80232AF8 (excluding `.init`): 37, all to
  `fn_801D426C` (32), `fn_801D40E0` (3), `fn_801D4340` (1) and `fn_800063F4` (1).
  Most callers are in the HBM area (0x802C5xxx–0x802C7xxx). The first two look like
  the C++ allocation operators defined on the game side (hypothesis, not named).
- Whether some code below 0x80232AF8 is third-party middleware (the disc has `fmv`
  videos and `Sounds`) is **unknown**.

### SDK libraries located by their version string

| Library | Version string | Registered by |
|---|---|---|
| EXI | 0x803972B8 | `EXIInit` |
| SI | 0x80397300 | `SIInit` |
| VI | 0x80397420 | `fn_80244408` |
| GX | 0x80397F20 | `GXInit` |
| DVD | 0x80398798 | `DVDInit` |
| AI | 0x8039A0A0 | `fn_802577E4` |
| AX | 0x8039A0E8 | `fn_80257C34` |
| DSP | 0x8039F808 | `fn_80264FD8` |
| SC | 0x8039FC28 | `SCInit` |
| WPAD | 0x8039FEF0 | `fn_80271358` |
| KPAD | 0x803A1AF0 | `fn_8027FC30` |
| HBM | 0x803A9498 | referenced by `fn_802C8FC8`, `fn_802C9ABC`, `fn_802CA3A0` |
| DWC | 0x803AD138 | referenced by `fn_802E49B8` |
| NCD | 0x803B3DA8 | `fn_8032149C` |
| NWC24 | 0x803B3E98 | `fn_80322658` |
| SO | 0x803B3FA0 | `fn_80323644` |
| SOCKET | 0x803B4180 | `fn_80324778` |
| NHTTP | 0x803B4420 | `fn_8032BE48` |
| REX-PPC (RevoEX) | 0x803B4720 | `fn_8032D5B8` |
| SSL | 0x803B4920 | `fn_8032E18C` |

(OS and NAND strings are not referenced through a dtk symbol; not located yet.)

## Data regions

Each data object was classified by the region of the functions that reference it.
In every section the order is game → SDK → runtime → MSL ("unknown" column = MSL math)
→ TRK, which matches the link order seen in `.text`.

| Section | Game | SDK | Runtime | MSL | Unknown (after MSL) | TRK |
|---|---|---|---|---|---|---|
| `.rodata` | 0x80345FC0–0x80349FF8 | 0x8034A8A0–0x803521A4 | 0x80352208–0x80352290 | 0x80352290–0x803528F8 | 0x80352F30–0x803531B0 | 0x80353268–0x8035334C |
| `.data` | 0x80353460–0x80389498 | 0x803894F0–0x803B4968 | — | 0x803B4AB8–0x803B51CC | — | 0x803B5210–0x803B5328 |
| `.bss` | 0x803B5380–0x8055CAB0 | 0x8055CB60–0x805F2920 | 0x805F5940 | 0x805F5950–0x805F5CA8 | — | 0x805F5CD0–0x805F8170 |
| `.sdata` | 0x805F81A0–0x805FA0F8 | 0x805FA100–0x805FAE20 | `fragmentID` 0x805FAE28 | 0x805FAE38–0x805FAE50 | — | — |
| `.sbss` | 0x805FAE60–0x805FBBE0 | 0x805FBBE8–0x805FC56C | 0x805FC570 | 0x805FC578–0x805FC594 | 0x805FC588–0x805FC5A0 | 0x805FC5A8–0x805FC5B0 |
| `.sdata2` | 0x805FC5E0–0x80600654 | 0x80600658–0x80600B30 | — | 0x80600B38–0x80600BC8 | 0x80600BD0–0x806010D8 | — |
| `.sbss2` | 0x806010E0–0x80601104 | 0x80601110 | — | — | — | — |

Ranges are the first and last **referenced** object of each class; unreferenced
objects in between are not classified. Exceptions:

- `.data` 0x803982F0–0x80398458: a few objects referenced by both game and SDK code,
  interleaved inside the SDK range. Not explained yet.
- `.data` 0x803B4978 and 0x803B4DD0: referenced by several regions.
- `.sdata` 0x805FAE58 and `.sbss` 0x805FC5B8–0x805FC5C8: referenced by the unknown
  block after TRK.

## Runtime.PPCEABI.H units

TU names and order are **hypotheses taken from other decompilation projects** using
the same Metrowerks runtime, both CC0-1.0:
[SMGCommunity/Petari](https://github.com/SMGCommunity/Petari) (`config/RMGP01/splits.txt`)
and [yannicksuter/mscharged-decomp](https://github.com/yannicksuter/mscharged-decomp)
(`config/R4QE01/splits.txt`), read on 2026-10-12. Only split boundaries were used, no
code. Each unit was checked against this DOL by function sizes and order, and by the
data its functions reference:

| Unit | `.text` | Other sections | Check against this DOL |
|---|---|---|---|
| `__mem.c` | 0x8032EEE4–0x8032EF00 | `.init` 0x80004338–0x8000446C | `strlen` 0x1C; `memcpy`/`__fill_mem`/`memset` same layout as Petari |
| `__va_arg.c` | 0x8032EF00–0x8032EFC8 | — | `__va_arg` 0xC8 |
| `global_destructor_chain.c` | 0x8032EFC8–0x8032F028 | `.sbss` 0x805FC570–0x805FC578 | 0x18 + 0x48; `__global_destructor_chain` |
| `NMWException.cpp` | 0x8032F028–0x8032F3D8 | `extab` 0x800064E0–0x80006528, `extabindex` 0x80006780–0x800067BC | 5 functions, sizes identical to Petari (0x104, 0xBC, 0xF8, 0x78, 0x80); extab 0x48 / extabindex 0x3C as in Petari |
| `ptmf.c` | 0x8032F3D8–0x8032F400 | — | only one function (0x28 = `__ptmf_scall` size; it adjusts `this` and calls through `r12`). The other ptmf functions are absent (dead-stripped, hypothesis) |
| `runtime.c` | 0x8032F400–0x8032FCAC | `.rodata` 0x80352208–0x80352220 | first function 0x5C (`__cvt_fp2unsigned` size), `__save_fpr`…`__mod2u` named by dtk; `.rodata` 0x18 as in Petari. Two extra functions (0xB0, 0xA0) not in Petari |
| `__init_cpp_exceptions.cpp` | 0x8032FCAC–0x8032FD1C | `.ctors$10`, `.dtors$10`, `.dtors$15`, `.sdata` `fragmentID` | required by the GC 3.0a5.2 linker |
| `Gecko_ExceptionPPC.cpp` | 0x8032FD1C–0x8032FD80 | `.bss` 0x805F5940–0x805F5950 | `__register_fragment` + `__unregister_fragment` = 0x64; `fragmentinfo` |
| `GCN_mem_alloc.c` | 0x8032FD80–0x8032FEF0 | `.rodata` 0x80352220–0x80352290 | two 0xB8 functions (0x170 total, as in mscharged); `.rodata` 0x70 as in Petari; strings `GCN_Mem_Alloc.c` and "Metrowerks CW runtime library initializing default heap" in this DOL |

All units are `NonMatching` (nothing decompiled yet); the build still matches.

## MetroTRK units

Names, order and sections come from
[yannicksuter/mscharged-decomp](https://github.com/yannicksuter/mscharged-decomp)
`config/R4QE01/splits.txt` (CC0-1.0); Petari (`config/RMGP01`) and
[zeldaret/tp](https://github.com/zeldaret/tp) (`config/RZDP01`, CC0-1.0) have the same
unit list. Read on 2026-10-12; only split boundaries were used, no code.

Check against this DOL: function sizes were aligned with mscharged (sequence alignment
of size lists). 151 of 152 functions match in size and order, from `mainloop.c` to
`MWCriticalSection_gc.c`; every unit's `.text` size is identical to the reference,
except:

- `mslsupp.c`: 0x5BC instead of 0x640; the missing 0x84 is `__close_console`, absent
  here (dead-stripped, hypothesis);
- `InitMetroTRK`: dtk sized it 0x94 instead of 0x98; the missing 4 bytes are its final
  `blr` (0x80344230 = `4E800020`), left as a gap by the analysis. Corrected in
  `symbols.txt` (not a version difference).

Data was assigned from the objects each unit's functions reference, and every range
size was compared with the reference (e.g. `dispatch.c` `.data` 0x6C, `msghndlr.c` two
0x1C jump tables, `targimpl.c` `.rodata` 0x88, `cc_gdev.c` `.bss` 0x500 + 0x1C,
`dolphin_trk_glue.c` `.rodata` 0x108 starting at `EndofProgramInstruction`).
`.init` parts: `mem_TRK.c` (`TRK_memset`/`TRK_memcpy` sizes 0x30/0x24),
`__exception.s` (`gTRKInterruptVectorTable` … `gTRKInterruptVectorTableEnd`,
0x1F34 as in the reference), `dolphin_trk.c` (`fn_800063F4`, 0x2C).
Small differences: `targimpl.c` `.bss` is 0x580 here (0x57C in the reference), and a
few ranges end on dtk symbol sizes (padding included) where dtk sized a symbol larger
than the reference.

## MSL_C units

References (all CC0-1.0, split boundaries only, no code):
[zeldaret/tp](https://github.com/zeldaret/tp) `config/RZDP01` (primary: the most mature
project), [SMGCommunity/Petari](https://github.com/SMGCommunity/Petari) `config/RMGP01`
and [yannicksuter/mscharged-decomp](https://github.com/yannicksuter/mscharged-decomp)
`config/R4QE01`, read on 2026-10-12.

This MSL is a different version from each reference, so units were placed with a
**three-way view**: function sizes aligned against each reference separately, then each
of our functions attributed to the unit that at least one reference aligns to it.
Where alignment by size alone was ambiguous, the code itself was used:

- **`w_*` wrappers** (0x8033F1F4–0x8033F210, 4 bytes each) are single `b` instructions
  to their `e_*` function: `e_acos`, `e_asin`, `e_atan2`, `e_fmod`, `e_log`, `e_log10`,
  `e_pow` (sizes equal to the references). This corrected an earlier size-only split:
  0x8033F1F8 is `w_asin.c`, not `w_atan2.c`. `w_log.c` is named by the `w_<name>.c`
  convention; no reference has it (hypothesis).
- **`extras.c`**: the three 4-byte functions inside it (0x8033B1B8, 0x8033B1BC,
  0x8033B310) branch to `__msl_strnicmp` / `__msl_itoa`, so they are `strnicmp`-style
  and `itoa` aliases, not math wrappers (TP and Petari align them as `w_*` by size).
- **`arith.c`** starts at 0x80332ED0: both 0x10 functions there have the `abs` code
  (`srawi`/`xor`/`subf`), i.e. `abs` and `labs`, followed by `div`.
- **`alloc.c`** includes `fn_80330F50` (0x318): it uses the allocator pools and calls
  `allocate_from_fixed_pools` / `allocate_from_var_pools`.
- **`uart_console_io_gcn.c`** includes `fn_8033B08C`, which sits between two functions
  of that unit (units are contiguous).

Data: `extab`/`extabindex` ranges come from this DOL (each entry belongs to a function
of the unit). Other sections were kept when their size equals at least one reference,
e.g. `ansi_files.c` `.bss` 0x300 (three 0x100 buffers), `scanf.c` `.data` 0x120 (three
jump tables), `strtoul.c` `.data` 0x88, `signal.c` `.bss` 0x20,
`abort_exit_ppc_eabi.c` `.sbss` 0x8 (`__stdio_exit` included), `e_log.c` /
`e_log10.c` `.sbss` 0x8 each, `e_pow.c` `.rodata` 0x30 + `.sdata2` 0x110.

59 units are split, covering 0x8032FEF0–0x8033F460 except three gaps left as `auto_*`:

| Gap | Content | Why not split |
|---|---|---|
| 0x80333B34–0x80333B60 | one 0x2C function calling `__close_console` | no reference has it |
| 0x8033ABF0–0x8033AE98 | three functions doing date arithmetic, calling an OS function | time functions, not in any reference |
| 0x8033AFB4–0x8033AFBC | `li r3, 0; blr`, called by the date code | owner unknown |

Data not assigned yet (owner unclear, shared between units): `printf.c` data (the
references give `.rodata` 0x30, `.data` 0x250, `.sdata` 0x8, `.sdata2` 0x8), the 0x48
table at 0x803B4DD0 (used by `file_io.c`, `mbstring.c`, `scanf.c`, `strtoul.c`,
`extras.c`), `errno`-like 0x805FC580 (used by math and `strtoul.c`) and
0x805FAE48 / 0x805FAE4C.

## RVL_SDK units

References: TP (`config/RZDP01`, `revolution/*` and `homebuttonLib`), Petari
(`config/RMGP01`, `RVL_SDK/*`) and mscharged-decomp (`config/R4QE01`, `RVL_SDK/*`, the
only one with the network libraries DWC/GameSpy/NHTTP/SSL). All CC0-1.0; split
boundaries only, no code. Read on 2026-10-12.

Method (automated, scripts kept out of the repository):

1. Each of our 3634 functions in 0x80232AF8-0x8032EEE4 gets, per reference, the unit of
   the reference function with the **same name and size** (254 functions are named by
   dtk), otherwise the unit given by **size alignment**, counting only aligned runs of
   at least 3 functions. Priority on conflict: name before size, then TP > Petari >
   mscharged (5 functions disagreed). 2312 functions got a label.
2. Unlabeled functions are absorbed only when both neighbours carry the same unit.
3. A unit is kept only when it is **anchored**: its label appears in a single run, and
   the functions right before and after it are labeled with other units. Units touching
   an unlabeled gap could extend into it and were not split.
4. Data: objects referenced **only** by the unit's functions, forming a contiguous block
   with nothing else inside, kept only when the block size equals the same unit's
   section size in at least one reference. Two ranges were then removed because dtk
   reported a misaligned section start (`bte_main.c` `.sbss`, `vi3in1.c` `.sbss`): an
   unreferenced object probably belongs at their start, which is not proven.

5. Second pass: a unit adjacent to an unlabeled gap was also split when its `.text`
   size is **exactly** the size of the same unit in a reference (the unit is then
   complete and cannot extend into the gap). 23 units: `OSRtc.c`, `OSSync.c`,
   `OSIpc.c`, `OSPlayRecord.c`, `OSPlayTime.c`, `__ppc_eabi_init.cpp`, `GXInit.c`,
   `AXCL.c`, `AXVPB.c`, `AXFXHooks.c`, `dsp_task.c`, `NANDCheck.c`, `NANDLogging.c`,
   `scsystem.c`, `scapi_prdinfo.c`, `arc.c`, `euart.c`, `TPL.c`, `dwc_common.c`,
   `hashtable.c`, `gpiInfo.c`, `gpiUnique.c`, `gbucket.c`. `arc.c` and `TPL.c` are
   also confirmed by the source file name strings `arc.c` / `TPL.c` used by a function
   inside each range. Data ranges ending on an unaligned address were dropped (two in
   `TPL.c`), since the following split would start misaligned.
   Correction: the 0x40 block taken as `AXVPB.c` is in fact `AXProf.c`
   (`__AXGetCurrentProfile`); `AXVPB.c` is 0x8025A0B0-0x8025B3F4 (see
   `notes/PROVENANCE.md`, third pass). A size match alone is not enough.

Data check: the reference scan was rerun over every disassembly file (including the
per-unit subfolders created by earlier splits); all 162 data sections of the first pass
were confirmed.

Result: **177 units**, 0x82EEC of the 0xFC3EC bytes of SDK code (about 52 %), with 177
data sections. For 148 units the `.text` size is identical to the same unit in a
reference; the other 29 differ (version differences) but are anchored by their
neighbours. No overlap, no dtk warning, `main.dol` matches. Unit paths follow the
references under `RVL_SDK/` (e.g. `RVL_SDK/os/OSAlarm.c`,
`RVL_SDK/gamespy/serverbrowsing/sb_server.c`).

Source file name strings that locate one function of a unit not split yet (extent
unknown): `OSStateTM.c` (0x8023E080-0x8023E330), `OSLaunch.c` (0x8023FC84),
`dwci_nd.c` (0x80320B8C), `ncdsystem.c` (0x8032149C), `NHTTP_bgnend.c` (0x80325ED4),
`NHTTP_os_RVL.c` (0x8032683C), `d_nhttp.c` (0x8032C2B0).

Not split yet (`auto_*`, 0x4a83c bytes in 4 regions of the RVL_SDK range, recomputed 2026-10-14): mostly AX/AXFX, HBM and its
`nw4hbm` layout code, DWC, GameSpy and NCD/NWC24/SO/NHTTP/SSL, whose versions differ
from every reference. Units next to these regions whose size does not match a
reference (e.g. `OSStateTM.c`, `mtx.c`, `GXFrameBuf.c`, `ai.c`, `mem_expHeap.c`,
`HBMGUIManager.cpp`, `dwc_main.c`) stay unsplit.

0x802608AC-0x80265690 was inside the `mem_list.c` split until 2026-10-14 (the unit is
only 0x114 bytes). It starts with the MIX library, now split as `mix/mix.c`
(0x802608AC-0x80263A38) and `mix/remote.c` (0x80263A38-0x80263F18), both matching.
0x80263F18-0x80264F90 (calls `AXSetVoice*`) is not identified yet; the DSP functions
after it are split as `dsp.c`, `dsp_debug.c` and `dsp_task.c`.

| Range | Size |
|---|---|
| 0x8025D2E0-0x8025F934 | 0x2654 |
| 0x80263F18-0x80264F90 | 0x1078 |
| 0x802B3E88-0x802E5EEC | 0x32064 |
| 0x802E6298-0x802FB3A4 | 0x1510c |

## Splits in `splits.txt`

| Unit | Status |
|---|---|
| `RVL_SDK/*` (177 units) | Split, NonMatching (about 52 % of SDK code) |
| `Runtime.PPCEABI.H/*` (9 units) | Split, NonMatching |
| `MSL_C/*` (59 units) | Split, NonMatching (3 small gaps, see above) |
| `MetroTRK/*` (29 units) | Split, NonMatching |
| Everything else | dtk `auto_*` objects (gap-based, not TU-accurate) |
