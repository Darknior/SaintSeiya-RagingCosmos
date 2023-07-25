void main()
{
	lifeBar();
}

void lifeBar()
{//Script used to manage bosses hud properties
	void self	= getlocalvar("self");
	int dead	= getentityproperty(self, "dead");
	
	//DETECTS IF THE ENTITY IS IN-GAME AND ALIVE
	if(!dead){

		//THE LIFEBAR POSITION DEFINES ALL OTHER ELEMENT'S POSITION
		int xLife	= 225;
		int yLife	= 461;
		int xName	= xLife+50;
		int yName	= yLife-9;
		int xIcon	= xLife+1;
		int yIcon	= yLife-36;
		int layer1	= 10000;
		int layer2	= 60000;
		int layer3	= 50000;

		//LOAD ASSETS
		if(getglobalvar("charIcon3") == NULL()){setglobalvar("charIcon3", loadsprite("data/sprites/charicon3.png"));}
		if(getglobalvar("lifebar3") == NULL()){setglobalvar("lifebar3", loadsprite("data/sprites/lifebar3.png"));}			

		//ADJUST BOSSES HUD POSITION
		changeentityproperty(self, "lifeposition", 9999, 9999);
		changeentityproperty(self, "nameposition", xName, yName);
		changeentityproperty(self, "iconposition", xIcon, yIcon);

		setdrawmethod(NULL(),0,256,256,0,0,0,0);
		drawsprite(getglobalvar("charIcon3"), xLife-1, yLife-47, layer1); //BOSS CHARACTER ICON OVERLAY
		setdrawmethod(NULL(),1,256,256,0,0,0,2);
		drawsprite(getglobalvar("lifebar3"), xLife+48, yLife+1, layer2); //BOSS LIFEBAR OVERLAY

		//DRAW CUSTOM LIFE BARS
		int maxLife	= getentityproperty(self, "maxhealth");
		int life	= getentityproperty(self, "health");
		float xSize	= 300; //BAR WIDTH INCREASE FACTOR, MORE VALUE IS MORE SIZE (PLAYER LIFE)
		float ySize	= 9; //BAR HEIGHT INCREASE FACTOR, MORE VALUE IS MORE SIZE

		//LIFE BAR
		if(life > 0){
			life	= (life*xSize)/(maxLife); //CALCULATE REMAINING LIFE BAR SIZE
			maxLife	= (maxLife*xSize)/(maxLife); //CALCULATE MAX LIFE BAR SIZE
			drawbox(xLife+49, yLife+1, life, ySize, layer3, rgbcolor(255,255,255), 0); //YELLOW BAR, LIFE REMAINING
		}
	}
	else
	{
		//REMOVE HUD IF DIES
		changeentityproperty(self, "lifeposition", 9999, 9999);
		changeentityproperty(self, "nameposition", 9999, 9999);
		changeentityproperty(self, "iconposition", 9999, 9999);
	}
}