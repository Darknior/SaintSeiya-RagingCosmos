void tintEffect(void vAnim, void effect, int duration, int itemType)
{
	void self	= getlocalvar("self");
	void target	= getlocalvar("damagetaker");
	void cAnim	= getentityproperty(self, "animationID"); //CURRENT ANIMATION
	int blocked	= getlocalvar("blocked"); //CHECK IF THE ATTACK WAS BLOCKED
	float time	= openborvariant("elapsed_time");

	if(!blocked){
		if(cAnim == vAnim){
			float mult	= duration;
			float limit = 200*mult;
			setentityvar(target, effect+"Time", time+limit);
		}
	}

	if(itemType){
		float mult	= duration;
		float limit = 200*mult;
		setentityvar(target, effect+"Time", time+limit);
	}
}

void autoLand()
{//Enable native autoland function in any knockdown hit

	if(!getlocalvar("blocked") && getlocalvar("drop")){
		changeentityproperty(getlocalvar("damagetaker"), "aiflag", "projectile", 2);
	}
}

void rushFont()
{//Trigger a temporary effect for fonts during a rush combo by sending a timed variable to the updated.c event
	void self	= getlocalvar("self");
	void type	= getentityproperty(self, "type");
	int minRush	= 1;
	int delay	= 75;
	float time	= openborvariant("elapsed_time");
	
	if(type != openborconstant("TYPE_PLAYER")){
		void parent	= getentityproperty(self, "parent");
		int rush	= getentityproperty(parent, "rush_count");
		if(rush >= minRush){setentityvar(parent, "rushFont", time+delay);}
	}
	else
	{
		int rush	= getentityproperty(self, "rush_count");
		if(rush >= minRush){setentityvar(self, "rushFont", time+delay);}
	}
}