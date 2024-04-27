#import "data/scripts/library/spawn.h"
#import "data/scripts/player.c"

void main()
{//Turn on/off generic effects according to global variables
	
	if(openborvariant("in_selectscreen")){
		blinkSelect();
	}

	if(openborvariant("in_level")){
		blinkDamage();
		blinkCharge();
	}

	afterEffect();
}

void afterEffect()
{//Check defined conditions before use "after image" effect
	void self	= getlocalvar("self");
	void name	= getentityproperty(self, "defaultname");
	void ani	= getentityproperty(self, "animationID");
	
	//THESE ANIMATIONS WILL TRIGGER THE SHADOW TRAILS
	if(	ani == openborconstant("ANI_RUN")||
		ani == openborconstant("ANI_RUNATTACK")||
		ani == openborconstant("ANI_RUNJUMP")||
		ani == openborconstant("ANI_RUNJUMPATTACK")||
		ani == openborconstant("ANI_DODGE")||
		ani == openborconstant("ANI_JUMPATTACK2")||
		ani == openborconstant("ANI_FREESPECIAL5")){
		
		//THESE ENTITIES WILL SHOW THE SHADOW TRAILS
		if(name == "seiya"){afterImage("seiyaS");}
		if(name == "shiryu"){afterImage("shiryuS");}
		if(name == "hyoga"){afterImage("hyogaS");}
		if(name == "shun"){afterImage("shunS");}
		if(name == "ikki"){afterImage("ikkiS");}
	}
}

void afterImage(void shadowEntity, float rate, int legacy)
{//Make "shadow trails" effect for ANY animation (AFTER IMAGE EFFECT)
	void self		= getlocalvar("self");
	void height		= getentityproperty(self, "y");
	float time		= openborvariant("elapsed_time");
	int map			= getentityproperty(self, "map");
	int yLimit		= -20;

	if(rate == NULL()){rate = 16;}
	
	if(time%rate == 0 && height >= yLimit){
		void vSpawn	= spawn01(shadowEntity, 0, 0, -1);
		void vAniID	= getentityproperty(self, "animationID");
		int map		= getentityproperty(self, "map");
		int frame	= getentityproperty(self, "animpos");
		
		changeentityproperty(vSpawn, "parent", self);
		changeentityproperty(vSpawn, "map", map);
		changeentityproperty(vSpawn, "animation", vAniID);
		changeentityproperty(vSpawn, "subject_to_hole", 1);
		changeentityproperty(vSpawn, "no_adjust_base", 0);
		updateframe(vSpawn, frame);
	}
}

void blinkSelect()
{//Turn on/off generic blink effects during the "select" animation
	void self		= getlocalvar("self");
	void animation	= getentityproperty(self, "animationID");
	int frame		= getentityproperty(self, "animpos");
	
	//SELECT SCREEN
	if(animation == openborconstant("ANI_PICK")){ //SELECT ANIMATION
		int tintMode	= 1;
		float time		= openborvariant("elapsed_time");
		float rate		= 22;
	
		//APPLY BLINK EFFECTS
		if(frame <= 1){
			changedrawmethod(self, "enabled", 1);
			changedrawmethod(self, "tintmode", tintMode);
			changedrawmethod(self, "tintcolor", rgbcolor(time*rate, time*rate, time*rate));
		}
		else
		{
			if(getdrawmethod(self, "enabled")){blinkReset();}
		}
	}

	//CAUTION!!! THE LINES BELOW ARE VERY IMPORTANT BECAUSE IS THE ONLY WAY TO GET ALL PREVIEW MODELS SPAWNED BY THE ENGINE IN THE SELECT SCREEN!
	//THIS CODE WILL SAVE THE DEFAULT PREVIEW MODELS IN A VARIABLE AND ALLOWS TO EDIT SOME PROPERTIES
	//THE ONLY WAY TO DEFINE EVERY INDEX IS BY GETTING THE X POSITION AT THE SELECT SCREEN, PLEASE CHECK EVERY POSITION IN THE LEVELS.TXT FILE
	//NEED TO REMEMBER THAT THE ENTITES IN THE SELECT SCREEN WILL NOT ALLOW YOU TO GET SOME PROPERTIES BY REGULAR METHODS, LIKE "PLAYERINDEX"
	
	void type = getentityproperty(self, "type");
	int xPos  = getentityproperty(self, "x");
	int index;

	if(xPos == 100){index = 0;}else
	if(xPos == 200){index = 1;}else
	if(xPos == 300){index = 2;}else
	if(xPos == 400){index = 3;}

	if(type != openborconstant("TYPE_NPC")){setglobalvar("defaultEntity"+index, self);}
}

