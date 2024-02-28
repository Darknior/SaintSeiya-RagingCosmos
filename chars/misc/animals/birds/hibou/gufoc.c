#include "data/scripts/common/H/enemies/vel.H" 
#include "data/scripts/common/H/enemies/selfkill.H" 

void check(int a)
{
void self = getlocalvar("self");
void opp = findtarget(self);
int xs = getentityproperty(self, "x");
int xp = getentityproperty(opp, "x");
int oz = getentityproperty(opp, "z");
int zs = getentityproperty(self, "z");

if((xs-xp)*(xs-xp) <= a && (xs-xp)*(xs-xp) <= a)
	{
	changeentityproperty(self, "animation", openborconstant("ANI_FOLLOW1"));
	}
}


