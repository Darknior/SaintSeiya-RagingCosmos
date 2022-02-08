void main()
{//Turn on/off generic blink effects according to global variables, works with any feature
	
	if(openborvariant("in_selectscreen")){
		blinkSelect();
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

void blinkReset(void varReset)
{//Reset any blink effect and all used variables
	void self = getlocalvar("self");

	//GLOBAL RESET
	if(varReset != NULL()){setglobalvar(varReset, NULL());}changedrawmethod(self, "reset", 1);

	//USED ONLY FOR BREAK CHARACTER
	if(getentityvar(self, "speedLV") == 2){

		//USED TO RESET "BREAK" CHARACTER TO PREVIOUS DRAWMETHOD "REDDISH" TONE
		changedrawmethod(self, "enabled", 1);
		changedrawmethod(self, "tintmode", 3);
		changedrawmethod(self, "tintcolor", rgbcolor(0xA0, 0x60, 0x60));
	}
	else
	if(getentityvar(self, "speedLV") == 3){

		//USED TO RESET "BREAK" CHARACTER TO PREVIOUS DRAWMETHOD "REDDISH" TONE
		changedrawmethod(self, "enabled", 1);
		changedrawmethod(self, "tintmode", 3);
		changedrawmethod(self, "tintcolor", rgbcolor(0xC0, 0x60, 0x60));
	}
}