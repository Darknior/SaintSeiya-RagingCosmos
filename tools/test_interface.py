#!/usr/bin/env python3
from native_test import Session,ROOT
from generate_interface import catalog
from pathlib import Path
import time,json,argparse,re
p=argparse.ArgumentParser();p.add_argument('--output',required=True);a=p.parse_args();s=Session(a.output,Path('/workspace/squashfs-root/usr/bin/OpenBOR'),title=True);checks=[]
def press_wait(key,pattern):
 mark=len(s.read())
 for _ in range(3):
  s.press(key)
  if pattern in s.read()[mark:]:return
  time.sleep(.5)
 raise RuntimeError('Navigation did not reach '+pattern+' '+s.read()[-900:])
def pause():
 mark=len(s.read())
 for _ in range(12):
  s.press(40)
  if 'pause=2' in s.read()[mark:]:return
 raise RuntimeError('Cannot pause')
try:
 s.wait('title=1',120);time.sleep(1);s.capture('language-en')
 for lang in range(1,7):
  press_wait(81,'lang='+str(lang)+' ');s.capture('language-'+str(lang))
 checks.append('Seven selectable native language names, including accents/CJK/Cyrillic')
 s.press(81);s.press(81);press_wait(40,'confirmed=1');s.capture('title-fr')
 saved=s.out/'Saves/bor/ragingcosmos-language.txt';assert saved.read_text().strip()=='1'
 checks.append('Start validates and writes French without launching the game')
 press_wait(40,'menu=1');s.capture('main-fr');s.press(81);s.press(81);press_wait(4,'view=2');s.capture('main-guide-fr')
 press_wait(79,'page=1');s.press(7);s.press(82);s.press(82);s.press(4);time.sleep(.8);s.press(4);s.press(81);s.press(4)
 s.wait('Level Loaded:',120)
 for _ in range(12):
  s.press(40)
  if 'NATIVE_PLAYER_READY' in s.read():break
 s.wait('NATIVE_PLAYER_READY');time.sleep(5);pause();s.capture('pause-fr')
 checks.append('Localized main menu opens the animated guide; start enters play')
 s.press(81);s.press(81);press_wait(4,'view=2');s.capture('guide-fr-page-1');before=(s.out/'guide-fr-page-1.png').read_bytes();time.sleep(.23);s.capture('guide-moving');assert (s.out/'guide-moving.png').read_bytes()!=before
 for i in range(1,7):press_wait(79,'page='+str(i)+' ');s.capture('guide-fr-page-'+str(i+1))
 values=re.findall(r'NATIVE_UI pause=2 view=2 .* time=(\d+)',s.read());assert len(set(values))==1,values
 checks.append('Seven guide pages animate while the simulation clock stays frozen')
 for lang in [2,3,4,5,6,0,1]:press_wait(27,'lang='+str(lang)+' ');s.capture('guide-language-'+str(lang))
 checks.append('Guide renders in seven languages in the native engine')
 s.press(7);s.press(82);press_wait(4,'view=1');s.capture('levels-fr')
 target=next((i,e) for i,e in enumerate(catalog()[1]['levels']) if 'temple8B-midboss' in e['path'])
 row,entry=target
 for _ in range(row//9):s.press(79)
 for _ in range(row%9):s.press(81)
 s.capture('level-target');mark=len(s.read());s.press(4);s.wait("Level Loaded:    'data/levels/temple8B-midboss.txt'",120,mark);time.sleep(6);s.capture('restored-route')
 checks.append('Pause Level Select loads Scorpio B at its exact campaign index '+str(entry['index']))
 pause();s.press(7);time.sleep(.5);pause();s.press(82);s.press(4);s.wait('title=1',90,mark)
 checks.append('Resume and End Game work after a level jump')
 s.restart();s.wait('lang=1',120);s.wait('title=1',120);s.capture('title-fr-restored');assert saved.read_text().strip()=='1'
 checks.append('French survives a complete engine restart')
 result={'passed':True,'checks':checks}
except Exception as e:result={'passed':False,'checks':checks,'error':str(e)}
finally:s.close();(s.out/'results.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result))
raise SystemExit(not result['passed'])
