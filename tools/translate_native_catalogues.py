#!/usr/bin/env python3
"""Translate public game text, caching each response; preserve printf tokens.
Uses the publicly accessible Google Translate endpoint. No credentials are used.
Generated translations require linguistic review; technical checks run locally.
"""
from pathlib import Path
from concurrent.futures import ThreadPoolExecutor,as_completed
import json,re,urllib.request,urllib.parse,time,argparse,threading
ROOT=Path(__file__).resolve().parents[1];SOURCE=json.loads((ROOT/'translations/native-source.json').read_text());LOCK=threading.Lock()
FORMAT=re.compile(r'%(?:[-+0 #]*\d*(?:\.\d+)?[a-zA-Z]|%)')
def request(rows,code):
 protected=[];tokens=[]
 for key,text in rows:
  placeholders=[]
  def mark(m):placeholders.append(m[0]);return '['+str(len(placeholders)-1)+']'
  text=FORMAT.sub(mark,text);protected.append(text);tokens.append(placeholders)
 query='\n'.join(protected);params={'client':'gtx','sl':'en','tl':code,'dt':'t','q':query}
 url='https://translate.googleapis.com/translate_a/single?'+urllib.parse.urlencode(params)
 error=None
 for attempt in range(4):
  try:
   with urllib.request.urlopen(urllib.request.Request(url,headers={'User-Agent':'Mozilla/5.0'}),timeout=40) as response:data=json.load(response)
   translated=''.join(x[0] for x in data[0] if x and x[0]).split('\n')
   if len(translated)!=len(rows):raise ValueError('Translation line count mismatch '+str((len(translated),len(rows))))
   result={}
   for (key,original),text,ph in zip(rows,translated,tokens):
    for i,value in enumerate(ph):
     token='['+str(i)+']';pattern=r'\[\s*'+str(i)+r'\s*\]'
     if not re.search(pattern,text,re.I):raise ValueError('Missing printf placeholder '+token)
     text=re.sub(pattern,lambda _:value,text,flags=re.I)
    if FORMAT.findall(text)!=FORMAT.findall(original):raise ValueError('Changed printf arguments')
    result[key]=text.strip() if not original.startswith(' ') else ' '+text.strip()
   return result
  except Exception as e:error=e;time.sleep(.5*(attempt+1))
 # Retry individual strings if a batch was reflowed.
 if len(rows)>1:
  combined={}
  for row in rows:combined.update(request([row],code))
  return combined
 raise RuntimeError(str(error)+' '+str(rows))
def translate(code):
 dest=ROOT/'translations'/f'native-{code}.json';cache=json.loads(dest.read_text()) if dest.exists() else {}
 if code=='en':cache={key:value['text'] for key,value in SOURCE.items()}
 rows=[(key,v['text']) for key,v in SOURCE.items() if key not in cache]
 batches=[];batch=[];size=0
 for row in rows:
  if size+len(row[1])>1700 and batch:batches.append(batch);batch=[];size=0
  batch.append(row);size+=len(row[1])+1
 if batch:batches.append(batch)
 for i,batch in enumerate(batches):
  cache.update(request(batch,code));dest.write_text(json.dumps(cache,ensure_ascii=False,indent=2)+'\n');print(code,i+1,'/',len(batches),len(cache),flush=True)
 dest.write_text(json.dumps(cache,ensure_ascii=False,indent=2)+'\n')
 return code,len(cache)
if __name__=='__main__':
 p=argparse.ArgumentParser();p.add_argument('--codes',nargs='*',default=['en','fr','es','pt','zh-CN','ru','ja']);a=p.parse_args()
 with ThreadPoolExecutor(max_workers=3) as pool:
  for f in as_completed([pool.submit(translate,c) for c in a.codes]):print(f.result(),flush=True)
