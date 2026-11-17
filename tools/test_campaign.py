#!/usr/bin/env python3
from concurrent.futures import ThreadPoolExecutor,as_completed
from pathlib import Path
from native_test import Session
from generate_interface import catalog
import time,json,argparse,re
p=argparse.ArgumentParser();p.add_argument('--output',required=True);p.add_argument('--new-only',action='store_true');p.add_argument('--levels',nargs='*');a=p.parse_args();out=Path(a.output);out.mkdir(exist_ok=False,parents=True)
levels=sorted(set(Path(e['path']).stem for mode in catalog() for e in mode['levels']))
if a.levels:levels=a.levels
if a.new_only:levels=[x['name'] for x in json.loads((Path(__file__).resolve().parents[1]/'tools/route-layout.json').read_text())]
def run(level):
 s=None
 try:
  s=Session(out/level,Path('/workspace/squashfs-root/usr/bin/OpenBOR'),level,all_soldiers=(level==levels[0]))
  s.wait('Level Loaded:',120)
  for _ in range(10):
   s.press(40)
   if 'NATIVE_PLAYER_READY' in s.read():break
  s.wait('NATIVE_PLAYER_READY',90);time.sleep(3);s.capture('entry');s.keys.write_text('4');time.sleep(3);s.keys.write_text('');s.capture('combat')
  log=s.read();bad=[l for l in log.splitlines() if 'returned an exception' in l or 'Script compile error' in l or "An Error Occurred" in l]
  if bad:raise RuntimeError('; '.join(bad))
  return {'level':level,'passed':True,'all_soldiers_forced_load':level==levels[0]}
 except Exception as e:return {'level':level,'passed':False,'error':str(e)}
 finally:
  if s:s.close()
results=[]
with ThreadPoolExecutor(max_workers=2) as pool:
 for future in as_completed([pool.submit(run,l) for l in levels]):
  r=future.result();results.append(r);print(json.dumps(r),flush=True);(out/'results.json').write_text(json.dumps(results,indent=2)+'\n')
print('PASSED',sum(x['passed'] for x in results),'/',len(results),flush=True)
raise SystemExit(any(not x['passed'] for x in results))
