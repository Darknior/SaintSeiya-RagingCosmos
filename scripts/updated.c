void zoom()
{
   void vscreen = openborvariant("vscreen");
   int maxz=openborvariant("PLAYER_MAX_Z")+1000;
   int zoom_value=getglobalvar("zoomvalue");
   int zoom_x=getglobalvar("zoomx");
   int zoom_y=getglobalvar("zoomy");
   void ent=getglobalvar("zoomentity");
   int px=getentityproperty(ent,"x") +  zoom_x - openborvariant("xpos");
   int py=getentityproperty(ent,"z") + zoom_y - openborvariant("ypos") - getentityproperty(ent,"a");
   void zoom_scr = getglobalvar("zoomscreen");
   if(!zoom_scr){
      zoom_scr = allocscreen(openborvariant("hResolution"),openborvariant("vResolution"));
      setglobalvar("zoomscreen",zoom_scr);
   }
   clearscreen(zoom_scr);

   //draw what we need 
   drawspriteq(zoom_scr,0,openborconstant("MIN_INT"),maxz,0,0);
   //setup drawMethod
   changedrawmethod(NULL(),"reset",1);
   changedrawmethod(NULL(),"enabled",1);
   changedrawmethod(NULL(),"scalex",zoom_value);
   changedrawmethod(NULL(),"scaley",zoom_value);
   changedrawmethod(NULL(),"centerx",px);
   changedrawmethod(NULL(),"centery",py);
   //Draw the resized customized screen to main screen.
   drawscreen(zoom_scr,px,py, maxz+1);
   changedrawmethod(NULL(),"enabled", 0);
   drawspriteq(vscreen, 0, maxz+1,maxz+1, 0, 0);
   drawspriteq(vscreen, 0, maxz+2,openborconstant("MAX_INT"), 0, 0);
   clearspriteq();
}


