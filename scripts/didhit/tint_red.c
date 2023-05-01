#import "data/scripts/didhit/main.h"

void main()
{
	tintEffect(openborconstant("ANI_IDLE"), "red", 1);
	tintEffect(openborconstant("ANI_RUNATTACK"), "red", 1);
	tintEffect(openborconstant("ANI_ATTACKBOTH"), "red", 1);
	tintEffect(openborconstant("ANI_SPECIAL"), "red", 1);
	tintEffect(openborconstant("ANI_SPECIAL2"), "red", 1);
	tintEffect(openborconstant("ANI_FREESPECIAL"), "red", 1);
	tintEffect(openborconstant("ANI_FREESPECIAL2"), "red", 1);
	tintEffect(openborconstant("ANI_FREESPECIAL3"), "red", 1);
	tintEffect(openborconstant("ANI_FREESPECIAL5"), "red", 1);
	tintEffect(openborconstant("ANI_FREESPECIAL6"), "red", 1);
	genmaken();
	autoLand();
	rushFont();
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
		int dead	= getentityproperty(target, "dead");

		if(!blocked && !dead){
			if(anim == openborconstant("ANI_FREESPECIAL6")){
				if(atk == openborconstant("ATK_NORMAL6")){

					//GET NEW VALUES
					void vSpawn1;
					void vSpawn2;
					void target		= getlocalvar("damagetaker");
					int tintMode	= 2;
					int dir			= getentityproperty(self, "direction");
					int Px			= getentityproperty(target, "x");
					int Py			= getentityproperty(target, "y");
					int Pz			= getentityproperty(target, "z");
					
					//APPLY EFFECTS
					changedrawmethod(target, "enabled", 1);
					changedrawmethod(target, "tintmode", tintMode);
					changedrawmethod(target, "tintcolor", rgbcolor(0xDD, 0x00, 0x00));

					//UPPER FLAME SPAWN
					clearspawnentry();
					setspawnentry("name", "shaka_flame");
					vSpawn1 = spawn();
					changeentityproperty(vSpawn1, "animation", openborconstant("ANI_FOLLOW1"));
					changeentityproperty(vSpawn1, "position", Px, Pz+1, Py+60);
					changeentityproperty(vSpawn1, "direction", dir);

					//GOUND FLAME SPAWN
					clearspawnentry();
					setspawnentry("name", "ikki_fire");
					vSpawn2 = spawn();
					changeentityproperty(vSpawn2, "position", Px, Pz+1, Py);
					changeentityproperty(vSpawn2, "direction", dir);

					//SET THE GENMAKEN VARIABLE TO ON
					setentityvar(target, "genmaken", 1);
				}
			}
		}
	}
}