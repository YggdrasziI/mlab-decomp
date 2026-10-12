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
| Wii SDK (RVL) and network libraries | 0x80232AF8 | ~0x8032EF00 | `PPCMfmsr` 0x80232AF8; version strings registered by `EXIInit`, `SIInit`, `GXInit`, `DVDInit`, `SCInit`, … up to SSL (`fn_8032E18C`) |
| Runtime.PPCEABI.H | 0x8032EF00 | 0x8032FD80 | `__va_arg`, `__register_global_object`, `__destroy_global_chain`, `__save_fpr`/`__restore_fpr`/`__save_gpr`/`__restore_gpr`, `__div2u`, `__mod2u`, `__init_cpp_exceptions`, `__fini_cpp_exceptions`, `__register_fragment`, `__unregister_fragment` (ends 0x8032FD80) |
| MSL_C | 0x8032FD80 | 0x8033AFB4? | `__close_all`, `__prep_buffer`, `__flush_buffer`, `__fwrite`, `_fseek`, `wcstombs`, `memmove`, `memchr`, `parse_format`, `long2str`, `float2str`, `__pformatter`, `vprintf`, `vsnprintf`, `sprintf`, `strcpy`, `strchr`, `fwide` 0x8033AF30 |
| Unknown block (43 unnamed functions) | 0x8033AFB4 | 0x8033F460 | references a large `.sdata2` block (floating point constants); possibly MSL math. **Unproven** |
| MetroTRK | 0x8033F460 | 0x80344CC8 | `TRKNubMainLoop` … `gdev_cc_initialize` |
| Unknown block (39 unnamed functions) | 0x80344CC8 | 0x80345AD8 | after the last TRK communication functions. **Unproven** |

Uncertain: `strlen` (0x8032EEE4, weak) sits right before `__va_arg`; it is not yet
attributed to the SDK, the runtime or MSL.

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
In every section the order is game → SDK → runtime → MSL → unknown → TRK, which matches
the link order seen in `.text`.

| Section | Game | SDK | Runtime | MSL | Unknown (after MSL) | TRK |
|---|---|---|---|---|---|---|
| `.rodata` | 0x80345FC0–0x80349FF8 | 0x8034A8A0–0x803521A4 | 0x80352208 | 0x80352220–0x803528F8 | 0x80352F30–0x803531B0 | 0x80353268–0x8035334C |
| `.data` | 0x80353460–0x80389498 | 0x803894F0–0x803B4968 | — | 0x803B4AB8–0x803B51CC | — | 0x803B5210–0x803B5328 |
| `.bss` | 0x803B5380–0x8055CAB0 | 0x8055CB60–0x805F2920 | — | 0x805F5950–0x805F5CA8 | — | 0x805F5CD0–0x805F8170 |
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

## Splits in `splits.txt`

| Unit | Status |
|---|---|
| `Runtime.PPCEABI.H/__init_cpp_exceptions.cpp` | Split (required by the GC 3.0a5.2 linker), NonMatching |
| Everything else | dtk `auto_*` objects (gap-based, not TU-accurate) |
