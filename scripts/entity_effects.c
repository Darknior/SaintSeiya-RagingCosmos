void main()
{//Turn on/off generic effects according to global variables
	
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
	void type			= getentityproperty(self, "type");
	void animation		= getentityproperty(self, "animationID");
	int boss			= getentityproperty(self, "boss");
	int maxHealth		= getentityproperty(self, "maxhealth");
	int health			= getentityproperty(self, "health");
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
	float rainbowTime	= getentityvar(self, "rainbowTime");
	float dyingTime		= getentityvar(self, "dyingTime");
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
	if(goldTime > time){
		
        //GET NEW VALUES
        tintMode    = 1;
        rate        = 5.5;

        //APPLY EFFECTS
        changedrawmethod(self, "enabled", 1);
        changedrawmethod(self, "tintmode", tintMode);
        changedrawmethod(self, "tintcolor", rgbcolor(time*rate, time*rate, 0x00));
    }else

	//PURPLE EFFECT
	if(purpleTime > time && !dead){
		
        //GET NEW VALUES
        tintMode    = 2;
        rate        = 5.5;

        //APPLY EFFECTS
        changedrawmethod(self, "enabled", 1);
        changedrawmethod(self, "tintmode", tintMode);
        changedrawmethod(self, "tintcolor", rgbcolor(time*rate, 0x00, time*rate));
    }else

	//RED EFFECT
	if(redTime > time){
		
		//GET NEW VALUES
        tintMode    = 2;
        rate        = 5.5;

        //APPLY EFFECTS
        changedrawmethod(self, "enabled", 1);
        changedrawmethod(self, "tintmode", tintMode);
        changedrawmethod(self, "tintcolor", rgbcolor(time*rate, 0x00, 0x00));
    }else


	//GREEN EFFECT
	if(greenTime > time){
		
		//GET NEW VALUES
		tintMode	= 6;
		rate		= 5.5;
		
		//APPLY EFFECTS
		changedrawmethod(self, "enabled", 1);
		changedrawmethod(self, "tintmode", tintMode);
		changedrawmethod(self, "tintcolor", rgbcolor(0x00, time*rate, 0x00));

	}else

	//SILVER EFFECT
    if(silverTime > time){

        //GET NEW VALUES
        tintMode    = 1;
        rate        = 5.5;

        //APPLY EFFECTS
        changedrawmethod(self, "enabled", 1);
        changedrawmethod(self, "tintmode", tintMode);
        changedrawmethod(self, "tintcolor", rgbcolor(0x00, 0x00, time*rate));
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

	//RAINBOW EFFECT
	if(rainbowTime > time && !dead){
		
		//GET NEW VALUES
		tintMode	= 1;
		rate		= 8;
		
		//APPLY EFFECTS
		changedrawmethod(self, "enabled", 1);
		changedrawmethod(self, "tintmode", tintMode);
		changedrawmethod(self, "tintcolor", rgbcolor(time*rate, time*rate/2, time*rate/4));
	}else

	//SLOW DYING EFFECT
	if(health <= maxHealth/4 && health > maxHealth/10 && !dead && (type == openborconstant("TYPE_PLAYER") || type == openborconstant("TYPE_ENEMY") && boss)){
		
		//GET NEW VALUES
		tintMode	= 1;
		rate		= 200;
		
		//APPLY EFFECTS
		if(dyingTime == NULL()){setentityvar(self, "dyingTime", 1);}

		if((openborvariant("elapsed_time")/rate)%2 == 0){
			changedrawmethod(self, "enabled", 1);
			changedrawmethod(self, "tintmode", tintMode);
			changedrawmethod(self, "tintcolor", rgbcolor(0xFF, 0x00, 0x00));
		}
		else
		{
			changedrawmethod(self, "enabled", 1);
			changedrawmethod(self, "tintmode", 0);
			changedrawmethod(self, "tintcolor", rgbcolor(0x00, 0x00, 0x00));
		}
	}else

	//FAST DYING EFFECT
	if(health <= maxHealth/10 && !dead && (type == openborconstant("TYPE_PLAYER") || type == openborconstant("TYPE_ENEMY") && boss)){
		
		//GET NEW VALUES
		tintMode	= 1;
		rate		= 100;
		
		//APPLY EFFECTS
		if(dyingTime == NULL()){setentityvar(self, "dyingTime", 1);}

		if((openborvariant("elapsed_time")/rate)%2 == 0){
			changedrawmethod(self, "enabled", 1);
			changedrawmethod(self, "tintmode", tintMode);
			changedrawmethod(self, "tintcolor", rgbcolor(0xFF, 0x00, 0x00));
		}
		else
		{
			changedrawmethod(self, "enabled", 1);
			changedrawmethod(self, "tintmode", 0);
			changedrawmethod(self, "tintcolor", rgbcolor(0x00, 0x00, 0x00));
		}
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
				whiteTime != NULL()		||
				rainbowTime != NULL()	||
				dyingTime != NULL()		){
				
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
					blinkReset("rainbowTime");
					blinkReset("dyingTime");
				}
			}
		}
	}
}

void blinkReset(void varReset)
{//Reset any blink effect and all used variables
	void self = getlocalvar("self");

	//VARIABLE RESET
	if(varReset != NULL()){
		setentityvar(self, varReset, NULL());
	}

	//GENMAKEN RESET
	if(getentityvar(self, "genmaken") != NULL()){
		damageentity(self, self, 0, 1, openborconstant("ATK_NORMAL"));
		setentityvar(self, "genmaken", NULL());
	}

	//DRAWMETHOD RESET
	changedrawmethod(self, "reset", 1);
}