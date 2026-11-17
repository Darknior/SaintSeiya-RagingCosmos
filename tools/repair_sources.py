#!/usr/bin/env python3
from pathlib import Path
import re,json
ROOT=Path(__file__).resolve().parents[1]

def texts():
 return [p for p in ROOT.rglob('*') if p.suffix.lower() in ('.txt','.c','.h') and '.git' not in p.parts and 'tools' not in p.parts]
def main():
 files=[p for p in ROOT.rglob('*') if p.is_file() and '.git' not in p.parts]
 bycase={str(p.relative_to(ROOT)).lower():p for p in files};base={}
 for p in files:base.setdefault(p.name.lower(),[]).append(p)
 changes=[];missing={}
 for p in texts():
  s=p.read_text(errors='replace');original=s
  for token in sorted(set(re.findall(r'data/[^\s"\'<>;,()#]+',s)),key=len,reverse=True):
   relative=token[5:];candidate=ROOT/relative
   if candidate.exists():continue
   if not candidate.suffix:
    if any(Path(str(candidate)+e).exists() for e in ['.png','.gif','.pcx','.ogg','.wav']):continue
    # Directory fragments and runtime-constructed paths are checked separately.
    if token.endswith('/') or token in ('data/','data/bgs/temple'):continue
   match=bycase.get(relative.lower())
   if match is None and candidate.suffix:
    possibles=base.get(candidate.name.lower(),[])
    if len(possibles)==1:match=possibles[0]
   if match is not None:
    replacement='data/'+str(match.relative_to(ROOT));s=s.replace(token,replacement);changes.append([str(p.relative_to(ROOT)),token,replacement])
   elif candidate.suffix:missing.setdefault(token,[]).append(str(p.relative_to(ROOT)))
  if s!=original:p.write_text(s)
 (ROOT/'tools/asset-repairs.json').write_text(json.dumps(changes,indent=2)+'\n')
 (ROOT/'tools/unresolved-assets.json').write_text(json.dumps(missing,indent=2)+'\n')
 print('Path repairs:',len(changes),'unresolved tokens including inactive comments:',len(missing))
 for k,v in missing.items():print(k,v[:2])
if __name__=='__main__':main()
