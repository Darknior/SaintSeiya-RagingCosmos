void main()
{//Draw content
	void self			= getlocalvar("self");
	void name			= getentityproperty(self, "defaultname");
	void portName		= getglobalvar("portraitName")+"";
	void line1			= getglobalvar("line1");
	void line2			= getglobalvar("line2");
	void line3			= getglobalvar("line3");
	void line4			= getglobalvar("line4");
	int x				= getentityproperty(self, "x");
	int y				= getentityproperty(self, "z");
	int xPos			= x-openborvariant("xpos");
	int yPos			= y-openborvariant("ypos");
	int portraitLeft	= xPos+21;
	int portraitRight	= xPos+302;
	int yPortrait		= yPos+27;
	int textLeft		= xPos+27;
	int textRight		= xPos+114;
	int lineStart		= yPos+33;
	int lineAdd			= 18;
	int boxLayer		= 1000000002;
	int textLayer		= 1000000003;
	int spriteLayer		= 1000000004;
	int alpha			= 6;
	int channel			= 190;
	int color			= 0;
	int boxWidth		= 376;
	int boHeight		= 91;
	int xBox			= xPos+12;
	int yBox			= yPos+20;
	int font			= 5;
	int xPortrait;
	int xText;
	int yText;

	//REGISTER SELF AS THE CURRENT DIALOG ENTITY
	if(getglobalvar("dialog_entity") == NULL()){setglobalvar("dialog_entity", self);}

	//SET CHANNEL OPACITIY
	changedrawmethod(NULL(), "enabled", 1);
	changedrawmethod(NULL(), "channelr", channel);
	changedrawmethod(NULL(), "channelg", channel);
	changedrawmethod(NULL(), "channelb", channel);

	//SET PORTRAIT TO RIGHT AND TEXT TO LEFT SIDE
	if(getglobalvar("portraitSide") == "right"){
		xPortrait = portraitRight;
		xText = textLeft;
	}

	//SET PORTRAIT TO LEFT AND TEXT TO RIGHT SIDE
	if(getglobalvar("portraitSide") == "left"){
		xPortrait = portraitLeft;
		xText = textRight;
	}

	//LOAD PORTRAITS
	if(getglobalvar(portName) == NULL()){
		setglobalvar(portName, loadsprite("data/chars/dialogs/icons/"+portName+".png"));
	}

	//DRAW CONTENT
	if(line1 != NULL()){
		yText = lineStart;
		drawbox(xBox, yBox, boxWidth, boHeight, boxLayer, color, alpha);
		changedrawmethod(NULL(), "enabled", 0);
		drawsprite(getglobalvar(portName), xPortrait, yPortrait, spriteLayer);
		drawstring(xText, yText, font, line1, textLayer);yText = yText+lineAdd;
		drawstring(xText, yText, font, line2, textLayer);yText = yText+lineAdd;
		drawstring(xText, yText, font, line3, textLayer);yText = yText+lineAdd;
		drawstring(xText, yText, font, line4, textLayer);yText = yText+lineAdd;
	}
	
	hideEntity();
}

void hideEntity()
{//Hide some entities during the cutscenes
	void self			= getlocalvar("self");
	void entity_cursor	= NULL();
	int entity_count	= openborvariant("count_entities");
	int entity_index	= 0;
	int exists			= 0;

	//PERFORM THE MAIN TASKS
	for(entity_index = 0; entity_index < entity_count; entity_index++){

		entity_cursor = getentity(entity_index);

		if(!entity_cursor){
			continue;
		}

		exists = getentityproperty(entity_cursor, "exists");

		if(!exists){
			continue;
		}

		if(	getentityproperty(entity_cursor, "type") == openborconstant("TYPE_PLAYER")	||
			getentityproperty(entity_cursor, "type") == openborconstant("TYPE_ENEMY")	){
			int hideZ = -999;
			int showZ = 400;
			
			if(getglobalvar("lockButton") == 1){
				changeentityproperty(entity_cursor, "z", hideZ); //HIDE REAL ENTITIES DURING THE CUTSCENE
			}
			else
			{
				changeentityproperty(entity_cursor, "z", showZ); //SHOW BACK REAL ENTITIES AFTER THE CUTSCENE
			}
		}
	}
}