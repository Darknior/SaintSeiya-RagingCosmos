#import "data/scripts/native_localization_catalog.c"

void versusText(int y, int font, void text)
{
    int x=(openborvariant("hresolution")-strwidth(text,font))/2;
    drawstring(x,y,font,text,1000000220);
}

void versusPrepare(void fighter, int player)
{
    int target=openborconstant("TYPE_PLAYER");
    changeentityproperty(fighter,"candamage",target);
    changeentityproperty(fighter,"hostile",target);
    changeentityproperty(fighter,"maxhealth",400);
    changeentityproperty(fighter,"health",400);
    changeentityproperty(fighter,"position",260+280*player,414,0);
    changeentityproperty(fighter,"direction",1-player);
}

void versusTraining(void player)
{
    versusText(49,7,nativeText("Versus Training"));
    if(player==NULL())return;

    void dummy=getindexedvar(0);
    if(dummy==NULL() || !getentityproperty(dummy,"exists") ||
        getentityproperty(dummy,"health")<=0){
        clearspawnentry();
        setspawnentry("name","malesoldier");
        setspawnentry("coords",540,2,0);
        dummy=spawn();
        if(dummy==NULL())return;
        setindexedvar(0,dummy);
        changeentityproperty(dummy,"position",540,414,0);
        changeentityproperty(dummy,"direction",0);
        changeentityproperty(dummy,"noaicontrol",1);
    }

    changeentityproperty(player,"mp",getentityproperty(player,"maxmp"));
}

void main()
{
    if(openborvariant("game_paused"))return;

    // The arena owns its timer. Keep the campaign timeout inactive here.
    changeopenborvariant("game_time",40000);

    void p1=getplayerproperty(0,"entity");
    void p2=getplayerproperty(1,"entity");
    if(openborvariant("current_branch")=="versus_training"){
        versusTraining(p1);
        return;
    }

    int score1=getglobalvar("versus_score1");
    int score2=getglobalvar("versus_score2");
    int deadline=getglobalvar("versus_result_until");
    int winner=getglobalvar("versus_winner");
    int now=openborvariant("elapsed_time");
    if(score1==NULL())score1=0;
    if(score2==NULL())score2=0;

    if(deadline!=NULL()){
        void result=nativeText("Double KO");
        if(winner)result="P"+winner+" - "+nativeText("Wins");
        versusText(145,7,result);
        if(now>=deadline){
            setglobalvar("versus_result_until",NULL());
            if(score1==2 || score2==2){
                if(!playerkeys(0,1,"attack") && !playerkeys(1,1,"attack")){
                    setglobalvar("versus_result_until",deadline);
                    versusText(171,0,nativeText("Attack: rematch"));
                    return;
                }
                setglobalvar("versus_score1",NULL());
                setglobalvar("versus_score2",NULL());
            }
            changeplayerproperty(0,"lives",3);
            changeplayerproperty(1,"lives",3);
            setglobalvar("versus_round_started",NULL());
            jumptobranch("versus_ring",1);
        }
        return;
    }

    if(!getglobalvar("versus_round_started")){
        if(p1==NULL() || p2==NULL()){
            versusText(145,7,nativeText("Versus"));
            versusText(171,0,nativeText("P2: press Start"));
            return;
        }
        versusPrepare(p1,0);
        versusPrepare(p2,1);
        setglobalvar("versus_round_started",now+1);
    }

    versusText(49,7,"P1  "+score1+"  VS  "+score2+"  P2");
    int lost1=p1==NULL();
    int lost2=p2==NULL();
    if(!lost1)lost1=getentityproperty(p1,"health")<=0;
    if(!lost2)lost2=getentityproperty(p2,"health")<=0;
    // elapsed_time uses 200 ticks per second on OpenBOR 4.
    int ticks=19800-now+getglobalvar("versus_round_started")-1;
    if(ticks<0)ticks=0;
    int seconds=(ticks+199-(ticks+199)%200)/200;
    versusText(76,7,""+seconds);
    if(!lost1 && !lost2){
        if(ticks>0)return;
        int hp1=getentityproperty(p1,"health");
        int hp2=getentityproperty(p2,"health");
        lost1=hp1<=hp2;
        lost2=hp2<=hp1;
    }

    winner=0;
    if(lost2 && !lost1){winner=1;score1++;}
    if(lost1 && !lost2){winner=2;score2++;}
    setglobalvar("versus_winner",winner);
    setglobalvar("versus_score1",score1);
    setglobalvar("versus_score2",score2);
    setglobalvar("versus_result_until",now+250);
}

void ondestroy()
{
    setglobalvar("versus_round_started",NULL());
}
