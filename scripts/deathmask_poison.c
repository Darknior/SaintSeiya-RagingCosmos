void main()
{
	poisonEffect();
}

void poisonEffect(){
	void self	= getlocalvar("self");
	void target	= getlocalvar("damagetaker");
	void anim	= getentityproperty(self, "animationID");
	float time	= openborvariant("elapsed_time");

	if(	anim == openborconstant("ANI_RUNATTACK")	||
		anim == openborconstant("ANI_FREESPECIAL")	){
		
		float mult	= 2;
		float limit = 800*mult;
		setentityvar(target, "poisonTime", time+limit);
	}
}