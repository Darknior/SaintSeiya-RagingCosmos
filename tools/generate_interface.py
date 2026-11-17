#!/usr/bin/env python3
"""Rebuild native Seiya demonstrations, Unicode lettering and level catalogue."""
from pathlib import Path
from functools import lru_cache
from PIL import Image,ImageDraw,ImageFont
from fontTools.ttLib import TTFont
import json,re,hashlib,shutil
ROOT=Path(__file__).resolve().parents[1];CODES=['en','fr','es','pt','zh','ru','ja']
ANIMS=[['walk'],['run'],['attack1','attack2','attack3','attack4','attack5'],['jump','jumpland'],['jumpattack'],['block'],['freespecial3']]
def catalog():
 sets=[];index=0
 for l in (ROOT/'levels.txt').read_text().splitlines():
  w=l.split('#')[0].split()
  if not w:continue
  if w[0]=='set':sets.append({'name':w[1],'levels':[]});index=0
  elif sets and w[0] in ('select','skipselect','file','scene'):
   if w[0]=='file':
    title=Path(w[1]).stem.replace('temple','Temple ').replace('-road',' - Road').replace('-midboss',' - Guards').replace('-boss',' - Boss');sets[-1]['levels'].append({'index':index,'path':w[1],'label':title})
   index+=1
 return sets
@lru_cache(None)
def font(code,size):return ImageFont.truetype('/usr/share/fonts/opentype/noto/NotoSansCJK-Regular.ttc' if code in ('zh','ja') else '/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf',size,index=2 if code=='zh' else 0)
@lru_cache(None)
def cmap(code):return TTFont('/usr/share/fonts/opentype/noto/NotoSansCJK-Regular.ttc' if code in ('zh','ja') else '/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf',fontNumber=2 if code=='zh' else 0).getBestCmap()
def text(im,code,value,xy,width,size=17,color='#f3f5ff'):
 assert all(ord(c) in cmap(code) for c in value),(code,value,'missing glyph')
 d=ImageDraw.Draw(im);f=font(code,size)
 while d.textlength(value,font=f)>width and size>10:size-=1;f=font(code,size)
 assert d.textlength(value,font=f)<=width,(code,value,width)
 d.text(xy,value,font=f,fill=color,stroke_width=1,stroke_fill='#050d20',anchor='lt')
def save(im,p):
 q=im.convert('RGB').quantize(colors=255);raw=bytes(v+1 if a else 0 for v,a in zip(q.tobytes(),im.getchannel('A').tobytes()));out=Image.frombytes('P',im.size,raw);out.putpalette([255,0,255]+q.getpalette()[:765]);out.save(p,transparency=0)
def sequences():
 result={};s=(ROOT/'chars/seiya/00_seiya.txt').read_text()
 for b in re.finditer(r'^anim\s+(\S+)(.*?)(?=^anim\s|\Z)',s,re.M|re.S):
  seq=[];delay=10;offset=(0,0)
  for l in b[2].splitlines():
   w=l.split('#')[0].split()
   if not w:continue
   if w[0]=='delay':delay=int(w[1])
   if w[0]=='offset':offset=tuple(map(int,w[1:3]))
   if w[0]=='frame':seq.append({'source':w[1],'delay':max(1,delay)*10,'offset':offset})
  result[b[1].lower()]=seq
 return result
