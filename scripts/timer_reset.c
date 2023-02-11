void main()
{//Reset the current level if the timer ends
	void self	= getlocalvar("self");
	int pIndex	= getentityproperty(self, "playerindex");
	int lives	= getplayerproperty(pIndex, "lives");
	int atkType	= getlocalvar("attacktype");
	float time	= openborvariant("elapsed_time");
	float delay	= 400;

	//SAVE A VARIABLE TO TELL THE SCRIPT TO RESET CURRENT LEVEL AND DRAW THE "TIME OVER" MESSAGE
	if(atkType == openborconstant("ATK_TIMEOVER")){
		changeplayerproperty(pIndex, "lives", lives-1);
		setglobalvar("reseTimer", time+delay);
	}
}