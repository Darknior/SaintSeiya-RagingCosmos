void main()
{//Draw content
	void self	= getlocalvar("self");
	void name	= getentityproperty(self, "defaultname");
	void text;
	int x			= getentityproperty(self, "x");
	int y			= getentityproperty(self, "z");
	int xPos		= x-openborvariant("xpos");
	int yPos		= y-openborvariant("ypos");
	int xPortLeft	= xPos+21;
	int xPortRight	= xPos+302;
	int xTextLeft	= xPortLeft+86;
	int xTextRight	= xPortRight-8;
	int yPort		= yPos+12;
	int lineStart	= 20;
	int lineAdd		= 18;
	int textLayer	= 1001;
	int spriteLayer	= 1000000001;
	int alpha		= 6;
	int channel		= 190;
	int color		= 0;
	int boxWidth	= 376;
	int boHeight	= 91;
	int xBox		= xPos+12;
	int yBox		= yPos+5;
	int font;
	int xText;
	int yText;

	//START VARIABLES
	if(getglobalvar("dialog_entity") == NULL()){
		setglobalvar("portrait_seiya1", loadsprite("data/chars/dialogs/seiya1.png"));
		setglobalvar("portrait_seiya2", loadsprite("data/chars/dialogs/seiya2.png"));
		setglobalvar("portrait_aldebaran1", loadsprite("data/chars/dialogs/aldebaran1.png"));
		setglobalvar("portrait_aldebaran2", loadsprite("data/chars/dialogs/aldebaran2.png"));
		setglobalvar("portrait_aldebaran3", loadsprite("data/chars/dialogs/aldebaran3.png"));

		//REGISTER SELF AS THE CURRENT DIALOG ENTITY
		setglobalvar("dialog_entity", self);
	}

	//SET CHANNEL OPACITIY
	changedrawmethod(NULL(), "channelr", channel);
	changedrawmethod(NULL(), "channelg", channel);
	changedrawmethod(NULL(), "channelb", channel);

	//TEXT PART 1 (RIGHT ALIGNMENT)
	if(getglobalvar("drawText") == "part1"){
		drawsprite(getglobalvar("portrait_aldebaran1"), xPortRight, yPort, spriteLayer);
		drawbox(xBox, yBox, boxWidth, boHeight, textLayer, color, alpha);

		font = 0; text = "So here are the rebellious Bronze";
		xText = xTextRight-strwidth(text, font);yText = yPos+lineStart;
		drawstring(xText, yText, font, text, textLayer);

		font = 0; text = "Knights who dare to defy the Sanctuary";
		xText = xTextRight-strwidth(text, font);yText = yText+lineAdd;
		drawstring(xText, yText, font, text, textLayer);

		font = 0; text = "and the Grand Pope's orders!";
		xText = xTextRight-strwidth(text, font);yText = yText+lineAdd;
		drawstring(xText, yText, font, text, textLayer);
	}

	//TEXT PART 2 (LEFT ALIGNMENT)
	if(getglobalvar("drawText") == "part2"){
		drawsprite(getglobalvar("portrait_seiya1"), xPortLeft, yPort, spriteLayer);
		drawbox(xBox, yBox, boxWidth, boHeight, textLayer, color, alpha);

		font = 0; text = "Let us pass, we must cross the 12 temples";
		xText = xTextLeft;yText = yPos+lineStart;
		drawstring(xText, yText, font, text, textLayer);

		font = 0; text = "to reason with the great pope and save";
		xText = xTextLeft;yText = yText+lineAdd;
		drawstring(xText, yText, font, text, textLayer);

		font = 0; text = "Saori, the reincarnation of Athena!";
		xText = xTextLeft;yText = yText+lineAdd;
		drawstring(xText, yText, font, text, textLayer);
	}

	//TEXT PART 3 (RIGHT ALIGNMENT)
	if(getglobalvar("drawText") == "part3"){
		drawsprite(getglobalvar("portrait_aldebaran2"), xPortRight, yPort, spriteLayer);
		drawbox(xBox, yBox, boxWidth, boHeight, textLayer, color, alpha);

		font = 0; text = "I Aldebaran, golden knight of the bull";
		xText = xTextRight-strwidth(text, font);yText = yPos+lineStart;
		drawstring(xText, yText, font, text, textLayer);
		
		font = 0; text = "I will not let anyone cross my temple!!";
		xText = xTextRight-strwidth(text, font);yText = yText+lineAdd;
		drawstring(xText, yText, font, text, textLayer);
	}

	//TEXT PART 4 (LEFT ALIGNMENT)
	if(getglobalvar("drawText") == "part4"){
		drawsprite(getglobalvar("portrait_seiya2"), xPortLeft, yPort, spriteLayer);
		drawbox(xBox, yBox, boxWidth, boHeight, textLayer, color, alpha);

		font = 0; text = "That's what we're going to see!";
		xText = xTextLeft;yText = yPos+lineStart;
		drawstring(xText, yText, font, text, textLayer);

		font = 0; text = "On guard golden knight!";
		xText = xTextLeft;yText = yText+lineAdd;
		drawstring(xText, yText, font, text, textLayer);
	}

	//KILL THE DIALOG ENTITY AT THE END AND CLEAR VARIABLES
	if(getglobalvar("drawText") == "dialog1_end"){
		setglobalvar("dialog_entity", NULL());
		killentity(self);
	}
}