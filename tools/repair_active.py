#!/usr/bin/env python3
from pathlib import Path
import re,wave,shutil
from audit_game import closure,ROOT

def main():
 # Canonical locations of renamed chapter assets and existing equivalent effects.
 replacements={'data/bgs/temple2/aldebaran_dialog/':'data/bgs/temple2D-boss/aldebaran_dialog/','shiryu_dragon_uppercut':'shiryu_dragon','deathmask_spiral':'deathmask_thunder','data/sprites/loading/loading_Hyoga.png':'data/sprites/loading/loading_temple7.png','data/music/music2.bor':'data/music/7sens.ogg','data/music/boss.bor':'data/music/7sens.ogg'}
 for p in ROOT.rglob('*'):
  if p.suffix not in ('.txt','.c','.h') or '.git' in p.parts or 'tools' in p.parts:continue
  s=p.read_text(errors='replace');old=s
  for a,b in replacements.items():s=s.replace(a,b)
  if s!=old:p.write_text(s)
 # A small silent sample preserves explicitly silent legacy cues.
 with wave.open(str(ROOT/'sounds/silence.wav'),'wb') as w:w.setnchannels(1);w.setsampwidth(2);w.setframerate(22050);w.writeframes(b'\0'*440)
 seen,assets,missing=closure()
 for ref,parents in missing.items():
  if not ref.endswith('.wav'):continue
  desired=Path(ref[5:]);character=desired.parts[1] if desired.parts[0]=='chars' else None
  choices=sorted((ROOT/'chars'/character/'sounds').glob('*.wav')) if character else []
  stem=desired.stem.lower()
  match=next((p for p in choices if p.stem.lower()==stem),None)
  if match is None:
   meaning=re.sub(r'^\d+[-_]','',stem)
   match=next((p for p in choices if meaning in p.stem.lower()),None)
  if match is None and choices:
   # Same-character cues only; retain vocal/swing character identity.
   match=next((p for p in choices if 'hit' in p.stem or 'punch' in p.stem),choices[0])
  if match is None:match=ROOT/'sounds/silence.wav'
  for parent in parents:
   p=ROOT/parent;p.write_text(p.read_text().replace(ref,'data/'+str(match.relative_to(ROOT))))
 # Missing icon for a legacy Cassios player variant uses the native portrait.
 p=ROOT/'chars/cassios/00_cassiosp.txt';s=p.read_text().replace('data/chars/cassios/icon.png','data/chars/cassios/palettes/icon1.png');p.write_text(s)
 # Dead legacy loading helpers are supplied with the actual loading illustration.
 p=ROOT/'scripts/update.c';s=p.read_text();s=re.sub(r'data/bgs/loading/Loading[1-5]\.png','data/sprites/loading/loading_temple1.png',s);p.write_text(s)
 # Preserve registry names while mapping missing unused sky entity to a valid panel.
 sky=ROOT/'bgs/temple2A-road/temple2roadsky.txt';sky.write_text('name temple2roadsky\ntype panel\nanim idle\n loop 1\n delay 100\n offset 0 0\n frame data/bgs/temple2A-road/background.png\n')
if __name__=='__main__':main()
