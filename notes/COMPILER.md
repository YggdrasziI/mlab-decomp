# Compilateur, SDK et runtime — preuves

Toutes les adresses « offset » sont des offsets **fichier** dans `orig/RM7P4F/sys/main.dol`
(SHA-1 `50f6e9b94dbe4df6f663541493106d19e0a40c72`).

## Disque

| Info | Valeur | Preuve |
|---|---|---|
| ID | `RM7P4F` | `disc/header.bin` octets 0-5 |
| Région | PAL / Europe | 4e caractère de l'ID `P` |
| Révision | 0 | `disc/header.bin` octet 7 = `0x00` |
| RELs | aucun | aucun `.rel` dans `files/` |
| `.map` / `.elf` | aucun | aucun `.map`, `.elf`, `.sym` dans `files/` |

## SDK Wii (chaînes `<< RVL_SDK - … >>`)

| Offset | Bibliothèque | Build | Tag |
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

Également : `<< REX-PPC 2.1.255.1 (RevoEX-2.1plus) REL 080313085908 >>` (offset 0x3b0820).

## Runtime et indices CodeWarrior

- `Metrowerks Target Resident Kernel for PowerPC` (offset 0x5c0) : MetroTRK.
- `Metrowerks CW runtime library initializing default heap` (offset 0x34e356).
- Sections `extab` / `extabindex` non vides et symboles `__init_cpp_exceptions`
  (0x8032FCAC), `__fini_cpp_exceptions` (0x8032FCE8),
  `__init_cpp_exceptions_reference` (.ctors 0x80345AE0),
  `__destroy_global_chain_reference` (.dtors 0x80345FA0),
  `__fini_cpp_exceptions_reference` (.dtors 0x80345FA4), `fragmentID` (.sdata 0x805FAE28) :
  disposition « GC 2.7+ et Wii linkers » du template (deux entrées `.dtors`).

## Fichiers source cités dans le DOL

Jeu / moteur : `GCN_Mem_Alloc.c` (0x34e320), `UIHashTree.cpp` (0x37d510), `UIFont.cpp`,
`UIReapFont.cpp`, `UIFont.h`, `UIScreenSet.cpp`, `UIAssetMgr.cpp`, `UIElement.cpp`,
`UIText.cpp`, `UIImage.cpp`, `UISlideshow.cpp`, `UIModel.cpp`, `UIEntity.cpp`, `UIGrid.cpp`,
`UIParticleEffect.cpp`.
SDK : `OSReset.c`, `OSStateTM.c`, `OSPlayTime.c`, `OSLaunch.c`, `OS.c`, `vi.c`, `dvdfs.c`,
`dvd.c`, `arc.c`, `WUD.c`, `TPL.c`, `dwc_auth_interface.c`, `dwci_nd.c`, `ncdsystem.c`,
`NHTTP_bgnend.c`, `NHTTP_os_RVL.c`, `d_nhttp.c`.

Autres chaînes : `eidos`, `backbone` (offset 0x35c4f1) ; format d'assertion
`Assert: File - "%s", Function "%s", Line: %d, …` (offset 0x371ffa).

## Hypothèses à tester (non prouvées)

- Le code du jeu est compilé avec un mwcc **Wii** (`Wii/1.0RC1` à `Wii/1.7`) ou
  `GC/3.0a5.2`.
- `GC/3.0a5.2` est « Ver 4.1, build 60831 » dans `build/compilers/info.txt` : même
  nombre que le tag SDK `0x4199_60831`. Piste pour le compilateur du SDK, pas une
  preuve.
- `mw_comment_version: 11` dans `config.yml` et `linker_version = "GC/1.3.2"` dans
  `configure.py` sont les valeurs par défaut du template, **non vérifiées**.

## Tests de compilateur

À remplir : tableau compilateur × fonction, avec le diff.
