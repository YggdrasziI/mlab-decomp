# Provenance of reused code

Code taken from other decompilation projects, with its origin, license and the changes
made for this game. Every reused unit was checked against this DOL: objdiff reports
100 % code and data, and the full build still matches (`main.dol: OK`).

## Runtime.PPCEABI.H (9 units, Matching)

Source: [yannicksuter/mscharged-decomp](https://github.com/yannicksuter/mscharged-decomp)
(Mario Strikers Charged, Wii), commit `126acd42cb8ca0eea1efa75c5a8da7110ef9a142`,
license **CC0-1.0**. Fetched on 2026-10-12.

| Our file | Their file | Changes |
|---|---|---|
| `src/Runtime.PPCEABI.H/__mem.c` | `src/Runtime/__mem.c` | none |
| `src/Runtime.PPCEABI.H/__va_arg.c` | `src/Runtime/__va_arg.c` | none |
| `src/Runtime.PPCEABI.H/global_destructor_chain.c` | `src/Runtime/global_destructor_chain.c` | none |
| `src/Runtime.PPCEABI.H/NMWException.cpp` | `src/Runtime/NMWException.cpp` | none (built with `-Cpp_exceptions on`, as in mscharged) |
| `src/Runtime.PPCEABI.H/ptmf.c` | `src/Runtime/ptmf.c` | none |
| `src/Runtime.PPCEABI.H/runtime.c` | `src/Runtime/runtime.c` | 3 functions added, see below |
| `src/Runtime.PPCEABI.H/__init_cpp_exceptions.cpp` | `src/Runtime/__init_cpp_exceptions.cpp` | none |
| `src/Runtime.PPCEABI.H/Gecko_ExceptionPPC.cpp` | `src/Runtime/Gecko_ExceptionPPC.cpp` | none |
| `src/Runtime.PPCEABI.H/GCN_mem_alloc.c` | `src/Runtime/GCN_mem_alloc.c` | none |

Headers copied unchanged from the same commit: `include/decomp.h`,
`libs/Runtime/include/*` (6 files), `libs/MSL_C/include/{cstring,size_t.h,stdarg.h}`,
`libs/RVL_SDK/include/revolution/types.h`,
`libs/RVL_SDK/include/revolution/os/{OSAlloc,OSArena,OSContext,OSError,OSError_fwd}.h`.

### `runtime.c` additions

This game keeps three helpers that the mscharged source does not define (each game's
linker strips different unused helpers):

- `__shr2u` and `__cvt_dbl_usll`: from [zeldaret/tp](https://github.com/zeldaret/tp)
  `libs/PowerPC_EABI_Support/Runtime/Src/runtime.c`, commit
  `c8fa8c9e2aab72cf4e5db0e5d1c84a9ea6ee6eb0`, license **CC0-1.0**. The `#ifdef
  __MWERKS__` / `nofralloc` wrapper was dropped to match the style of the file.
- `__cvt_sll_dbl`: **transcribed from this game's DOL** (0x8032F9E8), not taken from
  another project.

The mscharged source also defines `__cvt_ull_dbl`, absent from this DOL; it is left in
the source and dead-stripped by the linker (the build matches).

## Symbols renamed for these units (`config/RM7P4F/symbols.txt`)

- Runtime functions named after the reference projects, each confirmed by a 100 %
  objdiff match: `__construct_new_array`, `__dt__26__partial_array_destructorFv`,
  `__construct_array`, `__destroy_arr`, `__destroy_new_array`, `__ptmf_scall`,
  `__cvt_fp2unsigned`, `__div2i`, `__mod2i`, `__shl2i`, `__shr2u`, `__cvt_sll_dbl`,
  `__cvt_ull_flt`, `__cvt_dbl_usll`, `__cvt_dbl_ull`, `__sys_alloc`, `__sys_free`.
- Symbols referenced by this code, identified by position: in each matching function,
  the calls and data accesses of the compiled object and of the original are at the same
  offsets, so each external name maps to exactly one address:
  `__dl__FPv` (`operator delete`, 0x801D426C, game code), `__dla__FPv`
  (`operator delete[]`, 0x801D4340, game code), `OSInitAlloc`, `OSCreateHeap`,
  `OSSetCurrentHeap`, `OSAllocFromHeap`, `OSFreeToHeap`, `OSSetArenaLo`,
  `__OSCurrHeap`, and `@stringBase0` (0x80352220, `GCN_mem_alloc.c` string pool).

## MSL_C (59 of 59 units Matching)

Source: [yannicksuter/mscharged-decomp](https://github.com/yannicksuter/mscharged-decomp),
same commit `126acd42cb8ca0eea1efa75c5a8da7110ef9a142`, license **CC0-1.0**: the 50
`src/MSL/*.c` files that exist for our units, copied unchanged to `src/MSL_C/`, plus the
headers they need (`libs/MSL_C/include/**`, more of `libs/RVL_SDK/include/revolution/**`,
`libs/MetroTRK/include/MetroTRK/trkenum.h`, `include/types.h`). Library flags and
per-file flags are the ones mscharged uses (`cflags_msl` = base flags + `-inline auto
-ipa file`; per-file `-Cpp_exceptions on`, `-use_lmw_stmw on`, `-rostr`,
`-str reuse,pool,readonly`, `-D_IEEE_LIBM`; `math_sun.c` with the runtime flags).

The units linked are those marked `Matching` in `configure.py` (all their sections
are byte-identical and the build matches).

Local changes to the mscharged sources (all marked in the code):

- functions absent from this DOL are disabled with `#if 0` (the original link
  dead-stripped them; ours keeps them because of their exception tables):
  `__get_char` and `fgetc` (`char_io.c`), `rewind` (`FILE_POS.c`), `mbstowcs`
  (`mbstring.c`), `__fpclassifyf` (`math_api.c`), `strcspn` (`string.c`), `fwrite`
  (`direct_io.c`).

Compiler: `ansi_fp.c`, `strtoul.c`, `e_pow.c`, `e_rem_pio2.c`, `k_tan.c`, `s_atan.c`,
`e_asin.c` are built with `GC/3.0a5` (they only match with a compiler older than 3.0a5.2), the
other files with `GC/3.0a5.2`; see `notes/COMPILER.md`.

`uart_console_io_gcn.c`: `__close_console` added from zeldaret/tp (CC0-1.0), and the
8-byte function at 0x8033B094 (`li r3, 0; blr`, called by `fn_80333B34`) written for
this project under its address-based name `fn_8033B094` (real name unknown). External
names mapped by position: `InitializeUART` (0x80280414), `WriteUARTN` (0x8028045C),
`__TRK_write_console` (0x80344024, MetroTRK).

`arith.c`: the third function (0x80332EF0, a floored signed remainder called by the
date code) has no source in any reference; it was **decompiled from this DOL** for this
project, under its address-based name `fn_80332EF0`, and is byte-identical.

Additions from zeldaret/tp (CC0-1.0), same commit as above: `labs` in `arith.c`,
`strnicmp` in `extras.c` (both byte-identical). In `wstring.c`, `wcscpy` and `wcschr`
are disabled with `#if 0` (absent from this DOL), and 0x8033AE98 is `wcslen` (size
alignment had confused it with `wcscpy`, also 0x1C bytes).

Split fix: `printf.c` gets its data, which the region map had left unassigned:
`.rodata` 0x80352890-0x803528C0 (`@stringBase0`, 0x2B), `.data` 0x803B4E18-0x803B5068
(its switch jump tables, 0x250), `.sdata` 0x805FAE30-0x805FAE38 (`@wstringBase0`),
`.sdata2` 0x80600B88-0x80600B90; sizes equal the compiled sections.

Split fix: `string.c` also owns `.sdata2` 0x80600B90-0x80600B98, the empty string that
`strtok_null` / `strtok_ptr` point to (no code references it, so the region map missed
it).

`alloc.c`: two functions have no source in any reference project (they were
dead-stripped from those games) and were decompiled from this DOL (own code, no copy):

- 0x80330F50 is **`realloc`**: [DarkRTA/rb3](https://github.com/DarkRTA/rb3) (Rock Band 3,
  Wii, `config/SZBE69/symbols.txt`) has the same MSL layout (`deallocate_from_fixed_pools`
  0x1EC, an unnamed 0x260 function, `malloc` 0xA0, `free` 0x130, `realloc` 0x318).
  Only the **name** is taken from rb3. Written as an inline `__pool_realloc` (name is
  ours, not proven) on top of the file's `__pool_alloc` / `__pool_free`, like `malloc`.
- 0x80330B20 (0x260): unnamed in rb3 too; resizes a block in place (merges the next free
  sub-block, splits off the tail) and returns whether the new size fits. Keeps its
  address-based name. It must sit before `__pool_alloc` in the file: the `.text` and
  `extab` order of the DOL put it before `malloc`.

### Units with no mscharged source

- `e_asin.c`: from [SMGCommunity/Petari](https://github.com/SMGCommunity/Petari)
  `src/MSL_C/MSL_Common_Embedded/Math/Double_precision/e_asin.c`, commit
  `6804bd31724eaa01b33b68c622fcda6d1b433e17`, **CC0-1.0**, include line adapted; built
  with `GC/3.0a5` (TP's version and 3.0a5.2 do not match).
- `signal.c`, `e_fmod.c`, `e_log.c`, `e_log10.c`: from
  [zeldaret/tp](https://github.com/zeldaret/tp) `libs/PowerPC_EABI_Support/MSL/...`,
  commit `c8fa8c9e2aab72cf4e5db0e5d1c84a9ea6ee6eb0`, **CC0-1.0**. Only the include lines
  were adapted to this project's headers (`<internal/fdlibm.h>`, plus `<errno.h>`,
  `<internal/math_nan.h>`, `<stdlib.h>` where needed); `signal.c` uses this project's
  `sig_func` type name instead of `__signal_func_ptr`. All match.
- `w_asin.c`, `w_fmod.c`, `w_log.c`, `w_log10.c`: written for this project from the
  fdlibm wrapper pattern of mscharged's `w_acos.c` (built with `-D_IEEE_LIBM`, like the
  other wrappers). All match.

### Symbols renamed for MSL_C

- 105 functions renamed after the mscharged names; each one is **byte-identical** to the
  compiled source (relocated fields masked) after size alignment inside its unit. One
  name was skipped because it would clash (`parse_format` of `scanf.c` vs `printf.c`).
- External names used by the matching code, mapped by reference position in
  byte-identical functions or in the `__files` data table: `__dec2num`,
  `__ieee754_pow`, `__ieee754_rem_pio2`, `__kernel_tan`, `atan`, `raise`,
  `__open_file`, `__close_file`, `__read_file`, `__write_file`, `__position_file`,
  `__read_console` (these six are in MetroTRK's `mslsupp.c`), `__close_console`
  (0x8033B08C; the references aligned it by size with the next function, the `__files`
  pointers prove it), `__double_huge`, `__float_huge`, `__float_nan`,
  `_current_locale` (0x803B4DD0), `__lconv`, `errno` (0x805FC580), `__aborting`,
  `stdin_buff`, `stdout_buff`, `stderr_buff`, `__strtoull` (0x8033A4D4, called by
  `scanf.c`), `__ieee754_fmod`, `__ieee754_log`, `__ieee754_log10`, `asin`, `fmod`,
  `log`, `log10` (byte-identical), `__ieee754_asin` (0x8033B5D8, branch target of
  `asin`).

## MetroTRK (29 of 29 units Matching)

Source: [yannicksuter/mscharged-decomp](https://github.com/yannicksuter/mscharged-decomp),
commit `126acd42cb8ca0eea1efa75c5a8da7110ef9a142`, **CC0-1.0**: the 29 `src/MetroTRK/*`
files (28 C files and `__exception.s`) copied to `src/MetroTRK/`, plus the headers they
need. Library compiler `GC/2.7` and flags as in mscharged (`cflags_metrotrk`: base flags
without `-enc SJIS`, with `-fp_contract on -str reuse,readonly -use_lmw_stmw on
-inline on,deferred -func_align 4 -sdata 0 -sdata2 0`; `cflags_metrotrk_deferred_auto`
for 9 files; `-sdata 8` for `cc_gdev.c` and `serpoll.c`).

Every function present in this DOL is byte-identical. `InitMetroTRK` first looked
4 bytes shorter: dtk had cut its final `blr` into a gap; its size is corrected to 0x98 in
`symbols.txt`.
The sources also define functions absent from this DOL (`TRKStandardACK`,
`TRKSendACK`, `TRKReadBuffer1_*`, `GetThreadInfo`, …): they are inlined where used and
their stand-alone copies are dead-stripped by the linker, so the sources are kept as
they are. One change: `__close_console` in `mslsupp.c` is disabled with `#if 0`, since
this DOL's `__close_console` is the MSL_C one (0x8033B08C).

Symbols: 85 functions renamed after byte-identical matches, and by call position:
`TRK_memset` (0x8000446C), `TRK_memcpy` (0x8000449C), `__TRK_reset` (0x800063F4), and
`DBInitComm`, `DBInitInterrupts`, `DBQueryData`, `DBRead`, `DBWrite`, `DBOpen`,
`DBClose` (0x80345110-0x80345388). The latter identify the unknown block after
MetroTRK as the debugger communication driver (`DebuggerDriver.c`-style API), whose
function sizes differ from Petari's and TP's versions.

## RVL_SDK (165 of 179 units Matching so far)

Source: [yannicksuter/mscharged-decomp](https://github.com/yannicksuter/mscharged-decomp),
commit `126acd42cb8ca0eea1efa75c5a8da7110ef9a142`, **CC0-1.0**: 172 `src/RVL_SDK/**`
files matching our unit paths (5 of our units have no mscharged source), plus the
headers they need. Compilers and flags per object as in mscharged: `GC/3.0a5.2` with
`cflags_rvl_sdk` (base + `-inline auto -ipa file`) by default, the Broadcom BTE stack
with `GC/3.0a5` and `cflags_rvl_bte`, plus `cflags_rvl_spy` (GameSpy), `_wud`, `_wpad`,
`_dwc` where mscharged uses them. Sources are unchanged.

Function check: 121 of the 172 units are byte-identical function by function (the
others have version differences in some functions). 1492 functions were renamed after
byte-identical matches, plus 111 data/function names mapped by reference position.

Data placement (new method): for each byte-identical function, every relocation of
the compiled object (symbol + addend) is paired with the reference of the original
instruction at the same offset; this gives the DOL address where each data section of
the unit starts. A section is accepted only if all its references agree **and** its
compiled bytes equal the DOL bytes there (relocated fields masked; `.bss`/`.sbss`
are located by references only). 97 sections were byte-verified and added, 10
rejected. A few earlier region-map ranges were found too short and were extended to
the next unit (`ipcclt.c` `.sbss` to include its `initialized` static, `OSContext.c`
`.data` padding).

86 units whose compiled sections are all covered are linked; the build
matches.

More units linked afterwards:

- `rfc_port_if.c`: its `.bss` placed by link order (the gap between the previous and
  next units' `.bss` has exactly the compiled size, and no unit in between can own it).
- `GXAttr.c`, `GXTransform.c`, `bta_hh_api.c`, `gap_conn.c`, `l2c_utils.c`, `sdp_db.c`,
  `bte_logmsg.c`: their split data ranges are *smaller* than the compiled sections,
  because their sources contain functions that this DOL dead-stripped together with
  their data (jump tables, statics). Linked one by one, each keeps `main.dol` matching:
  our linker strips the same data. (`bta_dm_act.c`, `btm_inq.c`, `hcisu_h2.c` failed this
  test and stay NonMatching.)
- `scapi_prdinfo.c`, `bd.c`, `utl.c`, `gap_api.c`, `port_utils.c`: compiled sections
  that the split lacks belong to data the DOL dead-stripped; each linked alone keeps
  `main.dol` matching.

**Naming caveat.** Names come from ordered size alignment plus a byte comparison. When
several tiny functions have identical bytes (e.g. `blr` stubs), only their order tells
them apart, which fails if the source has dead-stripped functions of the same size in
between. This happened in `gki_ppc.c`: 0x80288288/0x8028828C/0x80288290 are
`GKI_run`, `GKI_sched_lock`, `GKI_sched_unlock` (proven by `BTA_Init` calling
0x80288288 where the compiled code calls `GKI_run`); the first naming was shifted.
Other units with same-byte functions were checked: in mutex_TRK, targsupp, UDP_Stubs,
cc_gdev and extras the function lists are identical to the source, so the order is
certain.

### Unit-by-unit data review (22 byte-identical units that failed to link)

Each unit was linked alone and the failure analysed (link error or first differing
DOL range). Fixes, all verified by `main.dol: OK`:

- **Version banners.** The `<< RVL_SDK - X \trelease build: ... >>` strings in
  mscharged carry the build dates of its own SDK. They were replaced by the dates found
  in this DOL (EXI, SI, VI, GX, DVD, NAND, OS, SC, WPAD); this is the only change to
  those lines.
- **Dead-stripped data.** The DOL lacks the strings, statics and objects used only by
  functions that its link stripped; the linker compacts what remains, so the split
  range is the DOL range, shorter than the compiled section (`SIBios.c`, `dvdfs.c`,
  `NANDCheck.c`, `gpiUnique.c` `.data`; `gstats.c` `.bss` without `challenge$1040`;
  `Pad.c` keeps only `__PADSpec`).
- **Wrong guessed symbol sizes** (dtk merged neighbouring objects, so ranges ended
  inside a symbol): `IdleContext`/`IdleThread` (`OSThread.c`), `lbl_8039F888`
  (`dsp_task.c`), `lbl_805C8000` (`gt2Utility.c`), corrected from the compiled layout.
- **Alignment.** `hcisu_h2.c` `.bss` is 0x24 bytes; the next unit (`uusb_ppc.c`, still
  auto-split) needs the 32-byte alignment of its compiled `.bss`, set with `align:32`
  on its first object.
- **Names by reference position** (byte-identical code): 42 BTE/GameSpy names, the DSP
  mailbox functions and task globals, AX command-list functions and globals,
  `DVDCancel`, `StatsServerHostname`, `gcd_gamename`, `gcd_secret_key`, and data
  defined twice once named (`bd_addr_null` in `bd.c` `.sbss2`, `BT_BD_ANY` in
  `btu_init.c` `.sdata2`).
- **Wrong split / wrong name found.** `AXCL.c` `.text` was a 0x30-byte fragment; the
  unit is 0x802589E0-0x80259458 (same order and sizes as the compiled code). The name
  `gpiProcessRegisterUniqueNick` was on the wrong twin: 0x8030CDB0 compares `\rn\`
  (the source's string), 0x8030CEDC compares `\rc\`.

Functions **written from this DOL** (absent from the mscharged sources, real names
unknown, marked in the code):

- `fn_8026829C` in `NANDCheck.c`: free blocks/files for the application (calls the
  static `nandCalcUsage`, so it belongs to this unit).
- `fn_80259414` in `AXCL.c`: empty function called by the AX shutdown function where AX
  init calls `__AXClInit`.
- `fn_8030CEDC` in `gpiUnique.c`: `\rc\` (CD key registration) reply handler, twin of
  `gpiProcessRegisterUniqueNick`.

rb3's `NANDCheck.c` was checked and does not contain `fn_8026829C`; no rb3 code was
used.

### Second pass: units whose code differed from mscharged

Method: list, per unit, the DOL functions whose code differs from the compiled
mscharged source; try the same function from zeldaret/tp
(`libs/revolution/src`, commit `c8fa8c9e`), SMGCommunity/Petari (commit
`6804bd31`) and DarkRTA/rb3 (commit `b9dfa78b`) in a temporary copy of the unit
(a few TP type names provided only for the trial build); keep a version only if
it compiles byte-identical. Every reuse is marked by a comment in the code.

Functions taken as they are (absent from mscharged or different there):

| Unit | Function | Source |
|---|---|---|
| `os/OSMutex.c` | `OSWaitCond` | TP (assertions dropped) |
| `os/OSReboot.c` | whole file (`__OSNextPartitionType` update) | Petari |
| `os/OSReset.c` | `OSResetSystem`, `OSReturnToMenu` | Petari |
| `os/OSReset.c` | `__OSShutdownDevices` | TP |
| `os/OS.c` | `OSGetAppGamename` | Petari |
| `gx/GXTexture.c` | `GXInitTexObjFilter` | TP (register macro adapted) |
| `ipc/ipcMain.c` | `IPCReInit` | TP |
| `nand/nand.c` | `NANDPrivateGetStatus` | TP |
| `nand/NANDSecret.c` | `NANDSecretGetUsage` (new unit) | rb3 (no license file, allowed by the owner) |
| `dvd/dvdFatal.c` | message tables and `__DVDShowFatalMessage` | Petari (RMGK01 branch dropped) |
| `dvd/dvd.c` | `stateCoverClosed_CMD` | TP |
| `vi/vi.c` | `getTiming` | TP |

Small edits made to match this SDK build, each one also found in the references:
`vi3in1.c` (overdrive level `VI_ODV_L1`), `OSFatal.c` (MEM1-end fallback when there
is no FST), `OSNet.c` (`__OSInIPL` test, `NWC24iSynchronizeRtcCounter(FALSE)`),
`GXInit.c` (`__GXSetGenMode` call), `fs.c` (`__fsInitialized`, file-stats callback,
`ISFS_GetFileStats` through an IPC block), `nand.c` (`nandMove` buffers initialized
with `""`). `sc.h` declares `SCGetProductGameRegion` as returning `s8`
(the DOL sign-extends its result).

GameSpy (no reference project has it): `gt2Socket.c` (bind error check inside the
protocol test), `sb_server.c` (server keys filtered by the qr2 function at
`0x803156EC`, real name unknown), receive buffer size 4096 (`gt2Main.h`).

### Third pass: remaining units, smallest first

- **`ax/AXVPB.c` (split redone) and `ax/AXProf.c` (new unit).** The old `AXVPB.c` split
  (0x8025B3F4-0x8025B434) held only `__AXGetCurrentProfile` (0x40, byte-identical to
  mscharged `AXProf.c`, which has no other function). `AXVPB.c` is 0x8025A0B0-0x8025B3F4:
  its functions follow the mscharged order and sizes, and its `.data`/`.bss`/`.sbss`/
  `.sdata2` ranges come from the references of that code. `AXProf.c` is copied
  unchanged from mscharged. Four functions of this newer AX build are missing from
  mscharged `AXVPB.c` and were added (own code; names from TP
  `libs/dolphin/src/ax/AXVPB.c`, commit `c8fa8c9e`, CC0):
  `AXSetVoiceItdOn` (same stores as TP), `AXGetMaxDspCycles` / `AXGetDspCycles`
  (return `__AXMaxDspCycles` / `__AXRecDspCycles`, identified by the `.sbss` layout),
  `__AXVPBQuit` (same position as in TP, between `__AXVPBInitCommon` and
  `AXSetVoiceSrcType`; the Wii version clears `__AXPB`, `__AXITD`, `__AXVPB` and
  `__AXMaxVoices`). `__AXGetStackHead`, `__AXPushFreeStack`, `__AXPushCallbackStack`
  and `__AXDepopVoice` (other AX units, not split yet) were named by pairing the
  relocations of the byte-identical `AXVPB.c` code.

- **`os/__ppc_eabi_init.cpp` (`.text`) and `os/init/__ppc_eabi_init.cpp` (`.init`, new
  unit).** No mscharged source. Taken from
  [SMGCommunity/Petari](https://github.com/SMGCommunity/Petari) (CC0), commit `6804bd31`:
  `src/RVL_SDK/os/__ppc_eabi_init.c` (only an `#include <revolution/base/PPCArch.h>`
  added; our file keeps the `.cpp` unit name) and `src/RVL_SDK/os/init/__ppc_eabi_init.cpp`
  (unchanged). Petari splits the file the same way (`.init` 0x800042E0-0x80004338 for
  `__init_hardware` / `__flush_cache`, `.text` 0xB4 for `__init_user`, `__init_cpp`,
  `exit`).

- **`dvd/dvdDeviceError.c`.** No mscharged source. Taken from Petari (CC0), commit
  `6804bd31`, `src/RVL_SDK/dvd/dvdDeviceError.c`; one change: `iosRev.major` becomes
  `iosRev.idLo` (our `OSIOSRev` names the byte at offset 1 `idLo`, as `OSNet.c` uses it).
  Data ranges added to the split (`.data` messages, `.bss` `CheckBuffer`, `.sdata`
  `lowDone` and the colour constant, `.sbss` `lowIntType`), each one bounded by the
  neighbouring units.

- **`sc/scapi.c`.** This SC build has one more config item than the mscharged
  source: `"BT.CDIF"` (DOL 0x3b3688, between `BT.DINF` and `BT.SENS`), so every later
  item ID is one higher. Added `SC_ITEM_BT_CDIF` to the enum in `sc.h`, the
  `"BT.CDIF"` entry in the `scsystem.c` name table, and `SCGetBtCmpDevInfoArray` /
  `SCSetBtCmpDevInfoArray` (Petari, CC0, commit `6804bd31`, `src/RVL_SDK/sc/scapi.c`;
  our header already declared them). Name fix: 0x8026AE54 was named
  `SCSetWpadMotorMode`; it writes item 0x1F (`BT.SPKV`), so it is
  `SCSetWpadSpeakerVolume`.

- **`nand/NANDLogging.c`.** No mscharged source. Taken from Petari (CC0), commit
  `6804bd31`, `src/RVL_SDK/nand/NANDLogging.c`, adapted to our headers only: `IOSFd` /
  `ISFSError` become `s32`, `ISFS_ERROR_OK` becomes `IPC_RESULT_OK`,
  `ISFS_ERROR_UNKNOWN` is written `-117` (its value in Petari's `fs.h`), `cal.mon` becomes
  `cal.month`, the `u8*` casts on ISFS buffers are dropped, C headers instead of
  `<cstdio>`/`<cstdlib>`/`<mem.h>`. New header `revolution/nand/NANDLogging.h`
  (`NANDLoggingCallback`, from Petari's `nandlogging.h`). Data ranges added to the
  split; `lbl_805FA2F0` renamed `s_fd` (size 4, the rest is padding).

- **`gamespy/GP/gpiUtility.c`.** `GPIConnection` (`gpi.h`) is 0x184 bytes larger in
  this GameSpy build: `errorCode` is at 0x5B8 instead of 0x434, and every later field
  used by `gpiInfo.c` is shifted by the same amount (0x450 -> 0x5D4 ... 0x474 -> 0x5F8),
  while offsets up to 0x2B4 are unchanged. A placeholder `unk_0x434[0x184]` was put
  just before `errorCode`; its real content, and whether it sits before or after
  `lastStatusString` / `lastLocationString`, is **not proven** (no linked code uses
  those fields). Names from byte-identical code and relocation pairing:
  `gpiCheckSocketConnect`, `gpiReadKeyAndValue`, `GSISocketSelect` (0x802FD51C),
  `Util_RandSeed`, `Util_RandInt`, `B64Encode`.

- **`wud/WUDHidHost.c`.** Newer WUD code, ported from Petari (CC0, commit `6804bd31`,
  `src/RVL_SDK/wud/WUDHidHost.c`) into our file with our names: the HID connection
  callback receives the `WUDDevInfo*` (typedef `WUDHidConnCallback` changed in both WUD
  headers, as in Petari's `wud.h`; `WPAD.c`'s `WPADiConnCallback` only adapted to the
  new signature, it does not match yet), the open-error path moves an
  authentication-failed device to the bottom of its list outside of a sync, and
  `BTA_HH_ADD_DEV_EVT` fills the three handle tables. Names from relocation pairing:
  `WUDHidHostCallback`, `WUDiRemoveDevice`, `WUD_DEBUGPrint`, `_wcb`, `_work`,
  `_dev_handle_to_bda`, `_dev_handle_queue_size`, `_dev_handle_notack_num`.

- **`os/OSPlayRecord.c`.** The only difference was the `.bss` layout: `Block` is at
  +0x13C (4-aligned) in the DOL, +0x140 in our build. `NANDCommandBlock` has one more
  field in this SDK, `BOOL simpleFlag` at 0xB8 (as in Petari's `nand.h`, CC0, commit
  `6804bd31`): the struct becomes 0xBC bytes, so MWCC no longer gives the object 8-byte
  alignment. No other unit changed (objdiff report compared before/after). This
  disproves the earlier guess that `FileInfo` and `Block` share an aggregate. Data
  ranges added to the split; `.bss` objects and `PlayRecordLastCloseTime` (one `s64`,
  dtk had two labels) named.

- **`os/OSPlayTime.c`.** No mscharged source. Taken from Petari (CC0, commit `6804bd31`,
  `src/RVL_SDK/os/OSPlayTime.c`; the DOL has `__OSExpireThread` like Petari, TP has
  `__OSExpireCallback` instead), adapted to our headers: `__OSActiveThreadQueue` declared
  with `AT_ADDRESS` as in `OSReset.c`, `thread->nextActive`, `__OSExpireAlarm.end`,
  `AIDMACallback`, `AIInitDMA(buffer, ...)` without the `u32` cast. Header additions
  from Petari: ES ticket types and five `ESP_*` prototypes in `esp/esp.h` (the local
  `ESTicketView` of `OSExec.c`, same 0xD8 size, removed), new `os/OSPlayTime.h`. Data
  ranges added to the split. 13 names from relocation pairing (the five static/inline
  helpers, `__OSHotResetForError`, `__OSSetVIForceDimming`, four `AI*` DMA functions,
  `ESP_GetConsumption`, `__OSExpireAIFade`).

- **`os/OSReset.c`.** Newer reset code ported from Petari (CC0, commit `6804bd31`,
  `src/RVL_SDK/os/OSReset.c`) into our file, with our field and constant names:
  `__OSGetDiscState` (the non-RMG variant), `OSRebootSystem`, `OSShutdownSystem`
  (sets `__OSIsReturnToIdle`), `OSRestart` (`__OSRelaunchTitle(resetCode)`),
  `__OSReturnToMenu`, `OSReturnToDataManager`, `OSReturnToSetting` (URL strings checked
  in the DOL), `__OSReturnToMenuForError`, `__OSHotResetForError`, and the obsolete
  `OSSetBootDol` (its code is dead-stripped, its string is in `.data`).
  `OSShutdownSystemForBS` is gone. Data ranges added to the split; names from
  relocation pairing and the matching code (`__OSRelaunchTitle`, `__OSReturnToMenul`,
  `__OSShutdownToSBY`, `bootThisDol`, `__OSIsReturnToIdle`). `.data` shows 99.5 % in
  objdiff only because of the section's end padding.

- **`dvd/dvderror.c`.** The DOL has the newer error log (ring of error records in NAND,
  13 functions). Whole file from Petari (CC0, commit `6804bd31`,
  `src/RVL_SDK/dvd/dvderror.c`), adapted to our header names: `DVDCBCallback` ->
  `DVDErrorCallback`, `DVDErrorInfo` fields `nextOffset` / `status` / `dateTime` ->
  `next` / `disr` / `sec`. `.bss` range added to the split; names from relocation
  pairing (the eight NAND callbacks, `DVDLowGetStatusRegister`, `NandInfo`, `NandCb`,
  `ExistFlag`, `NextOffset`, `__FirstErrorInfo`).

- **`gamespy/serverbrowsing/sb_queryengine.c`.** Two edits to the mscharged code:
  basic-query key values go through the same qr2 filter as in `sb_server.c`
  (`fn_803156EC`, name unknown) before `SBServerAddKeyValue`, and the receive buffer of
  `ProcessIncomingReplies` is 32-byte aligned (the DOL realigns the stack frame). Names
  from the matching code: `ParseSingleQR2Reply`, `ProcessIncomingReplies`, `recvfrom`
  (0x802FD36C), `SOHtoNl`.

- **`gx/GXFifo.c`.** `GXGetCPUFifo` has no `GXFlush()` call (Petari keeps it only for
  RMG), `GXGetGPFifo` copies the GP FIFO like `GXGetCPUFifo` does (prototype in
  `GXFifo.h` changed to `GXBool GXGetGPFifo(GXFifoObj*)`), and `GXGetFifoPtrs` was added
  (Petari, CC0, commit `6804bd31`). `.bss` range added to the split; FIFO objects and
  `.sbss` variables named from relocation pairing.
- **`bte/uusb_ppc.c` (not done).** The DOL has a smaller variant of the USB transport:
  `uusb_ReadIntrDataCB` (0x114), `uusb_ReadBulkDataCB` (0x19C), `uusb_issue_intr_read`
  (0xCC), `uusb_WriteCtrlDataCB` (0xEC), `uusb_WriteBulkDataCB` (0xE0) at
  0x80288B10-0x80289058 (roles from their calls; not renamed yet), no
  `__ntd_*`/trace functions, and shorter `UUSB_Register` / `UUSB_Write`. Neither
  mscharged nor Petari has this variant; needs hand decompilation.

- **`nand/NANDCore.c`.** Three changes to the mscharged code, each also in zeldaret/tp
  `libs/revolution/src/nand/NANDCore.c` (CC0, commit `c8fa8c9e`): the error map has the
  `MAXDEPTH` entry (-110 -> `NAND_RESULT_MAXDEPTH`, checked in the DOL table at
  0x8034A8F8), `nandConvertErrorCode` logs some errors with
  `NANDLoggingAddMessageAsync` (no callback in this build, 64-byte aligned buffer, no
  `__NANDPrintErrorMessage` call), and `NANDInit` uses `ESP_InitLib` / `ESP_GetTitleId`
  / `ESP_GetDataDir` / `ESP_CloseLib` (the local `/dev/es` helpers were removed;
  `ESP_GetDataDir` prototype added to `esp/esp.h`, from Petari). Data ranges added to the
  split. Names from byte-identical code: `NANDChangeDir`, `nandChangeDirCallback`,
  `NANDPrivateGetTypeAsync`, `nandGetTypeCallback` (guesses from call lists alone were
  wrong for two of them, so only byte pairing is used for names).

- **`usb/usb.c`.** Newer IUSB code, following Petari (CC0, commit `6804bd31`,
  `src/RVL_SDK/usb/usb.c`) in our file: request block with an isochronous callback and
  `xfer`/`spare` fields (clean list at 0x14, `nclean` at 0x34, as the DOL offsets show),
  `_intrBlkCtrlIsoCb` (name from its own log string; accepts `nclean` 4 and 2, resets
  it, calls the iso callback), `memset` of each request block after allocation,
  `USB_ERR` gated by `s_usb_err = 1`, IPC heap of 0x4000 bytes, and the two strings
  of dead-stripped functions that this file has (`packet %u too big`, insertion
  notification path error). Names from byte-identical code and relocation pairing.

- **`os/OS.c`.** Changes following Petari (CC0, commit `6804bd31`, `src/RVL_SDK/os/OS.c`):
  console type 0x300 -> 0x100 in `OSGetConsoleType` and its "RVA 1" report,
  `OSInit` clears the arenas only outside NAND boot / reboot, checks the device with
  `__DVDCheckDevice` for disc apps and calls `__OSInitPlayTime`; the arena pointers are
  read once into a temporary (as in Petari). Kernel build date "Jan 30 2008 01:38:43"
  and the `OS_ERROR` line numbers 1150 / 1168 come from this DOL. Data ranges added to
  the split (the `.data` jump table of `ReportOSInfo` was outside any unit, which made
  dtk treat the static function as external). `config.yml` gets two
  `block_relocations` (0x80233818, 0x80233890): `OSInit` builds the constant
  0x80004000 with `lis`/`addi`, and dtk turned it into a relocation to `__check_pad3`
  (same address).

- **`ipc/ipcclt.c`.** Reboot request handling of this SDK, after Petari (CC0, commit
  `6804bd31`, `src/RVL_SDK/ipc/ipcclt.c`): `IOS_IoctlvReboot` sets `__relnchFl` itself,
  keeps the request in `__relnchRpcSave` and a copy in the 32-byte aligned `__rpcBuf`;
  the send path no longer sets the flags; the reply handler compares with
  `__relnchRpcSave`; `IPCCltReInit` added (0x800-byte heap) while `IPCCltInit` now
  creates a 0x1000-byte heap. In this DOL the reboot path computes the queue
  difference with the `0xFFFFFFFF - b + a + 1` form (`subfic`) and sends with early
  returns, unlike the other callers; it uses two local copies of the queue/send
  inlines (`__ipcQueueRequestRb` / `__ipcSendRequestRb`, our names, no symbols).
  Data ranges added to the split; names from relocation pairing.

Names corrected on the way (earlier size-alignment errors, proven by code):
`gt2CloseAllConnectionsHard` (0x8030EE04), `ServerBrowserCount` (0x8031CE30).


- **`gamespy/gt2/gt2Message.c` (still NonMatching).** `gti2HandleMessage` (static,
  0x8031089C, formerly `fn_8031089C`; named from its place in the source order between
  `gti2HandleUnreliableMessage` and `gti2HandleConnectionReset`, and its 100 % match)
  is a newer version than the mscharged-decomp source, decompiled from this DOL: the
  magic-string app message advances `message` and `actualMessage` by
  `GTI2_MAGIC_STRING_LEN` (no `protocolOffset` reapplied), and an app unreliable
  message on a connection that is not yet `GTI2Connected` goes to
  `gti2UnrecognizedMessageCallback`. `gti2ReceiveMessages` is still 97 % (see the
  note in the source).

- **`sc/scsystem.c`.** The code already matched (mscharged-decomp source, plus the
  `"BT.CDIF"` entry above); the unit failed only because six static functions had
  address names and the split had no data. Renamed from the source order and exact
  sizes, each one then 100 %: `ReadCallbackFromReload` (0x80269414),
  `CloseCallbackFromReload` (0x802694FC), `FinishFromReload` (0x80269588),
  `ParseConfBuf` (0x802696C8), `SCFlushAsync` (0x8026A484), `MyNandCallback`
  (0x8026A6A0). Data ranges added from relocation pairing: `.rodata` path strings
  (0x8034AA40), `.data` name table and the `MyNandCallback` jump table (0x8039FC28),
  `.bss` `Control` / `ConfBuf` / `ConfBufForFlush` (0x80581080), `.sdata` from
  `__SCVersion` (0x805FA308), `.sbss` from `BgJobStatus` (0x805FC190); each one ends on
  the next unit's symbol.
