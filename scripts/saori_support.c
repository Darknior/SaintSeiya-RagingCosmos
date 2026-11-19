// Saori rallies injured fighters who return to her during the prologue.
void main()
{
    if(!openborvariant("in_level") || openborvariant("game_paused"))return;
    if(getglobalvar("line1") || getglobalvar("lockButton")==1)return;

    int player;
    for(player=0;player<openborvariant("maxplayers");player++){
        if(getlocalvar("saori_helped_"+player))continue;
        void hero=getplayerproperty(player,"entity");
        if(hero==NULL())continue;
        int current=getentityproperty(hero,"health");
        int maximum=getentityproperty(hero,"maxhealth");
        if(current<=0 || current*2>maximum)continue;

        void saori=NULL();
        int i;
        for(i=0;i<openborvariant("ent_max");i++){
            void actor=getentity(i);
            if(actor==NULL() || !getentityproperty(actor,"exists"))continue;
            if(getentityproperty(actor,"defaultname")=="athena" && getentityproperty(actor,"health")>0){
                saori=actor;
                break;
            }
        }
        if(saori==NULL())return;
        float dx=getentityproperty(hero,"x")-getentityproperty(saori,"x");
        float dz=getentityproperty(hero,"z")-getentityproperty(saori,"z");
        if(dx<-85 || dx>85 || dz<-40 || dz>40)continue;
        changeentityproperty(hero,"health",current+maximum/5);
        setlocalvar("saori_helped_"+player,1);
    }
}
