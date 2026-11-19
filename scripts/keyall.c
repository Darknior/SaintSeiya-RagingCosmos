#import "data/scripts/library/live_entity.c"

void main()
{//Global keyall scripts, detects player index
	int player = getlocalvar("player");
	
	if(player==NULL() || player<0 || player>=openborvariant("maxplayers"))return;
    if(openborvariant("in_selectscreen")){
        if(openborvariant("current_set")<=2)menuSelect(player);
    }else if(openborvariant("in_level") && playerkeys(player,1,"attack")){
        dialogs();
    }
}

void menuSelect(int player)
{//Change player preview model according to global variable "previewModel"+player check
	
	if(openborvariant("in_selectscreen")){ //LEVEL SELECT MENU STARTED
		void preview=liveEntity(getglobalvar("previewEntity"+player));
        if(preview==NULL())return;
        if(getentityvar(preview,"selection_preview")==player+1){
			void ani = getentityproperty(preview, "animationID");

			//THE CURRENT HIGHLIGHTED CHARACTER IS NOT CONFIRMED YET?? EXECUTE ALL TASKS BELOW
			if(ani != openborconstant("ANI_PICK")){
				float time	= openborvariant("elapsed_time"); //CURRENT TIME
				int column	= getglobalvar("previewCol"+player); //DEFAULT COLUMN VALUE
				int line	= getglobalvar("previewLine"+player); //DEFAULT LINE VALUE
				int maxCols;
				int maxLines;
				int add		= 1;

				//STORY MODE
				if(openborvariant("current_set") <= 2){
					maxCols		= 1;
					maxLines	= 1;

					//USED FOR LEFT-RIGHT KEYS
					if(playerkeys(player, 1, "moveright")){ //SELECT DESIRED OPTION
						if(line == maxLines){maxCols = 2;} //REDUCE MAX COLUMNS IN THE LAST LINE
						if(column >= 0 && column < maxCols){setglobalvar("previewCol"+player, column+add);}
						if(column == maxCols){setglobalvar("previewCol"+player, 0);}
					}else
					
					if(playerkeys(player, 1, "moveleft")){ //SELECT DESIRED OPTION
						if(line == maxLines){maxCols = 2;} //REDUCE MAX COLUMNS IN THE LAST LINE
						if(column > 0 && column <= maxCols){setglobalvar("previewCol"+player, column-add);}
						if(column == 0){setglobalvar("previewCol"+player, maxCols);}
					}else

					//USED FOR UP-DOWN KEYS
					if(playerkeys(player, 1, "moveup")){ //SELECT DESIRED OPTION
						playsample(openborvariant("global_sample_beep"), 0, 120, 120, 100, 0);
						if(line > 0 && line <= maxLines){setglobalvar("previewLine"+player, line-add);}
						if(line == 0){setglobalvar("previewLine"+player, maxLines);}
						if(column == 2){setglobalvar("previewCol"+player, maxCols);}
					}else
					
					if(playerkeys(player, 1, "movedown")){ //SELECT DESIRED OPTION
						playsample(openborvariant("global_sample_beep"), 0, 120, 120, 100, 0);
						if(line >= 0 && line < maxLines){setglobalvar("previewLine"+player, line+add);}
						if(line == maxLines){setglobalvar("previewLine"+player, 0);}
						if(column == 2){setglobalvar("previewCol"+player, maxCols);}
					}
				}
				changeModel(player, preview);
			}
		}
	}
}

void changeModel(int player, void preview)
{//Change model preview according to player changes
	
	if(preview != NULL()){
		void ani = getentityproperty(preview, "animationID");
		
		if(ani != openborconstant("ANI_PICK")){ //THE CURRENT HIGHLIGHTED CHARACTER IS NOT CONFIRMED YET?? EXECUTE ALL TASKS BELOW

			//USED TO TRANSLATE MODELS - STORY MODE
			if(openborvariant("current_set") <= 2){
				if(getglobalvar("previewLine"+player) == 0){
					if(getglobalvar("previewCol"+player) == 0){setglobalvar("previewModel"+player, "shiryu");}else
					if(getglobalvar("previewCol"+player) == 1){setglobalvar("previewModel"+player, "hyoga");}
				}else

				if(getglobalvar("previewLine"+player) == 1){
					if(getglobalvar("previewCol"+player) == 0){setglobalvar("previewModel"+player, "shun");}else
					if(getglobalvar("previewCol"+player) == 1){setglobalvar("previewModel"+player, "seiya");}
					if(getglobalvar("previewCol"+player) == 2){setglobalvar("previewModel"+player, "ikki");}
				}
			}
			
			//CHANGE CURRENT MODEL PREVIEW ACCORDING TO THE HIGHLIGHTED CHARACTER WHEN ANY DIRECTION IS PRESSED
			if(	playerkeys(player, 1, "moveleft")	||
				playerkeys(player, 1, "moveright")	||
				playerkeys(player, 1, "moveup")		||
				playerkeys(player, 1, "movedown")	){

				changeentityproperty(preview, "model", getglobalvar("previewModel"+player), 1);
				changeentityproperty(preview, "animation", openborconstant("ANI_SELECT"));
			}
		}
	}
}

void dialogs()
{
    if(getglobalvar("lockButton")!=0 || !getglobalvar("line1"))return;
    void entity=liveEntity(getglobalvar("dialog_entity"));
    if(entity==NULL())return;
    if(getentityvar(entity,"dialog_input")!=1)return;
    if(getentityproperty(entity,"animation_handle")==NULL())return;
    int frame=getentityproperty(entity,"animpos");
    if(frame>=0)updateframe(entity,frame+1);
}