void main(){//MAIN START
   if(getglobalvar("zoomentity"))
   {
      zoom();        
   }
   
void counter= getlocalvar("counter");  // Set a variable to trigger the spawn on and off (title)
void counter2= getlocalvar("counter2");  // Set a variable to trigger the spawn on and off (select)
void counter3= getlocalvar("counter3");  // Set a variable to trigger the spawn on and off (select)
if(openborvariant("in_menuscreen")) // Check if the game is in menu screen
  {counter = setlocalvar("counter",0); }
	if((openborvariant("in_titlescreen")) &&(counter!=1)) // Check the variable to avoid double spawn
		{void subent;
		loadmodel("bgtitle"); // name of the entity to be loaded
        clearspawnentry(); // clean the spawn entry
		setspawnentry("name", "bgtitle"); // define the entity to be spawn
		setspawnentry("coords", 1,0,-1); // set the position of the entity
		subent=spawn();  //  spawn the entity
		changeentityproperty(subent, "position", 1,0,-1); //for safe, set again the position
		counter = setlocalvar("counter",1); // turn on the variable, blocking a new spawn to be made
	} 

if(openborvariant("in_menuscreen")) // Check if the game is in menu screen
  {counter2 = setlocalvar("counter2",0); }
	if((openborvariant("in_selectscreen")) &&(counter2!=1)) // Check the variable to avoid double spawn
		{void subent;
		loadmodel("bgselect"); // name of the entity to be loaded
		clearspawnentry();
		setspawnentry("name", "bgselect");
		subent = spawn();
		changeentityproperty(subent, "position", 0, 0, 0);
		changeentityproperty(subent, "direction", 1);		
      counter2 = setlocalvar("counter2",1); // turn on the variable, blocking a new spawn to be made
	}	

// if(openborvariant("in_menuscreen")) // Check if the game is in menu screen
//   {counter3 = setlocalvar("counter3",0); }
// 	if((openborvariant("in_options")) &&(counter3!=1)) // Check the variable to avoid double spawn
// 		{void subent;
// 		loadmodel("options"); // name of the entity to be loaded
//         clearspawnentry(); // clean the spawn entry
// 		setspawnentry("name", "options"); // define the entity to be spawn
// 		setspawnentry("coords", 1,0,-1); // set the position of the entity
// 		subent=spawn();  //  spawn the entity
// 		changeentityproperty(subent, "position", 1,0,-1); //for safe, set again the position
// 		counter3 = setlocalvar("counter3",1); // turn on the variable, blocking a new spawn to be made
// 	}

	if(openborvariant("in_titlescreen")){

		//FREE PORTRAIT IMAGES
		if(getglobalvar("seiya1") != NULL()){free(getglobalvar("seiya1"));}
		if(getglobalvar("seiya2") != NULL()){free(getglobalvar("seiya2"));}
		if(getglobalvar("seiya3") != NULL()){free(getglobalvar("seiya3"));}
		if(getglobalvar("aldebaran1") != NULL()){free(getglobalvar("aldebaran1"));}
		if(getglobalvar("aldebaran2") != NULL()){free(getglobalvar("aldebaran2"));}
		if(getglobalvar("aldebaran3") != NULL()){free(getglobalvar("aldebaran3"));}
		if(getglobalvar("aldebaran4") != NULL()){free(getglobalvar("aldebaran4"));}
		if(getglobalvar("aldebaran5") != NULL()){free(getglobalvar("aldebaran5"));}
		if(getglobalvar("screen") != NULL()){free(getglobalvar("screen"));}

		//SCRIPT USED TO CLEAR ALL VARIABLES AND ASSETS
		if(getglobalvar("clearAll") != NULL()){clearlocalvar();clearglobalvar();}

		//WORKS WITH "SELECT SCREEN" FUNCTIONS
		if(getglobalvar("previewCol0") == NULL()){
			setglobalvar("previewCol0", 1);
			setglobalvar("previewCol1", 1);
			setglobalvar("previewCol2", 1);
			setglobalvar("previewCol3", 1);
			setglobalvar("previewLine0", 1);
			setglobalvar("previewLine1", 1);
			setglobalvar("previewLine2", 1);
			setglobalvar("previewLine3", 1);
		}
	}
	
	if(openborvariant("in_menuscreen")){

		//SET THE "CLEARALL" FLAG TO 1
		if(getglobalvar("clearAll") == NULL()){setglobalvar("clearAll", 1);}
		menuScreen();
	}

	//IN LEVEL
	if(openborvariant("in_level")){
		drawOverlay();
	}
	
}// MAIN END

void menuScreen()
{//Draw a menu screen with and allow to customize positions
	void vScreen	= openborvariant("vscreen");
	float scaleX	= 256;
	float scaleY	= 256;
	float xMenu		= -216;
	float yMenu		= 0;
	float xBar		= -100;
	float yBar		= 0;

	//START AND DEFINE VARIABLES
	if(getglobalvar("screen") == NULL()){
		setglobalvar("screen", allocscreen(openborvariant("hresolution"), openborvariant("vresolution")));
	}
	
	//CLEAR SCREEN
	clearscreen(getglobalvar("screen"));
	
	//DRAW BACKGROUND
	drawspriteq(getglobalvar("screen"), 0, openborconstant("MIN_INT"), openborconstant("MAX_INT")/3000000000, 0, 0);

	//DRAW MENU
	drawspriteq(getglobalvar("screen"), 0, openborconstant("MAX_INT")/2000000000, openborconstant("MAX_INT")/1000000000, xBar, yBar);
	drawspriteq(getglobalvar("screen"), 0, openborconstant("MAX_INT")/10, openborconstant("MAX_INT"), xMenu, yMenu);

	//DRAW SCREEN
	changedrawmethod(NULL(),"reset", 1);
	changedrawmethod(NULL(),"enabled", 1);
	changedrawmethod(NULL(),"scalex", scaleX);
	changedrawmethod(NULL(),"scaley", scaleY);
	drawscreen(getglobalvar("screen"), 0, 0, openborconstant("MAX_INT"));
	drawspriteq(vScreen, 0, openborconstant("MIN_INT"), openborconstant("MAX_INT"), 0, 0);
	clearspriteq();
}

