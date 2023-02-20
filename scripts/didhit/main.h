void tintEffect(void vAnim, void effect, int duration)
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
}

void autoLand()
{//Enable native autoland function in any knockdown hit

	if(!getlocalvar("blocked") && getlocalvar("drop")){
		changeentityproperty(getlocalvar("damagetaker"), "aiflag", "projectile", 2);
	}
}