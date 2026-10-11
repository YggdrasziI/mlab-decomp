# État du projet

## Fait

- Mise en place du template pour `RM7P4F` ; le build reconstruit un `main.dol`
  identique (`build.sha1` OK).
- `splits.txt` / `symbols.txt` : la seconde section `.sbss` renommée en `.sbss2` ;
  `fn_802E5A08` fusionnée dans `fn_802E50F0` (bloc des `case` du switch, table
  0x803AD338).
- Dépôt privé GitHub créé (`origin`), le template est conservé en `upstream`.
- Informations du jeu remplies dans `CLAUDE.md` (preuves dans `notes/COMPILER.md`).

- Étape 2 (partielle) : code du jeu compilé avec un mwcc **GC 3.0** (3.0a3 à
  3.0a5.2), `-fp_contract off` prouvé ; Wii 1.x et GC 2.x exclus
  (détails dans `notes/COMPILER.md`).

## En cours

- Étape 2 : départager les 7 versions GC 3.0 (code identique sur toutes les sondes).

## Bloqué

- Choix de la version exacte du compilateur : à décider (voir `notes/COMPILER.md`).

## À faire

- Fixer `mw_version`, `linker_version` et `mw_comment_version` (14 pour 3.0a3, 15 pour
  3.0a5+) et passer `-fp_contract off` dans `configure.py`.
- Étape 3 : découper SDK, runtime et jeu dans `splits.txt`.
- Étape 4 : objdiff, CI GitHub Actions, decomp.dev.
