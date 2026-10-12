# Compiler, SDK and runtime — evidence

All "offset" values are **file** offsets in `orig/RM7P4F/sys/main.dol`
(SHA-1 `50f6e9b94dbe4df6f663541493106d19e0a40c72`).

## Disc

| Info | Value | Evidence |
|---|---|---|
| ID | `RM7P4F` | `disc/header.bin` bytes 0-5 |
| Region | PAL / Europe | 4th character of the ID is `P` |
| Revision | 0 | `disc/header.bin` byte 7 = `0x00` |
| RELs | none | no `.rel` in `files/` |
| `.map` / `.elf` | none | no `.map`, `.elf` or `.sym` in `files/` |

## Wii SDK (`<< RVL_SDK - … >>` strings)

| Offset | Library | Build | Tag |
|---|---|---|---|
| 0x385628 | OS | Jan 30 2008 01:38:43 | 0x4199_60831 |
| 0x3933b8 | EXI | Aug  8 2007 01:59:22 | 0x4199_60831 |
| 0x393400 | SI | Aug  8 2007 02:07:10 | 0x4199_60831 |
| 0x393520 | VI | Aug  8 2007 02:07:17 | 0x4199_60831 |
| 0x394020 | GX | Dec 11 2007 01:34:15 | 0x4199_60831 |
| 0x394898 | DVD | Aug  8 2007 01:59:13 | 0x4199_60831 |
| 0x3961a0 | AI | Aug  8 2007 01:58:12 | 0x4199_60831 |
| 0x3961e8 | AX | Aug  8 2007 01:58:18 | 0x4199_60831 |
| 0x39b908 | DSP | Aug  8 2007 01:59:06 | 0x4199_60831 |
| 0x39bae0 | NAND | Dec 11 2007 01:35:48 | 0x4199_60831 |
| 0x39bd28 | SC | Aug  8 2007 02:09:03 | 0x4199_60831 |
| 0x39bff0 | WPAD | Dec 11 2007 01:35:07 | 0x4199_60831 |
| 0x39dbf0 | KPAD | Aug  8 2007 02:05:53 | 0x4199_60831 |
| 0x3a5598 | HBM | Oct 23 2007 10:25:01 | 0x4199_60726 |
| 0x3a9238 | DWC | Mar  7 2008 22:28:11 | 0x4199_60831 |
| 0x3afea8 | NCD | Dec 10 2007 10:02:22 | 0x4199_60831 |
| 0x3aff98 | NWC24 | Dec 10 2007 10:02:25 | 0x4199_60831 |
| 0x3b00a0 | SO | Dec 10 2007 10:02:35 | 0x4199_60831 |
| 0x3b0280 | SOCKET | Dec 10 2007 10:02:35 | 0x4199_60831 |
| 0x3b0520 | NHTTP | Oct 11 2007 20:49:15 | 0x4199_60726 |
| 0x3b0a20 | SSL | Nov  6 2007 23:10:08 | 0x4199_60726 |

Also: `<< REX-PPC 2.1.255.1 (RevoEX-2.1plus) REL 080313085908 >>` (offset 0x3b0820).

## Runtime and CodeWarrior hints

- `Metrowerks Target Resident Kernel for PowerPC` (offset 0x5c0): MetroTRK.
- `Metrowerks CW runtime library initializing default heap` (offset 0x34e356).
- Non-empty `extab` / `extabindex` sections and symbols `__init_cpp_exceptions`
  (0x8032FCAC), `__fini_cpp_exceptions` (0x8032FCE8),
  `__init_cpp_exceptions_reference` (.ctors 0x80345AE0),
  `__destroy_global_chain_reference` (.dtors 0x80345FA0),
  `__fini_cpp_exceptions_reference` (.dtors 0x80345FA4), `fragmentID` (.sdata 0x805FAE28):
  the template's "GC 2.7+ and Wii linkers" layout (two `.dtors` entries).

## Source file names found in the DOL

