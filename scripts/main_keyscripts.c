void ultra(void anim)
{//Perform ultra attacks
	void self	= getlocalvar("self");
	void vAniID	= getentityproperty(self,"animationID");
	int iPIndex	= getlocalvar("player");
	int mp		= getentityproperty(self,"mp");
	int cost	= getentityproperty(self,"energycost", "cost", anim);
	
	if(playerkeys(iPIndex, 1, "attack2")){
		int checkMp = (mp >= cost);

		if(checkMp){
			if(playerkeys(iPIndex, 0, "moveup")){
				if(vAniID == openborconstant("ANI_WALK") || vAniID == openborconstant("ANI_UP")){
					changeentityproperty(self, "mp", mp-cost);
					changeentityproperty(self, "velocity", 0, 0, 0);
					performattack(self, anim, 0);
				}
			}
		}
	}
}