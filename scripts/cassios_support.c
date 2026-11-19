// One brief reprieve during the Lion fight, with the original dialogue intact.
void main()
{
    if(!openborvariant("in_level") || openborvariant("game_paused"))return;
    if(getlocalvar("cassios_helped") || getglobalvar("lockButton")==1 || getglobalvar("line1"))return;

    void hero=NULL();
    float weakest=1.0;
    int i;
    for(i=0;i<openborvariant("maxplayers");i++){
        void candidate=getplayerproperty(i,"entity");
        if(candidate==NULL())continue;
        int hp=getentityproperty(candidate,"health");
        int maximum=getentityproperty(candidate,"maxhealth");
        if(hp<=0 || maximum<=0 || getentityproperty(candidate,"invincible"))continue;
        float ratio=hp*1.0/maximum;
        if(ratio<=0.25 && ratio<weakest){
            weakest=ratio;
            hero=candidate;
        }
    }
    if(hero==NULL())return;

    int now=openborvariant("elapsed_time");
    if(now<600)return;
    int bossActive=0;
    for(i=0;i<openborvariant("ent_max");i++){
        void actor=getentity(i);
        if(actor==NULL() || !getentityproperty(actor,"exists"))continue;
        if(getentityproperty(actor,"defaultname")=="aiolia" && getentityproperty(actor,"health")>0){
            bossActive=1;
            break;
        }
    }
    if(!bossActive)return;
    int direction=getentityproperty(hero,"direction");
    float x=getentityproperty(hero,"x")+32*(direction*2-1);
    float z=getentityproperty(hero,"z");
    clearspawnentry();
    setspawnentry("name","cassios_support");
    void cassios=spawn();
    if(cassios==NULL())return;
    changeentityproperty(cassios,"position",x,z,0);
    changeentityproperty(cassios,"direction",direction);
    changeentityproperty(hero,"invincible",1);
    changeentityproperty(hero,"invinctime",now+300);
    setlocalvar("cassios_helped",1);
}
