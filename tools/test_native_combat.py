#!/usr/bin/env python3
"""Input-only combat smoke tests: native HP, AI, damage and animation timing.
Private arenas isolate one soldier; the observer only reads game state.
Bot wins/losses are observations, not assertions about human difficulty.
"""
from pathlib import Path
from concurrent.futures import ThreadPoolExecutor,as_completed
import argparse,json,re,time
from native_test import Session,ROOT
from complete_soldiers import soldiers
PROBE='''
void combatProbe(){
 if(!openborvariant("in_level"))return;
 void p=getplayerproperty(0,"entity");if(p==NULL())return;
 if(getglobalvar("combat_player_guard")==NULL())setglobalvar("combat_player_guard",0);
 if(getglobalvar("combat_enemy_guard")==NULL())setglobalvar("combat_enemy_guard",0);
 int tick=openborvariant("elapsed_time");int next=getglobalvar("combat_next");if(next!=NULL() && tick<next)return;
 setglobalvar("combat_next",tick+10);
 void e=getglobalvar("combat_enemy");
 if(e==NULL()){
  int i;for(i=0;i<openborvariant("ent_max");i++){
   void candidate=getentity(i);
   if(candidate!=NULL() && getentityproperty(candidate,"exists") && getentityproperty(candidate,"type")==openborconstant("TYPE_ENEMY")){e=candidate;setglobalvar("combat_enemy",e);break;}
  }
 }
 if(e!=NULL() && getentityproperty(e,"exists")){
  int pa=getentityproperty(p,"animationid");int ea=getentityproperty(e,"animationid");
  if(pa==openborconstant("ANI_BLOCK") || pa==openborconstant("ANI_BLOCKPAIN"))setglobalvar("combat_player_guard",1);
  if(ea==openborconstant("ANI_BLOCK") || ea==openborconstant("ANI_BLOCKPAIN"))setglobalvar("combat_enemy_guard",1);
  log("COMBAT "+getentityproperty(p,"x")+" "+getentityproperty(p,"z")+" "+getentityproperty(p,"health")+" "+getentityproperty(p,"direction")+" "+getentityproperty(e,"x")+" "+getentityproperty(e,"z")+" "+getentityproperty(e,"health")+" "+getglobalvar("combat_player_guard")+" "+getglobalvar("combat_enemy_guard")+"\\n");
 }else if(e!=NULL())log("COMBAT_ENEMY_GONE\\n");
}
'''
def run(name,family,out,duration,hero):
 s=None;r={'model':name,'family':family,'hero':hero,'native_stats':True,'forced_animations':False,'controlled_defeats':False}
 try:
  s=Session(out/name,Path('/workspace/squashfs-root/usr/bin/OpenBOR'),'temple8C-road')
  config=s.out/'data/levels.txt';config.write_text(config.read_text().replace('skipselect seiya','skipselect '+hero))
  directory=s.out/'data/levels';directory.unlink();directory.mkdir()
  for p in (ROOT/'levels').iterdir():
   if p.name!='temple8C-road.txt':(directory/p.name).symlink_to(p)
  (directory/'temple8C-road.txt').write_text('panel data/bgs/temple8C-road/temple8C-road.png\norder a\ndirection right\nsettime 999\nspawn1 250 40 0\nspawn '+name+'\ncoords 460 430 0\nat 0\nwait\nat 100000\n')
  u=s.out/'data/scripts/update.c';u.write_text(u.read_text().replace(' nativeProbe();',' nativeProbe();combatProbe();',1)+PROBE)
  s.wait('Level Loaded:',120)
  for _ in range(10):
   s.press(40)
   if 'NATIVE_PLAYER_READY' in s.read():break
  s.wait('NATIVE_PLAYER_READY');start=time.monotonic();samples=[];guard_capture=False
  while time.monotonic()-start<duration:
   log=s.read();rows=re.findall(r'^COMBAT (.*)',log,re.M)
   if not rows:
    if s.proc.poll() is not None:raise RuntimeError('Engine stopped before telemetry')
    time.sleep(.1);continue
   values=rows[-1].replace('<VT_EMPTY>','0').split()
   try:state=[float(x) if x not in ('NULL','<NULL>') else 0 for x in values]
   except ValueError:raise RuntimeError('Bad combat telemetry: '+rows[-1])
   if len(state)!=9:raise RuntimeError('Bad telemetry width: '+rows[-1])
   samples.append(state);px,pz,ph,direction,ex,ez,eh,pg,eg=state
   if ph<=0 or eh<=0 or 'COMBAT_ENEMY_GONE' in log:break
   dt=time.monotonic()-start;keys=[]
   if dt<2.5:
    keys=[9]
    if (ex>px)!=(direction==1):keys.append(79 if ex>px else 80)
   else:
    if abs(ez-pz)>5:keys.append(81 if ez>pz else 82)
    if abs(ex-px)>65 or (ex>px)!=(direction==1):keys.append(79 if ex>px else 80)
    if int(dt/.16)%2==0:keys.append(4)
   s.keys.write_text(' '.join(map(str,keys)))
   if eg and not guard_capture:s.capture('enemy-guard');guard_capture=True
   if s.proc.poll() is not None:raise RuntimeError('Engine stopped during combat')
   time.sleep(.08)
  s.keys.write_text('');log=s.read();s.capture('combat-end')
  if not samples:raise RuntimeError('No combat samples')
  errors=[x for x in log.splitlines() if 'returned an exception' in x or 'Script compile error' in x]
  if errors:raise RuntimeError('; '.join(errors))
  p0=samples[0][2];e0=samples[0][6];pmin=min(x[2] for x in samples);emin=min(x[6] for x in samples)
  r.update(completed=True,enemy_initial_hp=e0,enemy_min_hp=emin,player_initial_hp=p0,player_min_hp=pmin,enemy_defeated=emin<=0 or 'COMBAT_ENEMY_GONE' in log,player_defeated=pmin<=0,player_guard_observed=any(x[7]>0 for x in samples),enemy_guard_observed=any(x[8]>0 for x in samples),damage_dealt=e0-emin,damage_received=p0-pmin,samples=len(samples))
 except Exception as e:r.update(completed=False,error=str(e))
 finally:
  if s:s.close()
 return r
if __name__=='__main__':
 p=argparse.ArgumentParser();p.add_argument('--output',required=True,type=Path);p.add_argument('--models',nargs='*');p.add_argument('--duration',type=int,default=35);p.add_argument('--hero',default='seiya');a=p.parse_args();a.output.mkdir(exist_ok=False,parents=True)
 chosen={}
 for name,path in soldiers():
  if a.models:
   if name in a.models:chosen[name]=(name,path.parent.name)
  else:chosen.setdefault(path.parent.name,(name,path.parent.name))
 results=[]
 with ThreadPoolExecutor(max_workers=2) as pool:
  for f in as_completed([pool.submit(run,n,fam,a.output,a.duration,a.hero) for n,fam in chosen.values()]):
   r=f.result();results.append(r);print(json.dumps(r),flush=True);(a.output/'results.json').write_text(json.dumps(results,indent=2)+'\n')
 raise SystemExit(any(not r['completed'] for r in results))
