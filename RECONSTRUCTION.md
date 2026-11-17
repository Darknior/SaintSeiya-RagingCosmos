# Saint Seiya — Raging Cosmos : reconstruction OpenBOR 4

Base d'origine : Darknior/SaintSeiya-RagingCosmos, commit
`110c8b5dc1d60593dd49ba5dee70c367f5bc7ee7`. Reconstruction du 17 novembre 2026.
Les fichiers de la première session n'étaient plus disponibles ; cette version
reconstitue les fonctions demandées, sans prétendre reproduire chaque octet des
anciennes archives ou des dessins générés. Moteur officiel stable vérifié :
**OpenBOR 4.0 build 7533**. Moteurs Windows, Linux x64 et Android inclus au pack.

## Modifications rétablies

- 28 modèles de soldats ordinaires, 12 familles ; joueurs et boss exclus de la
  complétion d'animations. 167 déclarations de réactions ajoutées avec leurs
  images natives : coups, chutes, sang, feu, glace et électricité. Doublons
  `rise`/`pain11` et nom erroné du modèle `backman` corrigés.
- Six attaques à une seule image enrichies par anticipation et retour en garde,
  avec les poses natives et sans modifier les dégâts de contact. Une parade
  nouvelle du soldat de boue et sa réaction au blocage. Le dessin est généré
  puis réduit à 90×101 pixels utiles, dans le canevas natif 350×250, avec les
  indices de palette du soldat et l'ancrage 146,205. Le portrait est dérivé du
  sprite natif. Aucun autre corps de soldat n'est redessiné.
- Sept étapes manquantes restaurées dans l'ordre du mode Histoire (43 étapes).
  `8B`, `10A`, `10B`, `11B`, `12A`, `12B`, `13B` : 38 rencontres et 107 soldats.
  Deux ennemis visibles à l'entrée, puis trois par rencontre. Des pommes et des
  étoiles de Cosmos peuvent tomber à l'entrée. Les décors sont réutilisés depuis
  les ressources originales : grotte du Scorpion, falaise du Capricorne, paysage
  glacé pour le Verseau et approche des Poissons/Grand Pope. Les murs natifs des
  falaises sont conservés. Courts textes d'ambiance inspirés de l'anime ; les
  patrouilles sont une adaptation de gameplay, pas des combats canoniques.
- Menus et pause natifs conservés : Continuer / Quitter la partie. Les entrées
  ajoutées auparavant (choix du niveau, guide dans la pause) sont retirées.
- Guide Seiya de sept pages animées, accessible par l’entrée native Comment
  jouer. 84 images du modèle natif, avec ses délais, sur le décor original du
  Taureau. Aucun panneau d’interface bleu ajouté.
- Sept langues au titre par Gauche/Droite : anglais, français, espagnol,
  portugais du Brésil, chinois simplifié, russe et japonais. Choix persistant.
  Menus natifs et réglages, 107 pages de dialogues, instructions de Mû,
  messages d’objets et HUD traduits. Polices natives étendues en MBS pour
  accents, cyrillique et CJK ; textes de dialogues en PNG indexés. Licences jointes.
- Ralenti de défaite des boss désactivé dans tous les fichiers de niveaux.
- Migration : constantes devenues variables (`SAMPLE_BEEP`, limites Z), casse et
  déplacements de chemins, références audio/images/modèles manquantes, portraits
  initialisés avant le HUD de connexion, remaps incomplets et icônes corrigés.
  Les suppressions depuis les scripts sont différées d'une mise à jour via
  `lifespancountdown` : le moteur 7533 déréférence son contexte natif `self` lors
  d'un `killentity` appelé depuis certains callbacks de dessin/dialogue.
  Les profondeurs de départ sont des décalages depuis `PLAYER_MIN_Z`, et non des positions absolues. Les limites du mode Histoire reprennent celles des modes dédiés. La grotte du Scorpion utilise le sol réel du décor (620–790), une fenêtre verticale fixe et des départs dégagés des rochers du premier plan.

## Barre jaune

Les positions `p1life` à `p4life` étaient déjà `999 999` dans le dépôt initial :
la barre native du haut était masquée. Elles restent identiques. La barre
**Energy** sous le portrait appartient au HUD d'origine et reste affichée. Le
harnais de test conserve l'en-tête HUD complet pour éviter une fausse barre.

## Ressources et limites

