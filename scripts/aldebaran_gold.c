void main()
{
	goldEffect();
}

void goldEffect(){
	void self	= getlocalvar("self");
	void target	= getlocalvar("damagetaker");
	void anim	= getentityproperty(self, "animationID");
	float time	= openborvariant("elapsed_time");

	if(	anim == openborconstant("ANI_SPECIAL")		||
		anim == openborconstant("ANI_SPECIAL2")		||
		anim == openborconstant("ANI_RUNATTACK")	||
		anim == openborconstant("ANI_FREESPECIAL")	||
		anim == openborconstant("ANI_FREESPECIAL2")	||
		anim == openborconstant("ANI_FREESPECIAL6")	){
		
		float mult	= 1;
		float limit = 200*mult;
		setentityvar(target, "goldTime", time+limit);
	}
}