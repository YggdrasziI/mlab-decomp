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

- **Address-named functions in the NonMatching SDK units (naming pass).** 35
  `fn_XXXXXXXX` / suffixed symbols renamed after the source function with the same
  size and the same place in the unit's order, kept only when the code then matches
  at 92 % or more (most at 99.9 %+, the rest being relocation names to unsplit data):
  vi (`OnShutdown`, `__VIRetraceHandler`, `VISetTimeToDimming`), dvd
  (`stateReadingFST`, `stateCheckID`, `cbForStateCheckID2`, `Callback`),
  dvd_broadway (12 `DVDLow*` functions), WPAD (`WPADiInitSub`), WPADHIDParser
  (`__a1_22_ack`, `__a1_33_data_type`, `__a1_36_data_type`), WUD
  (`EnableStackHandler`, `InitHandler`, `InitCore`, `WUDIsBusy`,
  `WUDSecurityCallback`, `WUDStoredLinkKeyCallback`), gpiInfo (6 functions).
  `OnShutdown` and `Callback` get `scope:local` (the names already exist elsewhere);
  dtk then suffixes such local names with their address, so objdiff still cannot pair
  them. Rejected (same size, but 11-20 % similar, so a different function):
  0x8023696C (`__OSBootDolSimple`), 0x80284578 (`WUDiAutoSync`), 0x80288B10
  (`uusb_issue_bulk_read`). With the `DVDLow*` names in place, dtk named ten
  dvd_broadway data symbols itself (`dvdContexts`, `diRegValCache`, `DiFD`,
  `requestInProgress`, `freeCommandBuf`, `freeDvdContext`, `spinUpValue`,
  `diCommand`, `@3872`); all of them exist in `dvd_broadway.c`.

