#!/usr/bin/env python3
"""Validate and build standard OpenBOR PACK v0 archives; source assets stay intact."""
from pathlib import Path
from collections import Counter
import struct,shutil,json,re,hashlib,argparse
from complete_soldiers import soldiers,REQUIRED,blocks
from audit_game import closure,ROOT
from generate_interface import catalog
DIRS=('bgs','chars','levels','music','scenes','scripts','sounds','sprites','translations')
def validate():
 errors=[];report=[]
 for name,p in soldiers():
  s=p.read_text();counts=Counter(b[1].lower() for b in blocks(s))
  for key in REQUIRED:
   if key not in counts:errors.append(f'{name}: missing {key}')
  for key,n in counts.items():
   if n!=1:errors.append(f'{name}: duplicate {key}')
  if re.search(r'^\s*boss\s+1\b',s,re.M):errors.append(name+': boss wrongly included')
  declared=re.search(r'^name\s+(\S+)',s,re.M)
  if not declared or declared[1].lower()!=name.lower():errors.append(name+': registry name mismatch')
  for ref in re.findall(r'^\s*(?:frame|palette|icon)\s+(data/\S+)',s,re.M):
   asset=ROOT/ref[5:];choices=[asset] if asset.suffix else [Path(str(asset)+e) for e in ('.png','.gif','.pcx')]
   if not any(q.is_file() for q in choices):errors.append(name+': missing '+ref)
  report.append({'model':name,'family':p.parent.name,'animation_count':len(counts)})
 seen,assets,missing=closure()
 if missing:errors.append('Missing active assets: '+str(missing))
 sets=catalog()
 if len(sets[1]['levels'])!=43:errors.append('Story requires 43 stages')
 for route in json.loads((ROOT/'tools/route-layout.json').read_text()):
  s=(ROOT/'levels'/f'{route["name"]}.txt').read_text();triggers=[]
  if len(re.findall(r'^wait\b',s,re.M))!=route['encounters']:errors.append(route['name']+': wrong wave count')
  if len(re.findall(r'^spawn\s+(?:malesoldier|sticksoldier)\b',s,re.M))!=route['soldiers']:errors.append(route['name']+': wrong soldier count')
  if max(route['triggers'])>route['width']-800:errors.append(route['name']+': unreachable final wave')
 manifest=json.loads((ROOT/'sprites/interface/manifest.json').read_text())
 for code,digest in manifest['translations'].items():
  if hashlib.sha256((ROOT/'translations'/f'{code}.json').read_bytes()).hexdigest()!=digest:errors.append(code+': stale Unicode rendering')
 result={'soldier_models':len(report),'families':len(set(x['family'] for x in report)),'soldiers':report,'story_stages':43,'active_text_files':len(seen),'missing_assets':missing,'errors':errors}
 (ROOT/'tools/validation.json').write_text(json.dumps(result,indent=2)+'\n')
 print('Validation:',len(report),'soldiers,',result['families'],'families, 43 Story stages;',len(errors),'errors')
 for e in errors:print(e)
 if errors:raise SystemExit('Validation failed')
 return result

def sources():
 return sorted([p for d in DIRS for p in (ROOT/d).rglob('*') if p.is_file() and p.suffix.lower() not in ('.psd','.kra','.bak')]+[p for p in ROOT.glob('*.txt') if ' - Copie' not in p.name])
def prune(files):
 texts=[p for d in DIRS for p in (ROOT/d).rglob('*') if p.suffix.lower() in ('.txt','.c','.h')]+list(ROOT.glob('*.txt'))
 content='\n'.join(p.read_text(errors='replace') for p in texts).replace('\\','/').lower()
 prefixes=re.findall(r'''["'](data/[^"']*)["']\s*\+''',content)
 keep=[];omitted=[]
 for p in files:
  rel=p.relative_to(ROOT);name='data/'+rel.as_posix().lower();suffix=p.suffix.lower();referenced=name in content
  if suffix in ('.png','.gif','.pcx','.ogg','.mp3'):referenced|=name.rsplit('.',1)[0] in content
  implicit=rel.parts[0] in ('scripts','sprites','sounds','scenes','levels','music','translations') or len(rel.parts)<=2
  if suffix=='.db' and not referenced:implicit=False
  if referenced or implicit or suffix in ('.txt','.c','.h') or any(name.startswith(q) for q in prefixes):keep.append(p)
  else:omitted.append({'path':str(rel),'bytes':p.stat().st_size})
 report={'omitted_files':len(omitted),'omitted_bytes':sum(x['bytes'] for x in omitted),'source_assets_deleted':False,'policy':'Conservative references across all game text, implicit folders and dynamic prefixes','omitted':omitted}
 return keep,report

def build(output,light):
 validate();files=sources()
 if light:
  files,report=prune(files);output.with_suffix('.omitted.json').write_text(json.dumps(report,indent=2)+'\n');print('Omitted',report['omitted_files'],'assets,',round(report['omitted_bytes']/1048576,2),'MiB; source files retained')
 names=['data/'+p.relative_to(ROOT).as_posix() for p in files];assert len(names)==len(set(n.lower() for n in names))
 output.parent.mkdir(exist_ok=True,parents=True);temp=output.with_suffix('.tmp')
 with temp.open('wb') as f:
  f.write(b'PACK'+struct.pack('<I',0));entries=[]
  for p,name in zip(files,names):
   offset=f.tell()
   with p.open('rb') as src:shutil.copyfileobj(src,f)
   entries.append((name,offset,f.tell()-offset))
  index=f.tell()
  for name,offset,size in entries:
   b=name.encode()+b'\0';f.write(struct.pack('<III',12+len(b),offset,size)+b)
  f.write(struct.pack('<I',index))
 temp.replace(output)
 print(str(output),len(files),'files;',output.stat().st_size,'bytes')
if __name__=='__main__':
 p=argparse.ArgumentParser();p.add_argument('output',type=Path);p.add_argument('--light',action='store_true');a=p.parse_args();build(a.output.resolve(),a.light)