void drawOverlay()
{//Draw image for overlay effect in lifebar
	void player1 = getplayerproperty(0, "entity");
	void player2 = getplayerproperty(1, "entity");
	void player3 = getplayerproperty(2, "entity");
	void player4 = getplayerproperty(3, "entity");
	
	if(!openborvariant("pause")){
		overlayFunction(player1, 0);
		overlayFunction(player2, 1);
		overlayFunction(player3, 2);
		overlayFunction(player4, 3);
	}
}

void overlayFunction(void player, int pIndex)
{//Script used to reduce code size

	//USED FOR PLAYERS ALREADY IN-GAME
	if(player != NULL()){
		void type	= getentityproperty(player, "type");
		int exists	= getentityproperty(player, "exists");
		
		//DETECTS IF THE DEFINED PLAYER IS IN-GAME AND ALIVE
		if(exists){
			void target	= getentityproperty(player, "opponent");
			int noLife	= getentityproperty(target, "nolife");
			int xPos	= 6;
			int xDif	= 200;
			int xAdd	= xDif*pIndex;
			int yPos1	= 6;
			int yPos2	= 114;
			int yAdd1	= 63;
			int yAdd2	= 78;
			int yAdd3	= 27;
			int yText1	= 54;
			int yText2	= 79;
			int layer1	= 10000;
			int layer2	= 60000;
			int layer3	= 50000;

			//LOAD ASSETS
			if(getglobalvar("charIcon1") == NULL()){setglobalvar("charIcon1", loadsprite("data/sprites/charicon1.png"));}
			if(getglobalvar("charIcon2") == NULL()){setglobalvar("charIcon2", loadsprite("data/sprites/charicon2.png"));}
			if(getglobalvar("lifebar1") == NULL()){setglobalvar("lifebar1", loadsprite("data/sprites/lifebar1.png"));}
			if(getglobalvar("lifebar2") == NULL()){setglobalvar("lifebar2", loadsprite("data/sprites/lifebar2.png"));}
			if(getglobalvar("mpbar") == NULL()){setglobalvar("mpbar", loadsprite("data/sprites/mpbar.png"));}

			//PLAYERS OVERLAY
			setdrawmethod(NULL(),0,256,256,0,0,0,0);
			drawsprite(getglobalvar("charIcon1"), xPos+xAdd, yPos1, layer1); //PLAYER CHARACTER ICON OVERLAY
			setdrawmethod(NULL(),1,256,256,0,0,0,2);
			drawsprite(getglobalvar("lifebar1"), xPos+xAdd, yPos1+yAdd1, layer2); //PLAYER LIFEBAR OVERLAY
			drawsprite(getglobalvar("mpbar"), xPos+xAdd, yPos1+yAdd2, layer2); //PLAYER MPBAR OVERLAY
			drawstring(xPos+xAdd, yPos1+yText1, 0, "energy", layer1);
			drawstring(xPos+xAdd, yPos1+yText2, 0, "cosmos", layer1);

			//DRAW CUSTOM LIFE BARS
			int pIndex		= getentityproperty(player, "playerindex");
			int maxPLife	= getentityproperty(player, "maxhealth");
			int maxELife	= getentityproperty(target, "maxhealth");
			int pLife		= getentityproperty(player, "health");
			int eLife		= getentityproperty(target, "health");
			int maxMp		= getentityproperty(player, "maxmp");
			int mp			= getentityproperty(player, "mp");
			int xPLife		= 7;
			int xELife		= 7;
			int xMp			= 7;
			int yPLife		= 69;
			int yELife		= 141;
			int yMp			= 94;
			int xDif		= 200;
			int xAdd		= xDif*pIndex;
			float xLSize	= 150; //BAR WIDTH INCREASE FACTOR, MORE VALUE IS MORE SIZE (PLAYER LIFE)
			float xESize	= 75; //BAR WIDTH INCREASE FACTOR, MORE VALUE IS MORE SIZE (ENEMY LIFE)
			float xMSize	= 100; //BAR WIDTH INCREASE FACTOR, MORE VALUE IS MORE SIZE (PLAYER MP)
			float ySize		= 9; //BAR HEIGHT INCREASE FACTOR, MORE VALUE IS MORE SIZE

			//PLAYERS
			if(pLife > 0){

				//LIFE BAR
				pLife		= (pLife*xLSize)/(maxPLife); //CALCULATE REMAINING LIFE BAR SIZE
				maxPLife	= (maxPLife*xLSize)/(maxPLife); //CALCULATE MAX LIFE BAR SIZE
				drawbox(xPLife+xAdd, yPLife, pLife, ySize, layer3, rgbcolor(255,255,255), 0); //YELLOW BAR, LIFE REMAINING
				
				//MP BAR
				mp			= (mp*xMSize)/(maxMp); //CALCULATE REMAINING LIFE BAR SIZE
				maxMp		= (maxMp*xMSize)/(maxMp); //CALCULATE MAX LIFE BAR SIZE
				drawbox(xMp+xAdd, yMp, mp, ySize, layer3, rgbcolor(255,255,255), 0); //YELLOW BAR, LIFE REMAINING
			}
			
			//OPPONENTS
			if(target != NULL()){
				if(type == openborconstant("TYPE_PLAYER")){
					if(!noLife){ //ENEMIES

						//OPPONENTS OVERLAY
						setdrawmethod(NULL(),0,256,256,0,0,0,0);
						drawsprite(getglobalvar("charIcon2"), xPos+xAdd, yPos2, layer1); //OPPONENT CHARACTER ICON OVERLAY
						setdrawmethod(NULL(),1,256,256,0,0,0,2);
						drawsprite(getglobalvar("lifebar2"), xPos+xAdd, yPos2+yAdd3, layer2); //OPPONENT LIFEBAR OVERLAY

						//LIFE BAR
						if(pLife > 0){
							eLife		= (eLife*xESize)/(maxELife); //CALCULATE REMAINING LIFE BAR SIZE
							maxELife	= (maxELife*xESize)/(maxELife); //CALCULATE MAX LIFE BAR SIZE
							drawbox(xELife+xAdd, yELife, eLife, ySize, layer3, rgbcolor(255,255,255), 0); //YELLOW BAR, LIFE REMAINING
						}
					}
				}
			}
		}
	}
	else //USED FOR PLAYERS JOINING IN A GAME
	if(getplayerproperty(pIndex, "joining")){
		int xPos	= 6;
		int xDif	= 200;
		int xAdd	= xDif*pIndex;
		int yPos1	= 6;
		int layer1	= 10000;

		setdrawmethod(NULL(),0,256,256,0,0,0,0);
		drawsprite(getglobalvar("charIcon1"), xPos+xAdd, yPos1, layer1); //PLAYER CHARACTER ICON OVERLAY
	}
}


// Import functions.
#import "data/scripts/common/effect_tint.c"

void main(){

    void    ent         = NULL();   // Target entity pointer.
    int     ent_count   = 0;        // Entity count.
    int     i           = 0;        // Loop counter.

    // Get entity count.
    ent_count = openborvariant("ent_max");

    // Loop over each entity index.
    for(i=0; i<ent_count; i++)
    {
        // Get entity pointer.
		ent = getentity(i);

		// Execute tint effect on entity.
        effect_tint(ent);
    }

    // If you have any screen controlling functions, such
    // as full screen zooming, place them here.


}