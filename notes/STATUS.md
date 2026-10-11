# État du projet

## Fait

- Mise en place du template pour `RM7P4F` ; le build reconstruit un `main.dol`
  identique (`build.sha1` OK).
- `splits.txt` / `symbols.txt` : la seconde section `.sbss` renommée en `.sbss2` ;
  `fn_802E5A08` fusionnée dans `fn_802E50F0` (bloc des `case` du switch, table
  0x803AD338).
- Dépôt privé GitHub créé (`origin`), le template est conservé en `upstream`.
- Informations du jeu remplies dans `CLAUDE.md` (preuves dans `notes/COMPILER.md`).

## En cours

- Étape 2 : identification du compilateur et du linker.

## Bloqué

- Rien.

## À faire

- Fixer `linker_version`, `mw_version` et `mw_comment_version` une fois prouvés.
- Étape 3 : découper SDK, runtime et jeu dans `splits.txt`.
- Étape 4 : objdiff, CI GitHub Actions, decomp.dev.