- **`wud/WUD.c` (newer WUD, from Petari).** Petari (CC0, commit `6804bd31`,
  `src/RVL_SDK/wud/WUD.c`) matches this DOL's WUD function by function: same order and
  sizes for all 66 functions (Petari has one more, `WUDGetAllocatedMemSize`, 4 bytes,
  not in this DOL). Its body replaces ours, with our include block. Adaptations: the
  four SC flush callbacks take `SCStatus` (our `SCFlushAsync` type), the fiber stack
  arguments are cast to `void*`, `ASSERT` is defined as a no-op in the file (as in
  Petari's `os.h`; without it the calls compiled to `bl ASSERT`), and the
  `WUD_DEBUGPrint("start\n")` of `_WUDStartSyncDevice` is removed: that string is not
  in this DOL while every other literal of the file is. `WUDSecurityCallback` and
  `App_MEMfree` keep our versions (our BTE `tBTA_DM_AUTH_CMPL` has no `key_type`; this
  DOL's BTE is the older one). Headers: `WUDCB` gains `delayCnt`/`incomeCnt` at the end,
  `WUDDevInfo` field 0x5B is `sync_type` (was `UNK_0x5B`, also in `WUDHidHost.c`),
  `WUDShutdown` takes `BOOL exec`, and `sc.h` gets Petari's spellings as layout-identical
  aliases (`num`/`info[]` next to `numRegist`/`devices[]`, `SCBtDeviceInfoSingle`,
  `SCBtCmpDevInfoSingle`). `WPAD.c` passes `FALSE` to `WUDShutdown` for now (WPAD is
  not ported yet). Names: 17 WUD functions from the Petari alignment
  (`ShutFlushCallback`, `InitFlushCallback`, `WUDiIsSyncDisabled`, `WUDiSyncDone`,
  `SyncHandler`, `DeleteAllHandler`, `WUDiGetRegisteredDevice`, `ShutdownHandler`,
  `WUDShutdown`, `StartSyncDevice`, `WUDStartFastSyncSimple`, `WUDCancelSyncDevice`,
  `WUDiInitSub`, `WUDiRegisterDevice`, `WUDSearchCallback`,
  `WUDVendorSpecificCallback`, `WUDPowerManagerCallback`); data names from relocation
  pairing (`_scArray`, `_spArray`, `__WUDHandlerStack`, `_discResp`, `_scFlush` and the
  file's statics). `.bss` range added to the split (0x8058E140-0x805900E8).

- **`wpad/WPAD.c` (newer WPAD, after Petari).** This DOL's WPAD banner is
  "Dec 11 2007 01:35:07", the build Petari matches for RMGK01 (CC0, commit `6804bd31`,
  `src/RVL_SDK/wpad/WPAD.c`); function order and sizes follow it, except that this DOL
  also has `__wpadSyncCallback`, `WPADGetRadioSensitivity` and `WPADGetInfo` and lacks
  `WPADSetAutoSleepTime` and `WPADGetWorkMemorySize`. Our file keeps its own names
  (Petari's field and constant names differ too much for a mechanical copy); the newer
  behaviour was applied function by function from Petari: `WPADiSendSetReportType`
  takes `powerSave` (continuous-report byte 0 or 4; `WPADHIDParser.c` callers pass
  `pwrSave`), `WPADSetDataFormat` restructured, `__ClearControlBlock` clears `pwrSave`,
  `WPADiInitSub` registers the version before creating the alarm, `WPADInit` sets
  `_startup` and the reconnect counters, `OnShutdown` cancels a running sync first and
  handles return-to-menu with `__OSIsReturnToIdle`, `__WPADReconnect(exec)` only arms
  a delayed reconnect that `WPADiManageHandler` runs through `__reconnect` /
  `WPADiShutdown(exec)`, `WPADiRumbleMotor` checks the controller itself,
  `WPADiAfh` is called on `_afhCnt == 60000`, `firmwareCheckCallback` computes the port
  first, and `WPADiConnCallback` sets the device type from the device name
  ("Nintendo RVL-CNT") and clears `extEnc`. `_startup`/`_recCnt` are declared without
  initializer after `_initialized` (their `.sbss` place in this DOL); `_recFlag = -1`
  sits in `.sdata` after `__WPADVersion`. `WPADCB`'s last bytes are named `extEnc`,
  `pwrSave`, `padding2` (same size). Names: 8 functions from the Petari alignment; the
  five 4-byte stubs at 0x802715D8-0x802715E8 were misnamed (their branch targets show
  `WPADStartFastSimpleSync`, `WPADStopSimpleSync`, `WPADSetSimpleSyncCallback`,
  `WPADRegisterAllocator`, `WPADGetStatus`); `_wmb` (0x8058CBD0) from relocation
  pairing. Data ranges added to the split (`.data`, `.bss`, `.sbss`).

- **`wpad/WPADHIDParser.c` (from Petari).** Petari (CC0, commit `6804bd31`,
  `src/RVL_SDK/wpad/WPADHIDParser.c`, matching for all its versions) has the same 22
  functions in the same order and sizes as this DOL; our mscharged-based file was an
  older version. Petari's body is used with our include block and a short
  compatibility block at the top of the file: type aliases (`WPADControlBlock`,
  `WPADCmdQueue`, `WPADConfig`, `WPADMEMGameInfo`), Petari's data format names, its
  error codes as plain int macros (as enum constants, `(err < 0) ? -3 : 0` compiled
  with a branch instead of the DOL's `srawi`/`and`), `WPADExtensionCallback` as a
  pointer type, and `<math.h>` (without it `cos`/`sin` were implicit int functions).
  Control-block fields (`p_wpd->`, `p_cb->`) renamed to ours with the same table as
  for `WPAD.c`; Petari's flat `rxBuf + idx * WPAD_RX_DATASIZE` becomes
  `(u8*)rxBufs + ...`; callbacks take `WPADResult`; two `WPADiSend*` calls and one
  comparison cast `extensionCallback` to `WPADCallback`. Names: 13 functions from the
  Petari alignment, which also corrects 0x8027A048 to `__a1_34_data_type` (the naming
  pass had called it `__a1_36_data_type`, same size by chance; `__a1_36_data_type` is
  0x8027AAD4). The existing data ranges of the split fit the object exactly.

- **`bte/uusb_ppc.c` (from Petari).** Petari (CC0, commit `6804bd31`,
  `src/RVL_SDK/bte/uusb_ppc.c`, GC/3.0a5.2, matching for all its versions) has the
  same functions and sizes as this DOL after its first one (`__ntd_get_allocated_mem_size`,
  not in this DOL). Our older file (debug `OSReport` traces) is replaced by Petari's
  body after our include block, with Petari's empty `UUSBDBG`, its `unk*_t` macros,
  `fd` typed `IPCResult` (our IUSB API) and the fiber arguments cast to `void*`.
  Names from the alignment: `uusb_ReadIntrDataCB`, `uusb_ReadBulkDataCB`,
  `uusb_issue_intr_read`, `uusb_WriteCtrlDataCB`, `uusb_WriteBulkDataCB`. `.sdata`
  range added to the split (0x805FA4F8-0x805FA508: `wait4hci`, the file descriptor,
  `"oh0"`, `"oh1"`).

- **`dvd/dvd_broadway.c` (from Petari).** Petari (CC0, commit `6804bd31`,
  `src/RVL_SDK/dvd/dvd_broadway.c`, matching for all its versions) has the same 30
  functions in the same order and sizes. Petari's body is used after our include block
  with: `bool` replaced by `u8` (Petari's MSL `bool` is `unsigned char`; ours is `int`),
  `IOSFd`/`IOSError`/`IOSIoVector`/`ESTicket`/`ESTitleMeta` as typedefs of our types,
  and Petari's `IOS_ERROR_*` values as int macros. The file defines the DVDLow API with
  this SDK's signatures (`u8` results, new functions), so `dvd_broadway.h` now hides
  its older prototypes when `DVD_BROADWAY_NO_PROTOTYPES` is defined; only this file
  defines it, the other units still see the old prototypes (the build stays identical).
  Names from the alignment: `DVDLowFinalize`,
  `DVDLowOpenPartitionWithTmdAndTicketView`, `DVDLowGetNoDiscBufferSizes`,
  `DVDLowGetNoDiscOpenPartitionParams`, and 0x8025661C is `DVDLowInquiry` (the naming
  pass had `DVDLowWaitForCoverClose`: same size, 93.6 % similar). Lesson: two names of
  that pass were wrong at 92-94 % similarity (this one and `__a1_34_data_type`); only
  names confirmed by an exact alignment or a 100 % match should be trusted.

- **`vi/vi.c` (functions from Petari).** Same VI banner as Petari's ("Aug  8 2007
  02:07:17"); Petari (CC0, commit `6804bd31`, `src/RVL_SDK/vi/vi.c`, GC/3.0a5.2,
  matching for all its versions) has the same function order and sizes, plus four
  unused functions this DOL does not have. Our file keeps its globals; these functions
  were replaced by Petari's: `__VIRetraceHandler`, `VIInit`, `setFbbRegs`,
  `setHorizontalRegs` (now defined after `setFbbRegs`, its place in the DOL, with a
  prototype earlier), `setVerticalRegs`, `VIConfigure`, `VIConfigurePan`, `VIFlush`,
  `VISetTimeToDimming`; added: `__VIResetDev0Idle` (inlined) and `VIResetDimmingCount`;
  `VIGetTvFormat` gains `case 8`. Aliases at the top: `__VIRegs` for our `VI_HW_REGS`,
  `VI_XFBMODE_*` for our `VI_XFB_MODE_*`. The file now defines the three PAL
  progressive render modes after `taps` (`GXPal528Prog`, `GXPal528ProgSoft`,
  `GXPal524ProgAa`, from Petari; `VI_MAX_WIDTH_PAL`/`VI_MAX_HEIGHT_PAL` = 720/574):
  without them `.data` was 0xC0 bytes short. Names: `setHorizontalRegs`,
  `setVerticalRegs`, `VIResetDimmingCount` from the alignment,
  `__DVDStopMotorCommandBlock` (0x80563450) from relocation pairing. `.data` range
  added to the split (0x80397420-0x80397978).

- **`gamespy/GP/gpiInfo.c` (struct layouts from mkw).** Two structures are the newer
  GameSpy layout of mkw (doldecomp/mkw, CC0, commit `fa127308`,
  `lib/gamespy/GP/gpiProfile.h` and `gpiPeer.h`): `GPIProfile` gains
  `buddyStatusInfo` before `cache` (`cache` at 0x10), and `GPIPeer` replaces `sock`
  with `ip`/`port` and gains `peerOpQueue` before `pnext` (`profile` at 0x10, `pnext`
  at 0x54, the offsets read by `gpiProcessGetInfo`). No other linked unit uses these
  structures. Names from relocation pairing: `gpiGetProfile`, `gpiProfileListAdd`,
  `gpiAddOperation`, `gpiProcess`, `gpiAppendStringToBuffer`, `gpiAppendIntToBuffer`,
  `_strlwr` (0x802FDFB4). `.sdata2` range added to the split (0x80600B08-0x80600B10,
  the float used by `gpiProcessGetInfo`, plus padding). Our object has four extra
  functions the DOL does not have (`gpiSetInfod`, `gpiSetInfoMask`,
  `gpiGetInfoNoWait`, `gpiSetInfoCache`); the linker strips them.

- **`dvd/dvd.c` (functions from Petari).** Same DVD banner as Petari's US/PAL builds
  ("Aug  8 2007 01:59:13"); Petari (CC0, commit `6804bd31`, `src/RVL_SDK/dvd/dvd.c`)
  has every function of this unit at the same offset and size (plus
  `DVDCheckDiskAsync`, which this DOL does not have). Our file keeps its headers and
  enum names; these functions were taken from Petari: `stateBusy`, `cbForStateBusy`
  (with `IsImmCommandWithResult` / `IsDmaCommand` and `ImmCommand` / `DmaCommand`),
  `DVDCancelAsync`, `__DVDResetWithNoSpinup`; these were updated after Petari:
  `DVDInit` (ESP ticket view and TMD read when `__OSLockedFlag == 0x80`),
  `cbForStateReadingPartitionInfo` (boot partition from 0x80003198 /
  `__OSLaunchPartitionType`), `stateOpenPartition` / `stateOpenPartition2`
  (`DVDLowOpenPartitionWithTmdAndTicketView` path), and commands 41 / 42 added to
  `cbForStateGoToRetry`, `stateCoverClosed` and `DVDCancel`. Adaptations: Petari's
  `currTransferSize` / `transferredSize` are our `transferSize` / `transferTotal`,
  `ESTitleMeta` / `ESTicket` are `ESPTmd` / `ESPTicket`, numbers replaced by our
  `COMMAND_*` / `DVD_STATE_*` names, `MIN` defined locally (our headers lack it),
  `DVDPartitionParams` copied into the file from Petari's `revolution/dvd.h`,
  `ESP_DiGetTmd` prototype added to `esp.h`. The `OSPanic` line of `stateReadingFST`
  is 1083 (0x43B in the DOL). `__DVDNumTmdBytes` is declared after `GameToc`
  (32-byte aligned, as in Petari) to give the `.sbss` layout of the DOL. Names from
  the alignment with Petari: `cbForStateGoToRetry`, `cbForStateReadingPartitionInfo`,
  `cbForStateBusy`, `DVDCancelAsync`, `__DVDResetWithNoSpinup`; data names
  (`CommandInfoCounter`, `BB2`, `LastState`...) from relocation pairing with our
  object; `__DVDStopMotorCommandBlock` was 0x60 bytes in `symbols.txt` and is split
  into it and `__DVDRestartMotorCommandBlock` (0x80563480). Data ranges added to the
  split: `.data` 0x80398798-0x80398B08, `.bss` 0x8055E800-0x805634B0, `.sbss`
  0x805FBEC0-0x805FBF58.

- **`os/OSExec.c` (from Petari).** This DOL has the layout of Petari's RMGK01 build
  (0x1670 bytes: no `__OSRelaunchTitle` / `LaunchCommon` block), every function at
  the same offset and size. The file was rewritten from Petari (CC0, commit
  `6804bd31`, `src/RVL_SDK/os/OSExec.c`, KOR path, `-opt nolifetimes,noloop` as in
  Petari's `configure.py`) with our headers: DVDLow prototypes declared in the file
  with this SDK's signatures, `ESTitleMeta` reduced to a local `ESTitleMetaStart`
  (only `sysVersion` at 0x184 is read), `DVDGameTOC` 8 bytes as in Petari's
  `dvd.h`, `__PIRegs` written `PI_HW_REGS`, interrupt masks written as values
  (0xFFFFFFF0 = MEM|DSP|AI|EXI|PI, `PI_ACR`), `__OSSetExecParams` made static (inlined,
  no symbol in the DOL). `ESP_LaunchTitle` / `ESP_GetTicketViews` prototypes added to
  `esp.h`. Names from the alignment with Petari: `Utf16ToArg`, `PackInstallerArgs`,
  `__OSLaunchNextFirmware`, `__OSBootDolSimple`, and in ESP (same sizes as Petari's
  RMGK01 symbols) `ESP_LaunchTitle` (0x80268A1C), `ESP_GetTicketViews` (0x80268AB0),
  `ESP_GetTmdView` (0x80268D5C); data `id`, `DVDLowIntType`, `Prepared` from
  relocation pairing. Data ranges added to the split: `.data` 0x8038A0E0-0x8038A150,
  `.sbss` 0x805FBC50-0x805FBC68.

- **`mix/mix.c`, `mix/remote.c` (from mscharged-decomp).** Found in 0x802608AC-0x80263F18,
  the region that was inside the old `mem_list.c` split. Sources and headers
  (`revolution/mix.h`, `revolution/mix/MIXPrivate.h`) taken from mscharged-decomp (CC0,
  commit `e15f617c`, `src/RVL_SDK/mix/`). This DOL has a newer `mix.c`; changes, checked
  instruction by instruction against the DOL: `MIXInit` also tests `!__init`; added
  `MIXQuit` (clears `__MIXChannel`, `__MIXRmtChannel`, `__init`), `MIXGetSoundMode`,
  `MIXMute`, `MIXUnMute`, `MIXIsMute`, `MIXSetFader` (bodies after TP's GameCube
  `mix.c`, CC0, commit `c8fa8c9e`, with the Wii mute bit 8 seen in the DOL) and
  `fn_80262218` (sets `auxC` like `MIXSetAuxB`; no reference gives its name).
  `MIXRmtSetFader` is not in the DOL (stripped). Names: position, size and order
  against mscharged-decomp's and rb3's MIX symbols (`MIXQuit` 0x14 and `MIXSetFader`
  0x24 in rb3), GameCube MIX API order for `MIXGetSoundMode` / `MIXMute` /
  `MIXUnMute` / `MIXIsMute`; `AXIsInit` (0x80257CD0) by relocation pairing. Data
  ranges: `.data` 0x8039DEE0-0x8039EA70, `.bss` 0x80579228-0x8057D5A8, `.sbss`
  0x805FC120-0x805FC138.

- **`dsp/dsp.c`, `dsp/dsp_debug.c` (from mscharged-decomp), `dsp/dsp_task.c` bounds.**
  The `dsp_task.c` split started after `__DSPHandler` (0x8026525C, 0x434 bytes, the
  size of mscharged-decomp's); it now starts there, and `dsp.c` (0x80264F90) and
  `dsp_debug.c` (0x8026520C) are split before it, in mscharged-decomp's order. Sources
  from mscharged-decomp (CC0, commit `e15f617c`, `src/RVL_SDK/dsp/`), with the build
  date of this DOL in both the banner and `DSPInit`'s debug print ("Aug  8 2007
  01:59:06", found at 0x8039F870 and in the `__DSPVersion` string), and
  `DSPCancelTask` added (0x80265110: `task->flags |= 2` with interrupts disabled, as
  in TP's GameCube `dsp.c`, CC0, commit `c8fa8c9e`; prototype added to `dsp.h`).
  Names by position and size against mscharged-decomp: `DSPInit`, `DSPCheckInit`,
  `DSPAddTask`, `DSPAssertTask`, `__DSPHandler`; data `__DSPVersion`,
  `__DSP_init_flag`. Data ranges: `.data` 0x8039F808-0x8039F888, `.sdata`
  0x805FA2A8-0x805FA2B0, `.sbss` 0x805FC160-0x805FC168.

- **`esp/esp.c` (from Petari).** 0x80268980-0x80269074 holds the ten functions of
  Petari's `esp.c` with the same sizes (found by a size-sequence search over the
  reference clones). Taken from Petari (CC0, commit `601f78cb`,
  `src/RVL_SDK/esp/esp.c`, matching for all its versions) with our headers: Petari's
  type names aliased to ours (`IOSIoVector`, `ESTicket`, `ESTitleMeta`, `ESTmdView`),
  and `NULL` redefined as `((void*)0)` in this file (Petari's C definition; the DOL
  compares pointers with `li`/`cmplw`, our `NULL` 0 gives `cmpwi`). `.sdata`
  0x805FA2F8-0x805FA308 (`__esFd`, "/dev/es").

- **`kpad/KPAD.c` (from Petari).** 0x8027D270-0x80280318 has the functions of Petari's
  RMGJ01 `KPAD.c` with the same sizes (the DOL lacks `KPADSetBtnRepeat`,
  `KPADSetHoriParam`, `KPADSetDistParam`, `KPADSetSensorHeight`: stripped). Taken from
  Petari (CC0, commit `601f78cb`, `src/RVL_SDK/kpad/KPAD.c` and
  `include/revolution/kpad.h`) with: this DOL's banner ("Aug  8 2007 02:05:53
  (0x4199_60831)"), a 16-entry `uniRingBuf` instead of 120 (the DOL tests
  `idx >= 16` and `KPADInsideStatus` is 0x538 bytes), real calls to `sqrt`/`sin`/`cos`
  (24 `bl sqrt` in the DOL; our `math.h` inlines `sqrt`), `ASSERT` defined empty,
  Petari's WPAD names mapped to their values, casts for our WPAD prototypes, and
  `Vec2` taken from our `mtx/vec2.h`. 0x8027D28C is `KPADSetAccParam`, not
  `KPADSetHoriParam` (same size; it writes `acc_play_radius` at 0x9C). `DEBUGPrint`
  (0x8027D220) stays outside the split (not in Petari's KPAD). `PSMTXMultVec`
  (0x80248448) and the data names from relocation pairing. Data ranges: `.data`
  0x803A1AF0-0x803A1B48, `.bss` 0x8058CC20-0x8058E140, `.sdata`
  0x805FA470-0x805FA4E0, `.sbss` 0x805FC258-0x805FC278, `.sdata2`
  0x80600990-0x806009F0 (our object has 0x1C more `.sdata2`, constants of stripped
  code that the linker drops).

- **`os/OSStateTM.c` (from Petari).** 0x8023DD90-0x8023E484 has the twelve functions of
  mkw's (RMCP01) `OSStateTM` symbols with the same sizes; the code is Petari's RMGK01
  path (CC0, commit `601f78cb`, `src/RVL_SDK/os/OSStateTM.c`): the `OSPanic` lines
  316 / 380 / 788 in the DOL are those of that path. Added `OSSetResetCallback` (same
  shape as the RMGK01 `OSSetPowerCallback`: default callback when NULL, NULL returned
  for the default), checked against the DOL. Our headers: `__VIRegs` declared at
  0xCC002000, `__PIRegs` is `PI_HW_REGS`, `IOS_ERROR_OK` is `IPC_RESULT_OK`,
  `__OSSetVIForceDimming` takes `u32` as in our `OSStateTM.h`. Petari's extra flags
  (`no-opt_generateconditionalassignments`, `no-opt_rebuildconditionals`) are not
  needed here. Names: `OSSetResetCallback`, `OSSetPowerCallback`,
  `__OSSetIdleLEDMode`, `__OSVIDimReplyHandler` by position and size against mkw and
  Petari; `StmIm*`/`StmVd*` buffers by relocation pairing. 0x8023E484 (64 bytes,
  `OSGetTime` scaled to milliseconds) is not in Petari's file and stays outside the
  split. Data ranges: `.data` 0x80396DC8-0x80396E90, `.bss` 0x8055D980-0x8055DA40,
  `.sbss` 0x805FBCC0-0x805FBCE0.

- **`os/OSLaunch.c` (from Petari).** 0x8023F91C-0x802406DC has the eight functions of
  Petari's RMGK01 `OSLaunch.c` with the same sizes (0xDC0 in both). Taken from Petari
  (CC0, commit `601f78cb`, `src/RVL_SDK/os/OSLaunch.c`) with our headers: `NULL`
  redefined as `((void*)0)` (as in `esp.c`), `OSTitleId` typedef, our field names for
  `OSStateFlags` (`shutdownType`, `discState`), `const DVDDiskID*` for
  `DVDGetCurrentDiskID`. Headers: `OSNandbootInfo` now has Petari's field layout
  (same 0x1020 size, our lower-case names), `esp.h` gains Petari's `ESTmdView` types
  and the `ESP_GetTmdView` prototype. Names by position and size: `PackArgs`,
  `__OSCheckCompanyCode`, `__OSCheckTmdSysVersion`, `__OSGetValidTicketIndex`,
  `__OSLaunchTitle`, `LaunchCommon`; `OSLaunchNoReturnFlag` by relocation pairing.
  `lbl_80397124` was a 0x194-byte blob in `symbols.txt`, now the 0xB-byte string it
  starts with. Data ranges: `.data` 0x803970C8-0x803971E0, `.sdata`
  0x805FA190-0x805FA198, `.sbss` 0x805FBD20-0x805FBD28.
