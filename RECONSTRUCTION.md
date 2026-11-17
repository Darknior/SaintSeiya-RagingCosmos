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
- Menu pause : Continuer, Choix du niveau, Comment jouer, Quitter la partie.
  La simulation et le son restent gérés par la pause native. Le chargement d'une
  étape et le retour au titre sont différés après la fermeture de cette boucle.
  Le saut conserve personnage et vies et reprend au bon indice de campagne.
- Guide Seiya de sept pages animées : déplacement, course, combo, saut, attaque
  aérienne, garde, Cosmos. 84 images préparées depuis le modèle natif, avec ses
  délais. Décor original généré du Sanctuaire, Pégase et cadre zodiacal doré.
- Sept langues au Press Start : anglais, français, espagnol, portugais du Brésil,
  chinois simplifié, russe et japonais. Choix persistant, réouvert par Block.
  Menu principal personnalisé, pause, niveaux et 49 pages de guide localisés.
  DejaVu Sans et Noto Sans CJK sont rendues en PNG indexés : accents et écritures
  non latines restent lisibles avec le moteur byte-based. Licences jointes.
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

Les dialogues, cinématiques, menus internes de réglage et choix des modes natifs
conservent leur langue d'origine. Il ne s'agit pas d'une traduction de toute la
campagne. Les moteurs Windows et Android sont officiels et inclus ; les essais
matériels réalisés ici utilisent uniquement le moteur Linux officiel. Les tests
ne constituent pas une partie complète jusqu'à la fin de chaque niveau.

## Reconstruction et vérifications

Python 3, Pillow, fontTools, DejaVu Sans et Noto Sans CJK pour régénérer les textes.
Les joueurs n'ont besoin d'installer aucune police.

```sh
python3 tools/generate_interface.py
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
