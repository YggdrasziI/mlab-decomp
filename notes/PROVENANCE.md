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

## MSL_C (55 of 59 units Matching so far)

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

Compiler: `ansi_fp.c`, `strtoul.c`, `e_pow.c`, `e_rem_pio2.c`, `k_tan.c`, `s_atan.c`
are built with `GC/3.0a5` (they only match with a compiler older than 3.0a5.2), the
other files with `GC/3.0a5.2`; see `notes/COMPILER.md`.

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

Not Matching yet, with the reason:

- `alloc.c`: two functions differ (0x80330B20, 0x80330F50);
- `arith.c`: `abs` and `labs` match, but the third function (0x80332EF0)
  is a signed remainder helper whose name is unknown (it is not TP's `div`);
- `uart_console_io_gcn.c`: `__write_console` is identical up to symbol names, but the
  unit also has `__close_console` and an 8-byte function (0x8033B094, `li r3,0; blr`)
  whose name no reference gives;
- `e_asin.c` (from TP, see below): its single function differs from this DOL.

### Units with no mscharged source

- `signal.c`, `e_asin.c`, `e_fmod.c`, `e_log.c`, `e_log10.c`: from
  [zeldaret/tp](https://github.com/zeldaret/tp) `libs/PowerPC_EABI_Support/MSL/...`,
  commit `c8fa8c9e2aab72cf4e5db0e5d1c84a9ea6ee6eb0`, **CC0-1.0**. Only the include lines
  were adapted to this project's headers (`<internal/fdlibm.h>`, plus `<errno.h>`,
  `<internal/math_nan.h>`, `<stdlib.h>` where needed); `signal.c` uses this project's
  `sig_func` type name instead of `__signal_func_ptr`. All but `e_asin.c` match.
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
