void main()
{
	iceEffect();
}

void iceEffect(){
	void self	= getlocalvar("self");
	void target	= getlocalvar("damagetaker");
	float time	= openborvariant("elapsed_time");
	float mult	= 1;
	float limit = 200*mult;

	setentityvar(target, "iceTime", time+limit);
}