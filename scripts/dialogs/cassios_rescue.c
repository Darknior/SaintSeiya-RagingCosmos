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
	int vFrame		= getentityproperty(self, "animpos");
	int pFrame		= getentityproperty(parent, "animpos");
	int dx			= getentityproperty(self, "x");
	int px			= getentityproperty(parent, "x");
	int xBind		= 35;
	int yBind		= 55;
	int zBind		= -1;
	int dBind		= -1;
	int dist		= 160;
	
	if(vAnim == openborconstant("ANI_IDLE")){
		if(dx >= px-xBind){
			bindentity(parent, self, xBind, zBind, yBind, dBind);
			changeentityproperty(self, "velocity", 0, NULL(), NULL());
			changeentityproperty(self, "animation", openborconstant("ANI_FOLLOW1"));
			updateframe(parent, pFrame+1);
		}
	}

	if(vAnim == openborconstant("ANI_FOLLOW1")){
		if(getglobalvar("cassios_rescue") == "fall"){
			bindentity(parent, NULL());
			changeentityproperty(self, "animation", openborconstant("ANI_FOLLOW2"));
			changeentityproperty(self, "position", xPos+dist);
			changeentityproperty(parent, "position", xPos+dist*2, NULL(), 0);
			changeentityproperty(parent, "direction", 0);
			updateframe(parent, pFrame+1);
		}
	}

	if(vAnim == openborconstant("ANI_FOLLOW2")){
		if(getglobalvar("aiolia") == "fall"){
			updateframe(parent, pFrame+1);
			setglobalvar("aiolia", NULL());
		}
	}
}