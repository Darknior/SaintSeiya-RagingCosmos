void main()
{//Script used to manage bosses hud properties
	void self	= getlocalvar("self");
	int dead	= getentityproperty(self, "dead");
	
	//DETECTS IF THE ENTITY IS IN-GAME AND ALIVE
	if(!dead){

		//THE LIFEBAR POSITION DEFINES ALL OTHER ELEMENT'S POSITION
		int xLife	= 249;
		int yLife	= 461;
		int xName	= xLife+50;
		int yName	= yLife-9;
		int xIcon	= xLife+1;
		int yIcon	= yLife-46;
		int layer1	= 10000;
		int layer2	= 60000;

		//LOAD ASSETS
		if(getglobalvar("charIcon3") == NULL()){setglobalvar("charIcon3", loadsprite("data/sprites/charicon3.png"));}
		if(getglobalvar("lifebar3") == NULL()){setglobalvar("lifebar3", loadsprite("data/sprites/lifebar3.png"));}			

		//ADJUST BOSSES HUD POSITION
		changeentityproperty(self, "lifeposition", xLife, yLife);
		changeentityproperty(self, "nameposition", xName, yName);
		changeentityproperty(self, "iconposition", xIcon, yIcon);

		setdrawmethod(NULL(),0,256,256,0,0,0,0);
		drawsprite(getglobalvar("charIcon3"), xLife, yLife-47, layer1); //BOSS CHARACTER ICON OVERLAY
		setdrawmethod(NULL(),1,256,256,0,0,0,2);
		drawsprite(getglobalvar("lifebar3"), xLife, yLife+1, layer2); //BOSS LIFEBAR OVERLAY
	}
	else
	{
		//REMOVE HUD IF DIES
		changeentityproperty(self, "lifeposition", 9999, 9999);
		changeentityproperty(self, "nameposition", 9999, 9999);
		changeentityproperty(self, "iconposition", 9999, 9999);
	}
}