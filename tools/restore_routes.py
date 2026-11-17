#!/usr/bin/env python3
from pathlib import Path
from PIL import Image
import re,json
ROOT=Path(__file__).resolve().parents[1]
PLAN=[('temple8B-midboss','temple8A-road',6,'Milo protects the Scorpio Temple. Cross his guard patrols.'),('temple10A-road','temple10C-road',6,'Shura awaits at Capricorn. Climb the guarded mountain path.'),('temple10B-midboss','temple10C-road',5,'Shura\'s guards defend the approach to the Capricorn Temple.'),('temple11B-midboss','temple8C-road',5,'Camus freezes the road to Aquarius. Break through the icy patrols.'),('temple12A-road','temple12C-road',6,'The path of Pisces leads to Aphrodite and his deadly roses.'),('temple12B-midboss','temple12C-road',5,'Cross the last guard posts on the approach to Pisces.'),('temple13B-midboss','temple13A-road',5,'Marin points the way. The Grand Pope waits beyond the last guards.')]
def main():
 results=[]
 for name,source,waves,story in PLAN:
  original=(ROOT/'levels'/f'{source}.txt').read_text();header=original.split('#-------- Players spawn position')[0]
  # Keep scenic layers and native cliff geometry; omit old entry coordinates.
  header='\n'.join(l for l in header.splitlines() if not re.match(r'^\s*(spawn[1-4]|nofadeout|nosave|blocked|noslow)\b',l))
  panel=re.search(r'^\s*panel\s+(data/\S+)',header,re.M)[1]
  width=Image.open(ROOT/panel[5:]).width
  text=header+'\n# '+story+'\n'
  for i,(x,z) in enumerate([(50,405),(90,430),(120,455),(150,410)],1):text+=f'spawn{i} {x} {z} 0\n'
  triggers=[0]+[round(600+(width-1400-600)*i/(waves-2)) for i in range(waves-1)]
  soldiers=0
  for wave,trigger in enumerate(triggers):
   text+='\nwait\nat '+str(trigger)+'\n'
   for enemy,(model,x,z) in enumerate([('malesoldier',470,405),('sticksoldier',670,455)]+([] if wave==0 else [('malesoldier',700,465)])):
    text+=f'\nspawn {model}\ncoords {x} {z} 0\n'
    if wave==0 and enemy==0:text+='item apple\n'
    if wave==0 and enemy==1:text+='item cosmos-stars\n'
    text+='at '+str(trigger)+'\n';soldiers+=1
  (ROOT/'levels'/f'{name}.txt').write_text(text+'\n')
  results.append({'name':name,'source_scenery':source,'width':width,'encounters':waves,'soldiers':soldiers,'triggers':triggers,'story':story})
 (ROOT/'tools/route-layout.json').write_text(json.dumps(results,indent=2)+'\n');print('Routes:',len(results),'soldiers:',sum(x['soldiers'] for x in results),'encounters:',sum(x['encounters'] for x in results))
if __name__=='__main__':main()
