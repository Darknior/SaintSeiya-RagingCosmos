void main()
{
	debugProjectile(); //USED ON PROJECTILE ENTITIES IN ONDRAW SCRIPT EVENT
}

void debugProjectile()
{//This script activates a Debug Mode to make adjustments on PROJECTILE positions, frame by frame
 //To use correctly, copy this line inside your ondraw script file in the first line: #import "data/scripts/_debugs/DEBUG_PROJ.c"
 //If your entity doesn't have an ondraw script event, need to create one. You can call the script using debugProjectile(); function
 //You can call an ondraw event in any character using this code at the header: ondrawscript data/scripts/_debugs/DEBUG_PROJECTILE.c
 //You can remove it after the task is finished
 
 //BUTTONS:
 //Esc					- debug mode on/off
 //Screenshot			- freeze/unfreeze the game slowly
 //Start				- create save file
 //Special+Up/Down		- hold special and press up/down to change Z projectile position
 //Directional			- change X/Y projectile position
 //Attack+Up/Down		- hold attack and press up/down to change projectile frame
 //Attack+Left/Right	- hold attack and press left/right to change caller's frame
 
	void self	 = getlocalvar("self");
	void target	 = getentityproperty(self, "parent");
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
	int add		 = 1;
	float time	 = openborvariant("elapsed_time");
	
	//USED TO ACTIVATE OR DEACTIVATE THE DEBUG MODE
	if(openborvariant("in_level")){ //IN ANY LEVEL?
		if(!openborvariant("pause") && !openborvariant("in_options")){ //NOT PAUSED AND OR IN OPTIONS?
			if(playerkeys(0, 1, "esc")){ //ESC KEY IS PRESSED??
				if(getglobalvar("debugMode") != "on"){ //DEBUG MODE IS DEACTIVATED??
					changeopenborvariant("slowmotion", 1); //ACTIVATE SLOW MOTION
					playsample(openborconstant("SAMPLE_BEEP"), 0, openborvariant("effectvol"), openborvariant("effectvol"), 100, 0); //PLAY SAMPLE
					setglobalvar("activeText", "debugProjectile");
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

		int vFrame = getentityproperty(self, "animpos"); //GET PROJECTILE ENTITY FRAME
		int tFrame = getentityproperty(target, "animpos"); //GET CALLER'S ENTITY FRAME
		
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
						updateframe(self, vFrame+1); //INCREASE PROJECTILE ENTITY FRAME
					}
				}
				if(playerkeys(0, 1, "movedown")){
					bindentity(self, NULL());
					if(vFrame > 0){
						updateframe(self, vFrame-1); //REDUCE PROJECTILE ENTITY FRAME
					}
				}
				if(playerkeys(0, 1, "moveright")){
					bindentity(self, NULL());
					if(tFrame >= 0){
						updateframe(target, tFrame+1); //INCREASE CALLER'S ENTITY FRAME
					}
				}
				if(playerkeys(0, 1, "moveleft")){
					bindentity(self, NULL());
					if(tFrame > 0){
						updateframe(target, tFrame-1); //REDUCE CALLER'S ENTITY FRAME
					}
				}
			}
			else
			if(playerkeys(0, 0, "special")){ //SPECIAL KEY IS HELD??
				if(playerkeys(0, 1, "moveup")){
					bindentity(self, NULL());
					changeentityproperty(self, "position", NULL(), Pz-add, NULL()); //REDUCE PROJECTILE ENTITY Z POSITION
				}
				if(playerkeys(0, 1, "movedown")){
					bindentity(self, NULL());
					changeentityproperty(self, "position", NULL(), Pz+add, NULL()); //INCREASE PROJECTILE ENTITY Z POSITION
				}
			}
			else
			{//NO ATTACK OR SPECIAL KEY IS HELD??
				if(time%4 == 0){ //USED TO MOVE THE ENTITY MORE SLOWLY
					if(playerkeys(0, 0, "moveright")){
						bindentity(self, NULL());
						changeentityproperty(self, "position", Px+add, NULL(), NULL()); //INCREASE PROJECTILE ENTITY X POSITION
					}
					if(playerkeys(0, 0, "moveleft")){
						bindentity(self, NULL());
						changeentityproperty(self, "position", Px-add, NULL(), NULL()); //REDUCE PROJECTILE ENTITY X POSITION
					}
					if(playerkeys(0, 0, "moveup")){
						bindentity(self, NULL());
						changeentityproperty(self, "position", NULL(), NULL(), Py+add); //INCREASE PROJECTILE ENTITY Y POSITION
					}
					if(playerkeys(0, 0, "movedown")){
						bindentity(self, NULL());
						changeentityproperty(self, "position", NULL(), NULL(), Py-add); //REDUCE PROJECTILE ENTITY Y POSITION
					}
				}
			}
			
			//USED TO INVERT THE X DIFFERENCE VALUE IF THE CALLER IS FACING LEFT
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
			
			//USED TO SAVE ALL VALUE INSIDE THE TXT FILE
			if(playerkeys(0, 1, "start")){
				void file = openfilestream("Saves/PROJECTILE.cfg");
				
				if(file == -1){file = createfilestream();}
				
				filestreamnextline(file);
				setfilestreamposition(file, 0);filestreamappend(file, "FRAME "+tFrame+"# ", 1);
				setfilestreamposition(file, 1);filestreamappend(file, Rx, 1);
				setfilestreamposition(file, 2);filestreamappend(file, Ry, 1);
				setfilestreamposition(file, 3);filestreamappend(file, Rz, 1);
				
				savefilestream(file, "PROJECTILE.cfg", "Saves/");
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
		drawstring(xPos, yPos+yAdd, font0, "debug_proj", layer); //DRAW DEBUG MODE TITLE
		yPos = yPos+yAdd;
		drawstring(xPos, yPos+yAdd, font1, "PROJECTILE_FRAME:_"+(vFrame), layer); //DRAW CURRENT PROJECTILE FRAME IN THE SCREEN
		yPos = yPos+yAdd;
		drawstring(xPos, yPos+yAdd, font1, "PARENT_FRAME:_"+(tFrame), layer); //DRAW CURRENT PARENT FRAME IN THE SCREEN
		yPos = yPos+yAdd;
		drawstring(xPos, yPos+yAdd, font1, "X:_"+Rx, layer); //DRAW X POSITION ADJUST IN THE SCREEN
		yPos = yPos+yAdd;
		drawstring(xPos, yPos+yAdd, font1, "Y:_"+Ry, layer); //DRAW Y POSITION ADJUST IN THE SCREEN
		yPos = yPos+yAdd;
		drawstring(xPos, yPos+yAdd, font1, "Z:_"+Rz, layer); //DRAW Z POSITION ADJUST IN THE SCREEN
		yPos = yPos+yAdd;
		
		//FILE SAVED MESSAGE
		if(time - getlocalvar("saved") < 100){drawstring(xPos, yPos+yAdd, font2, "FILE_SAVED", layer);}
	}
}