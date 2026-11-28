#import "data/scripts/library/live_entity.c"
#import "data/scripts/native_localization_catalog.c"
#import "data/scripts/library/safe_expire.c"
void main()
{
	void self = getlocalvar("self");
	
	if(openborvariant("in_selectscreen")){
		if(openborvariant("current_set") <= 2){
            int clock = 0;
            if(getglobalvar("ss_cursor_clock") != NULL()){clock = getglobalvar("ss_cursor_clock");}
            setglobalvar("ss_cursor_clock", (clock + 1) % 96);
            drawStory();
        }
	}
	
	if(!openborvariant("in_selectscreen")){expireEntity(self);}

}

void drawStory()
{//Draw characters attributes in select screen
 //Thanks to msmalik681!!!
	void vName1	= getplayerproperty(0, "name");
	void vName2	= getplayerproperty(1, "name");
	void vName3	= getplayerproperty(2, "name");
	void vName4	= getplayerproperty(3, "name");
	
	//CHARACTERS INFO
	if(vName1){
		if(getglobalvar("previewModel0") == NULL()){setglobalvar("previewModel0", vName1);} //USED TO GET THE PREVIEW MODEL BY THE FIRST TIME
		storyInfo(getglobalvar("previewModel0"), 0);
	}
	if(vName2){
		if(getglobalvar("previewModel1") == NULL()){setglobalvar("previewModel1", vName2);} //USED TO GET THE PREVIEW MODEL BY THE FIRST TIME
		storyInfo(getglobalvar("previewModel1"), 1);
	}
	if(vName3){
		if(getglobalvar("previewModel2") == NULL()){setglobalvar("previewModel2", vName3);} //USED TO GET THE PREVIEW MODEL BY THE FIRST TIME
		storyInfo(getglobalvar("previewModel2"), 2);
	}
	if(vName4){
		if(getglobalvar("previewModel3") == NULL()){setglobalvar("previewModel3", vName4);} //USED TO GET THE PREVIEW MODEL BY THE FIRST TIME
		storyInfo(getglobalvar("previewModel3"), 3);
	}
}

