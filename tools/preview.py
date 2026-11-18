#!/usr/bin/env python3
"""Offline interactive guide and factual sprite / level comparison gallery."""
from pathlib import Path
from PIL import Image,ImageDraw,ImageFont
import json,base64,io,re
ROOT=Path(__file__).resolve().parents[1]
def uri(p):return 'data:image/png;base64,'+base64.b64encode(p.read_bytes()).decode()
def main():
 out=Path('/workspace/deliverables/apercus');out.mkdir(exist_ok=True)
 bg=uri(ROOT/'bgs/temple2D-boss/templeboss.png');seq=json.loads((ROOT/'sprites/interface/animations.json').read_text());frames={f['sprite']:uri(ROOT/f['sprite'][5:]) for page in seq for f in page}
 langs=[]
 for code in ['en','fr','es','pt','zh','ru','ja']:
  d=json.loads((ROOT/'translations'/f'{code}.json').read_text());langs.append({'name':d['name'],'pages':[uri(ROOT/f'sprites/interface/{code}/guide-{i}.png') for i in range(7)]})
 data={'background':bg,'frames':seq,'sprites':frames,'languages':langs}
 html='''<!doctype html><html lang="fr"><meta charset="utf-8"><meta name="viewport" content="width=device-width"><title>Saint Seiya — Guide multilingue</title><style>body{background:#060d1d;color:#eef4ff;font:17px system-ui;margin:0}main{max-width:1000px;margin:auto;padding:24px}h1{color:#e7ca85}canvas,img{max-width:100%;height:auto;border-radius:8px}button,select{background:#203753;color:white;border:1px solid #c4ac75;border-radius:6px;padding:10px;margin:8px;font:inherit}nav{text-align:center}p{color:#bdcbdf}</style><main><h1>Saint Seiya — Comment jouer</h1><p>Aperçu composé avec les mêmes sprites, textes et délais que le jeu. Les collisions et les effets de combat ne sont pas simulés.</p><nav><select id="locale"></select></nav><canvas id="guide" width="800" height="480"></canvas><nav><button onclick="go(-1)">← Précédent</button><span id="count"></span><button onclick="go(1)">Suivant →</button></nav><p>Dans le jeu : langue au Press Start. Guide depuis l’entrée Comment jouer du menu principal natif.</p></main><script>const d=DATA;let page=0,lang=1,start=performance.now();const cache={};function image(src){if(!cache[src]){let x=new Image;x.src=src;cache[src]=x}return cache[src]}Object.values(d.sprites).forEach(image);image(d.background);d.languages.forEach(l=>{l.pages.forEach(image)});const select=document.getElementById('locale');d.languages.forEach((l,i)=>{const x=document.createElement('option');x.value=i;x.textContent=l.name;select.append(x)});select.value=lang;select.onchange=()=>{lang=+select.value};function go(n){page=(page+n+7)%7;start=performance.now()}document.addEventListener('keydown',e=>{if(e.target.tagName==='SELECT')return;if(['ArrowRight','ArrowDown',' '].includes(e.key)){e.preventDefault();go(1)}if(['ArrowLeft','ArrowUp'].includes(e.key)){e.preventDefault();go(-1)}});function paint(now){const seq=d.frames[page],total=seq.reduce((s,f)=>s+f.delay,0);let t=(now-start)%total,f=seq[0];for(const q of seq){f=q;if(t<q.delay)break;t-=q.delay}const c=document.getElementById('guide').getContext('2d');c.clearRect(0,0,800,480);for(const [src,x,y] of [[d.background,0,0],[d.sprites[f.sprite],40,126],[d.languages[lang].pages[page],0,0]]){const i=image(src);if(i.complete&&i.naturalWidth)c.drawImage(i,x,y);if(src===d.background){c.fillStyle='rgba(0,0,0,.5)';c.fillRect(0,0,800,480)}}document.getElementById('count').textContent=(page+1)+' / 7';requestAnimationFrame(paint)}requestAnimationFrame(paint);</script></html>'''.replace('DATA',json.dumps(data,ensure_ascii=False))
 (out/'guide-multilingue.html').write_text(html)
 # Native and new enemy artwork: same palette, same ground anchor, nearest pixels.
 pal=Image.open(ROOT/'chars/soldier_mud/testpal.gif').getpalette();sheet=Image.new('RGB',(1080,650),'#101a29');dr=ImageDraw.Draw(sheet);font=ImageFont.truetype('/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf',20)
 dr.text((20,18),'SOLDAT DE BOUE — NATIF / NOUVELLE PARADE',font=font,fill='#e7ca85')
 metrics=[]
 for i,(label,path) in enumerate([('Natif : attente','idle1.gif'),('Natif : apparition','spawn4.gif'),('Ajout : parade','block1.png')]):
  im=Image.open(ROOT/'chars/soldier_mud'/path);alpha=im.point(lambda v:0 if v==im.getpixel((0,0)) else 255).convert('L') if False else None
  background=im.getpixel((0,0));mask=Image.frombytes('L',im.size,bytes(0 if v==background else 255 for v in im.tobytes()));im.putpalette(pal);im=im.convert('RGBA');im.putalpha(mask);bbox=mask.getbbox();metrics.append({'sprite':path,'visible_size':[bbox[2]-bbox[0],bbox[3]-bbox[1]],'canvas':list(im.size),'palette':'native testpal.gif'})
  # Shared native reference frame origin, not independently stretched silhouettes.
  crop=im.crop((65,70,230,225)).resize((330,310),Image.Resampling.NEAREST);sheet.paste(crop,(20+i*355,110),crop);dr.text((20+i*355,73),label,font=font,fill='white');dr.line((20+i*355,388,350+i*355,388),fill='#56748a');dr.text((20+i*355,445),str(metrics[-1]['visible_size'])+' px',font=font,fill='white')
 dr.text((20,510),'Même palette et ancrage 146,205. Nouveau dessin : silhouette et',font=font,fill='white');dr.text((20,542),'ombrages à apprécier ; les autres compléments utilisent les sprites natifs.',font=font,fill='white');sheet.save(out/'comparatif-soldat-boue.png');(out/'comparatif-mesures.json').write_text(json.dumps(metrics,indent=2)+'\n')
 # Actual runtime images are labelled separately from the composed preview.
 for code in ['en','fr','es','pt','zh','ru','ja']:
  for name in ['native-main','native-options','native-video','native-howto']:
   p=Path('/workspace/menu-ja-bordered-retry' if code=='ja' else '/workspace/menu-final-bordered')/code/(name+'.png')
   if p.exists():(out/(code+'-'+name+'.png')).write_bytes(p.read_bytes())
  for name in ['dialog','mu']:
   p=Path('/workspace/dialogue-frames-punctuation')/(name+'-'+code+'.png')
   if p.exists():(out/p.name).write_bytes(p.read_bytes())
 p=Path('/workspace/package-final-bordered/run/pause-native-fr.png')
 if p.exists():(out/p.name).write_bytes(p.read_bytes())
 routes=json.loads((ROOT/'tools/route-layout.json').read_text());gallery='<!doctype html><html lang="fr"><meta charset="utf-8"><title>Routes restaurées</title><style>body{background:#0a1322;color:white;font:18px system-ui;max-width:1000px;margin:auto;padding:24px}img{max-width:100%}h2{color:#e7ca85}</style><h1>Les sept étapes restaurées</h1><p>Captures natives OpenBOR : ennemis conservés. Les décors proviennent des ressources d’origine ; il ne s’agit pas de sept nouveaux dessins de fond.</p>'
 for r in routes:
  src=Path('/workspace/geometry-campaign')/r['name']/'entry.png'
  if src.exists():
   target=out/(r['name']+'.png');target.write_bytes(src.read_bytes());gallery+=f'<h2>{r["name"]}</h2><p>{r["encounters"]} vagues, {r["soldiers"]} soldats ; décor {r["source_scenery"]}.</p><img src="{uri(target)}">'
 (out/'sept-routes.html').write_text(gallery)
 print('Previews, sprite comparison and native route gallery:',out)
if __name__=='__main__':main()
