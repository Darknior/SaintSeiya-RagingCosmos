#ifndef RAGING_DIALOG_POSITIONS
#define RAGING_DIALOG_POSITIONS 1
void dialogPositions(int hidden)
{
    int i;int count=openborvariant("ent_max");
    for(i=0;i<count;i++){
        void entity=getentity(i);
        if(entity==NULL() || !getentityproperty(entity,"exists"))continue;
        int type=getentityproperty(entity,"type");
        if(type!=openborconstant("TYPE_PLAYER") && type!=openborconstant("TYPE_ENEMY"))continue;
        if(hidden){
            if(getentityvar(entity,"dialog_hidden")!=1){
                setentityvar(entity,"dialog_saved_z",getentityproperty(entity,"z"));
                setentityvar(entity,"dialog_hidden",1);
            }
            changeentityproperty(entity,"z",-999);
        }else if(getentityvar(entity,"dialog_hidden")==1){
            changeentityproperty(entity,"z",getentityvar(entity,"dialog_saved_z"));
            setentityvar(entity,"dialog_saved_z",NULL());
            setentityvar(entity,"dialog_hidden",NULL());
        }
    }
    if(hidden)setglobalvar("dialog_hidden_active",1);
    else setglobalvar("dialog_hidden_active",NULL());
}
void restoreClosedDialog()
{
    if(getglobalvar("dialog_hidden_active")!=1)return;
    int i;int count=openborvariant("ent_max");
    for(i=0;i<count;i++){
        void entity=getentity(i);
        if(entity!=NULL() && getentityproperty(entity,"exists") && getentityproperty(entity,"type")==openborconstant("TYPE_TEXTBOX"))return;
    }
    // Some native timelines expire without an explicit lockButton=0 frame.
    // Once the last textbox/fade is gone, release and restore its combatants.
    setglobalvar("lockButton",0);
    setglobalvar("dialog_entity",NULL());
    dialogPositions(0);
}
#endif
