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

## Valeurs du template non vérifiées

- `mw_comment_version: 11` dans `config.yml` et `linker_version = "GC/1.3.2"` dans
  `configure.py` sont les valeurs par défaut du template (voir les résultats
  ci-dessous : 11 ne correspond à aucun candidat restant).

## Tests de compilateur (2026-10-11)

### Méthode

Fonctions du jeu réécrites en C, compilées avec chaque `mwcceppc.exe` de
`build/compilers/` (compilers_20251118), puis `.text` comparé octet par octet au DOL.
Les champs relogés (`R_PPC_REL24`, `ADDR16_HA/LO`, `EMB_SDA21`) sont masqués. Le script
de comparaison et les sources de test étaient dans le scratchpad de session (non
versionnés).

Flags de base : `-nodefaults -proc gekko -align powerpc -enum int -fp hardware
-Cpp_exceptions off -O4,p -inline auto -nosyspath -RTTI off -fp_contract off
-str reuse -multibyte` (Wii : `-func_align 4` ajouté pour les sondes).

### Résultats

| Fonction | Contenu | GC 2.0–2.7 | GC 3.0a3 → 3.0a5.2 (7 versions) | Wii 1.0RC1 → 1.7 |
|---|---|---|---|---|
| `fn_8018E8D8` | test de nullité, appel | ✗ | ✓ | ✓ |
| `fn_8018EE3C` | 2 appels, 3 registres sauvés | ✗ | ✓ | ✓ |
| `fn_8018EF9C` | appels variadiques (`crclr`) | ✗ | ✓ | ✓ |
| `fn_8007DD7C` | norme 3D, `frsqrte` inline | ✗ | ✓ | ✓ |
| `fn_800E2DF4` | moyenne pondérée flottante | ✗ | ✓ | ✓ |
| `fn_80207D40` | `switch` de 7 `case` (0-6), table de saut | ✗ | ✓ | ✗ |

(✓ = identique au DOL, ✗ = différent. Les 8 Wii testés sur les 6 fonctions, même
résultat pour tous. GC 2.0, 2.0p1, 2.5, 2.6, 2.7 testés ; GC 1.x non testés.)

### Ce qui est prouvé

- **`-fp_contract off`** : avec `on`, `fn_8007DD7C` et `fn_800E2DF4` produisent des
  `fmadds`/`fnmsub` absents de l'original, avec tous les compilateurs. Avec `off`,
  correspondance.
- **Pas un compilateur Wii (1.0RC1 à 1.7)** : leur seuil de table de saut est de
  8 valeurs de `case` (vérifié en `-O1`, `-O2`, `-O3`, `-O4,p`, `-O4,s`, `-inline off`,
  `-opt nopeephole` ; seul `-O0` descend à 4). Le jeu contient 20 tables de 7 entrées
  dans du code optimisé (`fn_80207D40`, `fn_8019914C` zone UI, `fn_801353C0`…). Wii
  génère une chaîne `cmpwi`/`beq` pour `fn_80207D40`, GC 3.0 la table exacte.
- **Pas GC 2.x** : `cmplwi` au lieu de `cmpwi` sur les pointeurs, `mr.` fusionné
  (`fn_8018E8D8`, `fn_8018EE3C`, `fn_8018EF9C`).
- GC 3.0a5.2 produit le `psq_st f31` de prologue vu dans `fn_80207DCC`.

### Ce qui reste indéterminé

- **Version exacte parmi GC 3.0a3, 3.0a3p1, 3.0a3.2, 3.0a3.3, 3.0a3.4, 3.0a5,
  3.0a5.2** : code identique sur toutes les sondes (`probe.cpp`, `probe2.cpp` :
  classes, virtuelles, exceptions on/off, templates, `long long`, champs de bits,
  conversions ; `-O2`, `-O4,s`, `-O4,p`).
- Indice non probant : le tag SDK `0x4199_60831` correspond au numéro de build de
  GC 3.0a5.2 (« Ver 4.1, build 60831 » dans `info.txt`).
- `mw_comment_version` : 3.0a3 écrit **14**, 3.0a5 et 3.0a5.2 écrivent **15**
  (octet 0xB de `.comment`, relevé sur un objet compilé). La valeur actuelle **11**
  (GC 2.7) est donc fausse pour tous les candidats, mais 14 ou 15 dépend de la version.
- Les autres flags (`-O4,p`, `-inline auto`, `-str reuse`…) sont **compatibles**
  avec les 6 fonctions, mais pas prouvés uniques.
- Le compilateur du SDK n'a pas été testé.
