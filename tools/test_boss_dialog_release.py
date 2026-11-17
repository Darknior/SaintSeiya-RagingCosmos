#!/usr/bin/env python3
"""Observe real boss intro timelines, without skipping or forcing their frames."""
from pathlib import Path
from concurrent.futures import ThreadPoolExecutor,as_completed
import argparse,json,time,re
from native_test import Session
PROBE='''
void bossDialogProbe(){
 if(!openborvariant("in_level"))return;
 void p=getplayerproperty(0,"entity");if(p==NULL())return;
 if(getentityvar(p,"dialog_hidden")==1){
  if(!getglobalvar("boss_hidden_seen")){setglobalvar("boss_hidden_seen",1);log("BOSS_DIALOG_HIDDEN\\n");}
 }else if(getglobalvar("boss_hidden_seen") && getglobalvar("lockButton")!=1 && getentityproperty(p,"z")>0){
  if(!getglobalvar("boss_released")){setglobalvar("boss_released",1);log("BOSS_DIALOG_RELEASED z="+getentityproperty(p,"z")+"\\n");}
 }
}
'''
def run(level,out):
 s=Session(out/level,Path('/workspace/squashfs-root/usr/bin/OpenBOR'),level);r={'level':level,'native_timeline_timing':True}
 try:
  probe=PROBE
  if level=='temple2D-boss':
   # The first-frame intro can precede a joining player's creation. Spawn the
   # native boss after join, as when arriving from a prior campaign stage.
   directory=s.out/'data/levels';directory.unlink();directory.mkdir()
   for f in (Path(__file__).resolve().parents[1]/'levels').iterdir():
    if f.name!=level+'.txt':(directory/f.name).symlink_to(f)
   text=(Path(__file__).resolve().parents[1]/'levels'/f'{level}.txt').read_text()
   text=re.sub(r'^spawn\s+aldebaran\n.*?^at\s+0[^\n]*','',text,flags=re.M|re.S)
   (directory/f'{level}.txt').write_text(text+'\nwait\nat 100000\n')
   probe=probe.replace(' if(getentityvar(p,"dialog_hidden")==1){',''' if(!getglobalvar("boss_spawned_after_join")){
    setglobalvar("boss_spawned_after_join",1);clearspawnentry();setspawnentry("name","aldebaran");setspawnentry("boss",1);void boss=spawn();changeentityproperty(boss,"position",650,400,0);
   }
   if(getentityvar(p,"dialog_hidden")==1){''')
   r['boss_spawned_after_join']=True
  u=s.out/'data/scripts/update.c';u.write_text(u.read_text().replace(' nativeProbe();',' nativeProbe();bossDialogProbe();',1)+probe)
  s.wait('Level Loaded:',120)
  for _ in range(10):
   s.press(40)
   if 'NATIVE_PLAYER_READY' in s.read():break
  s.wait('BOSS_DIALOG_HIDDEN',50);log=s.wait('BOSS_DIALOG_RELEASED',100);s.capture('after-dialog')
  if 'returned an exception' in log:raise RuntimeError('Script exception')
  r['passed']=True
 except Exception as e:r.update(passed=False,error=str(e))
 finally:s.close()
 return r
if __name__=='__main__':
 p=argparse.ArgumentParser();p.add_argument('--output',type=Path,required=True);a=p.parse_args();a.output.mkdir(exist_ok=False,parents=True);results=[]
 with ThreadPoolExecutor(max_workers=2) as pool:
  for f in as_completed([pool.submit(run,level,a.output) for level in ['temple2D-boss','temple13D-boss']]):
   r=f.result();results.append(r);print(json.dumps(r),flush=True);(a.output/'results.json').write_text(json.dumps(results,indent=2)+'\n')
 raise SystemExit(any(not r['passed'] for r in results))
