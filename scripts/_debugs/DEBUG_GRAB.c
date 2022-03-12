void main()
{
	debugGrab(); //USED ON GRABBED ENTITIES IN ONDRAW SCRIPT EVENT
}

void debugGrab()
{//This script activates a Debug Mode to make adjustments on GRAB positions, frame by frame

 //To use correctly, copy this line inside your ondraw script file in the first line:
 //#import "data/scripts/_debugs/DEBUG_GRAB.c"

 //If your entity doesn't have an ondraw script event, need to create one. You can call the script using debugGrab();
 
 //You can call an ondraw event in any character using this code at the header:
 //ondrawscript data/scripts/_debugs/DEBUG_GRAB.c

 //You can remove it after the task is finished
 
 //BUTTONS:
 //Esc					- debug mode on/off
 //Screenshot			- freeze/unfreeze the game slowly
 //Start				- create save file
 //Special+Up/Down		- hold special and press up/down to change Z grabbed position
 //Directional			- change X/Y grabbed position
 //Attack+Up/Down		- hold attack and press up/down to change grabbed frame
 //Attack+Left/Right	- hold attack and press left/right to change grabber frame
 //Jump					- change grabbed entity facing direction
 
	void self	 = getlocalvar("self");
	void target  = getentityproperty(self, "opponent");
	int vDir 	 = getentityproperty(self, "direction");
	int tDir 	 = getentityproperty(target, "direction");
	int Px		 = getentityproperty(self, "x");
	int Py		 = getentityproperty(self, "y");
	int Pz		 = getentityproperty(self, "z");
	int Tx		 = getentityproperty(target, "x");
	int Ty		 = getentityproperty(target, "y");
	int Tz		 = getentityproperty(target, "z");
	int Rx;
	int Ry;
	int Rz;
	int facing;
	int add		 = 1;
	float time	 = openborvariant("elapsed_time");
	
	//USED TO ACTIVATE OR DEACTIVATE THE DEBUG MODE
	if(openborvariant("in_level")){ //IN ANY LEVEL?
		if(!openborvariant("pause") && !openborvariant("in_options")){ //NOT PAUSED AND OR IN OPTIONS?
			if(playerkeys(0, 1, "esc")){ //ESC KEY IS PRESSED??
				if(getglobalvar("debugMode") != "on"){ //DEBUG MODE IS DEACTIVATED??
					changeopenborvariant("slowmotion", 1); //ACTIVATE SLOW MOTION
					playsample(openborconstant("SAMPLE_BEEP"), 0, openborvariant("effectvol"), openborvariant("effectvol"), 100, 0); //PLAY SAMPLE
					setglobalvar("activeText", "debugGrab");
					setglobalvar("debugMode", "on");
				}
				else
				{
					changeopenborvariant("slowmotion", 0); //DEACTIVATE SLOW MOTION
					changeopenborvariant("nopause", 0); //UNLOCK PAUSE COMMAND
					changeopenborvariant("textbox", 0); //CLEAR TEXTBOX TO UNFREEZE THE GAME
					playsample(openborconstant("SAMPLE_BEEP"), 0, openborvariant("effectvol"), openborvariant("effectvol"), 100, 0); //PLAY SAMPLE
					setglobalvar("activeText", 0);
					setglobalvar("debugMode", 0);
				}
			}
		}
	}
	
	if(getglobalvar("debugMode") == "on"){ //IS IN DEBUG MODE??

		//USED TO FREEZE OR UNFREEZE THE GAME SLOWLY
		if(playerkeys(0, 0, "screenshot")){ //USED TO UNFREEZE THE GAME SLOWLY
			if(openborvariant("nopause")){ //USED TO RUN ONCE
				changeopenborvariant("nopause", 0); //UNLOCK PAUSE COMMAND
				changeopenborvariant("textbox", 0); //CLEAR TEXTBOX TO UNFREEZE THE GAME
			}
		}
		else
		{
			if(!openborvariant("nopause")){ //USED TO RUN ONCE
				changeopenborvariant("nopause", 1); //LOCK PAUSE COMMAND
				changeopenborvariant("textbox", 1); //CALL TEXTBOX TO FREEZE THE GAME
			}
		}

		int vFrame = getentityproperty(self, "animpos"); //GET GRABBED ENTITY FRAME
		int tFrame = getentityproperty(target, "animpos"); //GET GRABBER ENTITY FRAME
	
		if(getlocalvar("background") == NULL()){
			void background = loadsprite("data/scripts/_debugs/back.png"); //LOAD BACKGROUND FOR FURTHER USE
			setlocalvar("background", background);
		}
		
		//COMMANDS DURING DEBUG MODE
		if(openborvariant("nopause")){
			if(playerkeys(0, 0, "attack")){ //ATTACK KEY IS HELD??
				if(playerkeys(0, 1, "moveup")){
					bindentity(self, NULL());
					if(vFrame >= 0){
						updateframe(self, vFrame+1); //INCREASE GRABBED ENTITY FRAME
					}
				}
				if(playerkeys(0, 1, "movedown")){
					bindentity(self, NULL());
					if(vFrame > 0){
						updateframe(self, vFrame-1); //REDUCE GRABBED ENTITY FRAME
					}
				}
				if(playerkeys(0, 1, "moveright")){
					bindentity(self, NULL());
					if(tFrame >= 0){
						updateframe(target, tFrame+1); //INCREASE GRABBER ENTITY FRAME
					}
				}
				if(playerkeys(0, 1, "moveleft")){
					bindentity(self, NULL());
					if(tFrame > 0){
						updateframe(target, tFrame-1); //REDUCE GRABBER ENTITY FRAME
					}
				}
			}
			else
			if(playerkeys(0, 0, "special")){ //SPECIAL KEY IS HELD??
				if(playerkeys(0, 1, "moveup")){
					bindentity(self, NULL());
					changeentityproperty(self, "position", NULL(), Pz-add, NULL()); //REDUCE GRABBED ENTITY Z POSITION
				}
				if(playerkeys(0, 1, "movedown")){
					bindentity(self, NULL());
					changeentityproperty(self, "position", NULL(), Pz+add, NULL()); //INCREASE GRABBED ENTITY Z POSITION
				}
			}
			else
			{//NO ATTACK OR SPECIAL KEY IS HELD??
				if(time%4 == 0){ //USED TO MOVE THE ENTITY MORE SLOWLY
					if(playerkeys(0, 0, "moveright")){
						bindentity(self, NULL());
						changeentityproperty(self, "position", Px+add, NULL(), NULL()); //INCREASE GRABBED ENTITY X POSITION
					}
					if(playerkeys(0, 0, "moveleft")){
						bindentity(self, NULL());
						changeentityproperty(self, "position", Px-add, NULL(), NULL()); //REDUCE GRABBED ENTITY X POSITION
					}
					if(playerkeys(0, 0, "moveup")){
						bindentity(self, NULL());
						changeentityproperty(self, "position", NULL(), NULL(), Py+add); //INCREASE GRABBED ENTITY Y POSITION
					}
					if(playerkeys(0, 0, "movedown")){
						bindentity(self, NULL());
						changeentityproperty(self, "position", NULL(), NULL(), Py-add); //REDUCE GRABBED ENTITY Y POSITION
					}
				}
			}
		
			//USED TO CHANGE GRABBED ENTITY DIRECTION DURING FALL7 ANIMATION
			if(playerkeys(0, 1, "jump")){
				bindentity(self, NULL());
				
				if(vDir == 0){changeentityproperty(self, "direction", 1);}else{changeentityproperty(self, "direction", 0);}
			}
			
			//USED TO INVERT THE X DIFFERENCE VALUE IF THE GRABBER IS FACING LEFT
			if(tDir == 0){
				Rx = Tx-Px;
			}
			else
			{
				Rx = Px-Tx;
			}
			
			//USED TO CALCULATE Y AND Z DIFFERENCE VALUE
			Ry = Py-Ty;
			Rz = Pz-Tz;
			
			//USED TO DEFINE FACING VALUE ACCORDING TO BINDENTITY FUNCTION
			if(vDir == tDir){
				facing = 1;
			}
			else
			{
				facing = -1;
			}
			
			//USED TO SAVE ALL VALUE INSIDE THE TXT FILE
			if(playerkeys(0, 1, "start")){
				void file = openfilestream("Saves/GRAB.cfg");
				
				if(file == -1){file = createfilestream();}
				
				filestreamnextline(file);
				setfilestreamposition(file, 0);filestreamappend(file, "FRAME "+tFrame+"# ", 1);
				setfilestreamposition(file, 1);filestreamappend(file, vFrame, 1);
				setfilestreamposition(file, 2);filestreamappend(file, Rx, 1);
				setfilestreamposition(file, 3);filestreamappend(file, Ry, 1);
				setfilestreamposition(file, 4);filestreamappend(file, Rz, 1);
				setfilestreamposition(file, 5);filestreamappend(file, facing, 1);
				
				savefilestream(file, "GRAB.cfg", "Saves/");
				closefilestream(file);
				setlocalvar("saved", time);
			}
		}
		
		//USED TO DRAW ALL INFO IN THE SCREEN
		int font0 = 2;
		int font1 = 0;
		int font2 = 1;
		int xPos  = 1;
		int yPos  = 60;
		int yAdd  = 10;
		int layer = 1000000003;
		
		setdrawmethod(NULL(),1,256,256,0,0,0,2);
		drawsprite(getlocalvar("background"), -335, 0, 1000000001);
		drawstring(xPos, yPos+yAdd, font0, "debug_grab", layer); //DRAW DEBUG MODE TITLE
		yPos = yPos+yAdd;
		drawstring(xPos, yPos+yAdd, font1, "GRABBER_FRAME:_"+(tFrame), layer); //DRAW CURRENT GRABBER FRAME IN THE SCREEN
		yPos = yPos+yAdd;
		drawstring(xPos, yPos+yAdd, font1, "GRABBED_FRAME:_"+(vFrame), layer); //DRAW CURRENT GRABBED FRAME IN THE SCREEN
		yPos = yPos+yAdd;
		drawstring(xPos, yPos+yAdd, font1, "X:_"+Rx, layer); //DRAW X POSITION ADJUST IN THE SCREEN
		yPos = yPos+yAdd;
		drawstring(xPos, yPos+yAdd, font1, "Y:_"+Ry, layer); //DRAW Y POSITION ADJUST IN THE SCREEN
		yPos = yPos+yAdd;
		drawstring(xPos, yPos+yAdd, font1, "Z:_"+Rz, layer); //DRAW Z POSITION ADJUST IN THE SCREEN
		yPos = yPos+yAdd;
		drawstring(xPos, yPos+yAdd, font1, "FACING:_"+(facing), layer); //DRAW CURRENT FACING DIRECTION IN THE SCREEN
		yPos = yPos+yAdd;
		
		//FILE SAVED MESSAGE
		if(time - getlocalvar("saved") < 100){drawstring(xPos, yPos+yAdd, font2, "FILE_SAVED", layer);}
	}
}