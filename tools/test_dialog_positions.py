#!/usr/bin/env python3
"""Exercise production textbox drawing across unlocked/locked/restored states."""
from pathlib import Path
import argparse,json,time,re,subprocess
from native_test import Session,ROOT
PROBE='''
void dialogProbe(){
 if(!openborvariant("in_level"))return;
 void p=getplayerproperty(0,"entity");if(p==NULL())return;
 int phase=getglobalvar("dialog_test_phase");if(phase==NULL())phase=0;
 int now=openborvariant("ticks");int next=getglobalvar("dialog_test_next");if(next!=NULL() && now<next)return;
 setglobalvar("dialog_test_next",now+300);
 if(phase==0){
  changeentityproperty(p,"z",430);setglobalvar("lockButton",0);
  clearspawnentry();setspawnentry("name","probe_dialog");void e=spawn();
  setglobalvar("dialog_test_box",e);log("DIALOG_TEST spawned="+(e!=NULL())+"\\n");
 }else if(phase==1){
  log("DIALOG_TEST unlocked="+getentityproperty(p,"z")+"\\n");setglobalvar("lockButton",1);
 }else if(phase==2){
  log("DIALOG_TEST hidden="+getentityproperty(p,"z")+" saved="+getentityvar(p,"dialog_saved_z")+"\\n");setglobalvar("lockButton",0);
 }else if(phase==3){
  log("DIALOG_TEST restored="+getentityproperty(p,"z")+"\\n");changeentityproperty(p,"z",455);
 }else if(phase==4){
  log("DIALOG_TEST movable="+getentityproperty(p,"z")+"\\n");setglobalvar("lockButton",1);
 }else if(phase==5){
  log("DIALOG_TEST hidden_again="+getentityproperty(p,"z")+"\\n");setglobalvar("lockButton",0);
 }else if(phase==6){
  log("DIALOG_TEST restored_again="+getentityproperty(p,"z")+"\\n");setglobalvar("lockButton",1);
 }else if(phase==7){
  log("DIALOG_TEST hidden_final="+getentityproperty(p,"z")+"\\n");changeentityproperty(getglobalvar("dialog_test_box"),"lifespancountdown",-1);
 }else if(phase==8){
  log("DIALOG_TEST closed="+getentityproperty(p,"z")+"\\nDIALOG_TEST_DONE\\n");
 }
 setglobalvar("dialog_test_phase",phase+1);
}
'''
if __name__=='__main__':
 p=argparse.ArgumentParser();p.add_argument('--output',required=True,type=Path);p.add_argument('--original-dialog',action='store_true');a=p.parse_args();s=Session(a.output,Path('/workspace/squashfs-root/usr/bin/OpenBOR'),'temple8C-road');result={}
 try:
  folder=s.out/'data/scripts/dialogs';folder.unlink();folder.mkdir()
  for f in (ROOT/'scripts/dialogs').iterdir():
   if f.name!='dialog.c':(folder/f.name).symlink_to(f)
  source=subprocess.check_output(['git','show','7661bdc:scripts/dialogs/dialog.c'],cwd=ROOT).decode() if a.original_dialog else (ROOT/'scripts/dialogs/dialog.c').read_text()
  (folder/'dialog.c').write_text(source)
  (s.out/'data/test_dialog.txt').write_text('name probe_dialog\ntype text\nshadow 0\nondrawscript data/scripts/dialogs/dialog.c\nanim idle\nloop 1\ndelay 10\noffset 0 0\nframe data/chars/dialogs/empty.png\n')
  m=s.out/'data/models.txt';m.write_text(m.read_text()+'\nload probe_dialog data/test_dialog.txt\n')
  levels=s.out/'data/levels';levels.unlink();levels.mkdir()
  for f in (ROOT/'levels').iterdir():
   if f.name!='temple8C-road.txt':(levels/f.name).symlink_to(f)
  (levels/'temple8C-road.txt').write_text('panel data/bgs/temple8C-road/temple8C-road.png\norder a\ndirection right\nsettime 999\nspawn1 250 40 0\nwait\nat 100000\n')
  u=s.out/'data/scripts/update.c';u.write_text(u.read_text().replace(' nativeProbe();',' nativeProbe();dialogProbe();',1)+PROBE)
  s.wait('Level Loaded:',120)
  for _ in range(10):
   s.press(40)
   if 'NATIVE_PLAYER_READY' in s.read():break
  log=s.wait('DIALOG_TEST_DONE',40);values={k:float(v) for k,v in re.findall(r'DIALOG_TEST (unlocked|hidden|restored|movable|hidden_again|restored_again|hidden_final|closed)=(-?[\d.]+)',log)}
  passed=values=={'unlocked':430,'hidden':-999,'restored':430,'movable':455,'hidden_again':-999,'restored_again':455,'hidden_final':-999,'closed':455}
  result={'passed':passed,'original_dialog':a.original_dialog,'positions':values}
 except Exception as e:result={'passed':False,'error':str(e)}
 finally:s.close();(a.output/'results.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result))
 raise SystemExit(not result['passed'])