Le pack de test conserve les fichiers référencés par tous les textes du jeu,
même les commentaires et modèles inutilisés, les préfixes construits dans les
scripts et les dossiers implicites (sons, scènes, musique, sprites, etc.). Les
ressources écartées sont listées dans le manifeste `.omitted.json` ; aucun fichier
source n'est supprimé. Les vidéos référencées restent incluses. Les sources
complètes sont sauvegardées séparément pour pouvoir reconstruire le PAK intégral.

Les dialogues français ont été relus ; les autres langues proviennent d’une
traduction automatique avec corrections de terminologie et demandent encore une
relecture linguistique. Les vidéos et voix préenregistrées restent originales.
Les moteurs Windows et Android sont officiels et inclus ; les essais
matériels réalisés ici utilisent uniquement le moteur Linux officiel. Les tests
ne constituent pas une partie complète jusqu'à la fin de chaque niveau.

## Reconstruction et vérifications

Python 3, Pillow, fontTools, DejaVu Sans et Noto Sans CJK pour régénérer les textes.
Les joueurs n'ont besoin d'installer aucune police.

```sh
python3 tools/generate_interface.py
python3 tools/generate_native_localization.py
python3 tools/audit_game.py
python3 tools/build_release.py /chemin/jeu.pak --light
python3 tools/build_release.py /chemin/jeu-integral.pak
```

`tools/complete_soldiers.py`, `restore_routes.py` et les outils `repair_*` sont
les scripts utilisés pour reconstituer les sources ; ne pas les rejouer sur des
sources déjà retouchées manuellement. Les fichiers réparés sont eux-mêmes inclus.

```sh
cc -shared -fPIC -O2 tools/input_bridge.c -ldl -o /workspace/runtime/input_bridge.so
python3 tools/test_interface.py --output /chemin/dossier-neuf
python3 tools/test_campaign.py --output /chemin/dossier-neuf
python3 tools/preview.py
python3 tools/build_bundle.py /chemin/pack.zip /chemin/jeu.pak
```

Les harnais utilisent une copie privée des scripts, sautent les vidéos d'ouverture
et la sélection initiale pour les essais de niveaux, sans déplacer ni supprimer
les adversaires. Les captures de galerie sont produites par le moteur. Le guide
HTML est un aperçu composé avec les assets du jeu, sans simulation des collisions.
Les rapports précisent le périmètre des vérifications. Les sources d'art générées
sont conservées dans `artwork-reference` pour ne pas perdre ces éléments.

## Conservation sur Google Drive

Le pack de test contient jeu, moteurs et lanceurs, sans sources ni captures.
L'archive des sources contient tout le projet reconstruit, hors métadonnées Git.
Une archive de correctifs plus petite permet aussi de retrouver les différences
par rapport au commit d'origine. Un patch binaire complet inclut les nouveaux
fichiers. Les archives sont vérifiées par CRC et SHA-256 avant envoi ; les liens
Drive sont lus après transfert, sans modifier leurs permissions de partage.

## Vérification de la reconstruction

Les réactions ajoutées aux soldats utilisent leurs images natives et des reprises
de séquences existantes : 167 déclarations ne signifient pas 167 nouveaux dessins.
La nouvelle parade du soldat de boue est le seul corps de soldat redessiné.
Le comparatif et les aperçus indiquent cette distinction.

La vérification des niveaux contrôle désormais les limites réelles du niveau testé
et la profondeur de départ native du joueur ; l'ancien harnais réutilisait les
limites du premier niveau. Les tests de chargement précédents ne validaient donc
pas le placement des acteurs. Le rapport `reports/stage-geometry.json` donne les
173 déclarations de départ contrôlées (valeurs natives conservées lorsqu’elles sont valides).

Dernière passe : 45 niveaux uniques chargés avec début de combat et contrôle de
la profondeur native de départ, puis 5 essais ciblés après les derniers réglages.
Le pack final est extrait et exécuté sous Linux : guide en 7 langues, 7 pages,
pause et retour au jeu validés. Les chiffres et limites figurent dans
`reports/reconstruction-status.json`.

## Progression et réactions : contrôles supplémentaires

`tools/test_route_progression.py` parcourt les sept routes ajoutées avec les
entrées directionnelles natives, compte les 107 ennemis et les 38 vagues, puis
constate le chargement du véritable niveau suivant. Les ennemis sont retirés
automatiquement dans une copie privée : ce test valide les événements et les
sorties, pas les combats ni leur difficulté. Les sept parcours ont réussi.

`tools/test_soldier_callbacks.py` déclenche 534 séquences sur les 28 modèles de
soldats ordinaires, dont les deux nouvelles séquences de parade du soldat de
boue : 3 102 images et leurs callbacks ont été exécutés sans erreur fatale. Les
images sont avancées explicitement, sans respecter leurs délais ; la fluidité,
les dégâts et l'IA nécessitent encore des essais de jeu ordinaires. Les copies
de test n'altèrent ni les statistiques ni les fichiers du pack distribué.

