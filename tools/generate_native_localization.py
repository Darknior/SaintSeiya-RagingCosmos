#!/usr/bin/env python3
"""Native menu translation tables, MBS glyph banks and localized dialogue text.
Retains native menu layout/input and all original ASCII glyphs. No engine fork.
"""
from pathlib import Path
from PIL import Image,ImageDraw,ImageFont
import json,re,shutil,hashlib
ROOT=Path(__file__).resolve().parents[1];CODES=['en','fr','es','pt','zh','ru','ja'];SOURCE=json.loads((ROOT/'translations/native-source.json').read_text())
FORMAT=re.compile(r'%(?:[-+0 #]*\d*(?:\.\d+)?[a-zA-Z]|%)')
KEYS={
'en':['Attack','Jump','Block','Special','Back Attack','Charge'],
'fr':['Attaque','Saut','Garde','Spécial','Attaque arrière','Cosmos'],
'es':['Ataque','Salto','Bloqueo','Especial','Ataque atrás','Cosmos'],
'pt':['Ataque','Pulo','Defesa','Especial','Ataque traseiro','Cosmos'],
'zh':['攻击','跳跃','防御','必杀技','后方攻击','蓄积小宇宙'],
'ru':['Атака','Прыжок','Блок','Спецприём','Удар назад','Космос'],
'ja':['攻撃','ジャンプ','ガード','必殺技','後方攻撃','小宇宙チャージ']}
def font(code,size):return ImageFont.truetype('/usr/share/fonts/opentype/noto/NotoSansCJK-Regular.ttc' if code in ('zh','ja') else '/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf',size,index=2 if code=='zh' else 0)
def indexed(im,path):
 # Index zero is transparent in the engine; reserve it explicitly.
 q=im.convert('RGB').quantize(colors=255);pixels=bytes((v+1) if a>100 else 0 for v,a in zip(q.tobytes(),im.getchannel('A').tobytes()));out=Image.frombytes('P',im.size,pixels);palette=[255,0,255]+q.getpalette()[:765];palette+=[0]*(768-len(palette));out.putpalette(palette);path.parent.mkdir(exist_ok=True,parents=True);out.save(path,transparency=0,bits=8);assert path.read_bytes()[24]==8
def wrap(value,code,size,width):
 f=font(code,size);d=ImageDraw.Draw(Image.new('RGB',(1,1)));words=list(value) if code in ('zh','ja') else value.split();lines=[];line=''
 for word in words:
  candidate=line+(' ' if line and code not in ('zh','ja') else '')+word
  if d.textlength(candidate,font=f)>width and line:lines.append(line);line=word
  else:line=candidate
 if line:lines.append(line)
 return lines

