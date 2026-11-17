#!/usr/bin/env python3
from pathlib import Path
import re,json
ROOT=Path(__file__).resolve().parents[1]
PAT=re.compile(r'data/[^\s"\'<>;,()#]+')
def active(s):return '\n'.join(l if l.lstrip().startswith('#import') else l.split('#')[0] for l in s.splitlines())
def closure():
 queue=[ROOT/p for p in ['models.txt','levels.txt','select-all.txt','menu.txt','video.txt','scripts/update.c','scripts/updated.c','scripts/keyall.c','scripts/loading.c','scripts/endlevel.c'] if (ROOT/p).exists()];queue+=list((ROOT/'scenes').glob('*.txt'));seen=set();missing={};assets=set()
 while queue:
  p=queue.pop()
  if p in seen:continue
  seen.add(p)
  for ref in set(PAT.findall(active(p.read_text(errors='replace')))):
   q=ROOT/ref[5:]
   if q.is_dir() or ref.endswith('/'):continue
   if not q.is_file() and not q.suffix:
    q=next((Path(str(q)+e) for e in ['.png','.gif','.pcx','.ogg','.wav'] if Path(str(q)+e).exists()),q)
   if not q.is_file():
    if q.suffix:missing.setdefault(ref,[]).append(str(p.relative_to(ROOT)))
    continue
   assets.add(q)
   if q.suffix.lower() in ('.txt','.c','.h'):queue.append(q)
 return seen,assets,missing
if __name__=='__main__':
 seen,assets,missing=closure();report={'checked_files':len(seen),'referenced_assets':len(assets),'missing_assets':missing};(ROOT/'tools/active-audit.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2))