## Combats natifs et lanceur livré

`tools/test_native_combat.py` observe des duels isolés avec les seules entrées
directionnelles, attaque et garde. Aucun changement de points de vie, de dégâts,
d'IA ou de vitesse d'animation : Seiya a vaincu les représentants des 12 familles
et sa garde a été observée dans les 12 essais. Un essai supplémentaire de Shiryu
contre le soldat de boue s'est également terminé par une victoire. La parade du
soldat de boue n'a pas été observée par l'IA dans ces duels ; ses images et ses
callbacks restent couverts par les essais explicites précédents.

Attention pour interpréter l'équilibrage : `offense all 10` était déjà présent
dans le fichier de Seiya au commit d'origine. Un coup de base à 10 inflige donc
100 avant les autres facteurs, contre des multiplicateurs de 0,7 à 1,3 pour les
autres héros. Cette valeur d'origine est conservée, pas normalisée. Les victoires
du bot ne valident pas une difficulté de campagne équilibrée pour tous les héros.

`tools/test_linux_launcher.py` démarre le véritable `Jouer-Linux.sh` distribué,
l'AppImage officielle et le PAK intact, sans extraction du PAK ni FUSE : ouverture
de la séquence vidéo et capture native réussies. Windows et Android ne sont pas
exécutables dans cet environnement (Wine, ADB et émulateur absents). L'exécutable
Windows est PE32 x86 ; l'APK contient ARM 32 et ARM 64 bits. Ces inspections ne
remplacent pas les essais matériels. Rapports dans `reports/`.

## Sept routes terminées avec les combats natifs

`tools/test_route_native_combat.py` pilote Seiya par le clavier à travers les
sept routes restaurées. Les 107 soldats ont été vaincus et les sept véritables
niveaux suivants ont chargé, sans perte de vie. Aucun ennemi n'est retiré par le
harnais ; statistiques, santé et déroulement des animations restent natifs. Les
duels ci-dessus et ce parcours utilisent toujours le multiplicateur ×10 présent
d'origine sur Seiya : ils ne garantissent pas la même difficulté avec les autres
héros. Le rapport est `reports/routes-native-combat.json`. La campagne d'origine
entière, tous ses boss et les plateformes Windows/Android ne sont pas validés
par cet essai. Le pack distribué n'a pas nécessité de modification supplémentaire.

Pour reproduire une sauvegarde locale après chaque commit :

```sh
python3 tools/archive_checkpoint.py --output /chemin/sauvegarde
```

L'outil exige un arbre Git propre et produit les fichiers modifiés, un patch
binaire, les commits dans un bundle, une liste des suppressions et la référence
exacte du commit sauvegardé. L'envoi Drive reste une action distincte.

## Corrections des dialogues et des retours de boss

La branche du boss des Gémeaux est désormais `temple3BOSS` : le nom `temple2BOSS`
était partagé avec le Taureau. Un redémarrage natif chargeait donc le mauvais
boss. Le comparatif moteur reproduit le retour au Taureau avant correction et
le retour aux Gémeaux après correction. La validation refuse désormais les
branches dupliquées dans un même mode.

Les dialogues conservent la profondeur de chaque combattant, la restaurent
une seule fois à la fin du masquage et ne forcent plus tous les personnages à
400 à chaque image. La restauration intervient aussi après disparition de la
dernière boîte de texte ou du fondu, même sans déverrouillage explicite dans la
timeline. Un test natif vérifie deux cycles de masquage/restauration, la liberté
de déplacement ensuite et la disparition de la boîte encore verrouillée.

Les boucles des dialogues, trous, ascenseurs et effets d'eau parcourent tous les
emplacements d'entités (`ent_max`) : le nombre d'entités vivantes n'est pas une
limite correcte pour leurs indices lorsqu'un emplacement est libre. Six niveaux
concernés et le pack extrait ont été retestés. Rapports avant/après dans `reports/`.

Les timelines natives du Taureau et du Grand Pope ont également été observées
jusqu'au réaffichage du joueur à sa profondeur de départ (403). Pour le Taureau,
le boss est créé après l'arrivée du joueur dans une copie privée, afin que son
intro immédiate ne précède pas la connexion du joueur au premier niveau de test.
Les délais et images du dialogue restent natifs. Le rapport est
`reports/boss-dialog-release.json`.
