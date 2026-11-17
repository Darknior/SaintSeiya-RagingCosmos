#import "data/scripts/interface_catalog.c"

// Portable UI: generated Unicode sprites, native pause freeze and remappable keys.
void uiInit()
{
    if(getglobalvar("ui_ready")!=NULL()) return;
    int language=0;int confirmed=0;
    int file=openfilestream("ragingcosmos-language.txt",1);
    if(file>=0){language=getfilestreamargument(file,0,"int");closefilestream(file);if(language>=0 && language<7) confirmed=1;else language=0;}
    setglobalvar("ui_lang",language);setglobalvar("ui_confirmed",confirmed);
    setglobalvar("ui_cache",0);setglobalvar("ui_selector",0);setglobalvar("ui_menu",0);
    setglobalvar("ui_view",0);setglobalvar("ui_page",0);setglobalvar("ui_option",0);
    setglobalvar("ui_row",0);setglobalvar("ui_owner",0);setglobalvar("ui_ready",1);
}
void uiRelease()
{
    int i;int count=getglobalvar("ui_cache");
    for(i=0;i<count;i++){void sprite=getglobalvar("ui_sprite_"+i);if(sprite!=NULL()) free(sprite);setglobalvar("ui_sprite_"+i,NULL());setglobalvar("ui_path_"+i,NULL());}
    setglobalvar("ui_cache",0);
}
void uiSave()
{
    int file=createfilestream();filestreamappend(file,getglobalvar("ui_lang"),2);
    savefilestream(file,"ragingcosmos-language.txt");closefilestream(file);setglobalvar("ui_confirmed",1);
}
void uiSprite(void path,int x,int y,int layer)
{
    int i;int count=getglobalvar("ui_cache");void sprite=NULL();
    for(i=0;i<count;i++) if(getglobalvar("ui_path_"+i)==path) sprite=getglobalvar("ui_sprite_"+i);
    if(sprite==NULL()){sprite=loadsprite(path);setglobalvar("ui_path_"+count,path);setglobalvar("ui_sprite_"+count,sprite);setglobalvar("ui_cache",count+1);}
    if(sprite!=NULL()) drawsprite(sprite,x,y,layer);
}
void uiText(void name,int x,int y,int layer)
{
    uiSprite("data/sprites/interface/"+localeCode(getglobalvar("ui_lang"))+"/"+name+".png",x,y,layer);
}
void uiClearKeys()
{
    setglobalvar("ui_key_moveleft",0);setglobalvar("ui_key_moveright",0);setglobalvar("ui_key_moveup",0);setglobalvar("ui_key_movedown",0);
    setglobalvar("ui_key_attack",0);setglobalvar("ui_key_start",0);setglobalvar("ui_key_jump",0);setglobalvar("ui_key_esc",0);
}
int uiKey(int player,void key)
{
    if(getglobalvar("ui_main_guide")) return getglobalvar("ui_key_"+key);
    return playerkeys(player,1,key);
}
void uiOpenGuide()
{
    setglobalvar("ui_view",2);setglobalvar("ui_page",0);setglobalvar("ui_clock",openborvariant("ticks"));uiClearKeys();
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
    if(openborvariant("in_level")){
        if(playerkeys(player,1,"start") && getplayerproperty(player,"entity")!=NULL()) setglobalvar("ui_owner",player);
        return;
    }
    if(openborvariant("in_menuscreen") && !openborvariant("in_options") && !openborvariant("in_new_game") && !openborvariant("in_load_game") && !openborvariant("in_start_game")){
        int row=getglobalvar("ui_menu");
        if(playerkeys(player,1,"moveup")) row=(row+4)%5;
        if(playerkeys(player,1,"movedown")) row=(row+1)%5;
        setglobalvar("ui_menu",row);
        if(row==2 && (playerkeys(player,1,"attack") || playerkeys(player,1,"start"))){setglobalvar("ui_main_guide",1);setglobalvar("ui_owner",player);uiOpenGuide();changeplayerproperty(player,"newkeys",0);}
        return;
    }
    if(!openborvariant("in_titlescreen")) return;
    int active=getglobalvar("ui_selector");int language=getglobalvar("ui_lang");
    if(!getglobalvar("ui_confirmed")) active=1;
    if(!active && playerkeys(player,1,"special")){setglobalvar("ui_selector",1);changeplayerproperty(player,"newkeys",0);return;}
    if(!active) return;
    if(playerkeys(player,1,"moveup") || playerkeys(player,1,"moveleft")) language=(language+6)%7;
    if(playerkeys(player,1,"movedown") || playerkeys(player,1,"moveright")) language=(language+1)%7;
    if(language!=getglobalvar("ui_lang")){uiRelease();setglobalvar("ui_lang",language);}
    if(playerkeys(player,1,"attack") || playerkeys(player,1,"start")){uiSave();active=0;}
    if((playerkeys(player,1,"jump") || playerkeys(player,1,"esc")) && getglobalvar("ui_confirmed")){uiRelease();setglobalvar("ui_ready",NULL());uiInit();active=0;}
    setglobalvar("ui_selector",active);changeplayerproperty(player,"newkeys",0);
}
void uiMainMenu()
{
    if(!openborvariant("in_menuscreen") || openborvariant("in_options") || openborvariant("in_new_game") || openborvariant("in_load_game") || openborvariant("in_start_game")) return;
    int x=(openborvariant("hresolution")-800)/2;int y=(openborvariant("vresolution")-480)/2;int z=1000000200;
    setdrawmethod(NULL(),0,256,256,0,0,0,0);uiSprite("data/sprites/interface/sanctuary.png",x,y,z);
    drawbox(x+275,y+140+getglobalvar("ui_menu")*46,450,39,z+1,rgbcolor(45,63,89),0);uiText("main",x,y,z+2);
}
void uiGuide(int owner)
{
    int page=getglobalvar("ui_page");int old=page;int i;int x=(openborvariant("hresolution")-800)/2;int y=(openborvariant("vresolution")-480)/2;int z=1000000300;
    if(uiKey(owner,"moveleft") || uiKey(owner,"moveup")) page=(page+6)%7;
    if(uiKey(owner,"moveright") || uiKey(owner,"movedown") || uiKey(owner,"attack")) page=(page+1)%7;
    if(uiKey(owner,"jump") || uiKey(owner,"esc") || uiKey(owner,"start")){
        setglobalvar("ui_view",0);setglobalvar("ui_main_guide",NULL());uiClearKeys();
        for(i=0;i<4;i++) changeplayerproperty(i,"newkeys",0);return;
    }
    if(page!=old) setglobalvar("ui_clock",openborvariant("ticks"));setglobalvar("ui_page",page);
    for(i=0;i<4;i++) changeplayerproperty(i,"newkeys",0);
    setdrawmethod(NULL(),0,256,256,0,0,0,0);uiSprite("data/sprites/interface/sanctuary.png",x,y,z);
    uiSprite(guideFrame(page,openborvariant("ticks")-getglobalvar("ui_clock")),x+40,y+126,z+1);uiText("guide-"+page,x,y,z+2);uiClearKeys();
}
void uiResume()
{
    int i;for(i=0;i<4;i++) changeplayerproperty(i,"newkeys",openborconstant("FLAG_ESC"));
}
int uiPause()
{
    int paused=openborvariant("game_paused");int mode=openborvariant("current_set");int owner=getglobalvar("ui_owner");int count=levelCount(mode,0);int view=getglobalvar("ui_view");int row=getglobalvar("ui_row");int option=getglobalvar("ui_option");int i;
    if(!openborvariant("in_level")){setglobalvar("ui_pause",NULL());return 0;}
    if(paused!=2){
        setglobalvar("ui_pause",NULL());
        if(getglobalvar("ui_quit")!=NULL()){setglobalvar("ui_quit",NULL());gotomainmenu(7);return 1;}
        if(getglobalvar("ui_pending")!=NULL()){
            int target=getglobalvar("ui_pending");setglobalvar("ui_pending",NULL());setglobalvar("dialog_entity",NULL());setglobalvar("lockButton",0);setglobalvar("zoomentity",NULL());setglobalvar("timeOver",NULL());setglobalvar("reseTimer",NULL());changeopenborvariant("current_level",target-1);jumptobranch("",1);return 1;
        }
        return 0;
    }
    if(owner<0 || owner>3 || getplayerproperty(owner,"entity")==NULL()){
        for(i=0;i<4;i++) if(getplayerproperty(i,"entity")!=NULL()){owner=i;break;}setglobalvar("ui_owner",owner);
    }
    if(getglobalvar("ui_pause")==NULL()){
        view=0;option=0;row=0;for(i=0;i<count;i++) if(levelEntry(mode,i)==openborvariant("current_level")) row=i;
        setglobalvar("ui_pause",1);setglobalvar("ui_view",0);setglobalvar("ui_option",0);setglobalvar("ui_row",row);
    }
    if(view==2){uiGuide(owner);return 1;}
    int confirm=playerkeys(owner,1,"attack") || playerkeys(owner,1,"start");int cancel=playerkeys(owner,1,"jump") || playerkeys(owner,1,"esc");
    if(view==0){
        if(playerkeys(owner,1,"moveup")) option=(option+3)%4;
        if(playerkeys(owner,1,"movedown")) option=(option+1)%4;
        if(cancel || (confirm && option==0)){uiResume();return 1;}
        if(confirm && option==1 && count>0) view=1;
        if(confirm && option==2){uiOpenGuide();for(i=0;i<4;i++) changeplayerproperty(i,"newkeys",0);uiGuide(owner);return 1;}
        if(confirm && option==3){setglobalvar("ui_quit",1);uiResume();return 1;}
    }else{
        if(playerkeys(owner,1,"moveup")) row=(row+count-1)%count;
        if(playerkeys(owner,1,"movedown")) row=(row+1)%count;
        if(playerkeys(owner,1,"moveleft")){row=row-9;if(row<0) row=0;}
        if(playerkeys(owner,1,"moveright")){row=row+9;if(row>=count) row=count-1;}
        if(cancel) view=0;
        if(confirm){setglobalvar("ui_pending",levelEntry(mode,row));uiResume();return 1;}
    }
    for(i=0;i<4;i++) changeplayerproperty(i,"newkeys",0);
    setglobalvar("ui_view",view);setglobalvar("ui_row",row);setglobalvar("ui_option",option);
    int x=(openborvariant("hresolution")-560)/2;int y=(openborvariant("vresolution")-330)/2;int z=1000000100;
    setdrawmethod(NULL(),0,256,256,0,0,0,0);drawbox(0,0,openborvariant("hresolution"),openborvariant("vresolution"),z,rgbcolor(0,0,0),6);drawbox(x,y,560,330,z+1,rgbcolor(12,20,35),0);
    if(view==0){drawbox(x+24,y+76+option*40,512,30,z+2,rgbcolor(42,58,84),0);uiText("pause",x,y,z+3);}
    else{
        int first=(row/9)*9;uiText("levels",x,y,z+3);
        for(i=first;i<first+9 && i<count;i++){
            if(i==row) drawbox(x+24,y+76+(i-first)*20,512,20,z+2,rgbcolor(42,58,84),0);
            uiText("level-"+mode+"-"+i,x+28,y+77+(i-first)*20,z+3);
        }
        drawstring(x+190,y+268,0,""+(row+1)+" / "+count,z+3);
    }
    return 1;
}
int interfaceDraw()
{
    uiInit();int x=(openborvariant("hresolution")-800)/2;int y=(openborvariant("vresolution")-480)/2;int z=1000000200;
    if(!openborvariant("in_level") && openborvariant("in_titlescreen")){
        setdrawmethod(NULL(),0,256,256,0,0,0,0);
        if(getglobalvar("ui_selector") || !getglobalvar("ui_confirmed")){
            uiSprite("data/sprites/interface/sanctuary.png",x,y,z);drawbox(x+270,y+130+getglobalvar("ui_lang")*37,270,34,z+1,rgbcolor(45,63,89),0);uiText("selector",x,y,z+2);return 1;
        }
        drawbox(x+200,y+396,600,80,z,rgbcolor(6,13,29),0);uiText("title",x,y+399,z+1);
    }
    if(getglobalvar("ui_main_guide")){uiGuide(getglobalvar("ui_owner"));return 1;}
    if(uiPause()) return 1;
    return 0;
}
void uiRouteIntro()
{
    if(!openborvariant("in_level") || openborvariant("game_paused")==2) return;
    int stage=openborvariant("current_level");int mode=openborvariant("current_set");int count=levelCount(mode,0);int i;
    if(getglobalvar("ui_intro_stage")!=stage+1){setglobalvar("ui_intro_stage",stage+1);setglobalvar("ui_intro_until",openborvariant("elapsed_time")+400);}
    if(openborvariant("elapsed_time")>getglobalvar("ui_intro_until")) return;
    for(i=0;i<count;i++) if(levelEntry(mode,i)==stage){
        void path=levelPath(mode,i);
        if(path=="data/levels/temple8B-midboss.txt") uiSprite("data/sprites/interface/temple8B-midboss-intro.png",0,430,1000000001);
        if(path=="data/levels/temple10A-road.txt") uiSprite("data/sprites/interface/temple10A-road-intro.png",0,430,1000000001);
        if(path=="data/levels/temple10B-midboss.txt") uiSprite("data/sprites/interface/temple10B-midboss-intro.png",0,430,1000000001);
        if(path=="data/levels/temple11B-midboss.txt") uiSprite("data/sprites/interface/temple11B-midboss-intro.png",0,430,1000000001);
        if(path=="data/levels/temple12A-road.txt") uiSprite("data/sprites/interface/temple12A-road-intro.png",0,430,1000000001);
        if(path=="data/levels/temple12B-midboss.txt") uiSprite("data/sprites/interface/temple12B-midboss-intro.png",0,430,1000000001);
        if(path=="data/levels/temple13B-midboss.txt") uiSprite("data/sprites/interface/temple13B-midboss-intro.png",0,430,1000000001);
    }
}
