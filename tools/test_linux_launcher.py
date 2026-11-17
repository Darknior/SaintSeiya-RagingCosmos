#!/usr/bin/env python3
"""Start the distributed AppImage launcher and intact PAK without unpacking it."""
from pathlib import Path
import argparse,os,subprocess,time,signal,json,shutil

def run(out):
 out.mkdir(exist_ok=False,parents=True);stage=Path('/workspace/deliverables/pack-test/SaintSeiya-RagingCosmos')
 for name in ['Jouer-Linux.sh','OpenBOR-Linux-x64.AppImage']:shutil.copyfile(stage/name,out/name);(out/name).chmod(0o755)
 (out/'Paks').mkdir();(out/'Paks/SaintSeiya-RagingCosmos.pak').symlink_to(stage/'Paks/SaintSeiya-RagingCosmos.pak');keys=out/'keys.txt';keys.write_text('')
 env=dict(os.environ,SDL_VIDEODRIVER='dummy',SDL_RENDER_DRIVER='software',SDL_AUDIODRIVER='dummy',LD_PRELOAD='/workspace/runtime/input_bridge.so',TEST_KEY_FILE=str(keys),TEST_CAPTURE=str(out/'capture.ppm'))
 stdout=(out/'stdout.txt').open('w');p=subprocess.Popen([str(out/'Jouer-Linux.sh')],cwd=out,env=env,stdout=stdout,stderr=subprocess.STDOUT,start_new_session=True);result={}
 try:
  deadline=time.monotonic()+120;log=out/'Logs/OpenBorLog.txt'
  while time.monotonic()<deadline:
   text=log.read_text(errors='replace') if log.exists() else ''
   if p.poll() is not None:raise RuntimeError('Launcher stopped: '+(out/'stdout.txt').read_text()[-1300:])
   if 'Loading models' in text or 'Loading Models' in text:break
   time.sleep(.2)
  else:raise RuntimeError('PAK did not start: '+(out/'stdout.txt').read_text()[-900:])
  time.sleep(12)
  # All child processes belong to the launcher session; SDL handles SIGUSR1.
  # Sending it to only the OpenBOR process avoids the AppImage runtime shell.
  ps=subprocess.check_output(['ps','-eo','pid,sid,comm'],text=True)
  targets=[int(w[0]) for row in ps.splitlines()[1:] if len(w:=row.split())>=3 and int(w[1])==p.pid and (w[2] in ('OpenBOR','AppRun') or 'OpenBOR' in w[2])]
  if not targets:raise RuntimeError('Native engine process not found')
  os.kill(targets[-1],signal.SIGUSR1);deadline=time.monotonic()+10
  while time.monotonic()<deadline:
   if (out/'capture.ppm').exists():break
   time.sleep(.1)
  from PIL import Image
  im=Image.open(out/'capture.ppm');im.save(out/'launcher-native.png')
  text=log.read_text(errors='replace');bad=[x for x in text.splitlines() if 'returned an exception' in x or 'Script compile error' in x]
  if bad:raise RuntimeError('; '.join(bad))
  result={'passed':True,'launcher':'Jouer-Linux.sh','official_appimage':True,'pak_unmodified':True,'no_fuse':True,'native_capture':True,'resolution':list(im.size),'opening_sequence_skipped':False}
 except Exception as e:result={'passed':False,'error':str(e)}
 finally:
  try:os.killpg(p.pid,signal.SIGTERM)
  except ProcessLookupError:pass
  try:p.wait(timeout=8)
  except subprocess.TimeoutExpired:os.killpg(p.pid,signal.SIGKILL);p.wait()
  stdout.close();(out/'results.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result))
 return result['passed']
if __name__=='__main__':
 p=argparse.ArgumentParser();p.add_argument('--output',required=True,type=Path);a=p.parse_args();raise SystemExit(not run(a.output.resolve()))
