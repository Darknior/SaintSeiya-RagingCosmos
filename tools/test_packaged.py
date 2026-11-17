#!/usr/bin/env python3
from pathlib import Path
import struct,argparse,time,json,re
from native_test import Session

def unpack(pak,out):
 out.mkdir(exist_ok=False,parents=True)
 with pak.open('rb') as f:
  assert f.read(4)==b'PACK';f.seek(-4,2);end=f.tell();index=struct.unpack('<I',f.read(4))[0];f.seek(index);entries=[]
  while f.tell()<end:
   length,offset,size=struct.unpack('<III',f.read(12));name=f.read(length-12).rstrip(b'\0').decode();target=(out/name).resolve();assert target.is_relative_to(out.resolve());assert offset+size<=index;entries.append((target,offset,size))
  for target,offset,size in entries:
   target.parent.mkdir(exist_ok=True,parents=True);f.seek(offset);target.write_bytes(f.read(size))
 return out/'data'
if __name__=='__main__':
 from test_native_localized_menus import run
 p=argparse.ArgumentParser();p.add_argument('pak',type=Path);p.add_argument('--output',type=Path,required=True);a=p.parse_args();a.output.mkdir(exist_ok=False,parents=True);source=unpack(a.pak.resolve(),a.output/'packed')
 menu=run('fr',a.output/'menus',source=source);s=None;checks=[]
 def setup(session):
  save=session.out/'Saves/bor';save.mkdir(parents=True,exist_ok=True);(save/'ragingcosmos-language.txt').write_text('1')
  external=session.out/'translation';external.mkdir()
  for name in ['translation.txt','menu.txt']:(external/name).write_bytes((source/'translations/native/fr'/name).read_bytes())
 def transition(key,target):
  mark=len(s.read())
  for _ in range(5):
   s.press(key)
   if target in s.read()[mark:]:return
  raise RuntimeError('Not reached '+target)
 try:
  assert menu['passed'],menu
  s=Session(a.output/'run',Path('/workspace/squashfs-root/usr/bin/OpenBOR'),source=source,setup=setup)
  s.wait('Level Loaded:',120)
  for _ in range(12):
   s.press(40)
   if 'NATIVE_PLAYER_READY' in s.read():break
  s.wait('NATIVE_PLAYER_READY');time.sleep(5);transition(40,'pause=2');s.capture('pause-native-fr')
  assert 'view=2' not in s.read(),'A custom pause guide was opened'
  transition(40,'pause=0');time.sleep(1);transition(40,'pause=2');s.capture('pause-native-fr-again')
  checks=['Extracted final PAK loads in official Linux engine','Native French menus/options/video and existing How To Play work','Native two-entry pause resumes and reopens; no custom pause submenu']
  result={'passed':True,'checks':checks,'pak_bytes':a.pak.stat().st_size}
 except Exception as e:result={'passed':False,'checks':checks,'error':str(e)}
 finally:
  if s:s.close()
  (a.output/'results.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result))
 raise SystemExit(not result['passed'])
