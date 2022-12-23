void main()
{//Turn on/off generic blink effects according to global variables, works with any feature
	
	if(openborvariant("in_selectscreen")){
		blinkSelect();
	}

	if(openborvariant("in_level")){
		blinkDamage();
	}
}

void blinkSelect()
{//Turn on/off generic blink effects during the "select" animation
	void self		= getlocalvar("self");
	void animation	= getentityproperty(self, "animationID");
	int frame		= getentityproperty(self, "animpos");
	
	//SELECT SCREEN
	if(animation == openborconstant("ANI_PICK")){ //SELECT ANIMATION
		int tintMode	= 1;
		float time		= openborvariant("elapsed_time");
		float rate		= 22;
	
		//APPLY BLINK EFFECTS
		if(frame <= 1){
			changedrawmethod(self, "enabled", 1);
			changedrawmethod(self, "tintmode", tintMode);
			changedrawmethod(self, "tintcolor", rgbcolor(time*rate, time*rate, time*rate));
		}
		else
		{
			if(getdrawmethod(self, "enabled")){blinkReset();}
		}
	}

	//CAUTION!!! THE LINES BELOW ARE VERY IMPORTANT BECAUSE IS THE ONLY WAY TO GET ALL PREVIEW MODELS SPAWNED BY THE ENGINE IN THE SELECT SCREEN!
	//THIS CODE WILL SAVE THE DEFAULT PREVIEW MODELS IN A VARIABLE AND ALLOWS TO EDIT SOME PROPERTIES
	//THE ONLY WAY TO DEFINE EVERY INDEX IS BY GETTING THE X POSITION AT THE SELECT SCREEN, PLEASE CHECK EVERY POSITION IN THE LEVELS.TXT FILE
	//NEED TO REMEMBER THAT THE ENTITES IN THE SELECT SCREEN WILL NOT ALLOW YOU TO GET SOME PROPERTIES BY REGULAR METHODS, LIKE "PLAYERINDEX"
	
	void type = getentityproperty(self, "type");
	int xPos  = getentityproperty(self, "x");
	int index;

	if(xPos == 100){index = 0;}else
	if(xPos == 200){index = 1;}else
	if(xPos == 300){index = 2;}else
	if(xPos == 400){index = 3;}

	if(type != openborconstant("TYPE_NPC")){setglobalvar("defaultEntity"+index, self);}
}