void storyInfo(void vName, int pIndex)
{//Script used to reduce code size

	if(vName){
		
		//GLOBAL USAGE
		void self	= getlocalvar("self");
		void cursor;

		void previewEntity=liveEntity(getglobalvar("previewEntity"+pIndex));
        if(previewEntity!=NULL()){
            if(getentityvar(previewEntity,"selection_preview")!=pIndex+1)previewEntity=NULL();
        }
        if(previewEntity==NULL())setglobalvar("previewEntity"+pIndex,NULL());
		void str;
		float hRes	= openborvariant("hresolution")+1;
		int xCol	= 719;	//X DIFERRENCE BETWEEN PLAYERS COLUMNS
		int yLine	= 182;	//X DIFERRENCE BETWEEN PLAYERS LINES
		int aniDif	= 4;	//DIFFERENCE BETWEEN "FOLLOW" ANIMATIONS (EX: FOLLOW1, FOLLOW2, FOLLOW3, ETC...)

		//GET THE CURRENT ANIMATION OF THE HIGHLIGHTED/SELECTED CHARACTER
		void aniPrev	= getentityproperty(previewEntity, "animationID");
		void aniDef		= getentityproperty(getglobalvar("defaultEntity"+pIndex), "animationID");
		int pNumber		= pIndex+1;

		//CALCULATE LINES AND COLUMNS FOR MODEL PREVIEW POSITION
		int xPrev	= 40;	//X VALUE USED TO ADJUST PREVIEW MODEL IN SELECT SCREEN
		int yPrev	= 200;	//Y VALUE USED TO ADJUST PREVIEW MODEL IN SELECT SCREEN
		int facing	= 1;

		if(pNumber == 2){yPrev = yPrev+yLine;}
		if(pNumber == 3){xPrev = xPrev+xCol+facing;facing = 0;}
		if(pNumber == 4){xPrev = xPrev+xCol+facing;yPrev = yPrev+yLine;facing = 0;}

		//SPAWN MODEL PREVIEW BY THE FIRST TIME
		if(getglobalvar("previewEntity"+pIndex) == NULL()){
			clearspawnentry();
			setspawnentry("name", vName);
			previewEntity = spawn();
            if(previewEntity==NULL())return;
			changeentityproperty(previewEntity, "type", openborconstant("TYPE_NPC"));
			changeentityproperty(previewEntity, "position", xPrev, yPrev, 0);
			changeentityproperty(previewEntity, "direction", facing);
			changeentityproperty(previewEntity, "animation", openborconstant("ANI_SELECT"));
			changeentityproperty(previewEntity, "map", pIndex);
			setglobalvar("previewMap"+pIndex, pIndex);
			setentityvar(previewEntity, "selection_preview", pIndex+1);
			setglobalvar("previewEntity"+pIndex, previewEntity);
		}

		//SPAWN CURSOR BY THE FIRST TIME
		if(getlocalvar("cursor"+pNumber) == NULL()){
			clearspawnentry();
			setspawnentry("name", "cursor_a");
			cursor = spawn();
			changeentityproperty(cursor, "position", 0, 0, 0);
			changeentityproperty(cursor, "direction", 1);
			changeentityproperty(cursor, "animation", openborconstant("ANI_FOLLOW"+pNumber));
			setlocalvar("cursor"+pNumber, cursor);
		}
		
		//CHANGE CURSOR POSITION ACCORDING TO THE HERO'S NAME
		// Exact portrait origins (top left).

		if(getlocalvar("cursor"+pNumber) != NULL()){
			if(vName == "seiya"){	changeentityproperty(getlocalvar("cursor"+pNumber), "position", 354, 318, 0);}
			if(vName == "shiryu"){	changeentityproperty(getlocalvar("cursor"+pNumber), "position", 275, 193, 0);}
			if(vName == "hyoga"){	changeentityproperty(getlocalvar("cursor"+pNumber), "position", 434, 193, 0);}
			if(vName == "shun"){	changeentityproperty(getlocalvar("cursor"+pNumber), "position", 196, 318, 0);}
			if(vName == "ikki"){	changeentityproperty(getlocalvar("cursor"+pNumber), "position", 513, 318, 0);}
		}

		//CALCULATE LINES AND COLUMNS FOR CHARACTER'S INFO POSITION
		int xName	= 80;			//X POSITION FOR THE MODEL NAME
		int yName	= 215;			//Y POSITION FOR THE MODEL NAME
		int xDif	= 160;			//DIFFERENCE BETWEEN COLUMNS
		int yDif	= 37;			//Y DIFFERENCE FROM PLAYER NUMBER TO PLAYER NAME
		int font0	= 0;			//ATTRIBUTES NAME FONT

		//SMALL LEFT ALIGNMENT FOR PLAYERS 3 AND 4
		if(pNumber == 3 || pNumber == 4){xName = xName+1;}
		
		//DRAW CHARACTER'S INFO
		str = nativeDisplayName(vName);
		if(pNumber == 1){drawstring(xName-(strwidth(str, font0)/2), yName, font0, str);}
		if(pNumber == 2){drawstring(xName-(strwidth(str, font0)/2), yName+yDif, font0, str);}
		if(pNumber == 3){drawstring(hRes-xName-(strwidth(str, font0)/2), yName, font0, str);}
		if(pNumber == 4){drawstring(hRes-xName-(strwidth(str, font0)/2), yName+yDif, font0, str);}
		
		//THE CURRENT HIGHLIGHTED CHARACTER IS NOT CONFIRMED YET?? EXECUTE ALL TASKS BELOW
		if(aniPrev != openborconstant("ANI_PICK")){

			// Draw the highlighted cursor directly, independently of entity animation.
            if(aniDef != openborconstant("ANI_PICK")){
                drawCursorPulse(vName, pNumber);
            }

            //MAKE CHOSEN EFFECT WHEN THE HERO IS SELECTED
			if(aniDef == openborconstant("ANI_PICK")){
				if(getentityproperty(getlocalvar("cursor"+pNumber), "animationID") != openborconstant("ANI_FOLLOW"+(pNumber+aniDif))){
					changeentityproperty(getlocalvar("cursor"+pNumber), "animation", openborconstant("ANI_FOLLOW"+(pNumber+aniDif)));
				}

				//USED TO CONFIRM THE SELECTED CHARACTER AND CHANGE YOUR DEFAULT NAME/MODEL
				//CHANGE ANIMATION AND PLAYER NAME IF THE HIGHLIGHTED CHARACTER IS CHOSEN
				changeplayerproperty(pIndex, "name", getglobalvar("previewModel"+pIndex));
				changeentityproperty(getglobalvar("previewEntity"+pIndex), "animation", openborconstant("ANI_PICK"));
			}
		}
	}
}