void blinkDamage()
{//Turn on/off generic blink effects during some "takedamage" events
	void self			= getlocalvar("self");
	void type			= getentityproperty(self, "type");
	void animation		= getentityproperty(self, "animationID");
	int boss			= getentityproperty(self, "boss");
	int maxHealth		= getentityproperty(self, "maxhealth");
	int health			= getentityproperty(self, "health");
	int dead			= getentityproperty(self, "dead");
	int tintMode;
	float time			= openborvariant("elapsed_time");
	float poisonTime	= getentityvar(self, "poisonTime");
	float iceTime		= getentityvar(self, "iceTime");
	float goldTime		= getentityvar(self, "goldTime");
	float purpleTime	= getentityvar(self, "purpleTime");
	float redTime		= getentityvar(self, "redTime");
	float greenTime		= getentityvar(self, "greenTime");
	float silverTime	= getentityvar(self, "silverTime");
	float pinkTime		= getentityvar(self, "pinkTime");
	float whiteTime		= getentityvar(self, "whiteTime");
	float rainbowTime	= getentityvar(self, "rainbowTime");
	float dyingTime		= getentityvar(self, "dyingTime");
	float time;
	float rate;

	//POISON EFFECT
	if(poisonTime > time && !dead){
		
		//GET NEW VALUES
		tintMode	= 2;
		rate		= 2.5;
		
		//APPLY EFFECTS
		changedrawmethod(self, "enabled", 1);
		changedrawmethod(self, "tintmode", tintMode);
		changedrawmethod(self, "tintcolor", rgbcolor(time*rate, 0x00, time*rate));
	}else

	//ICE EFFECT
	if(iceTime > time && !dead){
		
		//GET NEW VALUES
		tintMode	= 6;
		rate		= 2.5;
		
		//APPLY EFFECTS
		changedrawmethod(self, "enabled", 1);
		changedrawmethod(self, "tintmode", tintMode);
		changedrawmethod(self, "tintcolor", rgbcolor(0x33, 0xFF, 0xFF));
	}else

	//GOLD EFFECT
	if(goldTime > time){
		
        //GET NEW VALUES
        tintMode    = 1;
        rate        = 5.5;

        //APPLY EFFECTS
        changedrawmethod(self, "enabled", 1);
        changedrawmethod(self, "tintmode", tintMode);
        changedrawmethod(self, "tintcolor", rgbcolor(time*rate, time*rate, 0x00));
    }else

	//PURPLE EFFECT
	if(purpleTime > time && !dead){
		
        //GET NEW VALUES
        tintMode    = 2;
        rate        = 5.5;

        //APPLY EFFECTS
        changedrawmethod(self, "enabled", 1);
        changedrawmethod(self, "tintmode", tintMode);
        changedrawmethod(self, "tintcolor", rgbcolor(time*rate, 0x00, time*rate));
    }else

	//RED EFFECT
	if(redTime > time){
		
		//GET NEW VALUES
        tintMode    = 2;
        rate        = 5.5;

        //APPLY EFFECTS
        changedrawmethod(self, "enabled", 1);
        changedrawmethod(self, "tintmode", tintMode);
        changedrawmethod(self, "tintcolor", rgbcolor(time*rate, 0x00, 0x00));
    }else


	//GREEN EFFECT
	if(greenTime > time){
		
		//GET NEW VALUES
		tintMode	= 6;
		rate		= 5.5;
		
		//APPLY EFFECTS
		changedrawmethod(self, "enabled", 1);
		changedrawmethod(self, "tintmode", tintMode);
		changedrawmethod(self, "tintcolor", rgbcolor(0x00, time*rate, 0x00));

	}else

	//SILVER EFFECT
    if(silverTime > time){

        //GET NEW VALUES
        tintMode    = 1;
        rate        = 5.5;

        //APPLY EFFECTS
        changedrawmethod(self, "enabled", 1);
        changedrawmethod(self, "tintmode", tintMode);
        changedrawmethod(self, "tintcolor", rgbcolor(0x00, 0x00, time*rate));
    }else

	//PINK EFFECT
	if(pinkTime > time && !dead){
		
		//GET NEW VALUES
		tintMode	= 6;
		rate		= 2.5;
		
		//APPLY EFFECTS
		changedrawmethod(self, "enabled", 1);
		changedrawmethod(self, "tintmode", tintMode);
		changedrawmethod(self, "tintcolor", rgbcolor(0xFF, 0x66, 0xB2));
	}else

	//WHITE EFFECT
	if(whiteTime > time && !dead){
		
		//GET NEW VALUES
		tintMode	= 6;
		rate		= 2.5;
		
		//APPLY EFFECTS
		changedrawmethod(self, "enabled", 1);
		changedrawmethod(self, "tintmode", tintMode);
		changedrawmethod(self, "tintcolor", rgbcolor(0xFF, 0xFF, 0xCC));
	}else

	//RAINBOW EFFECT
	if(rainbowTime > time && !dead){
		
		//GET NEW VALUES
		tintMode	= 1;
		rate		= 8;
		
		//APPLY EFFECTS
		changedrawmethod(self, "enabled", 1);
		changedrawmethod(self, "tintmode", tintMode);
		changedrawmethod(self, "tintcolor", rgbcolor(time*rate, time*rate/2, time*rate/4));
	}else

	//SLOW DYING EFFECT
	if(health <= maxHealth/4 && health > maxHealth/10 && !dead && (type == openborconstant("TYPE_PLAYER") || type == openborconstant("TYPE_ENEMY") && boss)){
		
		//GET NEW VALUES
		tintMode	= 1;
		rate		= 200;
		
		//APPLY EFFECTS
		if(dyingTime == NULL()){setentityvar(self, "dyingTime", 1);}

		if((openborvariant("elapsed_time")/rate)%2 == 0){
			changedrawmethod(self, "enabled", 1);
			changedrawmethod(self, "tintmode", tintMode);
			changedrawmethod(self, "tintcolor", rgbcolor(0x88, 0x00, 0x00));
		}
		else
		{
			changedrawmethod(self, "enabled", 1);
			changedrawmethod(self, "tintmode", 0);
			changedrawmethod(self, "tintcolor", rgbcolor(0x00, 0x00, 0x00));
		}
	}else

	//FAST DYING EFFECT
	if(health <= maxHealth/10 && !dead && (type == openborconstant("TYPE_PLAYER") || type == openborconstant("TYPE_ENEMY") && boss)){
		
		//GET NEW VALUES
		tintMode	= 1;
		rate		= 100;
		
		//APPLY EFFECTS
		if(dyingTime == NULL()){setentityvar(self, "dyingTime", 1);}

		if((openborvariant("elapsed_time")/rate)%2 == 0){
			changedrawmethod(self, "enabled", 1);
			changedrawmethod(self, "tintmode", tintMode);
			changedrawmethod(self, "tintcolor", rgbcolor(0x88, 0x00, 0x00));
		}
		else
		{
			changedrawmethod(self, "enabled", 1);
			changedrawmethod(self, "tintmode", 0);
			changedrawmethod(self, "tintcolor", rgbcolor(0x00, 0x00, 0x00));
		}
	}else

	//BLINK RESET
	{
		//IS NOT IN THE SELECT ANIMATION??
		if(animation != openborconstant("ANI_PICK")){
			if(	poisonTime != NULL()	||
				iceTime != NULL()		||
				goldTime != NULL()		||
				purpleTime != NULL()	||
				redTime != NULL()		||
				greenTime != NULL()		||
				silverTime != NULL()	||
				pinkTime != NULL()		||
				whiteTime != NULL()		||
				rainbowTime != NULL()	||
				dyingTime != NULL()		){
				
				if(getdrawmethod(self, "enabled")){
					blinkReset("poisonTime");
					blinkReset("iceTime");
					blinkReset("goldTime");
					blinkReset("purpleTime");
					blinkReset("redTime");
					blinkReset("greenTime");
					blinkReset("silverTime");
					blinkReset("pinkTime");
					blinkReset("whiteTime");
					blinkReset("rainbowTime");
					blinkReset("dyingTime");
				}
			}
		}
	}
}

