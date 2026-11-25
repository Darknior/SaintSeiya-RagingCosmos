# SaintSeiya-RagingCosmos
Fangame of Saint Seiya anime. Beat'em up made with OpenBOR

## Lancer le jeu / Run the game

Le dossier [build](build/) contient les moteurs officiels **OpenBOR 4.0 build 7533** pour Windows, Linux x64 et Android, ainsi que les lanceurs Windows et Linux. Le build 3984 (OpenBOR 3.0) ne prend pas en charge le menu Pause de cette version.

Depuis la racine du dépôt, générez le PAK avec Python 3 avec Pillow et fonttools (`python -m pip install Pillow fonttools`) :

```sh
python -X utf8 tools/build_release.py build/Paks/SaintSeiya-RagingCosmos.pak --light
```

- Windows : lancez `build/Jouer-Windows.bat`.
- Linux x64 : lancez `./build/Jouer-Linux.sh`.
- Android : installez `build/Android/OpenBOR-7533.apk` et copiez le PAK dans le dossier de jeux choisi dans OpenBOR.

The [official OpenBOR release](https://github.com/DCurrent/openbor/releases/tag/v7533) is bundled in `build/`. Generate the PAK as shown above and use the launcher for your platform.
