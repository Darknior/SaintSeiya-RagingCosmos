// The mentor helps each player once on the first road after the tutorial.
void main()
{
    if(!openborvariant("in_level") || openborvariant("game_paused"))return;
    if(openborvariant("elapsed_time")>3000)return;
    int player;
    for(player=0;player<openborvariant("maxplayers");player++){
        if(getlocalvar("mu_helped_"+player))continue;
        void hero=getplayerproperty(player,"entity");
        if(hero==NULL())continue;
        float dx=getentityproperty(hero,"x")-205;
        float dz=getentityproperty(hero,"z")-412;
        if(dx<-148 || dx>148 || dz<-95 || dz>95)continue;
        int maximum=getentityproperty(hero,"maxmp");
        int current=getentityproperty(hero,"mp");
        if(maximum<=0 || current>=maximum)continue;
        int restored=current+maximum/4;
        if(restored>maximum)restored=maximum;
        changeentityproperty(hero,"mp",restored);
        setlocalvar("mu_helped_"+player,1);
    }
}
