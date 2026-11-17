#!/usr/bin/env python3
"""Traverse restored routes using only keyboard inputs and native combat.
Unlike test_route_progression, no enemy expiry or player health changes occur.
"""
from pathlib import Path
from concurrent.futures import ThreadPoolExecutor,as_completed
import argparse,json,re,time
from native_test import Session,ROOT
from fix_stage_geometry import ranges
PROBE='''
void routeCombatProbe(){
 if(!openborvariant("in_level"))return;
 void p=getplayerproperty(0,"entity");if(p==NULL())return;
 int tick=openborvariant("elapsed_time");int next=getglobalvar("route_combat_next");if(next!=NULL() && tick<next)return;
 setglobalvar("route_combat_next",tick+10);
 void target=NULL();float best=9999999;int i;
 float px=getentityproperty(p,"x");float pz=getentityproperty(p,"z");
 for(i=0;i<openborvariant("ent_max");i++){
  void e=getentity(i);
  if(e!=NULL() && getentityproperty(e,"exists") && getentityproperty(e,"type")==openborconstant("TYPE_ENEMY")){
   if(!getentityvar(e,"route_observed")){log("NATIVE_ROUTE_ENEMY\\n");setentityvar(e,"route_observed",1);}
   if(getentityproperty(e,"health")>0){
    float dx=getentityproperty(e,"x")-px;float dz=getentityproperty(e,"z")-pz;
    if(dx<0)dx=-dx;if(dz<0)dz=-dz;
    if(dx+dz*2<best){target=e;best=dx+dz*2;}
   }
  }
 }
 float ex=px+500;float ez=pz;int eh=0;
 if(target!=NULL()){ex=getentityproperty(target,"x");ez=getentityproperty(target,"z");eh=getentityproperty(target,"health");}
 log("NATIVE_ROUTE "+px+" "+pz+" "+getentityproperty(p,"health")+" "+getentityproperty(p,"direction")+" "+ex+" "+ez+" "+eh+" "+openborvariant("current_level")+" "+getplayerproperty(0,"lives")+"\\n");
}
'''
def run(route,out,timeout):
 name=route['name'];s=None;r={'route':name,'input_only':True,'native_stats':True,'controlled_defeats':False}
 try:
  s=Session(out/name,Path('/workspace/squashfs-root/usr/bin/OpenBOR'),name)
  config=(ROOT/'levels.txt').read_text();story=config.split('set         Story_Mode',1)[1].split('\nend',1)[0];paths=re.findall(r'^file\s+data/(\S+)',story,re.M);path=f'levels/{name}.txt';successor=paths[paths.index(path)+1];lo,hi=ranges(config)[successor][0][1]
  fixture=s.out/'data/levels.txt';header=fixture.read_text().split('set         Story_Mode',1)[0]
  fixture.write_text(header+'set Story_Mode\nmaxplayers 4\nlives 4\ncredits 10\ncansave 0\nskipselect seiya\nz '+str(s.bounds[0])+' '+str(s.bounds[1])+f'\nfile data/{path}\nz {lo} {hi}\nfile data/{successor}\nend\n')
  u=s.out/'data/scripts/update.c';u.write_text(u.read_text().replace(' nativeProbe();',' nativeProbe();routeCombatProbe();',1)+PROBE)
  s.wait('Level Loaded:',120)
  for _ in range(10):
   s.press(40)
   if 'NATIVE_PLAYER_READY' in s.read():break
  s.wait('NATIVE_PLAYER_READY');start=time.monotonic();initial=None;samples=[];won=False
  while time.monotonic()-start<timeout:
   log=s.read();rows=re.findall(r'^NATIVE_ROUTE (.*)',log,re.M)
   if f"Level Loaded:    'data/{successor}'" in log:won=True;break
   if s.proc.poll() is not None:raise RuntimeError('Engine stopped')
   if not rows:time.sleep(.1);continue
   state=list(map(float,rows[-1].split()));samples.append(state);px,pz,hp,direction,ex,ez,eh,stage,lives=state
   if initial is None:initial=stage
   if stage>initial and f"'data/{successor}'" in log:won=True;break
   dt=time.monotonic()-start;keys=[]
   if eh>0:
    if abs(ez-pz)>5:keys.append(81 if ez>pz else 82)
    if abs(ex-px)>65 or (ex>px)!=(direction==1):keys.append(79 if ex>px else 80)
    if int(dt/.16)%2==0:keys.append(4)
   else:keys=[79]
   s.keys.write_text(' '.join(map(str,keys)));time.sleep(.08)
  s.keys.write_text('');s.capture('route-end');log=s.read();seen=len(re.findall(r'^NATIVE_ROUTE_ENEMY$',log,re.M))
  if not won:raise RuntimeError('Route not completed by bot; last state '+str(samples[-1] if samples else [])+'; observed enemies '+str(seen))
  # Exclude observations of the successor, which may spawn its own entry guard.
  prefix=log.split(f"Level Loaded:    'data/{successor}'")[0];seen=len(re.findall(r'^NATIVE_ROUTE_ENEMY$',prefix,re.M))
  if seen!=route['soldiers']:raise RuntimeError(f'Unexpected enemy count: {seen}/{route["soldiers"]}')
  bad=[x for x in log.splitlines() if 'returned an exception' in x or 'Script compile error' in x]
  if bad:raise RuntimeError('; '.join(bad))
  before=[x for x in samples if x[7]==initial]
  r.update(passed=True,observed_enemies=seen,successor=successor,seconds=round(time.monotonic()-start,1),minimum_player_hp=min(x[2] for x in before),initial_lives=before[0][8],minimum_lives=min(x[8] for x in before))
 except Exception as e:r.update(passed=False,error=str(e))
 finally:
  if s:s.close()
 return r
if __name__=='__main__':
 p=argparse.ArgumentParser();p.add_argument('--output',required=True,type=Path);p.add_argument('--levels',nargs='*');p.add_argument('--timeout',type=int,default=180);a=p.parse_args();a.output.mkdir(exist_ok=False,parents=True)
 routes=json.loads((ROOT/'tools/route-layout.json').read_text());routes=[r for r in routes if not a.levels or r['name'] in a.levels];results=[]
 with ThreadPoolExecutor(max_workers=2) as pool:
  for f in as_completed([pool.submit(run,r,a.output,a.timeout) for r in routes]):
   r=f.result();results.append(r);print(json.dumps(r),flush=True);(a.output/'results.json').write_text(json.dumps(results,indent=2)+'\n')
 raise SystemExit(any(not r['passed'] for r in results))
