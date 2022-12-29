#import "data/scripts/didhit/tint_main.h"

void main()
{
	tintEffect(openborconstant("ANI_IDLE"), "ice", 1);
	tintEffect(openborconstant("ANI_RUNATTACK"), "ice", 1);
	tintEffect(openborconstant("ANI_RISEATTACK"), "ice", 1);
	tintEffect(openborconstant("ANI_SPECIAL"), "ice", 1);
	tintEffect(openborconstant("ANI_SPECIAL2"), "ice", 1);
	tintEffect(openborconstant("ANI_FREESPECIAL"), "ice", 1);
	tintEffect(openborconstant("ANI_FREESPECIAL6"), "ice", 1);

	if(getlocalvar("attacktype") == openborconstant("ATK_FREEZE")){

		//GET NEW VALUES
		void target		= getlocalvar("damagetaker");
		int tintMode	= 6;
		
		//APPLY EFFECTS
		changedrawmethod(target, "enabled", 1);
		changedrawmethod(target, "tintmode", tintMode);
		changedrawmethod(target, "tintcolor", rgbcolor(0x33, 0xFF, 0xFF));
	}
}