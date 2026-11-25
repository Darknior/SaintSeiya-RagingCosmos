#!/usr/bin/env python3
"""Assemble a minimal portable test bundle with official Windows/Linux/Android engines."""
from pathlib import Path
import hashlib,shutil,zipfile,json,argparse
ROOT=Path(__file__).resolve().parents[1]
def build(output,pak):
 runtime=ROOT/'build';folder=output.parent/'pack-test'/'SaintSeiya-RagingCosmos';folder.mkdir(exist_ok=True,parents=True);(folder/'Paks').mkdir(exist_ok=True);(folder/'Android').mkdir(exist_ok=True)
 for name in ('OpenBOR.exe','OpenBOR-Linux-x64.AppImage','Android/OpenBOR-7533.apk','OpenBOR-LICENSE.txt','Jouer-Windows.bat','Jouer-Linux.sh','SHA256SUMS'):
  shutil.copy2(runtime/name,folder/name)
 shutil.copyfile(ROOT/'LICENSE',folder/'Jeu-LICENSE.txt')
 dest=folder/'Paks/SaintSeiya-RagingCosmos.pak'
 if dest.exists():dest.unlink()
 shutil.copyfile(pak,dest)
 (folder/'LIRE-MOI.txt').write_text('''SAINT SEIYA - RAGING COSMOS - PACK DE TEST
Moteur officiel OpenBOR 4.0 build 7533 inclus.

WINDOWS : extraire tout le ZIP, puis lancer Jouer-Windows.bat ou OpenBOR.exe.
LINUX x64 : extraire tout ; chmod +x Jouer-Linux.sh ; ./Jouer-Linux.sh.
Le lanceur Linux ne necessite pas FUSE.
ANDROID : installer Android/OpenBOR-7533.apk. Ouvrir OpenBOR une fois, choisir
son dossier de jeux si demande, puis y copier Paks/SaintSeiya-RagingCosmos.pak.
L'installation de l'APK et le choix du dossier restent necessaires sur Android.

Au titre, Gauche/Droite change la langue ; Start ouvre le menu natif.
Le choix de langue est conserve apres redemarrage.
Clavier par defaut : fleches, A=Attaque, D=Saut, Entree=Start, F=Block.
Commandes reconfigurables dans Options / Control Options.
Guide anime : Comment jouer, dans le menu principal natif.
Dialogues et tutoriel de Mu : vrais textes dans la police native partagee,
sans images de texte. Encadres blancs ; ponctuation liee au mot precedent.
Etoile rouge du logo original preservee. Noms affiches capitalises (Seiya).
Lettres et accents : meme police, taille et ligne de base.
Menu centre, police argentee et selection doree. Sous-titre Raging Cosmos
droit, bleu/argent et or comme Saint Seiya. Accents francais verifies : Systeme
s’ecrit bien Système dans les menus du jeu.
En pause en mode Histoire : Gauche/Droite choisit une destination, puis
Attaque charge le niveau. Continuer et Quitter la partie restent disponibles.
Versus : configurer les commandes du joueur 2 dans Options avant le duel.
Le ralenti a la defaite des boss est desactive.

Ce paquet retire uniquement les ressources non referencees selon l'audit
conservateur. Les cinemas references et toutes les animations utiles sont gardes.
Il ne contient pas les sources, historiques Git, captures, tests ou fichiers de
travail. Ceux-ci sont sauvegardes separement dans l'archive des sources.

Langues menus natifs, dialogues, tutoriel et messages : anglais, francais,
espagnol, portugais du Bresil, chinois simplifie, russe et japonais.
Les dialogues francais sont relus ; les autres langues demandent encore une
relecture linguistique. Videos et voix originales conservees.
Les moteurs officiels ne sont pas modifies. Sources :
https://github.com/DCurrent/openbor/releases/tag/v7533
https://github.com/Darknior/SaintSeiya-RagingCosmos

Tests : moteur Linux officiel uniquement. Les moteurs Windows et Android sont
inclus depuis la release officielle mais n'ont pas ete executes sur ces appareils.
Les tests de chargement et de debut de combat ne sont pas une partie complete.
''')
 for line in (folder/'SHA256SUMS').read_text().splitlines():
  expected,name=line.split('  ',1)
  if hashlib.sha256((folder/name).read_bytes()).hexdigest()!=expected:
   raise ValueError('OpenBOR checksum mismatch: '+name)
 contents={}
 for p in sorted(folder.rglob('*')):
  if p.is_file():
   with p.open('rb') as f:h=hashlib.file_digest(f,'sha256').hexdigest()
   contents[str(p.relative_to(folder))]={'bytes':p.stat().st_size,'sha256':h}
 (folder/'SHA256.json').write_text(json.dumps(contents,indent=2)+'\n')
 with zipfile.ZipFile(output,'w',zipfile.ZIP_DEFLATED,compresslevel=5,allowZip64=True) as z:
  for p in sorted(folder.rglob('*')):
   if p.is_file():z.write(p,'SaintSeiya-RagingCosmos/'+str(p.relative_to(folder)))
 with zipfile.ZipFile(output) as z:assert z.testzip() is None
 print(json.dumps({'bundle':str(output),'bytes':output.stat().st_size,'files':len(contents)+1,'zip_integrity':True}))
if __name__=='__main__':
 p=argparse.ArgumentParser();p.add_argument('output',type=Path);p.add_argument('pak',type=Path);a=p.parse_args();build(a.output.resolve(),a.pak.resolve())
