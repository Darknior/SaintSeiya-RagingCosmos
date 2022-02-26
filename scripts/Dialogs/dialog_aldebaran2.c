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
	int alpha		= 3;
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
		setglobalvar("portrait_aldebaran1", loadsprite("data/chars/dialogs/aldebaran1.png"));
		setglobalvar("portrait_aldebaran2", loadsprite("data/chars/dialogs/aldebaran2.png"));
		setglobalvar("portrait_aldebaran3", loadsprite("data/chars/dialogs/aldebaran3.png"));

		//REGISTER SELF AS THE CURRENT DIALOG ENTITY
		setglobalvar("dialog_entity", self);
	}

	//TEXT PART 1 (RIGHT ALIGNMENT)
	if(getglobalvar("drawText") == "part1"){
		drawsprite(getglobalvar("portrait_aldebaran2"), xPortRight, yPort, spriteLayer);
		drawbox(xBox, yBox, boxWidth, boHeight, textLayer, color, alpha);

		font = 0; text = "Agh... But how?? You are going to me...";
		xText = xTextRight-strwidth(text, font);yText = yPos+lineStart;
		drawstring(xText, yText, font, text, textLayer);
	}

	//TEXT PART 2 (RIGHT ALIGNMENT)
	if(getglobalvar("drawText") == "part2"){
		drawsprite(getglobalvar("portrait_aldebaran3"), xPortRight, yPort, spriteLayer);
		drawbox(xBox, yBox, boxWidth, boHeight, textLayer, color, alpha);

		font = 0; text = "Mwahaha!! You blew me away!!";
		xText = xTextRight-strwidth(text, font);yText = yPos+lineStart;
		drawstring(xText, yText, font, text, textLayer);

		font = 0; text = "You managed to break one of my horns!";
		xText = xTextRight-strwidth(text, font);yText = yText+lineAdd;
		drawstring(xText, yText, font, text, textLayer);
	}

	//TEXT PART 3 (RIGHT ALIGNMENT)
	if(getglobalvar("drawText") == "part3"){
		drawsprite(getglobalvar("portrait_aldebaran1"), xPortRight, yPort, spriteLayer);
		drawbox(xBox, yBox, boxWidth, boHeight, textLayer, color, alpha);

		font = 0; text = "I really underestimated you bronze";
		xText = xTextRight-strwidth(text, font);yText = yPos+lineStart;
		drawstring(xText, yText, font, text, textLayer);

		font = 0; text = "knights! By way of gratitude, I agree";
		xText = xTextRight-strwidth(text, font);yText = yText+lineAdd;
		drawstring(xText, yText, font, text, textLayer);

		font = 0; text = "to let you go on your way.";
		xText = xTextRight-strwidth(text, font);yText = yText+lineAdd;
		drawstring(xText, yText, font, text, textLayer);
	}

	//KILL THE DIALOG ENTITY AT THE END AND CLEAR VARIABLES
	if(getglobalvar("drawText") == "dialog2_end"){
		setglobalvar("dialog_entity", NULL());
		killentity(self);
	}
}