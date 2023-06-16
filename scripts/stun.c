void main()
{
	void self	= getlocalvar("self");
	void parent = getentityproperty(self, "parent");
	void pAnim	= getentityproperty(parent, "animationID");

	if(pAnim != openborconstant("ANI_FOLLOW1") && pAnim != openborconstant("ANI_RISE")){
		killentity(self);
	}
}