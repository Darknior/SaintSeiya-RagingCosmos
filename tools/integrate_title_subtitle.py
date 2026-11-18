#!/usr/bin/env python3
"""Integrate the image-tool subtitle into native animated frames; retain native logo/tower.
Original frames are recovered from the native base commit, keeping this reproducible.
"""
from pathlib import Path
from PIL import Image
import subprocess,json,hashlib
ROOT=Path(__file__).resolve().parents[1]
BASE='110c8b5dc1d60593dd49ba5dee70c367f5bc7ee7'
def main():
 import io
 artwork=ROOT/'art-source/title-subtitle-generated.png'
 generated=Image.open(artwork).convert('RGB').resize((800,480),Image.Resampling.LANCZOS)
 patch=generated.crop((45,128,330,163))
 for i in range(1,21):
  path=f'bgs/title/title{i}.png';original=subprocess.check_output(['git','show',BASE+':'+path],cwd=ROOT)
  im=Image.open(io.BytesIO(original)).convert('RGB');im.paste(patch,(45,134))
  # Reserve palette entry zero: OpenBOR treats index zero as transparent.
  q=im.quantize(colors=255);result=Image.frombytes('P',q.size,bytes(v+1 for v in q.tobytes()))
  palette=[255,0,255]+q.getpalette()[:765];result.putpalette(palette+[0]*(768-len(palette)));result.save(ROOT/path,bits=8,transparency=0)
 (ROOT/'reports/title-subtitle.json').write_text(json.dumps({'creative_edit':'image generation/edit tool','integration':'subtitle patch only, native logo, tower and twenty animation frames retained','source_sha256':hashlib.sha256(artwork.read_bytes()).hexdigest(),'native_base':BASE,'patch_source':[45,128,330,163],'patch_position':[45,134]},indent=2)+'\n')
if __name__=='__main__':main()
