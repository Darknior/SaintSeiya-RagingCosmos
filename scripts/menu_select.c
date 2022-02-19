void main()
{
	void self = getlocalvar("self");
	
	if(openborvariant("in_selectscreen")){
		if(openborvariant("current_set") == 13){drawStory();}
	}
	
	if(!openborvariant("in_selectscreen")){killentity(self);}

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
		void previewEntity;
		void str;
		float hRes	= openborvariant("hresolution")+1;
		int xCol	= 719;	//X DIFERRENCE BETWEEN PLAYERS COLUMNS
		int yLine	= 182;	//X DIFERRENCE BETWEEN PLAYERS LINES
		int aniDif	= 4;	//DIFFERENCE BETWEEN "FOLLOW" ANIMATIONS (EX: FOLLOW1, FOLLOW2, FOLLOW3, ETC...)

		//GET THE CURRENT ANIMATION OF THE HIGHLIGHTED/SELECTED CHARACTER
		void aniPrev	= getentityproperty(getglobalvar("previewEntity"+pIndex), "animationID");
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
			changeentityproperty(previewEntity, "type", openborconstant("TYPE_NPC"));
			changeentityproperty(previewEntity, "position", xPrev, yPrev, 0);
			changeentityproperty(previewEntity, "direction", facing);
			changeentityproperty(previewEntity, "animation", openborconstant("ANI_SELECT"));
			changeentityproperty(previewEntity, "map", pIndex);
			setglobalvar("previewMap"+pIndex, pIndex);
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
		int xBase = 330;	//BASE X POSITION
		int yBase = 326;	//BASE Y POSITION
		int xMove = 150;	//DIFERRENCE BETWEEN PLAYER'S PORTRAITS COLUMNS
		int yMove = 125;	//DIFERRENCE BETWEEN PLAYER'S PORTRAITS LINES

		if(getlocalvar("cursor"+pNumber) != NULL()){
			if(vName == "seiya"){			changeentityproperty(getlocalvar("cursor"+pNumber), "position", xBase, yBase, 0);}
			if(vName == "DRAGON-SHIRYU"){	changeentityproperty(getlocalvar("cursor"+pNumber), "position", xBase-(xMove/2), yBase-yMove, 0);}
			if(vName == "CYGNUS-HYOGA"){	changeentityproperty(getlocalvar("cursor"+pNumber), "position", xBase+(xMove/2), yBase-yMove, 0);}
			if(vName == "ANDROMEDA-SHUN"){	changeentityproperty(getlocalvar("cursor"+pNumber), "position", xBase-xMove, yBase, 0);}
			if(vName == "PHOENIX-IKKI"){	changeentityproperty(getlocalvar("cursor"+pNumber), "position", xBase+xMove, yBase, 0);}
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
		str = vName;
		if(pNumber == 1){drawstring(xName-(strwidth(str, font0)/2), yName, font0, str);}
		if(pNumber == 2){drawstring(xName-(strwidth(str, font0)/2), yName+yDif, font0, str);}
		if(pNumber == 3){drawstring(hRes-xName-(strwidth(str, font0)/2), yName, font0, str);}
		if(pNumber == 4){drawstring(hRes-xName-(strwidth(str, font0)/2), yName+yDif, font0, str);}
		
		//THE CURRENT HIGHLIGHTED CHARACTER IS NOT CONFIRMED YET?? EXECUTE ALL TASKS BELOW
		if(aniPrev != openborconstant("ANI_PICK")){

			//MAKE SHAKE EFFECT WHEN THE HERO IS HIGHLIGHTED
			if(	playerkeys(pIndex, 1, "moveleft")	||
				playerkeys(pIndex, 1, "moveright")	||
				playerkeys(pIndex, 1, "moveup")		||
				playerkeys(pIndex, 1, "movedown")	){

				if(getentityproperty(getlocalvar("cursor"+pNumber), "animationID") != openborconstant("ANI_FOLLOW"+(pNumber+aniDif))){
					updateframe(getlocalvar("cursor"+pNumber), 0);
				}
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