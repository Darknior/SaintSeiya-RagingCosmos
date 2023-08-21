#import "data/scripts/didhit/main.h"

void main()
{
	tintEffect(openborconstant("ANI_IDLE"), "ice", 1);
   tintEffect(openborconstant("ANI_PAIN10"), "ice", 1);
   tintEffect(openborconstant("ANI_PAIN11"), "ice", 1);
	tintEffect(openborconstant("ANI_RUNATTACK"), "ice", 1);
	tintEffect(openborconstant("ANI_RISEATTACK"), "ice", 1);
	tintEffect(openborconstant("ANI_SPECIAL"), "ice", 1);
	tintEffect(openborconstant("ANI_SPECIAL2"), "ice", 1);
	tintEffect(openborconstant("ANI_ATTACKBOTH"), "ice", 1);
	tintEffect(openborconstant("ANI_FREESPECIAL"), "ice", 1);
   tintEffect(openborconstant("ANI_FREESPECIAL2"), "ice", 1);
	tintEffect(openborconstant("ANI_FREESPECIAL5"), "ice", 1);
	tintEffect(openborconstant("ANI_FREESPECIAL6"), "ice", 1);
	autoLand();
	rushFont();

	//EXCEPTION FOR "FREEZE TYPE" ATTACKS
	if(getlocalvar("attacktype") == openborconstant("ATK_FREEZE")){
		if(!getlocalvar("blocked")){

			//GET NEW VALUES
			void target		= getlocalvar("damagetaker");
			int tintMode	= 6;
			
			//APPLY EFFECTS
			changedrawmethod(target, "enabled", 1);
			changedrawmethod(target, "tintmode", tintMode);
			changedrawmethod(target, "tintcolor", rgbcolor(0x33, 0xFF, 0xFF));
		}
	}

	//EXCEPTION FOR THE CUSTOM "PAIN10" ANIMATION
	if(getentityproperty(getlocalvar("self"), "defaultname") == "hyoga_diamonds"){
		tintEffect(openborconstant("ANI_IDLE"), "ice", 2.5);
	}
}