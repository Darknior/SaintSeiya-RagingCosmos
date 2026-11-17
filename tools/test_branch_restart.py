#!/usr/bin/env python3
"""Check the native branch restart used after a boss time-over.
Opening movies are omitted only in the test copy; branch names are unchanged.
"""
from pathlib import Path
import argparse,subprocess,re,json,time
from native_test import Session,ROOT
if __name__=='__main__':
 p=argparse.ArgumentParser();p.add_argument('--output',required=True,type=Path);p.add_argument('--original-branches',action='store_true');a=p.parse_args();s=Session(a.output,Path('/workspace/squashfs-root/usr/bin/OpenBOR'),'temple3D-boss');result={}
 try:
  config=(s.out/'data/levels.txt').read_text()
  if a.original_branches:
   start=config.index('#-- Temple 3 Gemini');end=config.index('#-- Temple 4 Cancer');config=config[:start]+config[start:end].replace('branch      temple3BOSS','branch      temple2BOSS')+config[end:]
  config=re.sub(r'^\s*scene\s+[^\n]+','',config,flags=re.M);(s.out/'data/levels.txt').write_text(config)
  section=config.split('#-- Temple 3 Gemini',1)[1].split('#-- Temple 4 Cancer',1)[0];target=re.findall(r'^branch\s+(\S+)',section,re.M)[-1]
  probe='''
void restartProbe(){
 if(!openborvariant("in_level") || getplayerproperty(0,"entity")==NULL())return;
 if(getglobalvar("restart_sent")==NULL()){
  setglobalvar("restart_sent",1);log("BRANCH_RESTART_SENT\\n");jumptobranch("TARGET",1);
 }
}
'''.replace('TARGET',target)
  u=s.out/'data/scripts/update.c';u.write_text(u.read_text().replace(' nativeProbe();',' nativeProbe();restartProbe();',1)+probe)
  s.wait('Level Loaded:',120)
  for _ in range(10):
   s.press(40)
   if 'BRANCH_RESTART_SENT' in s.read():break
  s.wait('BRANCH_RESTART_SENT');deadline=time.monotonic()+80
  while time.monotonic()<deadline:
   log=s.read();loaded=re.findall(r"Level Loaded:\s+'([^']+)'",log)
   if len(loaded)>=2:break
   if s.proc.poll() is not None:raise RuntimeError('Engine stopped')
   time.sleep(.1)
  else:raise RuntimeError('No native reload')
  result={'passed':loaded[-1]=='data/levels/temple3D-boss.txt','original_branches':a.original_branches,'restart_target':target,'loaded':loaded}
 except Exception as e:result={'passed':False,'error':str(e)}
 finally:s.close();(a.output/'results.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result))
 raise SystemExit(not result['passed'])
