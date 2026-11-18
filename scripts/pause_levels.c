#import "data/scripts/native_localization_catalog.c"

// Story Mode branch starts. Some lead with their original cinematic.
void pauseLevelBranch(int index)
{
    if(index==0)return "temple0";
    if(index==1)return "temple1";
    if(index==2)return "temple2";
    if(index==3)return "temple2BOSS";
    if(index==4)return "temple3";
    if(index==5)return "temple3BOSS";
    if(index==6)return "temple4";
    if(index==7)return "temple4BOSS";
    if(index==8)return "temple5";
    if(index==9)return "temple5BOSS";
    if(index==10)return "temple6";
    if(index==11)return "temple6BOSS";
    if(index==12)return "temple7";
    if(index==13)return "temple8";
    if(index==14)return "temple8BOSS";
    if(index==15)return "temple10";
    if(index==16)return "temple10BOSS";
    if(index==17)return "temple11";
    if(index==18)return "temple11BOSS";
    if(index==19)return "temple12";
    if(index==20)return "temple12BOSS";
    if(index==21)return "temple13";
    return "temple13BOSS";
}

void pauseLevelText(int center, int y, int font, void text)
{
    drawstring(center-strwidth(text,font)/2,y,font,text,1000000211);
}

int pauseLevelSelect()
{
    if(!openborvariant("in_level") || openborvariant("current_set")!=1 ||
        openborvariant("game_paused")!=2){
        setglobalvar("pause_level_index",NULL());
        return 0;
    }

    int index=getglobalvar("pause_level_index");
    int armed=1;
    if(index==NULL()){
        void current=openborvariant("current_branch");
        index=0;
        int i=0;
        while(i<23){
            if(pauseLevelBranch(i)==current)index=i;
            i++;
        }
        armed=0;
    }
    int confirm=0;
    int player=0;
    while(player<4){
        if(getplayerproperty(player,"entity")!=NULL() && armed){
            if(playerkeys(player,1,"moveleft"))index=(index+22)%23;
            if(playerkeys(player,1,"moveright"))index=(index+1)%23;
            if(playerkeys(player,1,"attack"))confirm=1;
        }
        player++;
    }
    setglobalvar("pause_level_index",index);

    if(confirm){
        setglobalvar("timeOver",NULL());
        setglobalvar("reseTimer",NULL());
        jumptobranch(pauseLevelBranch(index),1);
        // OpenBOR 4.0 SDID_ESC is bit 12. The native pause owner consumes
        // this key and closes the pause loop before the branch transition.
        player=0;
        while(player<4){
            changeplayerproperty(player,"newkeys",4096);
            player++;
        }
        return 1;
    }

    int center=openborvariant("hresolution")/2;
    int y=openborvariant("vresolution")-110;
    int temple=0;
    if(index>=2 && index<=11)temple=2+(index-2)/2;
    else if(index>=15)temple=10+(index-15)/2;
    else if(index>=13)temple=8;
    else if(index==12)temple=7;
    else temple=index;
    int boss=index==3 || index==5 || index==7 || index==9 || index==11 ||
        index==14 || index==16 || index==18 || index==20 || index==22;
    void label="<  "+temple;
    if(boss)label=label+" - Boss";
    label=label+"  >";
    drawbox(center-150,y-6,300,88,1000000210,rgbcolor(0,0,0),0);
    pauseLevelText(center,y,0,nativeText("Level Select"));
    pauseLevelText(center,y+19,1,label);
    pauseLevelText(center,y+39,0,nativeText("Left / Right to choose"));
    pauseLevelText(center,y+58,0,nativeText("Attack to go"));
    return 1;
}
