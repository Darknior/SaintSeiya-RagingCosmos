#import "data/scripts/didhit/tint_main.h"

void main()
{
	tintEffect(openborconstant("ANI_IDLE"), "red", 1);
	tintEffect(openborconstant("ANI_RUNATTACK"), "red", 1);
	tintEffect(openborconstant("ANI_SPECIAL"), "red", 1);
	tintEffect(openborconstant("ANI_SPECIAL2"), "red", 1);
	tintEffect(openborconstant("ANI_FREESPECIAL"), "red", 1);
	tintEffect(openborconstant("ANI_FREESPECIAL2"), "red", 1);
	tintEffect(openborconstant("ANI_FREESPECIAL3"), "red", 1);
	tintEffect(openborconstant("ANI_FREESPECIAL5"), "red", 1);
	tintEffect(openborconstant("ANI_FREESPECIAL6"), "red", 1);
	genmaken();
}

void genmaken()
{
	void self	= getlocalvar("self");
	void target	= getlocalvar("damagetaker");
	void name	= getentityproperty(self, "defaultname");

	if(name == "ikki"){
		void anim	= getentityproperty(self, "animationID");
		void atk	= getlocalvar("attacktype");
		int blocked	= getlocalvar("blocked");

		if(!blocked){
			if(anim == openborconstant("ANI_FREESPECIAL6")){
				if(atk == openborconstant("ATK_NORMAL6")){

					//GET NEW VALUES
					void target		= getlocalvar("damagetaker");
					int tintMode	= 2;
					
					//APPLY EFFECTS
					changedrawmethod(target, "enabled", 1);
					changedrawmethod(target, "tintmode", tintMode);
					changedrawmethod(target, "tintcolor", rgbcolor(0xDD, 0x00, 0x00));
					setentityvar(target, "genmaken", 1);
				}
			}
		}
	}
}