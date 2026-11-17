#!/usr/bin/env python3
from pathlib import Path
import re,json
ROOT=Path(__file__).resolve().parents[1]
REQUIRED='idle walk attack1 pain pain2 fall fall8 fall9 death rise pain10 pain11 pain12 pain15 fall15 pain20 fall20 pain30 fall30'.split()
def soldiers():
 for line in (ROOT/'models.txt').read_text().splitlines():
  w=line.split('#')[0].split()
  if len(w)>2 and w[0] in ('load','know') and '/soldier_' in w[2] and Path(w[2]).name.startswith(('00_','malesoldier_')):yield w[1],ROOT/w[2][5:]
def blocks(s):return list(re.finditer(r'^\s*anim\s+([^\s#]+)[^\n]*\n(?:(?!^\s*anim\s).)*',s,re.M|re.S|re.I))
def main():
 report=[]
 for name,p in soldiers():
  s=p.read_text();s=re.sub(r'^\s*blood_ground_medium\s*$','',s,flags=re.M)
  # Keep native reactions while eliminating duplicate engine IDs.
  seen={};parts=[];pos=0
  for b in blocks(s):
   key=b[1].lower();seen.setdefault(key,[]).append(b)
  for key,entries in seen.items():
   if len(entries)>1:
    if key=='pain11' and 'pain12' not in seen:
     b=entries[-1];s=s[:b.start()]+re.sub(r'anim\s+pain11','anim pain12',b[0],count=1,flags=re.I)+s[b.end():]
    else:
     for b in reversed(entries[:-1]):
      replacement=re.sub(r'anim\s+follow1','anim follow2',b[0],count=1,flags=re.I) if key=='follow1' else ''
      s=s[:b.start()]+replacement+s[b.end():]
  own={b[1].lower():b[0] for b in blocks(s)}
  donor={}
  for q in sorted(p.parent.glob('*.txt')):
   for b in blocks(q.read_text(errors='replace')):donor.setdefault(b[1].lower(),b[0])
  added=[]
  for key in REQUIRED:
   if key in own:continue
   if key in donor:block=donor[key]
   else:
    base='pain' if key.startswith('pain') else 'fall' if key.startswith('fall') or key=='death' else 'idle'
    block=own.get(base,own['idle'])
    block=re.sub(r'anim\s+[^\s#]+',f'anim {key}',block,count=1,flags=re.I)
    block=re.sub(r'@cmd[^\n]*','',block)
    effect={'10':'hyoga_iceA','11':'hyoga_iceB','12':'hyoga_iceD','15':'shaka_flame','20':'blood_projection_big','30':'shock1'}.get(re.sub(r'\D','',key))
    if effect:block=re.sub(r'(anim[^\n]*\n)',r'\1    @cmd spawnbind "'+effect+'" "ANI_IDLE" 0 1 40 1 0\n',block,count=1)
   s+='\n# Completed ordinary soldier reaction using native frames.\n'+block+'\n';own[key]=block;added.append(key)
  s=s.replace('soldier_basic_flame','shaka_flame')
  # Single-contact attacks gain native anticipation and recovery, keeping damage.
  for b in reversed(blocks(s)):
   if b[1].lower()!='attack1':continue
   frames=re.findall(r'^\s*frame\s+(\S+)',b[0],re.M)
   if len(frames)==1 and p.parent.name in ('soldier_base','soldier_purple','soldier_big'):
    contact=frames[0];wind=contact.rsplit('/',1)[0]+('/01.png' if p.parent.name=='soldier_big' else '/02.png');idle=contact.rsplit('/',1)[0]+'/01.png'
    text=b[0];idx=text.lower().index('    attack') if '    attack' in text.lower() else text.index('frame')
    # Disable contact during anticipation, then restore the exact native attack box.
    attack=re.search(r'^\s*(attack\S*\s+[^\n]+)',text,re.M)
    if attack:
     a=attack[1];text=text[:attack.start()]+'\n    attack 0 0 0 0 0\n    delay 8\n    frame '+wind+'\n    '+a+'\n    delay 10\n'+text[attack.end():]
    text+='\n    attack 0 0 0 0 0\n    delay 6\n    frame '+wind+'\n    frame '+idle+'\n'
    s=s[:b.start()]+text+s[b.end():]
  p.write_text(s);report.append({'model':name,'file':str(p.relative_to(ROOT)),'added':added})
 (ROOT/'tools/soldier-changes.json').write_text(json.dumps(report,indent=2)+'\n');print(len(report),'models;',sum(len(x['added']) for x in report),'completed declarations')
if __name__=='__main__':main()
