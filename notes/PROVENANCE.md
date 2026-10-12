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