Game / engine: `GCN_Mem_Alloc.c` (0x34e320), `UIHashTree.cpp` (0x37d510), `UIFont.cpp`,
`UIReapFont.cpp`, `UIFont.h`, `UIScreenSet.cpp`, `UIAssetMgr.cpp`, `UIElement.cpp`,
`UIText.cpp`, `UIImage.cpp`, `UISlideshow.cpp`, `UIModel.cpp`, `UIEntity.cpp`, `UIGrid.cpp`,
`UIParticleEffect.cpp`.
SDK: `OSReset.c`, `OSStateTM.c`, `OSPlayTime.c`, `OSLaunch.c`, `OS.c`, `vi.c`, `dvdfs.c`,
`dvd.c`, `arc.c`, `WUD.c`, `TPL.c`, `dwc_auth_interface.c`, `dwci_nd.c`, `ncdsystem.c`,
`NHTTP_bgnend.c`, `NHTTP_os_RVL.c`, `d_nhttp.c`.

Other strings: `eidos`, `backbone` (offset 0x35c4f1); assertion format
`Assert: File - "%s", Function "%s", Line: %d, …` (offset 0x371ffa).

## Working configuration (2026-10-12)

**GC/3.0a5.2 is a working hypothesis**, not a proven fact: the GC 3.0 family is proven,
but no game function has been found that tells 3.0a5.2 apart from 3.0a3…3.0a5 (see
below). It was picked because the SDK build tag `0x4199_60831` matches its build number.

- `configure.py`: `linker_version = "GC/3.0a5.2"`, `-fp_contract off` in `cflags_base`.
- `config.yml`: `mw_comment_version: 15` (value written by 3.0a5.2).
- The 3.0a5.2 linker warns that `global_destructor_chain.c` and
  `__init_cpp_exceptions.cpp` "need to be updated" until
  `Runtime.PPCEABI.H/__init_cpp_exceptions.cpp` is split with the "GC 2.7+ and Wii
  linkers" layout (`.ctors$10`, `.dtors$10`, `.dtors$15`, `fragmentID` in `.sdata`).
  With that split the build matches (`main.dol: OK`).
- The SDK compiler is still untested (`DolphinLib` keeps the template value, unused).

## Compiler tests (2026-10-11)

### Method

Game functions were rewritten in C, compiled with every `mwcceppc.exe` in
`build/compilers/` (compilers_20251118), and their `.text` compared byte by byte with
the DOL. Relocated fields (`R_PPC_REL24`, `ADDR16_HA/LO`, `EMB_SDA21`) are masked. The
comparison script and test sources lived in the session scratchpad (not versioned).

Base flags: `-nodefaults -proc gekko -align powerpc -enum int -fp hardware
-Cpp_exceptions off -O4,p -inline auto -nosyspath -RTTI off -fp_contract off
-str reuse -multibyte` (Wii: `-func_align 4` added for the probes).

### Results

| Function | Content | GC 2.0–2.7 | GC 3.0a3 → 3.0a5.2 (7 versions) | Wii 1.0RC1 → 1.7 |
|---|---|---|---|---|
| `fn_8018E8D8` | null check, call | ✗ | ✓ | ✓ |
| `fn_8018EE3C` | 2 calls, 3 saved registers | ✗ | ✓ | ✓ |
| `fn_8018EF9C` | variadic calls (`crclr`) | ✗ | ✓ | ✓ |
| `fn_8007DD7C` | 3D vector length, inline `frsqrte` | ✗ | ✓ | ✓ |
| `fn_800E2DF4` | weighted float average | ✗ | ✓ | ✓ |
| `fn_80207D40` | `switch` with 7 `case`s (0-6), jump table | ✗ | ✓ | ✗ |

(✓ = identical to the DOL, ✗ = different. All 8 Wii compilers were tested on the 6
functions, with the same result. GC 2.0, 2.0p1, 2.5, 2.6, 2.7 tested; GC 1.x not tested.)

### Proven

- **`-fp_contract off`**: with `on`, `fn_8007DD7C` and `fn_800E2DF4` produce
  `fmadds`/`fnmsub` instructions absent from the original, with every compiler. With
  `off`, they match.
