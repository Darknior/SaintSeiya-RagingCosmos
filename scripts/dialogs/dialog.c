#import "data/scripts/native_localization_catalog.c"
#import "data/scripts/localization.c"
#import "data/scripts/library/dialog_positions.c"
void main()
{//Draw content
	void self			= getlocalvar("self");
	void name			= getentityproperty(self, "defaultname");
	void type			= getentityproperty(self, "type");
	void portName		= getglobalvar("portraitName");
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
	int xPortrait = portraitRight;
	int xText = textLeft;
	int yText;

	if(line1 == NULL() || line1 == "" || portName == NULL()){if(type == openborconstant("TYPE_TEXTBOX")) hideEntity();return;}

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
	if(portName != "empty" && getglobalvar(portName) == NULL()){
		setglobalvar(portName, loadsprite("data/chars/dialogs/icons/"+portName+".png"));
	}

	//DRAW CONTENT
	if(line1 != NULL()){
		yText = lineStart;
		localizedTextFrame(xBox,yBox,boxWidth,boHeight,boxLayer);
		changedrawmethod(NULL(), "enabled", 0);
		if(getglobalvar(portName) != NULL()) drawsprite(getglobalvar(portName), xPortrait, yPortrait, spriteLayer);
        if(!localizedDialog(line1,line2,line3,line4,xText,yText,textLayer)){
            drawstring(xText,yText,font,nativeText(line1),textLayer);yText=yText+lineAdd;
            drawstring(xText,yText,font,nativeText(line2),textLayer);yText=yText+lineAdd;
            drawstring(xText,yText,font,nativeText(line3),textLayer);yText=yText+lineAdd;
            drawstring(xText,yText,font,nativeText(line4),textLayer);
        }
	}
	
	if(type == openborconstant("TYPE_TEXTBOX")){hideEntity();}
}

void hideEntity()
{// Preserve combatants' depths through the cutscene.
    dialogPositions(getglobalvar("lockButton")==1);
}
