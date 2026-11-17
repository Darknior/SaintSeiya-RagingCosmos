#!/usr/bin/env python3
"""Verify native menu localization, fonts and retained stock navigation."""
from pathlib import Path
from concurrent.futures import ThreadPoolExecutor,as_completed
import argparse,json,time
from native_test import Session,ROOT
CODES=['en','fr','es','pt','zh','ru','ja']
def run(code,out,source=ROOT):
 lang=CODES.index(code);s=None;r={'language':code}
 def setup(session):
  saved=session.out/'Saves/bor';saved.mkdir(exist_ok=True,parents=True);(saved/'ragingcosmos-language.txt').write_text(str(lang))
  u=session.out/'data/scripts/update.c';u.write_text(u.read_text().replace(' nativeProbe();',' nativeProbe();nativeMenuProbe();',1)+'''
void nativeMenuProbe(){
 int state=(openborvariant("in_options")!=0)+(openborvariant("in_video_options")!=0)*2+(openborvariant("in_sound_options")!=0)*4+(openborvariant("in_control_options")!=0)*8;
 if(getglobalvar("menu_probe")!=state+1){log("NATIVE_OPTIONS "+state+"\\n");setglobalvar("menu_probe",state+1);}
}
''')
 try:
  s=Session(out/code,Path('/workspace/squashfs-root/usr/bin/OpenBOR'),title=True,setup=setup,source=source)
  s.wait('title=1',120);s.wait('lang='+str(lang)+' ',120);s.capture('native-title')
  for name in ['menu.txt','translation.txt']:
   assert (s.out/'translation'/name).read_bytes().rstrip(b'\r\n')==(source/'translations/native'/code/name).read_bytes().rstrip(b'\r\n'),name+' export mismatch'
  s.press(40);s.wait('menu=1');time.sleep(.5);s.capture('native-main')
  s.press(81);s.press(81);s.press(4);s.wait('view=2');s.capture('native-howto')
  s.press(79);s.wait('page=1 ');s.press(7);s.wait('view=0');time.sleep(1.5)
  s.press(82);s.press(4);s.wait('NATIVE_OPTIONS 1');s.capture('native-options');time.sleep(1.5)
  for attempt in range(3):
   s.press(4)
   if 'NATIVE_OPTIONS 3' in s.read():break
   time.sleep(1)
  s.wait('NATIVE_OPTIONS 3');s.capture('native-video')
  r.update(passed=True,menu_files_exported=True,native_root_options_video=True,existing_howto_entry=True)
 except Exception as e:r.update(passed=False,error=str(e))
 finally:
  if s:s.close()
 return r
if __name__=='__main__':
 p=argparse.ArgumentParser();p.add_argument('--output',type=Path,required=True);p.add_argument('--codes',nargs='*',default=CODES);a=p.parse_args();a.output.mkdir(exist_ok=False,parents=True);results=[]
 with ThreadPoolExecutor(max_workers=2) as pool:
  for f in as_completed([pool.submit(run,c,a.output) for c in a.codes]):
   r=f.result();results.append(r);print(json.dumps(r),flush=True);(a.output/'results.json').write_text(json.dumps(results,indent=2)+'\n')
 raise SystemExit(any(not r['passed'] for r in results))
