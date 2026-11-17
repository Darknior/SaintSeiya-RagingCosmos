#!/usr/bin/env python3
"""Exercise native soldier animation frames and callbacks in an isolated fixture.
Frames are advanced explicitly; timing, damage balance and AI are not assessed.
"""
from pathlib import Path
import argparse,json,re,time
from native_test import Session,ROOT
from complete_soldiers import soldiers,REQUIRED,blocks

def main(out):
 cases=[]
 for name,path in soldiers():
  animations={b[1].lower():b[0] for b in blocks(path.read_text())}
  for anim in REQUIRED+(['block','blockpain'] if path.parent.name=='soldier_mud' else []):
   if anim not in animations:continue
   cases.append((name,anim,len(re.findall(r'^\s*frame\s',animations[anim],re.M))))
 s=Session(out,Path('/workspace/squashfs-root/usr/bin/OpenBOR'),'temple8C-road',all_soldiers=True)
 result={'cases':len(cases),'models':len(list(soldiers())),'explicit_frame_advance':True}
 try:
  directory=s.out/'data/levels';directory.unlink();directory.mkdir()
  for p in (ROOT/'levels').iterdir():
   if p.name!='temple8C-road.txt':(directory/p.name).symlink_to(p)
  (directory/'temple8C-road.txt').write_text('panel data/bgs/temple8C-road/temple8C-road.png\norder a\ndirection right\nsettime 999\nspawn1 50 30 0\nwait\nat 100000\n')
  funcs=''
  for func,column,typ in [('probeModel',0,'void'),('probeAnim',1,'void'),('probeFrames',2,'int')]:
   funcs+=typ+' '+func+'(int i){switch(i){\n'+''.join('case '+str(i)+':return '+(json.dumps(('DIE' if row[column]=='death' else row[column].upper()) if column==1 else row[column]) if column<2 else str(row[column]))+';\n' for i,row in enumerate(cases))+'}return '+('""' if column<2 else '0')+';}\n'
  probe='''
void soldierProbe(){
 if(!openborvariant("in_level")) return;
 void p=getplayerproperty(0,"entity");if(p==NULL()) return;
 changeentityproperty(p,"health",9999);
 int i=getglobalvar("anim_case");if(i==NULL()) i=0;
 if(i>=CASE_COUNT){if(!getglobalvar("anim_done")){log("ANIM_ALL_DONE\\n");setglobalvar("anim_done",1);}return;}
 int now=openborvariant("elapsed_time");int next=getglobalvar("anim_next");if(next!=NULL() && now<next)return;
 setglobalvar("anim_next",now+2);
 void e=getglobalvar("anim_entity");
 if(e==NULL()){
  log("ANIM_BEGIN "+i+" "+probeModel(i)+" "+probeAnim(i)+"\\n");
  clearspawnentry();setspawnentry("name",probeModel(i));e=spawn();
  if(e==NULL()){log("ANIM_FAILED spawn\\n");setglobalvar("anim_case",CASE_COUNT);return;}
  changeentityproperty(e,"position",450,430,0);
  int id=openborconstant("ANI_"+probeAnim(i));
  if(!getentityproperty(e,"animvalid",id)){log("ANIM_FAILED missing\\n");setglobalvar("anim_case",CASE_COUNT);return;}
  performattack(e,id,1);setglobalvar("anim_entity",e);setglobalvar("anim_frame",1);return;
 }
 int frame=getglobalvar("anim_frame");
 if(!getentityproperty(e,"exists")){log("ANIM_FAILED entity vanished\\n");setglobalvar("anim_case",CASE_COUNT);return;}
 if(frame<probeFrames(i)){updateframe(e,frame);setglobalvar("anim_frame",frame+1);return;}
 changeentityproperty(e,"lifespancountdown",-1);setglobalvar("anim_entity",NULL());setglobalvar("anim_case",i+1);
 log("ANIM_DONE "+i+"\\n");
}
'''.replace('CASE_COUNT',str(len(cases)))
  u=s.out/'data/scripts/update.c';u.write_text(u.read_text().replace(' nativeProbe();',' nativeProbe();soldierProbe();',1)+funcs+probe)
  s.wait('Level Loaded:',120)
  for _ in range(10):
   s.press(40)
   if 'NATIVE_PLAYER_READY' in s.read():break
  s.wait('NATIVE_PLAYER_READY');deadline=time.monotonic()+240
  while time.monotonic()<deadline:
   log=s.read()
   if 'ANIM_ALL_DONE' in log:break
   if s.proc.poll() is not None:raise RuntimeError('Engine stopped: '+log[-1200:])
   time.sleep(.2)
  else:raise RuntimeError('Timed out: '+s.read()[-800:])
  done=len(re.findall(r'^ANIM_DONE ',log,re.M));failures=re.findall(r'^ANIM_FAILED.*|^.*returned an exception.*',log,re.M)
  if done!=len(cases) or failures:raise RuntimeError(str(done)+' completed; '+str(failures)+' '+log[-600:])
  result.update(passed=True,completed=done,frames=sum(row[2] for row in cases))
 except Exception as e:result.update(passed=False,error=str(e))
 finally:s.close();(out/'results.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result),flush=True)
 return result['passed']
if __name__=='__main__':
 p=argparse.ArgumentParser();p.add_argument('--output',required=True,type=Path);a=p.parse_args();raise SystemExit(not main(a.output))
