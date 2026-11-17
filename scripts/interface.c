#import "data/scripts/native_localization_catalog.c"
#import "data/scripts/interface_catalog.c"
#import "data/scripts/localization.c"
// Native menus and native pause are owned by OpenBOR. Only the existing How
// To Play entry is animated; language changes are a title-screen shortcut.
void uiInit()
{
    if(getglobalvar("ui_ready")!=NULL())return;
    int lang=0;int file=openfilestream("ragingcosmos-language.txt",1);
    if(file>=0){lang=getfilestreamargument(file,0,"int");closefilestream(file);}
    if(lang<0 || lang>6)lang=0;
    setglobalvar("ui_lang",lang);setglobalvar("ui_confirmed",1);setglobalvar("ui_selector",0);
    setglobalvar("ui_menu",0);setglobalvar("ui_view",0);setglobalvar("ui_ready",1);
    localizationExport();
}
void uiSave()
{
    int file=createfilestream();filestreamappend(file,getglobalvar("ui_lang"),2);
    savefilestream(file,"ragingcosmos-language.txt");closefilestream(file);localizationExport();
}
void uiRelease(){localizationRelease();}
void uiSprite(void path,int x,int y,int layer){localizationSprite(path,x,y,layer);}
void uiText(void name,int x,int y,int layer){uiSprite("data/sprites/interface/"+locCode(getglobalvar("ui_lang"))+"/"+name+".png",x,y,layer);}
void uiClearKeys()
{
    setglobalvar("ui_key_moveleft",0);setglobalvar("ui_key_moveright",0);setglobalvar("ui_key_moveup",0);setglobalvar("ui_key_movedown",0);
    setglobalvar("ui_key_attack",0);setglobalvar("ui_key_start",0);setglobalvar("ui_key_jump",0);setglobalvar("ui_key_esc",0);
}
void uiInput(int player)
{
    uiInit();
    if(getglobalvar("ui_main_guide")){
        setglobalvar("ui_key_moveleft",playerkeys(player,1,"moveleft"));setglobalvar("ui_key_moveright",playerkeys(player,1,"moveright"));
        setglobalvar("ui_key_moveup",playerkeys(player,1,"moveup"));setglobalvar("ui_key_movedown",playerkeys(player,1,"movedown"));
        setglobalvar("ui_key_attack",playerkeys(player,1,"attack"));setglobalvar("ui_key_start",playerkeys(player,1,"start"));
        setglobalvar("ui_key_jump",playerkeys(player,1,"jump"));setglobalvar("ui_key_esc",playerkeys(player,1,"esc"));
        changeplayerproperty(player,"newkeys",0);return;
    }
    if(openborvariant("in_level"))return;
    if(openborvariant("in_titlescreen")){
        int lang=getglobalvar("ui_lang");
        if(playerkeys(player,1,"moveleft"))lang=(lang+6)%7;
        if(playerkeys(player,1,"moveright"))lang=(lang+1)%7;
        if(lang!=getglobalvar("ui_lang")){uiRelease();setglobalvar("ui_lang",lang);uiSave();}
        return;
    }
    if(openborvariant("in_menuscreen") && !openborvariant("in_options") && !openborvariant("in_new_game") && !openborvariant("in_load_game") && !openborvariant("in_start_game")){
        int row=getglobalvar("ui_menu");
        if(playerkeys(player,1,"moveup"))row=(row+4)%5;
        if(playerkeys(player,1,"movedown"))row=(row+1)%5;
        setglobalvar("ui_menu",row);
        if(row==2 && (playerkeys(player,1,"attack") || playerkeys(player,1,"start"))){
            setglobalvar("ui_main_guide",1);setglobalvar("ui_view",2);setglobalvar("ui_page",0);setglobalvar("ui_clock",openborvariant("ticks"));uiClearKeys();changeplayerproperty(player,"newkeys",0);
        }
    }
}
void uiGuide()
{
    int page=getglobalvar("ui_page");int old=page;int x=(openborvariant("hresolution")-800)/2;int y=(openborvariant("vresolution")-480)/2;int z=1000000300;
    if(getglobalvar("ui_key_moveleft") || getglobalvar("ui_key_moveup"))page=(page+6)%7;
    if(getglobalvar("ui_key_moveright") || getglobalvar("ui_key_movedown") || getglobalvar("ui_key_attack"))page=(page+1)%7;
    if(getglobalvar("ui_key_jump") || getglobalvar("ui_key_esc") || getglobalvar("ui_key_start")){
        setglobalvar("ui_main_guide",NULL());setglobalvar("ui_view",0);uiClearKeys();int i;for(i=0;i<4;i++)changeplayerproperty(i,"newkeys",0);return;
    }
    if(page!=old)setglobalvar("ui_clock",openborvariant("ticks"));setglobalvar("ui_page",page);
    setdrawmethod(NULL(),0,256,256,0,0,0,0);
    uiSprite("data/bgs/temple2D-boss/templeboss.png",x,y,z);
    drawbox(x,y,800,480,z+1,rgbcolor(0,0,0),6);
    uiSprite(guideFrame(page,openborvariant("ticks")-getglobalvar("ui_clock")),x+40,y+126,z+2);
    uiText("guide-"+page,x,y,z+3);uiClearKeys();
}
int interfaceDraw()
{
    uiInit();
    if(getglobalvar("ui_main_guide")){uiGuide();return 1;}
    if(openborvariant("in_titlescreen") && !openborvariant("in_level")){
        int frame=0;int i;
        for(i=0;i<openborvariant("ent_max");i++){
            void entity=getentity(i);
            if(entity!=NULL() && getentityproperty(entity,"exists") && getentityproperty(entity,"defaultname")=="bgtitle")frame=getentityproperty(entity,"animpos");
        }
        if(frame<0 || frame>19)frame=0;
        // Reuse the native animated artwork, so only the stock title prompt
        // changes language immediately, before native Tr reloads on menu entry.
        setdrawmethod(NULL(),0,256,256,0,0,0,0);
        uiSprite("data/bgs/title/title"+(frame+1)+".png",0,0,1000000200);
        if(openborvariant("elapsed_time")%200<100){
            void prompt=nativeText("PRESS START");drawstring((openborvariant("hresolution")-strwidth(prompt,0))/2,openborvariant("vresolution")/2,0,prompt,1000000202);
        }
        void label=nativeLanguageName(getglobalvar("ui_lang"));
        drawstring((openborvariant("hresolution")-strwidth(label,0))/2,openborvariant("vresolution")-26,0,label,1000000202);
        void hint=nativeText("Left / Right: language");drawstring((openborvariant("hresolution")-strwidth(hint,0))/2,openborvariant("vresolution")-40,0,hint,1000000202);
    }
    return 0;
}
