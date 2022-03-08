void main()
{//Move Cassios npc to rescue Seiya
	void self		= getlocalvar("self");
	void player1	= getplayerproperty(0, "entity");
	void player2	= getplayerproperty(1, "entity");
	void player3	= getplayerproperty(2, "entity");
	void player4	= getplayerproperty(3, "entity");
	void parent		= getentityproperty(self, "parent");
	void vAnim		= getentityproperty(self, "animationID");
	void pAnim		= getentityproperty(parent, "animationID");
	float xPos		= openborvariant("xpos");
	int dx			= getentityproperty(self, "x");
	int px			= getentityproperty(parent, "x");
	int xBind		= 40;
	int dist		= 160;
	
	if(vAnim == openborconstant("ANI_IDLE")){
		if(dx >= px){
			bindentity(parent, self, xBind, 0, 0, 0);
			changeentityproperty(self, "velocity", 0, NULL(), NULL());
			changeentityproperty(self, "animation", openborconstant("ANI_FOLLOW1"));
			updateframe(parent, getentityproperty(parent, "animpos")+1);
		}
	}

	if(vAnim == openborconstant("ANI_FOLLOW1")){
		if(getglobalvar("cassios_rescue") == "fall"){
			bindentity(parent, NULL());
			changeentityproperty(self, "animation", openborconstant("ANI_FOLLOW2"));
			changeentityproperty(self, "position", xPos+dist);
			changeentityproperty(parent, "position", xPos+dist*2);
			changeentityproperty(parent, "direction", 0);
		}
	}

	if(vAnim == openborconstant("ANI_FOLLOW2")){
		if(getglobalvar("aiolia") == "fall"){
			updateframe(parent, getentityproperty(parent, "animpos")+1);
			setglobalvar("aiolia", NULL());
		}
	}
}