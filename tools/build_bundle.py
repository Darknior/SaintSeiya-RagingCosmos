#!/usr/bin/env python3
"""Assemble a minimal portable test bundle with official Windows/Linux/Android engines."""
from pathlib import Path
import ctypes,ctypes.util,hashlib,shutil,zipfile,json,os,argparse
ROOT=Path(__file__).resolve().parents[1]
def extract_windows(src,dest):
 lib=ctypes.CDLL(ctypes.util.find_library('archive'));lib.archive_read_new.restype=ctypes.c_void_p
 for n in ['archive_read_support_format_all','archive_read_support_filter_all','archive_read_free']:getattr(lib,n).argtypes=[ctypes.c_void_p]
 lib.archive_read_open_filename.argtypes=[ctypes.c_void_p,ctypes.c_char_p,ctypes.c_size_t];lib.archive_read_next_header.argtypes=[ctypes.c_void_p,ctypes.POINTER(ctypes.c_void_p)];lib.archive_entry_pathname.argtypes=[ctypes.c_void_p];lib.archive_entry_pathname.restype=ctypes.c_char_p;lib.archive_read_data.argtypes=[ctypes.c_void_p,ctypes.c_void_p,ctypes.c_size_t];lib.archive_read_data.restype=ctypes.c_ssize_t
 a=lib.archive_read_new();lib.archive_read_support_format_all(a);lib.archive_read_support_filter_all(a);assert lib.archive_read_open_filename(a,str(src).encode(),10240)==0
 e=ctypes.c_void_p();buf=ctypes.create_string_buffer(1048576)
 while lib.archive_read_next_header(a,ctypes.byref(e))==0:
  name=lib.archive_entry_pathname(e).decode();p=(dest/name).resolve();assert p.is_relative_to(dest.resolve());p.parent.mkdir(exist_ok=True,parents=True)
  with p.open('wb') as f:
   while True:
    n=lib.archive_read_data(a,buf,len(buf));assert n>=0
    if not n:break
    f.write(buf.raw[:n])
 lib.archive_read_free(a)
def build(output,pak):
 runtime=Path('/workspace/runtime');folder=output.parent/'pack-test'/'SaintSeiya-RagingCosmos';folder.mkdir(exist_ok=True,parents=True);(folder/'Paks').mkdir(exist_ok=True);(folder/'Android').mkdir(exist_ok=True)
 extract_windows(runtime/'OpenBOR-Windows.rar',folder)
 shutil.copyfile(runtime/'OpenBOR.AppImage',folder/'OpenBOR-Linux-x64.AppImage');shutil.copyfile(runtime/'OpenBOR-Android.apk',folder/'Android/OpenBOR-7533.apk');shutil.copyfile(runtime/'OpenBOR-LICENSE.txt',folder/'OpenBOR-LICENSE.txt');shutil.copyfile(ROOT/'LICENSE',folder/'Jeu-LICENSE.txt')
 dest=folder/'Paks/SaintSeiya-RagingCosmos.pak'
 if dest.exists():dest.unlink()
 os.link(pak,dest)
 (folder/'Jouer-Windows.bat').write_bytes(b'@echo off\r\ncd /d "%~dp0"\r\nstart "Saint Seiya" /wait "OpenBOR.exe"\r\n')
 (folder/'Jouer-Linux.sh').write_text('''#!/bin/sh
set -eu
cd -- "$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)"
chmod +x ./OpenBOR-Linux-x64.AppImage
export APPIMAGE_EXTRACT_AND_RUN=1
exec ./OpenBOR-Linux-x64.AppImage
''');(folder/'Jouer-Linux.sh').chmod(0o755);(folder/'OpenBOR-Linux-x64.AppImage').chmod(0o755)
 (folder/'LIRE-MOI.txt').write_text('''SAINT SEIYA - RAGING COSMOS - PACK DE TEST
Moteur officiel OpenBOR 4.0 build 7533 inclus.

WINDOWS : extraire tout le ZIP, puis lancer Jouer-Windows.bat ou OpenBOR.exe.
LINUX x64 : extraire tout ; chmod +x Jouer-Linux.sh ; ./Jouer-Linux.sh.
Le lanceur Linux ne necessite pas FUSE.
ANDROID : installer Android/OpenBOR-7533.apk. Ouvrir OpenBOR une fois, choisir
son dossier de jeux si demande, puis y copier Paks/SaintSeiya-RagingCosmos.pak.
L'installation de l'APK et le choix du dossier restent necessaires sur Android.

Au titre, choisir une langue avec les directions et valider avec Attaque/Start.
Appuyer ensuite sur Start. Block rouvre le choix de langue. Le choix est sauve.
Clavier par defaut : fleches, A=Attaque, D=Saut, Entree=Start, F=Block.
Commandes reconfigurables dans Options / Control Options.
Guide anime : Comment jouer, au menu principal ou dans la pause.
Choix de niveau : menu pause. Le personnage et les vies sont conserves.

Ce paquet retire uniquement les ressources non referencees selon l'audit
conservateur. Les cinemas references et toutes les animations utiles sont gardes.
Il ne contient pas les sources, historiques Git, captures, tests ou fichiers de
travail. Ceux-ci sont sauvegardes separement dans l'archive des sources.

Langues interface/guide : anglais, francais, espagnol, portugais du Bresil,
chinois simplifie, russe, japonais. Dialogues/videos/reglages natifs non traduits.
Les moteurs officiels ne sont pas modifies. Sources :
https://github.com/DCurrent/openbor/releases/tag/v7533
https://github.com/Darknior/SaintSeiya-RagingCosmos

Tests : moteur Linux officiel uniquement. Les moteurs Windows et Android sont
inclus depuis la release officielle mais n'ont pas ete executes sur ces appareils.
Les tests de chargement et de debut de combat ne sont pas une partie complete.
''')
 assert hashlib.sha256((folder/'Android/OpenBOR-7533.apk').read_bytes()).hexdigest()=='5760cdebbb6cd1541aee3e6fa1c6c6b09c531026ee60a974bbe303aa8a81f654'
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
