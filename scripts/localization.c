#import "data/scripts/native_localization_catalog.c"
#ifndef NATIVE_LOCALIZATION_RUNTIME
#define NATIVE_LOCALIZATION_RUNTIME 1
void localizationSprite(void path,int x,int y,int layer)
{
    void sprite=getglobalvar("loc:"+path);
    if(sprite==NULL()){
        sprite=loadsprite(path);setglobalvar("loc:"+path,sprite);
        int count=getglobalvar("loc_count");if(count==NULL())count=0;
        setglobalvar("loc_sprite_"+count,sprite);setglobalvar("loc_path_"+count,path);setglobalvar("loc_count",count+1);
    }
    if(sprite!=NULL())drawsprite(sprite,x,y,layer);
}
void localizationRelease()
{
    int i;int count=getglobalvar("loc_count");if(count==NULL())count=0;
    for(i=0;i<count;i++){
        void sprite=getglobalvar("loc_sprite_"+i);void path=getglobalvar("loc_path_"+i);
        if(sprite!=NULL())free(sprite);setglobalvar("loc:"+path,NULL());setglobalvar("loc_sprite_"+i,NULL());setglobalvar("loc_path_"+i,NULL());
    }
    setglobalvar("loc_count",0);
}
void localizationCopy(void from,void name)
{
    int source=openfilestream(from,0);if(source<0)return;
    int dest=createfilestream();int i;int lines=0;
    if(name=="menu.txt")lines=14;else lines=nativeTranslationLines();
    // Files have no blank lines; a fixed line count avoids EOF ambiguity.
    for(i=0;i<lines;i++){
        filestreamappend(dest,getfilestreamline(source),2);filestreamappend(dest,"\n",2);filestreamnextline(source);
    }
    savefilestream(dest,name,"translation/");closefilestream(source);closefilestream(dest);
}
void localizationExport()
{
    int lang=getglobalvar("ui_lang");if(lang==NULL())lang=0;
    if(getglobalvar("loc_exported")==lang+1)return;
    localizationCopy("data/translations/native/"+locCode(lang)+"/translation.txt","translation.txt");
    localizationCopy("data/translations/native/"+locCode(lang)+"/menu.txt","menu.txt");
    setglobalvar("loc_exported",lang+1);
}
void localizedTextFrame(int x,int y,int width,int height,int layer)
{
    drawbox(x,y,width,height,layer,rgbcolor(0,0,0),6);
    int edge=rgbcolor(224,196,120);
    drawbox(x-1,y-1,width+2,1,layer,edge,0);
    drawbox(x-1,y+height,width+2,1,layer,edge,0);
    drawbox(x-1,y,1,height,layer,edge,0);
    drawbox(x+width,y,1,height,layer,edge,0);
}
void localizedParagraph(void key,int x,int y,int layer)
{
    int lang=getglobalvar("ui_lang");if(lang==NULL())lang=0;
    int i;
    if(getglobalvar("loc_paragraph_key")!=key || getglobalvar("loc_paragraph_lang")!=lang+1){
        for(i=0;i<5;i++)setglobalvar("loc_line_"+i,nativeParagraphLine(key,i));
        setglobalvar("loc_paragraph_key",key);setglobalvar("loc_paragraph_lang",lang+1);
    }
    for(i=0;i<5;i++){
        void text=getglobalvar("loc_line_"+i);
        if(text!="")drawstring(x,y+i*15,5,text,layer);
    }
}
int localizedDialog(void a,void b,void c,void d,int x,int y,int layer)
{
    if(a==NULL())a="";if(b==NULL())b="";if(c==NULL())c="";if(d==NULL())d="";
    int page=nativeDialogPage(a,b,c,d);if(page<0)return 0;
    localizedParagraph("dialog:"+page,x,y,layer);
    return 1;
}
void localizeFighterLabels()
{
    int i;for(i=0;i<openborvariant("ent_max");i++){
        void entity=getentity(i);
        if(entity==NULL() || !getentityproperty(entity,"exists") || getentityproperty(entity,"type")!=openborconstant("TYPE_ENEMY") || getentityvar(entity,"loc_named")==1)continue;
        void model=getentityproperty(entity,"defaultname");
        if(getentityproperty(entity,"name")==model){void label=nativeSoldierLabel(model);if(label!=NULL())changeentityproperty(entity,"name",label);}
        setentityvar(entity,"loc_named",1);
    }
}
void localizeSceneTitles()
{
    if(openborvariant("current_scene")!="data/scenes/gameover.txt")return;
    int x=(openborvariant("hresolution")-300)/2;
    setdrawmethod(NULL(),0,256,256,0,0,0,0);
    drawbox(x,35,300,42,1000000400,rgbcolor(0,0,0),0);
    localizationSprite("data/sprites/localization/"+locCode(getglobalvar("ui_lang"))+"/game-over.png",x,35,1000000401);
}
#endif
