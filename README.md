Monster Lab (Wii)
=================

Décompilation par correspondance (*matching decompilation*) de Monster Lab sur Wii,
en cours.

Ce dépôt ne contient **aucun** asset ni assembleur du jeu. Il faut une copie du jeu.

Version supportée :

- `RM7P4F` : révision 0 (Europe)

Dépendances
===========

Windows
-------

Sous Windows, il est **fortement recommandé** d'utiliser les outils natifs. WSL et
msys2 ne sont **pas** nécessaires. Sous WSL, [objdiff](#comparaison) ne reçoit pas
les notifications du système de fichiers pour les rebuilds automatiques.

- Installer [Python](https://www.python.org/downloads/) et l'ajouter au `%PATH%`.
- Télécharger [ninja](https://github.com/ninja-build/ninja/releases) et l'ajouter au
  `%PATH%`.
  - Installation rapide via pip : `pip install ninja`

macOS
-----

- Installer [ninja](https://github.com/ninja-build/ninja/wiki/Pre-built-Ninja-packages) :

  ```sh
  brew install ninja
  ```

[wibo](https://github.com/decompals/wibo), un wrapper minimal de binaires Windows
32 bits, est téléchargé et utilisé automatiquement.

Linux
-----

- Installer [ninja](https://github.com/ninja-build/ninja/wiki/Pre-built-Ninja-packages).

[wibo](https://github.com/decompals/wibo) est téléchargé et utilisé automatiquement.

Build
=====

- Cloner le dépôt :

  ```sh
  git clone https://github.com/YggdrasziI/mlab-decomp.git
  ```

- Copier l'image disque du jeu dans `orig/RM7P4F`.
  - Formats supportés : ISO (GCM), RVZ, WIA, WBFS, CISO, NFS, GCZ, TGC
  - Après le premier build, l'image disque peut être supprimée.

- Configurer :

  ```sh
  python configure.py
  ```

- Construire :

  ```sh
  ninja
  ```

Comparaison
===========

Une fois le premier build réussi, un fichier `objdiff.json` existe à la racine du
projet.

Télécharger la dernière version de [encounter/objdiff](https://github.com/encounter/objdiff).
Dans les paramètres du projet, renseigner `Project directory` : la configuration est
chargée automatiquement.

Choisir un objet dans la barre latérale pour lancer la comparaison. Le projet est
reconstruit automatiquement quand les sources, les en-têtes, `configure.py`,
`splits.txt` ou `symbols.txt` changent.

![](assets/objdiff.png)

Basé sur [encounter/dtk-template](https://github.com/encounter/dtk-template).
