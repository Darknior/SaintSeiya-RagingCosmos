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
		setglobalvar("portrait_aldebaran1", loadsprite("data/chars/dialogs/aldebaran1.png"));
		setglobalvar("portrait_aldebaran2", loadsprite("data/chars/dialogs/aldebaran2.png"));
		setglobalvar("portrait_aldebaran3", loadsprite("data/chars/dialogs/aldebaran3.png"));
	    setglobalvar("portrait_aldebaran4", loadsprite("data/chars/dialogs/aldebaran4.png"));
		setglobalvar("portrait_aldebaran5", loadsprite("data/chars/dialogs/aldebaran5.png"));

		//REGISTER SELF AS THE CURRENT DIALOG ENTITY
		setglobalvar("dialog_entity", self);
	}

	//SET CHANNEL OPACITIY
	changedrawmethod(NULL(), "channelr", channel);
	changedrawmethod(NULL(), "channelg", channel);
	changedrawmethod(NULL(), "channelb", channel);

	
	//TEXT PART 1 (RIGHT ALIGNMENT)
	if(getglobalvar("drawText") == "part0"){
		drawsprite(getglobalvar("portrait_aldebaran5"), xPortRight, yPort, spriteLayer);
		drawbox(xBox, yBox, boxWidth, boHeight, textLayer, color, alpha);

		font = 0; text = "The fun is over! You will";
		xText = 225;yText = 95+lineStart;
		drawstring(xText, yText, font, text, textLayer);
		
		font = 0; text = "receive my ultimate technique!";
		xText = 225;yText = yText+lineAdd;
		drawstring(xText, yText, font, text, textLayer);
	}
	//KILL THE DIALOG ENTITY AT THE END AND CLEAR VARIABLES
	if(getglobalvar("drawText") == "dialog0_end"){
		setglobalvar("dialog_entity", NULL());
		killentity(self);
	}
}