def main():
 out=ROOT/'sprites/interface';out.mkdir(exist_ok=True);pages=[];functions=[];seqs=sequences();palette=Image.open(ROOT/'chars/seiya/palettes/palette1.png').getpalette()
 for i,anims in enumerate(ANIMS):
  seq=[f.copy() for a in anims for f in seqs[a]]
  if i not in (0,1):seq += [f.copy() for f in seqs['idle'][:4]]
  elapsed=0;conditions=[]
  for j,f in enumerate(seq):
   im=Image.open(ROOT/f['source'][5:]);im=im.resize(tuple(round(v*1.5) for v in im.size),Image.Resampling.NEAREST)
   canvas=Image.new('P',(300,280),0);canvas.putpalette(palette);canvas.paste(im,(150-round(f['offset'][0]*1.5),250-round(f['offset'][1]*1.5)))
   target=out/f'seiya-{i}-{j}.png';canvas.save(target,transparency=0);f['sprite']='data/'+str(target.relative_to(ROOT));elapsed+=f['delay'];conditions.append(f'        if(t<{elapsed}) return "{f["sprite"]}";')
  functions.append(f'    if(page=={i}){{t=t%{elapsed};\n'+'\n'.join(conditions)+'\n    }');pages.append(seq)
 (out/'animations.json').write_text(json.dumps(pages,indent=2)+'\n')
 c='// Generated. Native frame timing, in milliseconds.\nvoid guideFrame(int page,int t)\n{\n'+'\n'.join(functions)+'\n return "data/sprites/interface/seiya-0-0.png";\n}\n'
 sets=catalog()
 for name,typ,default in [('levelCount','int','0'),('levelEntry','int','-1'),('levelLabel','void','"Unavailable"'),('levelPath','void','""')]:
  c+=f'{typ} {name}(int mode,int row)\n{{\n'
  for i,s in enumerate(sets):
   if name=='levelCount':c+=f' if(mode=={i}) return {len(s["levels"])};\n'
   else:
    c+=f' if(mode=={i}){{\n'
    for j,entry in enumerate(s['levels']):
     value=str(entry['index']) if name=='levelEntry' else json.dumps(entry['label' if name=='levelLabel' else 'path']);c+=f'  if(row=={j}) return {value};\n'
    c+=' }\n'
  c+=f' return {default};\n}}\n'
 c+='void localeCode(int language)\n{\n'+''.join(f' if(language=={i}) return "{code}";\n' for i,code in enumerate(CODES))+' return "en";\n}\n'
 (ROOT/'scripts/interface_catalog.c').write_text(c);(out/'levels.json').write_text(json.dumps(sets,indent=2)+'\n')
 dictionaries=[json.loads((ROOT/'translations'/f'{code}.json').read_text()) for code in CODES]
 for code,d in zip(CODES,dictionaries):
  target=out/code;target.mkdir(exist_ok=True);ui=d['ui']
  im=Image.new('RGBA',(800,480));text(im,code,ui['language'],(75,58),650,27,color='#e7ca85')
  for j,l in enumerate(dictionaries):text(im,CODES[j],l['name'],(310,140+j*37),400,23)
  text(im,code,ui['select_hint'],(65,432),670,15);save(im,target/'selector.png')
  im=Image.new('RGBA',(800,70));text(im,code,ui['press_start'],(220,5),550,19,color='#e7ca85');text(im,code,ui['change_language']+' • '+d['name'],(220,38),550,15);save(im,target/'title.png')
  im=Image.new('RGBA',(800,480));text(im,code,'SAINT SEIYA — RAGING COSMOS',(180,66),595,24,color='#e7ca85')
  for j,label in enumerate(d['main']):text(im,code,label,(320,150+j*46),410,22)
  text(im,code,ui['select_hint'],(65,432),670,15);save(im,target/'main.png')
  for page,p in enumerate(d['pages']):
   im=Image.new('RGBA',(800,480));text(im,code,ui['guide_title'],(44,48),575,19,color='#e7ca85');text(im,code,f'{ui["page"]} {page+1} / 7',(640,49),122,15);text(im,code,p['title'],(44,85),690,22);text(im,code,p['command'],(364,150),390,17,color='#e7ca85')
   for j in range(3):text(im,code,p[f'line{j+1}'],(364,194+j*25),392,16)
   text(im,code,ui['controls'],(364,309),392,14);text(im,code,ui['devices'],(364,334),392,14);text(im,code,ui['guide_footer'],(44,421),710,14);save(im,target/f'guide-{page}.png')
  im=Image.new('RGBA',(560,330));text(im,code,ui['pause'],(24,18),512,19)
  for j,key in enumerate(['continue','levels','howto','quit']):text(im,code,ui[key],(40,82+j*40),480,19)
  text(im,code,ui['choose_hint'],(24,281),510,14);text(im,code,ui['resume_hint'],(24,307),510,14);save(im,target/'pause.png')
  im=Image.new('RGBA',(560,330));text(im,code,ui['pause'],(24,18),512,19);text(im,code,ui['levels'],(24,46),510,16);text(im,code,ui['stage'],(24,263),155,14);text(im,code,ui['levels_hint'],(24,285),510,14);text(im,code,ui['restart_hint'],(24,307),510,13);save(im,target/'levels.png')
  for mode,s in enumerate(sets):
   for row,e in enumerate(s['levels']):
    label=e['label']
    for word,key in [('Temple','temple'),('Road','road'),('Guards','guards'),('Boss','boss')]:label=label.replace(word,ui[key])
    im=Image.new('RGBA',(500,20));text(im,code,label,(1,1),496,14);save(im,target/f'level-{mode}-{row}.png')
 for route in json.loads((ROOT/'tools/route-layout.json').read_text()):
  im=Image.new('RGBA',(800,40));text(im,'en',route['story'],(25,8),750,16,color='#e7ca85');save(im,out/(route['name']+'-intro.png'))
 manifest={'languages':CODES,'translations':{code:hashlib.sha256((ROOT/'translations'/f'{code}.json').read_bytes()).hexdigest() for code in CODES},'pages':len(pages),'frames':sum(len(x) for x in pages),'total_sprites':len(list(out.rglob('*.png'))),'text_sprites':sum(len(list((out/code).glob('*.png'))) for code in CODES)}
 (out/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
 licenses=ROOT/'translations/licenses';licenses.mkdir(exist_ok=True)
 for src,name in [('/usr/share/doc/fonts-dejavu-core/copyright','DejaVu.txt'),('/usr/share/doc/fonts-noto-cjk/copyright','Noto-CJK.txt')]:shutil.copyfile(src,licenses/name)
 print(json.dumps({'languages':7,'guide_pages':49,'demonstration_frames':manifest['frames'],'story_stages':len(sets[1]['levels']),'all_glyphs_verified':True}))
if __name__=='__main__':main()
