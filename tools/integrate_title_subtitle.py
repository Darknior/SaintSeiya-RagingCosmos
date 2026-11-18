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
 patch=generated.crop((45,132,330,170))
 for i in range(1,21):
  path=f'bgs/title/title{i}.png';original=subprocess.check_output(['git','show',BASE+':'+path],cwd=ROOT)
  native=Image.open(io.BytesIO(original));assert native.mode=='P'
  # Preserve every original RGB pixel outside the subtitle. Move the native
  # palette's dark red entry zero to an unused index before reserving zero.
  palette=native.getpalette();used=set(native.tobytes());spare=next(i for i in range(255,0,-1) if i not in used)
  old_zero=palette[:3];palette[spare*3:spare*3+3]=old_zero;palette[:3]=[255,0,255]
  result=Image.frombytes('P',native.size,bytes(spare if v==0 else v for v in native.tobytes()));result.putpalette(palette)
  mapped=patch.quantize(palette=result,dither=Image.Dither.NONE);assert 0 not in mapped.tobytes()
  result.paste(mapped,(45,134));result.save(ROOT/path,bits=8,transparency=0)
  assert result.convert('RGB').crop((0,0,800,134)).tobytes()==native.convert('RGB').crop((0,0,800,134)).tobytes(),'native logo/star changed'
 (ROOT/'reports/title-subtitle.json').write_text(json.dumps({'creative_edit':'image generation/edit tool','integration':'upright blue/silver RAGING and gold COSMOS subtitle; native logo, tower and twenty animation frames retained','source_sha256':hashlib.sha256(artwork.read_bytes()).hexdigest(),'native_base':BASE,'native_logo_and_red_star_rgb_preserved':True,'patch_source':[45,132,330,170],'patch_position':[45,134]},indent=2)+'\n')
if __name__=='__main__':main()
