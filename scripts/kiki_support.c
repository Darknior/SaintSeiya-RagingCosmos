#import "data/scripts/native_localization_catalog.c"

void main()
{
    if(!openborvariant("in_level") || openborvariant("game_paused"))return;
    if(getlocalvar("kiki_used"))return;

    int ready=0;
    int now=openborvariant("elapsed_time");
    int i;
    for(i=0;i<openborvariant("maxplayers");i++){
        void hero=getplayerproperty(i,"entity");
        if(hero==NULL())continue;
        if(getentityproperty(hero,"health")<=0)continue;
        float dx=getentityproperty(hero,"x")-400;
        float dz=getentityproperty(hero,"z")-409;
        if(dx<-80 || dx>80 || dz<-27 || dz>27)continue;
        ready=1;
        if(!playerkeys(i,1,"attack"))continue;

        // Libra's aid is optional. It holds nearby guards for a short opening.
        int j;
        int affected=0;
        for(j=0;j<openborvariant("ent_max");j++){
            void guard=getentity(j);
            if(guard==NULL() || !getentityproperty(guard,"exists"))continue;
            if(getentityproperty(guard,"type")!=openborconstant("TYPE_ENEMY"))continue;
            if(getentityproperty(guard,"health")<=0 || getentityproperty(guard,"a")>36)continue;
            if(getentityproperty(guard,"aiflag","frozen"))continue;
            changeentityproperty(guard,"aiflag","frozen",1);
            changeentityproperty(guard,"freezetime",now+240);
            affected=1;
        }
        if(affected)setlocalvar("kiki_used",1);
        break;
    }
    int x=400-openborvariant("xpos");
    int y=300-openborvariant("ypos");
    void text="Kiki";
    if(ready)text=text+" - "+nativeText("Attack");
    drawstring(x-strwidth(text,0)/2,y,0,text,1000000210);
}
