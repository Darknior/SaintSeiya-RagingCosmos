void cycleSprite(void type, int startIndex, int endIndex, float frameDelay, float cycleDelay)
{//Apply simple sprite cycle effects to avoid creating new entities
	float timeCurrent	= openborvariant("elapsed_time");
	float timeCounter	= getlocalvar("timeCounter"+type+startIndex);
	int levelCurrent	= openborvariant("current_level");
	int levelPrevious	= getlocalvar("levelPrevious");
	int frameCurrent	= getlocalvar("frameCurrent"+type+startIndex);
	int frameNext		= getlocalvar("frameNext"+type+startIndex);
	int frameStart		= getlocalvar("frameStart"+type+startIndex);

	//CLEAR ALL LOCAL VARIABLES WHEN THE LEVEL CHANGES
	if(levelCurrent != levelPrevious){clearlocalvar();setlocalvar("levelPrevious", levelCurrent);}
	
	//START ALL VARIABLES
	if(timeCounter == NULL()){setlocalvar("timeCounter"+type+startIndex, timeCurrent+frameDelay);}
	if(frameCurrent == NULL()){setlocalvar("frameCurrent"+type+startIndex, endIndex);}
	if(frameNext == NULL()){setlocalvar("frameNext"+type+startIndex, startIndex);}
	if(frameStart == NULL()){setlocalvar("frameStart"+type+startIndex, startIndex);}

	//DISABLE ALL LAYERS EXCEPT FOR THE LAST, WHEN THE SCRIPT STARTS
	while(frameStart < endIndex-1){
		frameStart = getlocalvar("frameStart"+type+startIndex);

		changelayerproperty(type, frameStart, "enabled", 0);
		setlocalvar("frameStart"+type+startIndex, frameStart+1);
	}
	
	//DELAY REACHED THE LIMIT?? EXECUTE ALL TASKS
	if(timeCurrent > timeCounter){

		//DEFINE THE CURRENT FRAME
		if(frameCurrent < endIndex){
			setlocalvar("frameCurrent"+type+startIndex, frameCurrent+1);
			setlocalvar("timeCounter"+type+startIndex, timeCurrent+frameDelay);
		}
		else
		{
			if(cycleDelay != NULL()){setlocalvar("timeCounter"+type+startIndex, timeCurrent+frameDelay+cycleDelay);}
			setlocalvar("frameCurrent"+type+startIndex, startIndex);
		}

		//DEFINE THE NEXT FRAME
		if(frameNext < endIndex){
			setlocalvar("frameNext"+type+startIndex, frameNext+1);
			setlocalvar("timeCounter"+type+startIndex, timeCurrent+frameDelay);
		}
		else
		{
			if(cycleDelay != NULL()){setlocalvar("timeCounter"+type+startIndex, timeCurrent+frameDelay+cycleDelay);}
			setlocalvar("frameNext"+type+startIndex, startIndex);
		}

		//CHANGE LAYERS
		changelayerproperty(type, frameCurrent, "enabled", 0);
		changelayerproperty(type, frameNext, "enabled", 1);
	}
}