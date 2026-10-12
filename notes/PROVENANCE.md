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

## MSL_C (58 of 59 units Matching)

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

Not Matching yet (1 unit): `alloc.c`, whose two remaining functions have no source in
any reference project (they were dead-stripped from those games) and must be decompiled
from this DOL:

- 0x80330F50 is **`realloc`**: [DarkRTA/rb3](https://github.com/DarkRTA/rb3) (Rock Band 3,
  Wii, `config/SZBE69/symbols.txt`) has the same MSL layout (`deallocate_from_fixed_pools`
  0x1EC, an unnamed 0x260 function, `malloc` 0xA0, `free` 0x130, `realloc` 0x318).
  Only the **name** is taken from rb3 (it has no license, so none of its code is used).
- 0x80330B20 (0x260): unnamed in rb3 too; an allocator helper that tries to resize a
  block in place (it calls `SubBlock_merge_next` and `Block_link`), called by `realloc`.
  Keeps its address-based name.

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

## RVL_SDK (121 of 177 units Matching so far)

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

- **Version banners.** The `<< RVL_SDK - X 	release build: ... >>` strings in
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
  `gpiProcessRegisterUniqueNick` was on the wrong twin: 0x8030CDB0 compares `n\`
  (the source's string), 0x8030CEDC compares `c\`.

Functions **written from this DOL** (absent from the mscharged sources, real names
unknown, marked in the code):

- `fn_8026829C` in `NANDCheck.c`: free blocks/files for the application (calls the
  static `nandCalcUsage`, so it belongs to this unit).
- `fn_80259414` in `AXCL.c`: empty function called by the AX shutdown function where AX
  init calls `__AXClInit`.
- `fn_8030CEDC` in `gpiUnique.c`: `c\` (CD key registration) reply handler, twin of
  `gpiProcessRegisterUniqueNick`.

rb3's `NANDCheck.c` was checked and does not contain `fn_8026829C`; no rb3 code was
used.