- **Not a Wii compiler (1.0RC1 to 1.7)**: their jump table threshold is 8 `case`
  values (checked with `-O1`, `-O2`, `-O3`, `-O4,p`, `-O4,s`, `-inline off`,
  `-opt nopeephole`; only `-O0` goes down to 4). The game has 20 jump tables with 7
  entries in optimized code (`fn_80207D40`, `fn_8019914C` in the UI area,
  `fn_801353C0`…). Wii emits a `cmpwi`/`beq` chain for `fn_80207D40`, GC 3.0 the exact
  table.
- **Not GC 2.x**: `cmplwi` instead of `cmpwi` on pointers, merged `mr.`
  (`fn_8018E8D8`, `fn_8018EE3C`, `fn_8018EF9C`).
- GC 3.0a5.2 emits the `psq_st f31` prologue seen in `fn_80207DCC`.

### Still undetermined

- **Exact version among GC 3.0a3, 3.0a3p1, 3.0a3.2, 3.0a3.3, 3.0a3.4, 3.0a5,
  3.0a5.2**: identical code on every probe (`probe.cpp`, `probe2.cpp`: classes,
  virtuals, exceptions on/off, templates, `long long`, bit fields, conversions;
  `-O2`, `-O4,s`, `-O4,p`).
- Non-conclusive hint: the SDK tag `0x4199_60831` matches the build number of
  GC 3.0a5.2 ("Ver 4.1, build 60831" in `info.txt`).
- `mw_comment_version`: 3.0a3 writes **14**, 3.0a5 and 3.0a5.2 write **15**
  (byte 0xB of `.comment`, read from a compiled object). The current value **11**
  (GC 2.7) is therefore wrong for every candidate, but 14 vs 15 depends on the version.
- The other flags (`-O4,p`, `-inline auto`, `-str reuse`…) are **compatible** with
  the 6 functions, but not proven unique.
- The SDK compiler has not been tested.

### Telling the GC 3.0 versions apart (2026-10-11)

- **Fuzzing**: 240 random C functions (int/float arithmetic, loops, switches, arrays,
  calls) compiled with the 7 versions under `-O4,p`, `-O4,s`, `-O2` and
  `-inline deferred`. Result: only **two classes** exist, {3.0a5.2} and {3.0a3,
  3.0a3p1, 3.0a3.2, 3.0a3.3, 3.0a3.4, 3.0a5} (identical to each other in every
  case). No difference at all at `-O2`.
- **The 3.0a5.2 change**: better elimination of int→float conversions whose result
  is dead. Minimal case:
  `int d1(S* p) { float u = p->f * (p->f > (float)garr[3] ? (float)garr[3] : p->f); return gi; }`
  - 3.0a3…3.0a5: `fcmpo` + conditional branch over two dead conversion stores
    (`stw` to 0x10/0x14), 0x20 stack frame;
  - 3.0a5.2: dead `fcmpo` with no branch and no stores, 0x10 stack frame.
  The other differences found by fuzzing are minor scheduling changes in functions
  that also contain such conversions.
- **Game scan**: neither signature (branch over dead conversion stores, or unused
  `fcmpo` result) appears anywhere in the DOL. The 5 game functions with dead
  conversion stores (`fn_80147800`, `fn_802D8844`, `fn_800EAF14`, `fn_800EB860`,
  `fn_801F6290`) are not this pattern: `fn_80147800` (dead `(short)` conversion)
  matches with **all 7** versions; `fn_802D8844` contains unreachable code and was
  not used as a test.
- **Conclusion**: with the evidence found so far, the game code cannot tell
  {3.0a5.2} from {3.0a3…3.0a5}. Every function tested matches equally with both
  classes. A function that matches with only one class would settle it; to be
  watched for during decompilation.

| Function | Content | GC 3.0a3 → 3.0a5 | GC 3.0a5.2 | Wii 1.0 | GC 2.7 |
|---|---|---|---|---|---|
| `fn_80147800` | type-tagged value decode, dead `(short)` conversion | ✓ | ✓ | ✗ | ✗ |
