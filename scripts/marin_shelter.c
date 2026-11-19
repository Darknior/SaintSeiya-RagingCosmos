void main()
{
    if(!openborvariant("in_level") || openborvariant("game_paused"))return;
    int now=openborvariant("elapsed_time");
    int deadline=getlocalvar("marin_shelter_until");
    if(deadline!=NULL() && now>=deadline)return;

    int i;
    if(deadline==NULL()){
        for(i=0;i<openborvariant("ent_max");i++){
            void actor=getentity(i);
            if(actor==NULL() || !getentityproperty(actor,"exists"))continue;
            if(getentityproperty(actor,"defaultname")!="marin_ally")continue;
            // The opening is ahead of the party, independent of Marin's chase AI.
            float x=openborvariant("xpos")+490;
            setlocalvar("marin_shelter_x",x);
            deadline=now+1200;
            setlocalvar("marin_shelter_until",deadline);
            break;
        }
        if(deadline==NULL())return;
    }

    float center=getlocalvar("marin_shelter_x");
    for(i=0;i<openborvariant("ent_max");i++){
        void guard=getentity(i);
        if(guard==NULL() || !getentityproperty(guard,"exists"))continue;
        if(getentityproperty(guard,"type")!=openborconstant("TYPE_ENEMY"))continue;
        if(getentityproperty(guard,"health")<=0)continue;
        float x=getentityproperty(guard,"x");
        float z=getentityproperty(guard,"z");
        if(x<center-90 || x>center+90 || z<362 || z>419)continue;
        // Keep bodies outside the refuge. Attacks can still reach its edge.
        if(x<center)changeentityproperty(guard,"x",center-94);
        else changeentityproperty(guard,"x",center+94);
    }

    int left=center-90-openborvariant("xpos");
    int top=362-openborvariant("ypos");
    int edge=rgbcolor(240,207,146);
    drawbox(left,top,180,58,289,rgbcolor(72,52,28),6);
    drawbox(left,top,180,1,289,edge,0);
    drawbox(left,top+57,180,1,289,edge,0);
    drawbox(left,top,1,58,289,edge,0);
    drawbox(left+179,top,1,58,289,edge,0);
    drawstring(left+90-strwidth("Marin",0)/2,top+43,0,"Marin",290);
}
