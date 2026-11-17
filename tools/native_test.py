#!/usr/bin/env python3
from pathlib import Path
import argparse,os,subprocess,time,signal,shutil,json,re
ROOT=Path(__file__).resolve().parents[1]
class Session:
 def __init__(self,out,engine,level='temple8A-road',title=False,source=ROOT,all_soldiers=False):
  self.out=Path(out);self.out.mkdir(exist_ok=False,parents=True);data=self.out/'data';data.mkdir();self.keys=self.out/'keys.txt';self.keys.write_text('');self.log=self.out/'Logs/OpenBorLog.txt'
  for p in source.iterdir():
   if p.name.startswith('.') or p.name in ('tools','scripts','levels.txt','scenes','models.txt'):continue
   (data/p.name).symlink_to(p,target_is_directory=p.is_dir())
  model_text=(source/'models.txt').read_text()
  if all_soldiers:model_text=re.sub(r'^know(\s+\S+\s+data/chars/soldier_[^\n]+)',r'load\1',model_text,flags=re.M)
  (data/'models.txt').write_text(model_text)
  scripts=data/'scripts';scripts.mkdir()
  for p in (source/'scripts').iterdir():
   if p.name!='update.c':(scripts/p.name).symlink_to(p,target_is_directory=p.is_dir())
  original=(source/'scripts/update.c').read_text().replace('void main()\n{','void main()\n{\n nativeProbe();',1)
  original+='\nvoid nativeProbe(){\n if(openborvariant("in_level")){changeopenborvariant("skiptoset",-1);void p=getplayerproperty(0,"entity");if(p!=NULL() && getglobalvar("test_player")==NULL()){log("NATIVE_PLAYER_READY\\n");log("NATIVE_POSITION z="+getentityproperty(p,"z")+" min="+openborvariant("player_min_z")+" max="+openborvariant("player_max_z")+"\\n");setglobalvar("test_player",1);}}\n int paused=openborvariant("game_paused");int view=getglobalvar("ui_view");int page=getglobalvar("ui_page");int lang=getglobalvar("ui_lang");int option=getglobalvar("ui_option");int title=openborvariant("in_titlescreen")!=0;int menu=openborvariant("in_menuscreen")!=0;int confirmed=getglobalvar("ui_confirmed");\n if(view==NULL()) view=0;if(page==NULL()) page=0;if(lang==NULL()) lang=0;if(option==NULL()) option=0;if(confirmed==NULL()) confirmed=0;\n int signature=paused+view*10+page*100+lang*1000+option*10000+title*100000+menu*1000000+confirmed*10000000;\n if(getglobalvar("test_signature")!=signature+1){log("NATIVE_UI pause="+paused+" view="+view+" page="+page+" lang="+lang+" option="+option+" title="+title+" menu="+menu+" confirmed="+confirmed+" time="+openborvariant("elapsed_time")+"\\n");setglobalvar("test_signature",signature+1);}\n if(paused==2 && playerkeys(0,1,"attack4")){uiRelease();setglobalvar("ui_lang",(lang+1)%7);}\n}\n'
  original='#import "data/scripts/interface.c"\n'+original
  (scripts/'update.c').write_text(original)
  config=(source/'levels.txt').read_text().replace('skipselect\nselect data/select-all.txt','skipselect seiya\nskipselect seiya')
  bounds=(378,484)
  for line in config.splitlines():
   w=line.split()
   if not w:continue
   if w[0]=='z':bounds=tuple(map(int,w[1:3]))
   if w[0]=='file' and w[1]==f'data/levels/{level}.txt':break
  self.bounds=bounds
  config=config.replace('file        data/levels/temple0.txt',f'z {bounds[0]} {bounds[1]}\nfile        data/levels/{level}.txt',1)
  (data/'levels.txt').write_text(('' if title else 'skiptoset 1\n')+config)
  scenes=data/'scenes';scenes.mkdir()
  for p in (source/'scenes').iterdir():
   if p.name not in ('intro.txt','logo.txt'):(scenes/p.name).symlink_to(p)
  (scenes/'intro.txt').write_text('# test skips introductory video\n');(scenes/'logo.txt').write_text('# test skips logo\n')
  (self.out/'bor.pak').touch();self.capture_file=self.out/'capture.ppm'
  env=dict(os.environ,SDL_VIDEODRIVER='dummy',SDL_RENDER_DRIVER='software',SDL_AUDIODRIVER='dummy',LD_PRELOAD='/workspace/runtime/input_bridge.so',TEST_KEY_FILE=str(self.keys),TEST_CAPTURE=str(self.capture_file))
  self.env=env;self.engine=engine
  self.stdout=(self.out/'stdout.txt').open('w');self.proc=subprocess.Popen([str(engine),str(self.out/'bor.pak')],cwd=self.out,env=env,stdout=self.stdout,stderr=subprocess.STDOUT)
 def read(self):return self.log.read_text(errors='replace') if self.log.exists() else ''
 def wait(self,text,timeout=90,start=0):
  end=time.monotonic()+timeout
  while time.monotonic()<end:
   s=self.read()[start:]
   if text in s:return s
   if self.proc.poll() is not None:raise RuntimeError('Engine stopped: '+s[-1700:])
   time.sleep(.1)
  raise RuntimeError('Timeout '+text+' '+self.read()[-1200:])
 def press(self,key):self.keys.write_text(str(key));time.sleep(.7);self.keys.write_text('');time.sleep(.7)
 def capture(self,name):
  from PIL import Image
  self.capture_file.unlink(missing_ok=True);self.proc.send_signal(signal.SIGUSR1);end=time.monotonic()+12
  while time.monotonic()<end:
   if self.capture_file.exists() and self.capture_file.stat().st_size>=1152015:
    Image.open(self.capture_file).save(self.out/(name+'.png'));return
   time.sleep(.1)
  raise RuntimeError('Capture timeout')
 def restart(self):
  self.close();self.log.unlink(missing_ok=True);self.stdout=(self.out/'stdout-restart.txt').open('w');self.proc=subprocess.Popen([str(self.engine),str(self.out/'bor.pak')],cwd=self.out,env=self.env,stdout=self.stdout,stderr=subprocess.STDOUT)
 def close(self):
  self.keys.write_text('')
  if self.proc.poll() is None:self.proc.terminate()
  try:self.proc.wait(timeout=6)
  except subprocess.TimeoutExpired:self.proc.kill();self.proc.wait()
  self.stdout.close()
if __name__=='__main__':
 p=argparse.ArgumentParser();p.add_argument('--output',required=True);p.add_argument('--level',default='temple8A-road');p.add_argument('--title',action='store_true');args=p.parse_args();s=Session(args.output,Path('/workspace/squashfs-root/usr/bin/OpenBOR'),args.level,args.title);result={}
 try:
  if args.title:time.sleep(15);s.capture('title')
  else:
   s.wait('Level Loaded:',120)
   for _ in range(12):
    s.press(40)
    if 'NATIVE_PLAYER_READY' in s.read():break
   s.wait('NATIVE_PLAYER_READY');time.sleep(2);s.capture('game')
  result={'passed':True}
 except Exception as e:result={'passed':False,'error':str(e)}
 finally:s.close();(s.out/'results.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result))
 raise SystemExit(not result['passed'])
