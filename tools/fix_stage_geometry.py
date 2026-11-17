#!/usr/bin/env python3
"""Normalize world bounds and relative player starts; explicit cave camera."""
from pathlib import Path
import re,json
ROOT=Path(__file__).resolve().parents[1]
def ranges(text):
 mode=None;bounds=(378,484);out={}
 for line in text.splitlines():
  w=line.split()
  if not w:continue
  if w[0]=='set':mode=w[1]
  elif w[0]=='z':bounds=tuple(map(int,w[1:3]))
  elif w[0]=='file':out.setdefault(w[1][5:],[]).append((mode,bounds))
 return out

def main():
 native=json.loads((ROOT/'reports/native-start-offsets.json').read_text())
 p=ROOT/'levels.txt';text=p.read_text();old=ranges(text)
 canonical={name:entries[-1][1] for name,entries in old.items()}
 for name in ('temple8A-road','temple8B-midboss'):canonical['levels/'+name+'.txt']=(620,790)
 # Use each stage's original dedicated-mode bounds in Story too. A bound is
 # written before every file so no subsequent stage inherits the cave bounds.
 lines=[]
 for line in text.splitlines():
  m=re.match(r'\s*file\s+data/(\S+)',line)
  if m:
   lo,hi=canonical[m[1]]
   if lines and re.match(r'^z\s',lines[-1]):lines.pop()
   lines.append(f'z           {lo} {hi}')
  lines.append(line)
 text='\n'.join(lines)+'\n';p.write_text(text);report=[]
 for name,bounds in canonical.items():
  p=ROOT/name;t=p.read_text();lo,hi=bounds
  def start(m):
   player=int(m[1]);offset=min(25+(player-1)*15,max(5,hi-lo-5))
   original=native.get(name,{}).get(str(player),0)
   if 0<original<hi-lo and Path(name).stem not in ('temple8A-road','temple8B-midboss'):offset=int(original)
   report.append({'file':name,'player':player,'offset':offset,'world_z':lo+offset,'bounds':bounds})
   x=str(350+(player-1)*50) if Path(name).stem in ('temple8A-road','temple8B-midboss') else m[2]
   return f'spawn{player} {x} {offset}'+m[4]
  t=re.sub(r'^spawn([1-4])\s+(\S+)\s+(\S+)([^\n]*)',start,t,flags=re.M)
  if p.stem in ('temple8A-road','temple8B-midboss'):
   # Retain native images; move the old 400..470 ground placements onto the
   # actual cave floor (620..790), with a stable camera window at world Y=330.
   if '# Geometry: cave floor' not in t:
    t=re.sub(r'^(coords\s+\S+\s+)(\d+(?:\.\d+)?)([^\n]*)',lambda m:m[1]+str(float(m[2])+230).removesuffix('.0')+m[3] if 350<=float(m[2])<=500 else m[0],t,flags=re.M)
    t+='\n# Geometry: cave floor and fixed vertical window.\nscrollz 330 330\nat 0\n'
   # scrollz must execute before encounter waits, rather than at end of file.
   block='\n# Geometry: cave floor and fixed vertical window.\nscrollz 330 330\nat 0\n'
   t=t.replace(block,'');match=re.search(r'\n(?:spawn |wait\b)',t);at=match.start() if match else len(t)
   if at<0:at=len(t)
   t=t[:at]+block+t[at:]
  p.write_text(t)
 (ROOT/'reports/stage-geometry.json').write_text(json.dumps(report,indent=2)+'\n')
 print('Relative starts:',len(report))
if __name__=='__main__':main()
