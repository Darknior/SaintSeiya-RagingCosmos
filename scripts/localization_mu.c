#import "data/scripts/native_localization_catalog.c"
#import "data/scripts/localization.c"
void main()
{
    void self=getlocalvar("self");int animation=getentityproperty(self,"animationid");void key="combo";
    if(animation==openborconstant("ANI_FOLLOW1"))key="down";
    if(animation==openborconstant("ANI_FOLLOW2"))key="up";
    if(animation==openborconstant("ANI_FOLLOW3"))key="special";
    if(animation==openborconstant("ANI_FOLLOW4"))key="dash";
    if(animation==openborconstant("ANI_FOLLOW5"))key="charge";
    if(animation==openborconstant("ANI_FOLLOW6"))key="throw";
    if(animation==openborconstant("ANI_FOLLOW7"))key="uppercut";
    if(animation==openborconstant("ANI_FOLLOW8"))key="ultra";
    if(animation==openborconstant("ANI_FOLLOW9"))key="end";
    if(animation==openborconstant("ANI_FOLLOW10"))key="jump";
    int x=getentityproperty(self,"x")-openborvariant("xpos");
    int y=getentityproperty(self,"z")-getentityproperty(self,"y")-openborvariant("ypos")+102;
    int layer=getentityproperty(self,"setlayer")+1;
    setdrawmethod(NULL(),0,256,256,0,0,0,0);
    int lang=getglobalvar("ui_lang");if(lang==NULL())lang=0;
    localizedParagraph("mu:"+key,x+30,y+18,layer+1);
}
