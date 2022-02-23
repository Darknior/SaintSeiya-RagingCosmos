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

if(openborvariant("in_menuscreen")) // Check if the game is in menu screen
  {counter3 = setlocalvar("counter3",0); }
	if((openborvariant("in_options")) &&(counter3!=1)) // Check the variable to avoid double spawn
		{void subent;
		loadmodel("options"); // name of the entity to be loaded
        clearspawnentry(); // clean the spawn entry
		setspawnentry("name", "options"); // define the entity to be spawn
		setspawnentry("coords", 1,0,-1); // set the position of the entity
		subent=spawn();  //  spawn the entity
		changeentityproperty(subent, "position", 1,0,-1); //for safe, set again the position
		counter3 = setlocalvar("counter3",1); // turn on the variable, blocking a new spawn to be made
	}

	if(openborvariant("in_titlescreen")){

		//FREE ASSETS
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