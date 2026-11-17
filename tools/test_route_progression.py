#!/usr/bin/env python3
"""Native route event/exit test, with controlled defeats in a private fixture.
Uses real rightward input. Expiry of enemies is injected solely to check event
ordering and exits; this is deliberately not a balance or complete combat test.
"""
from pathlib import Path
from concurrent.futures import ThreadPoolExecutor,as_completed
import argparse,json,re,time
from native_test import Session,ROOT
from fix_stage_geometry import ranges
PROBE='''
void routeProbe(){
 if(!openborvariant("in_level")) return;
 int stage=openborvariant("current_level");
 int initial=getglobalvar("progress_stage");
 if(initial!=NULL() && stage+1>initial){if(!getglobalvar("progress_done")){log("ROUTE_EXIT_OK\\n");setglobalvar("progress_done",1);}return;}
 void p=getplayerproperty(0,"entity");if(p==NULL()) return;
 if(initial==NULL()) setglobalvar("progress_stage",stage+1);
 changeentityproperty(p,"health",9999);
 int i;int count=openborvariant("ent_max");
 for(i=0;i<count;i++){
  void e=getentity(i);
  if(e!=NULL() && getentityproperty(e,"exists") && getentityproperty(e,"type")==openborconstant("TYPE_ENEMY")){
   if(!getentityvar(e,"progress_seen")){
    log("ROUTE_ENEMY x="+getentityproperty(e,"x")+" z="+getentityproperty(e,"z")+" scroll="+openborvariant("xpos")+"\\n");
    setentityvar(e,"progress_seen",1);
   }
   changeentityproperty(e,"lifespancountdown",-1);
  }
 }
 int bucket=openborvariant("elapsed_time")/200;
 if(getglobalvar("progress_bucket")!=bucket+1){
  log("ROUTE_POSITION x="+getentityproperty(p,"x")+" z="+getentityproperty(p,"z")+" scroll="+openborvariant("xpos")+" waiting="+openborvariant("waiting")+"\\n");
  setglobalvar("progress_bucket",bucket+1);
 }
}
'''
def run(route,out,timeout):
 name=route['name'];s=None;result={'route':name,'expected_enemies':route['soldiers'],'controlled_defeats':True}
 try:
  s=Session(out/name,Path('/workspace/squashfs-root/usr/bin/OpenBOR'),name)
  u=s.out/'data/scripts/update.c';t=u.read_text().replace(' nativeProbe();',' nativeProbe(); routeProbe();',1);u.write_text(t+PROBE)
  # Engine is still loading models. The fixture starts the real route and then
  # its actual Story successor, with no intervening opening cinematics.
  config=(ROOT/'levels.txt').read_text();story=config.split('set         Story_Mode',1)[1].split('\nend',1)[0]
  paths=re.findall(r'^file\s+data/(\S+)',story,re.M);path=f'levels/{name}.txt';successor=paths[paths.index(path)+1]
  bounds=ranges(config);lo,hi=bounds[successor][0][1]
  fixture=s.out/'data/levels.txt';header=fixture.read_text().split('set         Story_Mode',1)[0]
  fixture.write_text(header+'set Story_Mode\nmaxplayers 4\nlives 4\ncredits 10\ncansave 0\nskipselect seiya\nz '+str(s.bounds[0])+' '+str(s.bounds[1])+f'\nfile data/{path}\nz {lo} {hi}\nfile data/{successor}\nend\n')
  s.wait('Level Loaded:',120)
  for _ in range(10):
   s.press(40)
   if 'NATIVE_PLAYER_READY' in s.read():break
  s.wait('NATIVE_PLAYER_READY');s.keys.write_text('79');deadline=time.monotonic()+timeout
  while time.monotonic()<deadline:
   log=s.read()
   if 'ROUTE_EXIT_OK' in log:break
   if s.proc.poll() is not None:raise RuntimeError('Engine stopped')
   time.sleep(.25)
  else:raise RuntimeError('No natural exit: '+s.read()[-900:])
  s.keys.write_text('');log=s.read();seen=len(re.findall(r'^ROUTE_ENEMY ',log,re.M))
  if seen!=route['soldiers']:raise RuntimeError(f'Only {seen}/{route["soldiers"]} native spawns observed')
  if f"'data/{successor}'" not in log:raise RuntimeError('Expected successor did not load')
  scrolls=[float(x) for x in re.findall(r'^ROUTE_ENEMY .* scroll=([\d.]+)',log,re.M)]
  wave_counts=[sum(abs(s-trigger)<1 for s in scrolls) for trigger in route['triggers']]
  if wave_counts!=[2]+[3]*(route['encounters']-1):raise RuntimeError('Unexpected wave positions/counts: '+str(wave_counts))

  s.capture('next-stage');result.update(passed=True,observed_enemies=seen,successor=successor,wave_counts=wave_counts,positions=re.findall(r'^ROUTE_POSITION .*',log,re.M))
 except Exception as e:result.update(passed=False,error=str(e))
 finally:
  if s:s.close()
 return result
if __name__=='__main__':
 p=argparse.ArgumentParser();p.add_argument('--output',type=Path,required=True);p.add_argument('--levels',nargs='*');p.add_argument('--timeout',type=int,default=120);a=p.parse_args();a.output.mkdir(exist_ok=False,parents=True)
 routes=json.loads((ROOT/'tools/route-layout.json').read_text());routes=[r for r in routes if not a.levels or r['name'] in a.levels];results=[]
 with ThreadPoolExecutor(max_workers=2) as pool:
  for f in as_completed([pool.submit(run,r,a.output,a.timeout) for r in routes]):
   r=f.result();results.append(r);print(json.dumps(r),flush=True);(a.output/'results.json').write_text(json.dumps(results,indent=2)+'\n')
 raise SystemExit(any(not r['passed'] for r in results))