void drawCursorPulse(void hero, int player)
{
    int x = 0;
    int y = 0;
    int frame = getglobalvar("ss_cursor_clock") / 8;
    void sprite;
    void key = "ss_cursor_sprite_"+player+"_"+frame;
    if(hero == "seiya"){x=354;y=318;}
    else if(hero == "shiryu"){x=275;y=193;}
    else if(hero == "hyoga"){x=434;y=193;}
    else if(hero == "shun"){x=196;y=318;}
    else if(hero == "ikki"){x=513;y=318;}
    else{return;}
    sprite = getglobalvar(key);
    if(sprite == NULL()){
        if(player == 1 && frame == 0){sprite = loadsprite("data/bgs/select/cursor01_pulse00.png");}
        if(player == 1 && frame == 1){sprite = loadsprite("data/bgs/select/cursor01_pulse01.png");}
        if(player == 1 && frame == 2){sprite = loadsprite("data/bgs/select/cursor01_pulse02.png");}
        if(player == 1 && frame == 3){sprite = loadsprite("data/bgs/select/cursor01_pulse03.png");}
        if(player == 1 && frame == 4){sprite = loadsprite("data/bgs/select/cursor01_pulse04.png");}
        if(player == 1 && frame == 5){sprite = loadsprite("data/bgs/select/cursor01_pulse05.png");}
        if(player == 1 && frame == 6){sprite = loadsprite("data/bgs/select/cursor01_pulse06.png");}
        if(player == 1 && frame == 7){sprite = loadsprite("data/bgs/select/cursor01_pulse07.png");}
        if(player == 1 && frame == 8){sprite = loadsprite("data/bgs/select/cursor01_pulse08.png");}
        if(player == 1 && frame == 9){sprite = loadsprite("data/bgs/select/cursor01_pulse09.png");}
        if(player == 1 && frame == 10){sprite = loadsprite("data/bgs/select/cursor01_pulse10.png");}
        if(player == 1 && frame == 11){sprite = loadsprite("data/bgs/select/cursor01_pulse11.png");}
        if(player == 2 && frame == 0){sprite = loadsprite("data/bgs/select/cursor02_pulse00.png");}
        if(player == 2 && frame == 1){sprite = loadsprite("data/bgs/select/cursor02_pulse01.png");}
        if(player == 2 && frame == 2){sprite = loadsprite("data/bgs/select/cursor02_pulse02.png");}
        if(player == 2 && frame == 3){sprite = loadsprite("data/bgs/select/cursor02_pulse03.png");}
        if(player == 2 && frame == 4){sprite = loadsprite("data/bgs/select/cursor02_pulse04.png");}
        if(player == 2 && frame == 5){sprite = loadsprite("data/bgs/select/cursor02_pulse05.png");}
        if(player == 2 && frame == 6){sprite = loadsprite("data/bgs/select/cursor02_pulse06.png");}
        if(player == 2 && frame == 7){sprite = loadsprite("data/bgs/select/cursor02_pulse07.png");}
        if(player == 2 && frame == 8){sprite = loadsprite("data/bgs/select/cursor02_pulse08.png");}
        if(player == 2 && frame == 9){sprite = loadsprite("data/bgs/select/cursor02_pulse09.png");}
        if(player == 2 && frame == 10){sprite = loadsprite("data/bgs/select/cursor02_pulse10.png");}
        if(player == 2 && frame == 11){sprite = loadsprite("data/bgs/select/cursor02_pulse11.png");}
        if(player == 3 && frame == 0){sprite = loadsprite("data/bgs/select/cursor03_pulse00.png");}
        if(player == 3 && frame == 1){sprite = loadsprite("data/bgs/select/cursor03_pulse01.png");}
        if(player == 3 && frame == 2){sprite = loadsprite("data/bgs/select/cursor03_pulse02.png");}
        if(player == 3 && frame == 3){sprite = loadsprite("data/bgs/select/cursor03_pulse03.png");}
        if(player == 3 && frame == 4){sprite = loadsprite("data/bgs/select/cursor03_pulse04.png");}
        if(player == 3 && frame == 5){sprite = loadsprite("data/bgs/select/cursor03_pulse05.png");}
        if(player == 3 && frame == 6){sprite = loadsprite("data/bgs/select/cursor03_pulse06.png");}
        if(player == 3 && frame == 7){sprite = loadsprite("data/bgs/select/cursor03_pulse07.png");}
        if(player == 3 && frame == 8){sprite = loadsprite("data/bgs/select/cursor03_pulse08.png");}
        if(player == 3 && frame == 9){sprite = loadsprite("data/bgs/select/cursor03_pulse09.png");}
        if(player == 3 && frame == 10){sprite = loadsprite("data/bgs/select/cursor03_pulse10.png");}
        if(player == 3 && frame == 11){sprite = loadsprite("data/bgs/select/cursor03_pulse11.png");}
        if(player == 4 && frame == 0){sprite = loadsprite("data/bgs/select/cursor04_pulse00.png");}
        if(player == 4 && frame == 1){sprite = loadsprite("data/bgs/select/cursor04_pulse01.png");}
        if(player == 4 && frame == 2){sprite = loadsprite("data/bgs/select/cursor04_pulse02.png");}
        if(player == 4 && frame == 3){sprite = loadsprite("data/bgs/select/cursor04_pulse03.png");}
        if(player == 4 && frame == 4){sprite = loadsprite("data/bgs/select/cursor04_pulse04.png");}
        if(player == 4 && frame == 5){sprite = loadsprite("data/bgs/select/cursor04_pulse05.png");}
        if(player == 4 && frame == 6){sprite = loadsprite("data/bgs/select/cursor04_pulse06.png");}
        if(player == 4 && frame == 7){sprite = loadsprite("data/bgs/select/cursor04_pulse07.png");}
        if(player == 4 && frame == 8){sprite = loadsprite("data/bgs/select/cursor04_pulse08.png");}
        if(player == 4 && frame == 9){sprite = loadsprite("data/bgs/select/cursor04_pulse09.png");}
        if(player == 4 && frame == 10){sprite = loadsprite("data/bgs/select/cursor04_pulse10.png");}
        if(player == 4 && frame == 11){sprite = loadsprite("data/bgs/select/cursor04_pulse11.png");}
        setglobalvar(key, sprite);
    }
    if(sprite != NULL()){drawsprite(sprite, x, y, 2001, 0);}
}