void blinkDamage()
{//Turn on/off generic blink effects during some "takedamage" events
	void self			= getlocalvar("self");
	void animation		= getentityproperty(self, "animationID");
	int dead			= getentityproperty(self, "dead");
	int tintMode;
	float time			= openborvariant("elapsed_time");
	float poisonTime	= getentityvar(self, "poisonTime");
	float iceTime		= getentityvar(self, "iceTime");
	float goldTime		= getentityvar(self, "goldTime");
	float purpleTime	= getentityvar(self, "purpleTime");
	float redTime		= getentityvar(self, "redTime");
	float greenTime		= getentityvar(self, "greenTime");
	float silverTime	= getentityvar(self, "silverTime");
	float pinkTime		= getentityvar(self, "pinkTime");
	float whiteTime		= getentityvar(self, "whiteTime");
	float time;
	float rate;
	
	//POISON EFFECT
	if(poisonTime > time && !dead){
		
		//GET NEW VALUES
		tintMode	= 2;
		rate		= 2.5;
		
		//APPLY EFFECTS
		changedrawmethod(self, "enabled", 1);
		changedrawmethod(self, "tintmode", tintMode);
		changedrawmethod(self, "tintcolor", rgbcolor(time*rate, 0x00, time*rate));
	}else

	//ICE EFFECT
	if(iceTime > time && !dead){
		
		//GET NEW VALUES
		tintMode	= 6;
		rate		= 2.5;
		
		//APPLY EFFECTS
		changedrawmethod(self, "enabled", 1);
		changedrawmethod(self, "tintmode", tintMode);
		changedrawmethod(self, "tintcolor", rgbcolor(0x33, 0xFF, 0xFF));
	}else

	//GOLD EFFECT
	if(goldTime > time && !dead){
		
		//GET NEW VALUES
		tintMode	= 6;
		rate		= 2.5;
		
		//APPLY EFFECTS
		changedrawmethod(self, "enabled", 1);
		changedrawmethod(self, "tintmode", tintMode);
		changedrawmethod(self, "tintcolor", rgbcolor(0xFF, 0xFF, 0x00));
	}else

	//PURPLE EFFECT
	if(purpleTime > time && !dead){
		
		//GET NEW VALUES
		tintMode	= 6;
		rate		= 2.5;
		
		//APPLY EFFECTS
		changedrawmethod(self, "enabled", 1);
		changedrawmethod(self, "tintmode", tintMode);
		changedrawmethod(self, "tintcolor", rgbcolor(0x7F, 0x00, 0xFF));
	}else

	//RED EFFECT
	if(redTime > time && !dead){
		
		//GET NEW VALUES
		tintMode	= 6;
		rate		= 2.5;
		
		//APPLY EFFECTS
		changedrawmethod(self, "enabled", 1);
		changedrawmethod(self, "tintmode", tintMode);
		changedrawmethod(self, "tintcolor", rgbcolor(0xFF, 0x00, 0x00));
	}else

	//GREEN EFFECT
	if(greenTime > time && !dead){
		
		//GET NEW VALUES
		tintMode	= 6;
		rate		= 2.5;
		
		//APPLY EFFECTS
		changedrawmethod(self, "enabled", 1);
		changedrawmethod(self, "tintmode", tintMode);
		changedrawmethod(self, "tintcolor", rgbcolor(0x00, 0xFF, 0x00));
	}else

	//SILVER EFFECT
	if(silverTime > time && !dead){
		
		//GET NEW VALUES
		tintMode	= 6;
		rate		= 2.5;
		
		//APPLY EFFECTS
		changedrawmethod(self, "enabled", 1);
		changedrawmethod(self, "tintmode", tintMode);
		changedrawmethod(self, "tintcolor", rgbcolor(0xE0, 0xE0, 0xE0));
	}else

	//PINK EFFECT
	if(pinkTime > time && !dead){
		
		//GET NEW VALUES
		tintMode	= 6;
		rate		= 2.5;
		
		//APPLY EFFECTS
		changedrawmethod(self, "enabled", 1);
		changedrawmethod(self, "tintmode", tintMode);
		changedrawmethod(self, "tintcolor", rgbcolor(0xFF, 0x66, 0xB2));
	}else

	//WHITE EFFECT
	if(whiteTime > time && !dead){
		
		//GET NEW VALUES
		tintMode	= 6;
		rate		= 2.5;
		
		//APPLY EFFECTS
		changedrawmethod(self, "enabled", 1);
		changedrawmethod(self, "tintmode", tintMode);
		changedrawmethod(self, "tintcolor", rgbcolor(0xFF, 0xFF, 0xCC));
	}else

	//BLINK RESET
	{
		//IS NOT IN THE SELECT ANIMATION??
		if(animation != openborconstant("ANI_PICK")){
			if(	poisonTime != NULL()	||
				iceTime != NULL()		||
				goldTime != NULL()		||
				purpleTime != NULL()	||
				redTime != NULL()		||
				greenTime != NULL()		||
				silverTime != NULL()	||
				pinkTime != NULL()		||
				whiteTime != NULL()		){
				
				if(getdrawmethod(self, "enabled")){
					blinkReset("poisonTime");
					blinkReset("iceTime");
					blinkReset("goldTime");
					blinkReset("purpleTime");
					blinkReset("redTime");
					blinkReset("greenTime");
					blinkReset("silverTime");
					blinkReset("pinkTime");
					blinkReset("whiteTime");
				}
			}
		}
	}
}

void blinkReset(void varReset)
{//Reset any blink effect and all used variables
	void self = getlocalvar("self");

	//GLOBAL RESET
	if(varReset != NULL()){setentityvar(self, varReset, NULL());}changedrawmethod(self, "reset", 1);
}