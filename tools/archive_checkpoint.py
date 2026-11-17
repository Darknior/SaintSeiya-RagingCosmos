#!/usr/bin/env python3
"""Archive every committed reconstruction change, binary patch and Git history.
No network access or GitHub write permission is required. Output is local only.
"""
from pathlib import Path
import argparse,subprocess,json,hashlib,zipfile
ROOT=Path(__file__).resolve().parents[1]
BASE='110c8b5dc1d60593dd49ba5dee70c367f5bc7ee7'
def git(*args):return subprocess.check_output(['git',*args],cwd=ROOT)
def main(out):
 if git('status','--porcelain').strip():raise SystemExit('Commit the working tree before creating a checkpoint.')
 out.mkdir(exist_ok=True,parents=True);head=git('rev-parse','HEAD').decode().strip()
 bundle=out/'SaintSeiya-modifications-Git.bundle';subprocess.run(['git','bundle','create',str(bundle),BASE+'..HEAD'],cwd=ROOT,check=True)
 patch=out/'SaintSeiya-modifications.patch';patch.write_bytes(git('diff','--binary',BASE,'HEAD'))
 paths=git('diff','--name-only','--diff-filter=ACMRT','-z',BASE,'HEAD').decode().strip('\0').split('\0')
 deleted=git('diff','--name-only','--diff-filter=D','-z',BASE,'HEAD').decode().strip('\0').split('\0');deleted=[x for x in deleted if x]
 manifest={'base':BASE,'head':head,'repository':'https://github.com/Darknior/SaintSeiya-RagingCosmos','modified_files':len(paths),'deleted_paths':deleted,'git_bundle_prerequisite':BASE}
 target=out/'SaintSeiya-sources-modifiees.zip'
 with zipfile.ZipFile(target,'w',zipfile.ZIP_DEFLATED,compresslevel=5) as z:
  for name in paths:z.write(ROOT/name,'SaintSeiya-modifications/'+name)
  for p in (bundle,patch):z.write(p,p.name)
  z.writestr('checkpoint.json',json.dumps(manifest,indent=2)+'\n')
  z.writestr('RESTAURER.txt','Checkpoint '+head+'\n\nLes fichiers modifies sont dans SaintSeiya-modifications/. Le patch binaire inclut les suppressions.\nPour une restauration Git complete : cloner le depot original, puis\ngit fetch /chemin/SaintSeiya-modifications-Git.bundle HEAD:reconstruction\ngit checkout reconstruction\nLe bundle suppose que le commit de base est disponible dans le clone original.\n\nLa copie manuelle des fichiers modifies seule ne supprime pas les anciens chemins.\ncheckpoint.json donne leur liste. Conserver aussi les sources completes sauvegardees sur Drive.\n')
 with zipfile.ZipFile(target) as z:assert z.testzip() is None
 with target.open('rb') as f:digest=hashlib.file_digest(f,'sha256').hexdigest()
 result={'file':str(target),'head':head,'bytes':target.stat().st_size,'sha256':digest,'zip_integrity':True}
 (out/'checkpoint-local.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result))
if __name__=='__main__':
 p=argparse.ArgumentParser();p.add_argument('--output',type=Path,required=True);a=p.parse_args();main(a.output.resolve())