void blinkCharge()
{//Perform charged attacks and show blink effects
	void self	= getlocalvar("self");
	void type	= getentityproperty(self, "type");
	
	//APPLY BLINK EFFECTS
	if(type == openborconstant("TYPE_PLAYER")){
		void animation	= getentityproperty(self, "animationID");
		int valid		= getentityproperty(self, "animvalid", openborconstant("ANI_ATTACK5"));
		int pIndex		= getentityproperty(self, "playerindex");
		int dead		= getentityproperty(self, "dead");
		int mp			= getentityproperty(self, "mp");
		int limit		= 33;
		int tintMode	= 5;
		int warning		= 33;
		float time		= openborvariant("elapsed_time");
		float blinkRate	= 20;
		float delay		= 350;
		float color;

		//START THE CHARGE ATTACK VARIABLE IF THE BUTTON IS HELD 
		//CHECK IF AT LEAST THE ATTACK2 ANIMATION IS FOUND
		if(playerkeys(pIndex, 0, "attack") && valid  && mp >= limit){
			if(getglobalvar("chargeStart"+pIndex) == NULL()){
				setglobalvar("chargeStart"+pIndex, openborvariant("elapsed_time")+delay);
			}
			
			//START THE BLINKING EFFECT
			if(time < getglobalvar("chargeStart"+pIndex)){
				if(!dead){
					color = time - getglobalvar("chargeStart"+pIndex);
					changedrawmethod(self, "enabled", 1);
					changedrawmethod(self, "tintmode", tintMode);
					changedrawmethod(self, "tintcolor", rgbcolor(0, 0, color));

					//SPAWN THER CHARGE ATTACK EFFECT
					if(getglobalvar("chargeStart"+pIndex) - time < delay/1.2){
						if(getglobalvar("chargeEffect"+pIndex) == NULL()){
							void vSpawn = spawnbind("super", "ANI_IDLE", 0, 1, 1, 1, 0, 0);
							setglobalvar("chargeEffect"+pIndex, vSpawn);
						}
					}
				}
			}
			else
			{
				if(!dead){
					if((time/blinkRate)%2 == 0){
						color = 255;
						changedrawmethod(self, "enabled", 1);
						changedrawmethod(self, "tintmode", tintMode);
						changedrawmethod(self, "tintcolor", rgbcolor(0, 0, color));
					}
					else
					{
						changedrawmethod(self, "enabled", 0);
					}
				}
			}
		}else

		//PERFORM THE CHARGE ATTACK IF THE BUTTON IS RELEASED
		{
			//CHECK IF ALL REQUIRIMENTS ARE MET IN ORDER TO PERFORM THE CHARGED ATTACK
			if(time > getglobalvar("chargeStart"+pIndex)){

				//CHECK FOR THE ALLOWED ANIMATIONS
				if(	animation == openborconstant("ANI_IDLE")	||
					animation == openborconstant("ANI_IDLE2")	||
					animation == openborconstant("ANI_WALK")	||
					animation == openborconstant("ANI_WALK2")	||
					animation == openborconstant("ANI_UP")		||
					animation == openborconstant("ANI_DOWN")	||
					animation == openborconstant("ANI_SLEEP")	||
					animation == openborconstant("ANI_RUN")		){
					
					//RESET THE COMBOTIME PROPERTY AND PERFORM THE ATTACK10
					setidle(self, openborconstant("ANI_IDLE"));
					changeentityproperty(self, "combotime", 0);
					changeentityproperty(self, "velocity", 0, 0, 0);
					changeentityproperty(self, "mp", mp-limit);
					performattack(self, openborconstant("ANI_ATTACK10"), 0);
				}
			}
			
			//KILL THE CHARGE ATTACK EFFECT
			if(getglobalvar("chargeEffect"+pIndex) != NULL()){
				void effect = getglobalvar("chargeEffect"+pIndex);
				void vName = getentityproperty(effect, "defaultname");
				int exists = getentityproperty(effect, "exists");

				if(vName == "super" && exists){
					killentity(effect);
				}
				setglobalvar("chargeEffect"+pIndex, NULL());
			}
			
			//CLEAR THE CHARGE ATTACK VARIABLE NO MATTER THE RESULTS
			if(getglobalvar("chargeStart"+pIndex) != NULL()){
				blinkReset();
				setglobalvar("chargeStart"+pIndex, NULL());
			}
		}
	}
}

void blinkReset(void varReset)
{//Reset any blink effect and all used variables
	void self = getlocalvar("self");

	//VARIABLE RESET
	if(varReset != NULL()){
		setentityvar(self, varReset, NULL());
	}

	//GENMAKEN RESET
	if(getentityvar(self, "genmaken") != NULL()){
		damageentity(self, self, 0, 1, openborconstant("ATK_NORMAL"));
		setentityvar(self, "genmaken", NULL());
	}

	//DRAWMETHOD RESET
	changedrawmethod(self, "reset", 1);
}