#!/usr/bin/env python3
"""Native menu translation tables, MBS glyph banks and localized dialogue text.
Retains native menu layout/input; shares typeface and metrics across ASCII and accents.
"""
from pathlib import Path
from PIL import Image,ImageDraw,ImageFont
import json,re,shutil,hashlib,unicodedata,math
from fontTools.ttLib import TTFont
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
  d={key:unicodedata.normalize('NFC',value) for key,value in d.items()}
  catalogues[code]=d
  (ROOT/'translations'/f'native-{raw}.json').write_text(json.dumps(d,ensure_ascii=False,indent=2)+'\n')
 chars=sorted({c for d in catalogues.values() for s in d.values() for c in s if ord(c)>127}|{c for s in names.values() for c in s if ord(c)>127})
 glyphs={ch:bytes((128+i//127,128+i%127)) for i,ch in enumerate(chars)};assert len(chars)<127*127
 def encode(s):return b''.join(glyphs[c] if ord(c)>127 else c.encode('ascii') for c in s)
 def quote(s):return b'"'+encode(s).replace(b'\\',b'\\\\').replace(b'"',b'\\"').replace(b'\n',b'\\n')+b'"'
 def glyph_family(ch):
  n=ord(ch)
  if 0x3400<=n<=0x9fff:return 'zh'
  if 0x3000<=n<=0x30ff or 0xff00<=n<=0xffef:return 'ja'
  return 'fr'
 cmap={
  'fr':TTFont('/usr/share/fonts/truetype/dejavu/DejaVuSansCondensed-Bold.ttf').getBestCmap(),
  'ja':TTFont('/usr/share/fonts/opentype/noto/NotoSansCJK-Regular.ttc',fontNumber=0).getBestCmap(),
  'zh':TTFont('/usr/share/fonts/opentype/noto/NotoSansCJK-Regular.ttc',fontNumber=2).getBestCmap()}
 missing=[ch for ch in glyphs if ord(ch) not in cmap[glyph_family(ch)]]
 assert not missing,('unsupported Unicode characters',missing)
 (ROOT/'reports/unicode-font-coverage.json').write_text(json.dumps({'required_non_ascii_glyphs':len(glyphs),'missing':missing,'french_accent_sample':'Système','cjk_punctuation_sample':'、。々「」！，：；？～','cjk_punctuation_font':'Noto Sans CJK','normalization':'NFC'},ensure_ascii=False,indent=2)+'\n')
 # ASCII and accented Latin/Cyrillic use one face, point size and baseline.
 # Four extra vertical pixels retain capitals' accents and cedillas without
 # shrinking the character body or clipping its descenders.
 font_metrics={}
 for number in range(10):
  stem='font' if number==0 else 'font'+str(number+1);original=ROOT/'sprites'/f'{stem}.gif';bank=ROOT/'sprites'/stem;bank.mkdir(exist_ok=True)
  tile=Image.open(original).width//16;cell_width=math.ceil(tile*1.25) if number<4 else tile;cell_height=tile+4;baseline=tile
  ink=[(245,244,242),(252,242,48),(106,149,219),(106,149,219),(56,240,19),(255,255,255),(4,137,218),(251,250,247),(224,208,107),(251,75,87)][number]
  native_fonts={family:ImageFont.truetype('/usr/share/fonts/truetype/dejavu/DejaVuSansCondensed-Bold.ttf',tile) if family=='fr' else font(family,tile) for family in ('fr','ja','zh')}
  font_metrics[stem]={'point_size':tile,'latin_face':'DejaVu Sans Condensed Bold','cell':[cell_width,cell_height],'latin_horizontal_scale':cell_width/tile,'baseline':baseline,'same_ascii_and_accent_face':True}
  for first in [0]+sorted({v[0] for v in glyphs.values()}):
   im=Image.new('RGBA',(cell_width*16,cell_height*16))
   entries=[(chr(i),i) for i in range(32,127)] if first==0 else [(ch,seq[1]) for ch,seq in glyphs.items() if seq[0]==first]
   for ch,second in entries:
    family=glyph_family(ch);f=native_fonts[family];x=(second%16)*cell_width;y=(second//16)*cell_height
    token=Image.new('RGBA',(tile,cell_height));draw=ImageDraw.Draw(token)
    shift=max(0,-f.getbbox(ch,anchor='ls')[0]);draw.text((shift+1,baseline+1),ch,font=f,fill=(32,24,40),anchor='ls');draw.text((shift,baseline),ch,font=f,fill=ink,anchor='ls')
    if family=='fr':token=token.resize((cell_width,cell_height),Image.Resampling.NEAREST)
    im.alpha_composite(token,(x,y))
   ImageDraw.Draw(im).point((0,0),fill=ink)
   indexed(im,bank/f'{first:02x}.png');(bank/f'{first:02x}.gif').unlink(missing_ok=True)
 (ROOT/'reports/native-font-metrics.json').write_text(json.dumps(font_metrics,indent=2)+'\n')
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
 # Layout uses the exact advances computed by OpenBOR's proportional font loader.
 # The text stays as strings; no per-dialogue or tutorial text sprites are built.
 font_bank=ROOT/'sprites/font6';sheets={}
 def advance(ch):
  first,second=(0,ord(ch)) if ord(ch)<128 else glyphs[ch]
  if first not in sheets:sheets[first]=Image.open(font_bank/('00.png' if first==0 else f'{first:02x}.png'))
  sheet=sheets[first];tile=sheet.width//16;cell_height=sheet.height//16
  token=sheet.crop(((second%16)*tile,(second//16)*cell_height,(second%16+1)*tile,(second//16+1)*cell_height))
  xs=[x for y in range(cell_height) for x in range(tile) if token.getpixel((x,y))!=0]
  width=max(xs)-min(xs)+1+tile//10 if xs else 0
  return width if width>1 else tile//3
 advances={ch:advance(ch) for ch in {c for d in catalogues.values() for text in d.values() for c in text}}
 def text_lines(text,code,width=258):
  # Keep closing punctuation with its preceding word/character. Keep opening
  # quotes and brackets with the following word/character (kinsoku for CJK).
  closing=set('!?:;%,.。．、，！？：；％…)]}»›”’」』】〉》）］｝')
  opening=set('([{«‹“‘¿¡「『【〈《（［｛')
  chunks=list(text) if code in ('zh','ja') else text.split()
  units=[];pending=''
  for word in chunks:
   if all(ch in closing for ch in word) and units:
    units[-1]+=(' ' if code not in ('zh','ja') else '')+word
   elif all(ch in opening for ch in word):pending+=word+(' ' if code not in ('zh','ja') else '')
   else:units.append(pending+word);pending=''
  if pending:
   if units:units[-1]+=pending.rstrip()
   else:units=[pending.rstrip()]
  lines=[];line=''
  for word in units:
   separator=' ' if line and code not in ('zh','ja') else ''
   candidate=line+separator+word
   if sum(advances[ch] for ch in candidate)>width and line:lines.append(line);line=word
   else:line=candidate
  if line:lines.append(line)
  assert len(lines)<=5,(code,text,'paragraph overflows native window',len(lines))
  assert all(sum(advances[ch] for ch in line)<=width for line in lines),(code,text,'line too wide')
  assert all(not all(ch in closing or ch.isspace() for ch in line) for line in lines) or all(ch in closing or ch.isspace() for ch in text),(code,text,'isolated punctuation')
  if code in ('zh','ja'):
   assert all(line[0] not in closing for line in lines[1:]),(code,text,'CJK line starts with closing punctuation')
  return lines
 layouts={code+':'+key:{'font':5,'line_height':16,'lines':text_lines(value,code),'text':value} for code,d in catalogues.items() for key,value in d.items() if key.startswith(('dialog:','mu:'))}
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
 c+=b' return NULL();}\n'
 display={}
 for path in sorted((ROOT/'chars').glob('*/00_*.txt')):
  data=path.read_text(errors='replace');model=re.search(r'^name\s+(\S+)',data,re.M);kind=re.search(r'^type\s+(\S+)',data,re.M)
  if model and kind and kind[1] in ('player','enemy','npc') and not path.parent.name.startswith('soldier_'):
   label=path.parent.name.replace('_',' ').title();display[model[1]]=label
 for hero in ('seiya','shiryu','hyoga','shun','ikki'):display[hero+'S']=hero.capitalize()
 for path in (ROOT/'levels').glob('*.txt'):
  for alias in re.findall(r'^alias\s+([^#\n]+)',path.read_text(errors='replace'),re.M):
   alias=alias.strip()
   if alias and alias[0].islower() and alias.isascii():display.setdefault(alias,alias[0].upper()+alias[1:])
 c+=b'void nativeDisplayName(void value){\n'
 for model,label in display.items():c+=b' if(value=='+quote(model)+b')return '+quote(label)+b';\n'
 c+=b' return value;}\n#endif\n';(ROOT/'scripts/native_localization_catalog.c').write_bytes(c)
 (ROOT/'reports/character-display-names.json').write_text(json.dumps(display,ensure_ascii=False,indent=2)+'\n')
 # Paragraph catalogue: one shared native font, wrapped once at build time.
 c=(ROOT/'scripts/native_localization_catalog.c').read_bytes();assert c.endswith(b'#endif\n');c=c[:-7]
 c+=b'void nativeParagraphLine(void key,int line){int n=getglobalvar("ui_lang");\n'
 for key in SOURCE:
  if not key.startswith(('dialog:','mu:')):continue
  c+=b' if(key=='+quote(key)+b'){\n'
  for i,code in enumerate(CODES):
   c+=b'  if(n=='+str(i).encode()+b'){\n'
   for j,line in enumerate(layouts[code+':'+key]['lines']):c+=b'   if(line=='+str(j).encode()+b')return '+quote(line)+b';\n'
   c+=b'   return "";}\n'
  c+=b' }\n'
 c+=b' return "";}\n#endif\n';(ROOT/'scripts/native_localization_catalog.c').write_bytes(c)
 # Remove superseded generated text images; keep the native Mû bubble artwork.
 for code in CODES:
  for pattern in ('dialog-*.png','mu-*.png'):
   for obsolete in (ROOT/'sprites/localization'/code).glob(pattern):obsolete.unlink()
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
 (ROOT/'reports/native-localization.json').write_text(json.dumps({'languages':CODES,'entries_per_language':len(SOURCE),'dialogue_pages':sum(k.startswith('dialog:') for k in SOURCE),'mu_tutorials':sum(k.startswith('mu:') for k in SOURCE),'unicode_glyphs':len(glyphs),'native_mbs_encoding':True,'dialogue_renderer':'native drawstring, font 5','dialogue_text_images':0,'menu_font_point_size':10,'menu_font_cell':[13,14],'root_menu_display_scale':1.6,'tables':metadata,'translation_provenance':'Google Translate; French dialogue pages reviewed, UI labels and Saint Seiya terminology corrected; other locales need linguistic review; placeholders and layouts checked'},ensure_ascii=False,indent=2)+'\n')
 (ROOT/'translations/native-glyphs.json').write_text(json.dumps({ch:list(b) for ch,b in glyphs.items()},ensure_ascii=False,indent=2)+'\n')
 print('Native localization generated:',len(SOURCE),'entries x7;',len(glyphs),'glyphs')
if __name__=='__main__':main()
