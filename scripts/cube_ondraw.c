void main()
{
	void self = getlocalvar("self");
	void rightSprite;
	void leftSprite;
	float xPos	= openborvariant("xpos");
	float yPos	= openborvariant("ypos");
	float offsetX = 232;
	float offsetY = 325;
	float Px	= getentityproperty(self, "x");
	float Py	= getentityproperty(self, "y");
	float Pz	= getentityproperty(self, "z");
	float difY	= 240;
	float sprX	= Px-xPos-offsetX;
	float sprY	= Pz-yPos-Py-offsetY;
	float textX	= Px-xPos;
	float textY	= Pz-yPos-Py-difY;
	int dead	= getentityproperty(self, "dead");
	int maxHp	= getentityproperty(self, "maxhealth");
	int damageR = getentityvar(self, "damageRight");
	int damageL = getentityvar(self, "damageLeft");
	int layer	= 1001;
	int percent	= maxHp/10;

	//LOAD ALL SPRITES
	//RIGHT SIDE
	if(getglobalvar("R00") == NULL()){setglobalvar("R00", loadsprite("data/chars/cube/right/R00.png"));}
	if(getglobalvar("R01") == NULL()){setglobalvar("R01", loadsprite("data/chars/cube/right/R01.png"));}
	if(getglobalvar("R02") == NULL()){setglobalvar("R02", loadsprite("data/chars/cube/right/R02.png"));}
	if(getglobalvar("R03") == NULL()){setglobalvar("R03", loadsprite("data/chars/cube/right/R03.png"));}
	if(getglobalvar("R04") == NULL()){setglobalvar("R04", loadsprite("data/chars/cube/right/R04.png"));}
	if(getglobalvar("R05") == NULL()){setglobalvar("R05", loadsprite("data/chars/cube/right/R05.png"));}

	//LEFT SIDE
	if(getglobalvar("L00") == NULL()){setglobalvar("L00", loadsprite("data/chars/cube/left/L00.png"));}
	if(getglobalvar("L01") == NULL()){setglobalvar("L01", loadsprite("data/chars/cube/left/L01.png"));}
	if(getglobalvar("L02") == NULL()){setglobalvar("L02", loadsprite("data/chars/cube/left/L02.png"));}
	if(getglobalvar("L03") == NULL()){setglobalvar("L03", loadsprite("data/chars/cube/left/L03.png"));}
	if(getglobalvar("L04") == NULL()){setglobalvar("L04", loadsprite("data/chars/cube/left/L04.png"));}
	if(getglobalvar("L05") == NULL()){setglobalvar("L05", loadsprite("data/chars/cube/left/L05.png"));}
	
	//CALCULATE WHICH SPRITE WILL BE USED
	//RIGHT SIDE
	if(damageR < percent/2 || damageR == NULL())	{rightSprite = getglobalvar("R00");}
	if(damageR >= percent/2	&& damageR < percent)	{rightSprite = getglobalvar("R01");}
	if(damageR >= percent && damageR < percent*2)	{rightSprite = getglobalvar("R02");}
	if(damageR >= percent*2 && damageR < percent*3)	{rightSprite = getglobalvar("R03");}
	if(damageR >= percent*3 && damageR < percent*4)	{rightSprite = getglobalvar("R04");}
	if(damageR >= percent*4 && damageR < percent*5)	{rightSprite = getglobalvar("R05");}

	//LEFT SIDE
	if(damageL < percent/2 || damageL == NULL())	{leftSprite = getglobalvar("L00");}
	if(damageL >= percent/2	&& damageL < percent)	{leftSprite = getglobalvar("L01");}
	if(damageL >= percent && damageL < percent*2)	{leftSprite = getglobalvar("L02");}
	if(damageL >= percent*2 && damageL < percent*3)	{leftSprite = getglobalvar("L03");}
	if(damageL >= percent*3 && damageL < percent*4)	{leftSprite = getglobalvar("L04");}
	if(damageL >= percent*4 && damageL < percent*5)	{leftSprite = getglobalvar("L05");}

	//DRAW SPRITES ONLY IF THE CUBE IS "ALIVE"
	if(!dead){

		//WHITE BLINK EFFECT AT EVERY DAMAGE DURING THE "PAIN" ANIMATION
		if(getentityproperty(self, "aiflag", "inpain")){
			changedrawmethod(NULL(), "enabled", 1);
			changedrawmethod(NULL(), "tintmode", 1);
			changedrawmethod(NULL(), "tintcolor", rgbcolor(0xDD, 0xDD, 0xDD)); //WHITE COLOR
		}
		else
		{
			changedrawmethod(NULL(), "enabled", 0);
		}

		//DRAW ALL SPRITES
		float zFix = 35;
		drawsprite(rightSprite, sprX, sprY, Pz+zFix);
		drawsprite(leftSprite, sprX, sprY, Pz+zFix);

		//USED TO DEBUG ONLY, CAN BE REMOVED OR DISABLED AFTER ALL TESTS ARE FINISHED
		void txt1;
		void txt2;

		if(damageL == NULL()){txt1 = 0;}else{txt1 = damageL;}
		if(damageR == NULL()){txt2 = 0;}else{txt2 = damageR;}
		// drawstring(textX-100, textY, 0, txt1, layer);
		// drawstring(textX+100, textY, 0, txt2, layer);
	}
}