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
 p=argparse.ArgumentParser();p.add_argument('pak',type=Path);p.add_argument('--output',type=Path,required=True);a=p.parse_args();a.output.mkdir(exist_ok=False,parents=True);source=unpack(a.pak.resolve(),a.output/'packed');s=Session(a.output/'run',Path('/workspace/squashfs-root/usr/bin/OpenBOR'),source=source);checks=[]
 def transition(key,target):
  mark=len(s.read())
  for _ in range(3):
   s.press(key)
   if target in s.read()[mark:]:return
  raise RuntimeError('Not reached '+target)
 try:
  s.wait('Level Loaded:',120)
  for _ in range(10):
   s.press(40)
   if 'NATIVE_PLAYER_READY' in s.read():break
  s.wait('NATIVE_PLAYER_READY');time.sleep(5);transition(40,'pause=2');s.capture('pause')
  transition(81,'option=1');transition(81,'option=2');transition(4,'view=2')
  for i in range(1,7):transition(79,'page='+str(i)+' ')
  s.capture('guide-page-7');times=re.findall(r'NATIVE_UI pause=2 view=2 .* time=(\d+)',s.read());assert len(set(times))==1
  for lang in range(1,7):transition(27,'lang='+str(lang)+' ')
  s.capture('guide-japanese');transition(7,'view=0');transition(7,'pause=0');checks=['Light PACK extracted and loaded in official Linux engine','Seven animated guide pages and seven Unicode languages loaded from the package','Simulation stays frozen in guide','Return from guide and pause works'];result={'passed':True,'checks':checks,'pak_bytes':a.pak.stat().st_size}
 except Exception as e:result={'passed':False,'checks':checks,'error':str(e)}
 finally:s.close();(a.output/'results.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result))
 raise SystemExit(not result['passed'])
