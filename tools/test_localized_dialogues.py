#!/usr/bin/env python3
"""Render every translated dialogue page with the production native window."""
from pathlib import Path
import argparse,json,time,re
from native_test import Session,ROOT
CODES=['en','fr','es','pt','zh','ru','ja']
if __name__=='__main__':
 p=argparse.ArgumentParser();p.add_argument('--output',required=True,type=Path);a=p.parse_args();source=json.loads((ROOT/'translations/native-source.json').read_text());pages=[v['lines'] for k,v in source.items() if k.startswith('dialog:')];count=len(pages);result={}
 def setup(s):
  data=s.out/'data';models=data/'models.txt'
  (data/'test_dialog.txt').write_text('name loc_dialog\ntype text\nshadow 0\nondrawscript data/scripts/dialogs/dialog.c\nanim idle\nloop 1\ndelay 10\noffset 0 0\nframe data/chars/dialogs/empty.png\n')
  (data/'test_anchor.txt').write_text('name loc_anchor\ntype enemy\nnomove 1 1\nhealth 1\nshadow 0\nanim idle\nloop 1\ndelay 10\nframe data/chars/dialogs/empty.png\n')
  models.write_text(models.read_text()+'\nload loc_dialog data/test_dialog.txt\nload loc_anchor data/test_anchor.txt\n')
  directory=data/'levels';directory.unlink();directory.mkdir()
  for f in (ROOT/'levels').iterdir():
   if f.name!='temple8C-road.txt':(directory/f.name).symlink_to(f)
  (directory/'temple8C-road.txt').write_text('noslow 1\npanel data/bgs/temple8C-road/temple8C-road.png\norder a\ndirection right\nsettime 999\nspawn1 50 30 0\nspawn loc_anchor\ncoords 600 430 0\nat 0\nwait\nat 100000\n')
  f=data/'scripts/localization.c';f.unlink();t=(ROOT/'scripts/localization.c').read_text();t=t.replace('if(page<0)return 0;','if(page<0){log("LOC_PAGE_MISSING\\n");return 0;}');t=t.replace('    if(sprite!=NULL())drawsprite(sprite,x,y,layer);','    if(typeof(sprite)!=openborconstant("VT_PTR")){log("LOC_BAD_SPRITE "+path+"\\n");} log("LOC_LOAD "+path+"\\n"); if(sprite!=NULL())drawsprite(sprite,x,y,layer);');t=t.replace('    return 1;\n}', '    log("LOC_PAGE "+getglobalvar("ui_lang")+" "+page+"\\n");\n    return 1;\n}');f.write_text(t)
  code='void setLocPage(int i){switch(i){\n'
  for i,page in enumerate(pages):code+='case '+str(i)+':'+''.join('setglobalvar("line'+str(j+1)+'",'+json.dumps(text)+');' for j,text in enumerate(page))+'break;\n'
  code+='}}\n'
  code+='''
void localizationProbe(){
 if(!openborvariant("in_level"))return;
 void p=getplayerproperty(0,"entity");if(p==NULL())return;
 int now=openborvariant("ticks");int next=getglobalvar("loc_test_next");if(next!=NULL() && now<next)return;
 setglobalvar("loc_test_next",now+60);
 int step=getglobalvar("loc_test_step");if(step==NULL())step=0;
 if(step==0){
  clearspawnentry();setspawnentry("name","loc_dialog");void e=spawn();changeentityproperty(e,"position",200,100,0);setglobalvar("loc_test_box",e);
  setglobalvar("portraitName","seiya1");setglobalvar("portraitSide","left");setglobalvar("lockButton",0);
  log("LOC_BOSS_SLOW "+get_level_property(NULL(),openborconstant("LEVEL_PROPERTY_BOSS_SLOW"))+"\\n");
 }
 if(step<COUNT*7){
  int lang=0;if(step>=COUNT)lang=1;if(step>=COUNT*2)lang=2;if(step>=COUNT*3)lang=3;if(step>=COUNT*4)lang=4;if(step>=COUNT*5)lang=5;if(step>=COUNT*6)lang=6;int page=step%COUNT;setglobalvar("ui_lang",lang);setLocPage(page);
  log("LOC_REQUEST "+lang+" "+page+"\\n");
 }else if(step==COUNT*7){
  setglobalvar("line1",NULL());setglobalvar("line2",NULL());setglobalvar("line3",NULL());setglobalvar("line4",NULL());changeentityproperty(getglobalvar("loc_test_box"),"lifespancountdown",-1);
  loadmodel("mu_dialog");clearspawnentry();setspawnentry("name","mu_dialog");void mu=spawn();changeentityproperty(mu,"position",120,80,0);setglobalvar("loc_test_mu",mu);
 }else if(step<COUNT*7+1+77){
  int index=step-COUNT*7-1;int lang=0;if(index>=11)lang=1;if(index>=22)lang=2;if(index>=33)lang=3;if(index>=44)lang=4;if(index>=55)lang=5;if(index>=66)lang=6;int item=index%11;setglobalvar("ui_lang",lang);
  void mu=getglobalvar("loc_test_mu");int anim=openborconstant("ANI_IDLE");if(item>0)anim=openborconstant("ANI_FOLLOW"+item);performattack(mu,anim,1);
  log("LOC_MU "+lang+" "+item+"\\n");
 }else if(step==COUNT*7+1+77){log("LOC_RENDER_ALL_DONE\\n");}
 setglobalvar("loc_test_step",step+1);
}
'''.replace('COUNT',str(count))
  u=data/'scripts/update.c';u.write_text(u.read_text().replace(' nativeProbe();',' nativeProbe();localizationProbe();',1)+code)
 s=Session(a.output,Path('/workspace/squashfs-root/usr/bin/OpenBOR'),'temple8C-road',setup=setup)
 try:
  s.wait('Level Loaded:',120)
  for _ in range(10):
   s.press(40)
   if 'NATIVE_PLAYER_READY' in s.read():break
  start=time.monotonic();captured=set()
  while time.monotonic()-start<160:
   log=s.read()
   if 'LOC_RENDER_ALL_DONE' in log:break
   if s.proc.poll() is not None:raise RuntimeError('Engine stopped: '+log[-700:])
   requests=re.findall(r'^LOC_REQUEST (\d+) (\d+)',log,re.M)
   if requests:
    lang,page=map(int,requests[-1])
    if page in (3,4,5) and lang not in captured:s.capture('dialog-'+CODES[lang]);captured.add(lang)
   mus=re.findall(r'^LOC_MU (\d+) (\d+)',log,re.M)
   if mus:
    lang,item=map(int,mus[-1]);key='mu'+str(lang)
    if item<3 and key not in captured:s.capture('mu-'+CODES[lang]);captured.add(key)
   time.sleep(.015)
  else:raise RuntimeError('Rendering timeout')
  actual=set(tuple(map(int,q)) for q in re.findall(r'^LOC_PAGE (\d+) (\d+)',log,re.M));expected={(l,i) for l in range(7) for i in range(count)};missing=sorted(expected-actual)
  if missing:raise RuntimeError('Missing native dialogue pages '+str(missing[:12]))
  if 'LOC_PAGE_MISSING' in log or 'returned an exception' in log:raise RuntimeError('Localization callback error')
  result={'passed':True,'languages':CODES,'dialogue_pages_rendered':len(actual),'mu_tutorial_states':len(set(re.findall(r'^LOC_MU (\d+ \d+)',log,re.M))),'native_boss_slow_disabled':'LOC_BOSS_SLOW 0' in log}
 except Exception as e:result={'passed':False,'error':str(e)}
 finally:s.close();(a.output/'results.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result),flush=True)
 raise SystemExit(not result['passed'])
