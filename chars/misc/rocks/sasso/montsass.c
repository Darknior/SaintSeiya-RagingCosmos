

void main()
{
void self = getlocalvar("self");
int sanim = getentityproperty(self, "animationid");
int atktype = getlocalvar("attacktype");

if(sanim == 0/*idle*/)
{   
changeentityproperty(self, "animation", openborconstant("ANI_FOLLOW1"));
}

if(sanim == 81/*follow1*/)
{   
changeentityproperty(self, "animation", openborconstant("ANI_FOLLOW2"));
}

if(sanim == 82/*follow2*/)
{   
changeentityproperty(self, "animation", openborconstant("ANI_FOLLOW3"));
}

if(sanim == 83/*follow3*/)
{   
changeentityproperty(self, "animation", openborconstant("ANI_FOLLOW4"));
}

}