def main():
 catalogues={};metadata={};names={}
 for code in CODES:
  raw='zh-CN' if code=='zh' else code;d=json.loads((ROOT/'translations'/f'native-{raw}.json').read_text());old=json.loads((ROOT/'translations'/f'{code}.json').read_text());names[code]=old['name']
  overrides=json.loads((ROOT/'translations/native-overrides.json').read_text())
  d.update(overrides.get(code,{}))
  if code in ('zh','ja'):
   for key in d:
    if code=='zh':
     for a,b in [('黄金骑士团','黄金圣斗士'),('青铜骑士','青铜圣斗士'),('阿芙洛狄忒','阿布罗狄'),('沙卡','沙加')]:d[key]=d[key].replace(a,b)
    else:
     for a,b in [('黄金騎士団','黄金聖闘士'),('青銅騎士','青銅聖闘士'),('グレートポープ','教皇'),('ヒョウガ','氷河'),('セイヤ','星矢'),('シリュウ','紫龍'),('イッキ','一輝')]:d[key]=d[key].replace(a,b)
  # Use reviewed labels already selected for this game's UI.
  for src,index in [('Start Game',0),('Options',1),('How To Play',2),('Hall Of Fame',3),('Quit',4)]:d[src]=old['main'][index]
  for src,key in [('Continue','continue'),('End Game','quit'),('Pause','pause')]:d[src]=old['ui'][key]
  for src,text in zip(['Attack','Jump','Block','Special','Back Attack','Charge'],KEYS[code]):d[src]=text
  if code=='fr':d.update({'Control Options':'Commandes','Control Options...':'Commandes...','Video Options':'Affichage','Video Options...':'Affichage...','Sound Options':'Son','Sound Options...':'Son...','New Game':'Nouvelle partie','Load Game':'Charger une partie','Game Mode':'Mode de jeu','PRESS START':'APPUYEZ SUR START','Press Start':'Appuyez sur Start'})
  for key,entry in SOURCE.items():
   assert key in d,(code,key,'missing translation')
   if '%' in key and key!='COSMOS 100% UP':assert FORMAT.findall(d[key])==FORMAT.findall(entry['text']),(code,key,'printf mismatch')
  # Translate native mode labels without altering their IDs or menu structure.
  mode_labels={'en':['Tutorial','Story Mode'],'fr':['Tutoriel','Mode Histoire'],'es':['Tutorial','Modo Historia'],'pt':['Treinamento','Modo História'],'zh':['教程','故事模式'],'ru':['Обучение','Сюжетный режим'],'ja':['チュートリアル','ストーリーモード']}
  d['Tutorial_Mode'],d['Story_Mode']=mode_labels[code]
  from generate_interface import catalog
  for mode in catalog():
   identifier=mode['name']
   if identifier in ('Tutorial_Mode','Story_Mode') or not mode['levels']:continue
   stem=Path(mode['levels'][0]['path']).stem
   number=re.search(r'temple(\d+[A-D]?)',stem)
   if number:
    kind='boss' if 'BOSS' in identifier.upper() else 'guards' if 'MIDBOSS' in identifier.upper() else 'road'
    d[identifier]=old['ui']['temple']+' '+number[1]+' - '+old['ui'][kind]
  # Canonical terminology in the French Saint Seiya adaptation.
  if code=='fr':
   for key in d:
    d[key]=re.sub(r'grand pape','Grand Pope',d[key],flags=re.I)
  # Guide references use the same translated button names as Control Options.
  if code!='en':
   mapping=dict(zip(['Attack','Jump','Block','Special','Back Attack','Charge'],KEYS[code]))
   def controls(value):
    if isinstance(value,str):
     for source,target in sorted(mapping.items(),key=lambda q:-len(q[0])):value=re.sub(r'\b'+re.escape(source)+r'\b',target,value,flags=re.I)
     return value
    if isinstance(value,list):return [controls(v) for v in value]
    if isinstance(value,dict):return {k:controls(v) for k,v in value.items()}
    return value
   old=controls(old)
   old['ui']['controls']=d['Options']+' / '+d['Control Options']
   (ROOT/'translations'/f'{code}.json').write_text(json.dumps(old,ensure_ascii=False,indent=2)+'\n')
  directions={'en':['Up','Down','Left','Right','Start','Exit'],'fr':['Haut','Bas','Gauche','Droite','Start','Quitter'],'es':['Arriba','Abajo','Izquierda','Derecha','Start','Salir'],'pt':['Cima','Baixo','Esquerda','Direita','Start','Sair'],'zh':['上','下','左','右','开始','返回'],'ru':['Вверх','Вниз','Влево','Вправо','Старт','Назад'],'ja':['上','下','左','右','スタート','戻る']}
  for key,value in zip(['Move Up','Move Down','Move Left','Move Right','Start','Exit'],directions[code]):d[key]=value
  for key in d:
   if re.fullmatch(r'HEALTH \+ \d+',key):d[key]=d['Energy']+' + '+key.split()[-1]
   if re.fullmatch(r'\d+ PTS',key):d[key]=key.split()[0]+' '+{'en':'PTS','fr':'PTS','es':'PTS','pt':'PTS','zh':'分','ru':'ОЧК.','ja':'点'}[code]
  catalogues[code]=d
  (ROOT/'translations'/f'native-{raw}.json').write_text(json.dumps(d,ensure_ascii=False,indent=2)+'\n')
 chars=sorted({c for d in catalogues.values() for s in d.values() for c in s if ord(c)>127}|{c for s in names.values() for c in s if ord(c)>127})
 glyphs={ch:bytes((128+i//127,128+i%127)) for i,ch in enumerate(chars)};assert len(chars)<127*127
 def encode(s):return b''.join(glyphs[c] if ord(c)>127 else c.encode('ascii') for c in s)
 def quote(s):return b'"'+encode(s).replace(b'\\',b'\\\\').replace(b'"',b'\\"').replace(b'\n',b'\\n')+b'"'
 # The native engine MBS reader consumes two bytes for non-ASCII glyphs.
 # Both are high bytes, avoiding quotes, percent signs and control characters.
 for number in range(10):
  stem='font' if number==0 else 'font'+str(number+1);original=ROOT/'sprites'/f'{stem}.gif';bank=ROOT/'sprites'/stem;bank.mkdir(exist_ok=True);shutil.copyfile(original,bank/'00.gif')
  tile=Image.open(original).width//16
  ink=[(245,244,242),(252,242,48),(106,149,219),(106,149,219),(56,240,19),(255,255,255),(4,137,218),(251,250,247),(224,208,107),(251,75,87)][number]
  for first in sorted({b[0] for b in glyphs.values()}):
   im=Image.new('RGBA',(tile*16,tile*16));d=ImageDraw.Draw(im)
   for ch,sequence in glyphs.items():
    if sequence[0]!=first:continue
    second=sequence[1];x=(second%16)*tile;y=(second//16)*tile;f=font('zh' if '\u3400'<=ch<='\u9fff' else 'ja' if '\u3040'<=ch<='\u30ff' else 'fr',max(7,tile-1))
    glyph=Image.new('RGBA',(tile,tile));gd=ImageDraw.Draw(glyph)
    gd.text((1,1),ch,font=f,fill=(32,24,40),anchor='lt');gd.text((0,0),ch,font=f,fill=ink,anchor='lt');im.alpha_composite(glyph,(x,y))
   d.point((0,0),fill=ink) # Token zero carries the palette for the MBS bank.
   indexed(im,bank/f'{first:02x}.png')
   (bank/f'{first:02x}.gif').unlink(missing_ok=True)
 # Engine menu tables retain all stock menus/options and printf arguments.
 for code,d in catalogues.items():
  dest=ROOT/'translations/native'/code;dest.mkdir(exist_ok=True,parents=True);rows=[]
  for key in SOURCE:
   if key.startswith(('dialog:','mu:')):continue
   rows.extend([b'msgid '+quote(key),b'msgstr '+quote(d[key])])
  (dest/'translation.txt').write_bytes(b'\n'.join(rows)+b'\n')
  menu=[b'disablekey screenshot',b'fontmbs '+b' '.join([b'1']*10)]
  for key,src in [('attack','Attack'),('jump','Jump'),('special','Block'),('attack2','Special'),('attack3','Back Attack'),('attack4','Charge'),('moveup','Move Up'),('movedown','Move Down'),('moveleft','Move Left'),('moveright','Move Right'),('start','Start'),('esc','Exit')]:
   assert len(encode(d[src]))<32,(code,src,'native button name exceeds 31 bytes')
   menu.append(b'renamekey '+key.encode()+b' '+quote(d[src]))
  (dest/'menu.txt').write_bytes(b'\n'.join(menu)+b'\n');metadata[code]={'translation_lines':len(rows),'menu_lines':len(menu)}
 shutil.copyfile(ROOT/'translations/native/en/translation.txt',ROOT/'translation.txt');shutil.copyfile(ROOT/'translations/native/en/menu.txt',ROOT/'menu.txt')
 # Script strings use the exact same glyph encoding as stock engine labels.
 c=b'// Generated native localization; do not edit by hand.\n#ifndef NATIVE_LOCALIZATION_CATALOG\n#define NATIVE_LOCALIZATION_CATALOG 1\n'
 c+=b'void locCode(int n){\n'+b''.join(b' if(n=='+str(i).encode()+b')return "'+code.encode()+b'";\n' for i,code in enumerate(CODES))+b' return "en";}\n'
 c+=b'void nativeLanguageName(int n){\n'+b''.join(b' if(n=='+str(i).encode()+b')return '+quote(names[code])+b';\n' for i,code in enumerate(CODES))+b' return "English";}\n'
 c+=b'void nativeText(void text){if(text==NULL())return "";int n=getglobalvar("ui_lang");\n'
 for key in SOURCE:
  if key.startswith(('dialog:','mu:')):continue
  c+=b' if(text=='+quote(key)+b'){\n'+b''.join(b'  if(n=='+str(i).encode()+b')return '+quote(catalogues[code][key])+b';\n' for i,code in enumerate(CODES))+b' }\n'
 c+=b' return text;}\n'
 c+=b'int nativeDialogPage(void a,void b,void c,void d){\n'
 for key,entry in SOURCE.items():
  if not key.startswith('dialog:'):continue
  idx=int(key.split(':')[1]);c+=b' if(a=='+quote(entry['lines'][0])+b' && b=='+quote(entry['lines'][1])+b' && c=='+quote(entry['lines'][2])+b' && d=='+quote(entry['lines'][3])+b')return '+str(idx).encode()+b';\n'
 c+=b' return -1;}\n'
 c+=b'int nativeTranslationLines(){return '+str(metadata['en']['translation_lines']).encode()+b';}\n'
 from complete_soldiers import soldiers
 kinds={'soldier_blue':'Soldier','soldier_short':'Light Soldier','soldier_basic':'Guard','soldier_shield':'Shield Guard','soldier_muscles':'Strong Soldier','soldier_fat':'Heavy Guard','soldier_base':'Soldier','soldier_purple':'Elite Soldier','soldier_stick':'Staff Guard','soldier_big':'Giant Soldier','soldier_mud':'Mud Soldier','soldier_troll':'Troll Soldier'}
 c+=b'void nativeSoldierLabel(void model){\n'
 for model,path in soldiers():c+=b' if(model=='+quote(model)+b')return nativeText('+quote(kinds[path.parent.name])+b');\n'
 c+=b' return NULL();}\n#endif\n';(ROOT/'scripts/native_localization_catalog.c').write_bytes(c)
 # Native portrait windows stay unchanged; only their text is rasterized.
 layouts={}
 for code,d in catalogues.items():
  for key,entry in SOURCE.items():
   if not key.startswith(('dialog:','mu:')):continue
   width,height=(260,76) if key.startswith('dialog:') else (264,76);chosen=None
   for size in range(15,8,-1):
    lines=wrap(d[key],code,size,width-4)
    if len(lines)<=4 and len(lines)*(size+2)<=height:chosen=(size,lines);break
   if chosen is None:raise ValueError((code,key,'does not fit',d[key]))
   size,lines=chosen;im=Image.new('RGBA',(width,height));draw=ImageDraw.Draw(im)
   for i,line in enumerate(lines):draw.text((1,i*(size+2)),line,font=font(code,size),fill='white',stroke_width=0,anchor='lt')
   slug=key.replace(':','-');indexed(im,ROOT/'sprites/localization'/code/f'{slug}.png');layouts[code+':'+key]={'size':size,'lines':len(lines),'text':d[key]}
 # Translate the caption of the existing Game Over sequence too.
 for code,d in catalogues.items():
  im=Image.new('RGBA',(300,42));draw=ImageDraw.Draw(im);size=28
  while draw.textlength(d['GAME OVER'],font=font(code,size))>294:size-=1
  width=draw.textlength(d['GAME OVER'],font=font(code,size));draw.text(((300-width)/2,2),d['GAME OVER'],font=font(code,size),fill='white',anchor='lt')
  indexed(im,ROOT/'sprites/localization'/code/'game-over.png')
 # Preserve Mû's native border/portrait and original translucent bubble.
 # Blank only the old English lettering, then render the chosen language above.
 mu_base=ROOT/'sprites/localization/mu-base';mu_base.mkdir(exist_ok=True,parents=True)
 for original in (ROOT/'bgs/temple1/mu_dialog').glob('mu_dialog_*.png'):
  im=Image.open(original);bg=im.getpixel((0,0));ImageDraw.Draw(im).rectangle((24,15,298,95),fill=bg)
  im.save(mu_base/original.name,bits=8)
 (ROOT/'reports/native-localization-layouts.json').write_text(json.dumps(layouts,ensure_ascii=False,indent=2)+'\n')
 (ROOT/'reports/native-localization.json').write_text(json.dumps({'languages':CODES,'entries_per_language':len(SOURCE),'dialogue_pages':sum(k.startswith('dialog:') for k in SOURCE),'mu_tutorials':sum(k.startswith('mu:') for k in SOURCE),'unicode_glyphs':len(glyphs),'native_mbs_encoding':True,'tables':metadata,'translation_provenance':'Google Translate; French dialogue pages reviewed, UI labels and Saint Seiya terminology corrected; other locales need linguistic review; placeholders and layouts checked'},ensure_ascii=False,indent=2)+'\n')
 (ROOT/'translations/native-glyphs.json').write_text(json.dumps({ch:list(b) for ch,b in glyphs.items()},ensure_ascii=False,indent=2)+'\n')
 print('Native localization generated:',len(SOURCE),'entries x7;',len(glyphs),'glyphs')
if __name__=='__main__':main()
