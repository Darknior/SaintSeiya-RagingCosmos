import argparse,time
from pathlib import Path
from native_test import Session

def setup(s):
 p=s.out/'Saves/bor';p.mkdir(exist_ok=True,parents=True);(p/'ragingcosmos-language.txt').write_text('1')
 u=s.out/'data/scripts/update.c';u.write_text(u.read_text().replace(' nativeProbe();',' nativeProbe();controlProbe();',1)+'''\nvoid controlProbe(){int state=(openborvariant("in_control_options")!=0);if(getglobalvar("ctl_probe")!=state+1){log("CONTROL "+state+"\\n");setglobalvar("ctl_probe",state+1);}}\n''')
parser=argparse.ArgumentParser();parser.add_argument('--output',type=Path,required=True);args=parser.parse_args()
s=Session(args.output,Path('/workspace/squashfs-root/usr/bin/OpenBOR'),title=True,setup=setup)
def key(k):s.keys.write_text(str(k));time.sleep(.25);s.keys.write_text('');time.sleep(.7)
try:
 s.wait('title=1',120)
 for i in range(15):
  key(40 if i<3 else 4)
  if 'menu=1' in s.read():break
 s.wait('menu=1');key(81);key(4);time.sleep(1);s.capture('options')
 key(81);key(81);key(4);s.wait('CONTROL 1');s.capture('control-parent')
 key(4);time.sleep(1);s.capture('control-keys')
 print('Control screens captured')
finally:s.close()
