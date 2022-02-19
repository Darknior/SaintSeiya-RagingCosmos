void main()
{//Global keyall scripts, detects player index
	int player = getlocalvar("player");
	
	menuSelect(player);
}

void menuSelect(int player)
{//Change player preview model according to global variable "previewModel"+player check
	
	if(openborvariant("in_selectscreen")){ //LEVEL SELECT MENU STARTED
		if(getglobalvar("previewEntity"+player) != NULL()){
			void ani = getentityproperty(getglobalvar("previewEntity"+player), "animationID");

			//THE CURRENT HIGHLIGHTED CHARACTER IS NOT CONFIRMED YET?? EXECUTE ALL TASKS BELOW
			if(ani != openborconstant("ANI_PICK")){
				float time  = openborvariant("elapsed_time"); //CURRENT TIME
				int column  = getglobalvar("previewCol"+player); //DEFAULT COLUMN VALUE
				int line	   = getglobalvar("previewLine"+player); //DEFAULT LINE VALUE
				int maxCols;
				int maxLines;
				int add     = 1;

				//STORY MODE
				if(openborvariant("current_set") == 13){
					maxCols  = 1;
					maxLines	= 1;

					//USED FOR LEFT-RIGHT KEYS
					if(playerkeys(player, 0, "moveright")){ //SELECT DESIRED OPTION
						if(line == maxLines){maxCols = 2;} //REDUCE MAX COLUMNS IN THE LAST LINE
						if(column >= 0 && column < maxCols){setglobalvar("previewCol"+player, column+add);}
						if(column == maxCols){setglobalvar("previewCol"+player, 0);}
					}else
					
					if(playerkeys(player, 0, "moveleft")){ //SELECT DESIRED OPTION
						if(line == maxLines){maxCols = 2;} //REDUCE MAX COLUMNS IN THE LAST LINE
						if(column > 0 && column <= maxCols){setglobalvar("previewCol"+player, column-add);}
						if(column == 0){setglobalvar("previewCol"+player, maxCols);}
					}else

					//USED FOR UP-DOWN KEYS
					if(playerkeys(player, 0, "moveup")){ //SELECT DESIRED OPTION
						if(line > 0 && line <= maxLines){setglobalvar("previewLine"+player, line-add);}
						if(line == 0){setglobalvar("previewLine"+player, maxLines);}
						if(column == 2){setglobalvar("previewCol"+player, maxCols);}
					}else
					
					if(playerkeys(player, 0, "movedown")){ //SELECT DESIRED OPTION
						if(line >= 0 && line < maxLines){setglobalvar("previewLine"+player, line+add);}
						if(line == maxLines){setglobalvar("previewLine"+player, 0);}
						if(column == 2){setglobalvar("previewCol"+player, maxCols);}
					}
				}
				changeModel(player);
			}
		}
	}
}

void changeModel(int player)
{//Change model preview according to player changes
	
	if(getglobalvar("previewEntity"+player) != NULL()){
		void ani = getentityproperty(getglobalvar("previewEntity"+player), "animationID");
		
		if(ani != openborconstant("ANI_PICK")){ //THE CURRENT HIGHLIGHTED CHARACTER IS NOT CONFIRMED YET?? EXECUTE ALL TASKS BELOW

			//USED TO TRANSLATE MODELS - STORY MODE
			if(openborvariant("current_set") == 13){
				if(getglobalvar("previewLine"+player) == 0){
					if(getglobalvar("previewCol"+player) == 0){setglobalvar("previewModel"+player, "DRAGON-SHIRYU");}else
					if(getglobalvar("previewCol"+player) == 1){setglobalvar("previewModel"+player, "CYGNUS-HYOGA");}
				}else

				if(getglobalvar("previewLine"+player) == 1){
					if(getglobalvar("previewCol"+player) == 0){setglobalvar("previewModel"+player, "ANDROMEDA-SHUN");}else
					if(getglobalvar("previewCol"+player) == 1){setglobalvar("previewModel"+player, "seiya");}
					if(getglobalvar("previewCol"+player) == 2){setglobalvar("previewModel"+player, "PHOENIX-IKKI");}
				}
			}
			
			//CHANGE CURRENT MODEL PREVIEW ACCORDING TO THE HIGHLIGHTED CHARACTER WHEN ANY DIRECTION IS PRESSED
			if(playerkeys(player, 0, "moveleft")	||
				playerkeys(player, 0, "moveright")	||
				playerkeys(player, 0, "moveup")		||
				playerkeys(player, 0, "movedown")	){

				changeentityproperty(getglobalvar("previewEntity"+player), "model", getglobalvar("previewModel"+player), 1);
				changeentityproperty(getglobalvar("previewEntity"+player), "animation", openborconstant("ANI_SELECT"));
			}
		}
	}
}