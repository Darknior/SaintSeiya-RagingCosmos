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
    void paragraph="mu:"+key;int width=localizedParagraphWidth(paragraph)+24;
    int height=localizedParagraphLines(paragraph)*18+24;
    if(width<120)width=120;if(height<48)height=48;
    int left=x+18;int top=y+8+(100-height)/2;
    if(left+width>openborvariant("hresolution")-8)left=openborvariant("hresolution")-width-8;
    if(top<8)top=8;if(top+height>openborvariant("vresolution")-8)top=openborvariant("vresolution")-height-8;
    localizedTextFrame(left,top,width,height,layer+1);
    localizedParagraph(paragraph,left+12,top+12,layer+2);
}
