void grabStart()
{//Grab Starter for grab moves
 //Use SLAM or THROW after using this
	void self 	= getlocalvar("self");
	void target = getentityvar(self,"grabbed");

	if(target == NULL()){
		target = getentityproperty(self, "grabbing");
		if(target == NULL() || getentityproperty(target, "dead") == 1){
			setidle(self);
		}else{
			setentityvar(self, "grabbed", target);
		}
	}
	
	if(target != NULL()){
		damageentity(target, self, 0, 1, openborconstant("ATK_NORMAL7")); //Grab Starter
		lockMp();
		specialCost();
	}
}

void grabStart2()
{//Grab Starter for non-grab moves
 //Use SLAM or THROW after using this
	void self 	= getlocalvar("self");
	void target = getentityvar(self,"grabbed");

	if(target == NULL()){
		target = getentityproperty(self, "opponent");
		if(target == NULL() || getentityproperty(target, "dead") == 1){
			setidle(self);
		}else{
			setentityvar(self, "grabbed", target);
		}
	}
	
	if(target != NULL()){
		damageentity(target, self, 0, 1, openborconstant("ATK_NORMAL7")); //Grab Starter
		lockMp();
		specialCost();
	}
}

void position(int frame, float dx, float dy, float dz, int face)
{//Modify grabbed entity's position relative to grabber
 //Use grabstart 1st before using this
	void self 	= getlocalvar("self");
	void target = getentityvar(self,"grabbed");
	int dead	= getentityproperty(target,"dead");

	if(target == NULL()){
		target = getentityproperty(self, "grabbing");
		setentityvar(self, "grabbed", target);
	}
	
	if(target != NULL()){
		if(dead == 1){ //USED WHEN PLAYER DIES BY TIME OVER AND THE GRABBER IS THE ENEMY
			bindentity(target, NULL());
			damageentity(target, self, 0, 1, openborconstant("ATK_NORMAL"));
			damageentity(self, self, 0, 1, openborconstant("ATK_NORMAL"));
			lockMp();
			specialCost();
			setentityvar(self, "grabbed", NULL());
		}else{
			updateframe(target, frame);
			bindentity(target, self, dx, dz, dy, face, 0);
		}
	}
}

void depost()
{//Release grabbed entity
	void self 	= getlocalvar("self");
	void target = getentityvar(self,"grabbed");

	if(target == NULL()){
		target = getentityproperty(self, "grabbing");
		setentityvar(self, "grabbed", target);
	}
	
	if(target != NULL()){
		bindentity(target, NULL());
	}
}

void slam(int damage, int type, int x, int y, int z, int face)
{//Damage as slam finisher
	void self 	= getlocalvar("self");
	void target = getentityvar(self,"grabbed");
	int tDir 	= getentityproperty(target,"direction");
	int vDir;
	
	if(face == 0){ //SAME FACING?
		vDir = tDir;
	}

	if(face == 1){ //OPPOSITE FACING?
		if(tDir == 0){ //FACING LEFT?
			vDir = 1;
		}else{
			vDir = 0;
		}
	}

	if(target == NULL()){
		target = getentityproperty(self, "grabbing");
		setentityvar(self, "grabbed", target);
	}
	
	if(target != NULL()){
		void atkType;
		void projectile;
		void eType = getentityproperty(target,"type"); //Get opponent's type
		int dir    = getentityproperty(target,"direction"); //Get opponent's facing direction
		
		if(dir == 0){ //FACING LEFT?
			x = -x;
		}
		
		if(type == 1){
			atkType = openborconstant("ATK_NORMAL8"); //1st Finisher
		}

		if(type == 2){
			atkType = openborconstant("ATK_NORMAL9"); //2nd Finisher
		}
		
		if(eType == openborconstant("TYPE_PLAYER") || eType == openborconstant("TYPE_NPC")){
			changeentityproperty(target, "projectilehit", "type_player", "type_npc", "type_obstacle");
		}else if(eType == openborconstant("TYPE_ENEMY")){
			changeentityproperty(target, "projectilehit", "type_enemy", "type_obstacle");
		}
		
		damageentity(target, self, damage/1.5, 1, atkType); //SPLIT DAMAGE
		lockMp();
		specialCost();
		changeentityproperty(target, "direction", vDir);
		changeentityproperty(target, "aiflag", "projectile", 1);
		changeentityproperty(target, "damage_on_landing", damage/3); //RESET PROJECTILE STATUS TO 0 WHEN FALL ON THE GROUND, SPLIT DAMAGE
		tossentity(target, y, x, z); //TOSS OPPONENT
		antiWall();
		koCount(); //USED FOR DAMAGEENTITY BUG
		setentityvar(self, "grabbed", NULL()); //CLEAR ENTITYVAR
	}
}

void koCount()
{//Count each enemy killed, chaning your variable
 //Specific counter to avoid engine didhit bug in slam/finish using damageentity and without damageonlanding
 //The engine does not know who did the last hit if receive damage using damageentity
 //This script is already in didhitscript by default in each player char, these is used only for slams that end with @cmd slam
	void self 		= getlocalvar("self");
	void target		= getentityproperty(self,"opponent");
	void pType		= getentityproperty(self,"type"); //PLAYER TYPE
	void eType		= getentityproperty(target,"type"); //ENEMY TYPE
	int iPIndex		= getentityproperty(self,"playerindex");
	int dead		= getentityproperty(target,"dead");
	int ko1 		= getglobalvar("ko1");
	int ko2 		= getglobalvar("ko2");
	int ko3 		= getglobalvar("ko3");
	int ko4 		= getglobalvar("ko4");
	
	if(dead == 1){
		if(pType == openborconstant("TYPE_PLAYER") && eType == openborconstant("TYPE_ENEMY")){
			if(iPIndex == 0){
				setglobalvar("ko1",ko1+1);
			}else if(iPIndex == 1){
				setglobalvar("ko2",ko2+1);
			}else if(iPIndex == 2){
				setglobalvar("ko3",ko3+1);
			}else if(iPIndex == 3){
				setglobalvar("ko4",ko4+1);
			}
		}
	}
}

void throw(int damage, int type, int Vx, int Vy, int Vz, int face)
{//Damage as throw finisher
	void self 	= getlocalvar("self");
	void target = getentityvar(self,"grabbed");
	int z 		= getentityproperty(self,"z");
	int tDir 	= getentityproperty(target,"direction");
	int vDir;
	
	if(face == 0){ //SAME FACING?
		vDir = tDir;
	}

	if(face == 1){ //OPPOSITE FACING?
		if(tDir == 0){ //FACING LEFT?
			vDir = 1;
		}else{
			vDir = 0;
		}
	}

	if(target == NULL()){
		target = getentityproperty(self, "grabbing");
		setentityvar(self, "grabbed", target);
	}
	
	if(target != NULL()){
		void atkType;
		void projectile;
		void eType = getentityproperty(target,"type"); //Get opponent's type
		int dir    = getentityproperty(target,"direction"); //Get opponent's facing direction

		if(dir == 0){ //FACING LEFT?
			Vx = -Vx;
		}
		
		if(type == 1){
			atkType = openborconstant("ATK_NORMAL8"); //1st Finisher
		}

		if(type == 2){
			atkType = openborconstant("ATK_NORMAL9"); //2nd Finisher
		}
		
		if(z > (openborconstant("PLAYER_MIN_Z")+openborconstant("PLAYER_MAX_Z"))/2){
			Vz = -Vz ;
		}
		
		if(eType == openborconstant("TYPE_PLAYER") || eType == openborconstant("TYPE_NPC")){
			changeentityproperty(target, "projectilehit", "type_player", "type_npc", "type_obstacle");
		}else if(eType == openborconstant("TYPE_ENEMY")){
			changeentityproperty(target, "projectilehit", "type_enemy", "type_obstacle");
		}
		
		damageentity(target, self, 0, 1, atkType);
		lockMp();
		specialCost();
		changeentityproperty(target, "direction", vDir);
		changeentityproperty(target, "aiflag", "projectile", 1);
		changeentityproperty(target, "damage_on_landing", damage); //RESET PROJECTILE STATUS TO 0 WHEN FALL ON THE GROUND, TOTAL DAMAGE
		tossentity(target, Vy, Vx, Vz); //TOSS OPPONENT
		antiWall();
		setentityvar(self, "grabbed", NULL()); //CLEAR ENTITYVAR
	}
}

void lockMp()
{//Lock mprate with defined conditions. Used for Counter, Super and Rage animations that never fill mp
	void self 	= getlocalvar("self");
	void vAniID = getentityproperty(self,"animationID");
	int disable	= getentityproperty(self,"energycost", "disable", openborconstant(vAniID));
	int maxMp 	= getentityproperty(self,"maxmp");
	int mp 		= getentityproperty(self,"mp");
	int mpRate 	= getentityproperty(self,"mprate");
	
	if(disable == 6){
		if(mp < maxMp){
			changeentityproperty(self, "mp", mp-mpRate);
		}
	}
}

void specialCost()
{//Lock mprate according to "Special Cost" global variable. Used for Special, Special2 and Aerial Special animations
	void self 	  = getlocalvar("self");
	void vAniID   = getentityproperty(self,"animationID");
	void specialC = getglobalvar("specialCost");
	int disable	  = getentityproperty(self,"energycost", "disable", openborconstant(vAniID));
	int maxMp 	  = getentityproperty(self,"maxmp");
	int mp 		  = getentityproperty(self,"mp");
	int mpRate 	  = getentityproperty(self,"mprate");
	
	if(specialC != "life+heal_(time)" && specialC != "life+heal_(hits)" && specialC != "life_only"){
		if(disable == 5){
			if(mp < maxMp){
				changeentityproperty(self, "mp", mp-mpRate);
			}
		}
	}
	
	//USED BY "HEAL HIT" GLOBAL VARIABLE, TO RECOVER HEALTH BY EACH HIT INSTEAD OF RECOVERY BY TIME
	if(specialC == "life+heal_(hits)"){
		if(getglobalvar("addLife"+self) > 0){
			setglobalvar("healHit"+self, 1);
		}
	}
}

void antiWall()
{//Checks distance from the walls
 //If inside of the walls, entity will be moved away with defined movement
	void self 		= getlocalvar("self");
	void target 	= getentityvar(self, "grabbed");
	int direction 	= getentityproperty(self, "direction");
	int x 			= getentityproperty(self, "x");
	int Tx 			= getentityproperty(target, "x");
	int z 			= getentityproperty(self, "z");
	int Tz 			= getentityproperty(target, "z");
	int subWall		= getentityproperty(self, "subject_to_wall");
	float wall 		= checkwall(Tx, Tz);

	if(target != NULL()){
		if(wall){
			if(subWall == 1){
				changeentityproperty(target, "position", x);
				changeentityproperty(target, "velocity", 0, 0, NULL());
			}
		}
	}
}

void grabEnd()
{//End grab jump and free enemy on the ground if no attack is performed (MAX GRAB JUMP)
	void self 	= getlocalvar("self");
	void target = getentityvar(self,"grabbed");

	if(target == NULL()){
		target = getentityproperty(self, "grabbing");
		setentityvar(self, "grabbed", target);
	}
	
	if(target != NULL()){
		changeentityproperty(target,"damage_on_landing",0);
		changeentityproperty(target,"aiflag","falling",0);
		changeentityproperty(target,"aiflag","drop",0);
		changeentityproperty(target,"aiflag","projectile",0);
		changeentityproperty(target,"takeaction", "common_animation_normal");
		setidle(target);
		bindentity(target, NULL());
		antiWall();
		setentityvar(self, "grabbed", NULL());
	}
}

void fakeBind(int dist)
{//Move grabbed entity together with grabber with defined distance
 //Used ind the end of the vault animation if grabber is moved before touch wall/obstacle/platform/offscreen and stuck
 //Changes grabbed position relative to grabber without use bindentity
	void self 		= getlocalvar("self");
	void target 	= getentityproperty(self, "grabbing");
	int direction 	= getentityproperty(self, "direction");
	int x 			= getentityproperty(self, "x");
	int Tx 			= getentityproperty(target, "x");
	int z 			= getentityproperty(self, "z");
	int Tz 			= getentityproperty(target, "z");
	
	if(target != NULL()){
		if(direction == 1){
			if(Tx-x < dist){
				changeentityproperty(target,"position", x+dist);
			}else if(Tx-x > dist){
				changeentityproperty(target,"position", x+dist);
			}
		}
		
		if(direction == 0){
			if(x-Tx < dist){
				changeentityproperty(target,"position", x-dist);
			}else if(x-Tx > dist){
				changeentityproperty(target,"position", x-dist);
			}
		}
	}
}

void vaultStuck(int distL, int distW, int moveZ)
{//Checks if the entity can be stucked between Left edge of the screen and any Wall at your right side
 //If near of both at defined distance, entity (Self/Grabbed) will be moved away with defined movement
 //Specific script for stages st1a/st3b/st6b when camera go down, used on Grab animation before Vault animation
	void self 		= getlocalvar("self");
	void target		= getentityproperty(self, "grabbing");
	int direction 	= getentityproperty(self, "direction");
	int x 			= getentityproperty(self, "x");
	int Tx 			= getentityproperty(target, "x");
	int z 			= getentityproperty(self, "z");
	int Tz 			= getentityproperty(target, "z");
	int xPos 		= openborvariant("xpos");
	int subScreen	= getentityproperty(self, "subject_to_screen");
	int subWall		= getentityproperty(self, "subject_to_wall");
	float L 		= (xPos);
	float W			= checkwall(x+distW,z);
	
	if(target != NULL()){
		if(direction == 0){ //Is entity facing left?                  
			distW = -distW;
		}
		
		if(subScreen == 1 && subWall == 1){
			if(distL > x - L){// Near from the left edge of the screen?
				if(W){// There is a wall near at right side?
					changeentityproperty(self, "position", x, z+moveZ);// Move down in Z axis to gain more space
					changeentityproperty(target, "position", Tx, Tz+moveZ);// Move down in Z axis to gain more space
				}
			}
		}
	}
}

void vaultFix()
{//Fix position and some properties during vault animation
 //Used to execute all tasks needed by vault animation in one function only
    void self 	 = getlocalvar("self");
	void target  = getentityproperty(self, "grabbing");
	void ani	 = getentityproperty(self, "animationID");
	int dir   	 = getentityproperty(self, "direction");
	int x	  	 = getentityproperty(self, "x");
	int Tx	  	 = getentityproperty(target, "x");
	int z	  	 = getentityproperty(self, "z");
	int flip;
	int grabDist;
	float xPos	 = openborvariant("xpos");
	
	if(dir == 0){
		flip = 1;
		grabDist = (Tx-x)*2;
		x = x+grabDist;
	}else if(dir == 1){
		flip = 0;
		grabDist = (x-Tx)*2;
		x = x-grabDist;
	}
	
	if(ani == openborconstant("ANI_GRABATTACK2")){
		if(checkplatformbetween(x, z, 0, 1000)){ //PLAYER IS STUCKED IN ANY PLATFORM??
			changeentityproperty(self, "position", Tx, NULL(), NULL());
			fakeBind(grabDist/2);
		}else if(checkwall(x, z)){ //PLAYER IS STUCKED IN ANY WALL??
			changeentityproperty(self, "position", Tx, NULL(), NULL());
			fakeBind(grabDist/2);
		}else if(x > xPos+480 || x < xPos){ //PLAYER IS OUTSIDE OF THE SCREEN??
			changeentityproperty(self, "position", Tx, NULL(), NULL());
			fakeBind(grabDist/2);
		}else{ //NORMAL ADJUST
			changeentityproperty(self, "position", x, NULL(), NULL());
			fakeBind(grabDist/2);
		}
		changeentityproperty(self, "direction", flip);
		changeentityproperty(self, "takeaction", "common_grab");
	} 
	
	if(ani == openborconstant("ANI_GRABUP2")){
		changeentityproperty(self, "position", x, NULL(), NULL()); //NORMAL ADJUST
		changeentityproperty(self, "direction", flip);
		changeentityproperty(self, "takeaction", "common_grabattack");
		
		if(target != NULL()){
			setidle(target);
		}
	}
}

void doGrab()
{//Grab target with normal grab
    void self 	 = getlocalvar("self");
	void target  = getentityproperty(self,"opponent");
	void type 	 = getentityproperty(target,"type");
	void subType = getentityproperty(target,"subtype");
	
	if(target != NULL()){
		if(type != openborconstant("TYPE_OBSTACLE")){
			if(subType != openborconstant("SUBTYPE_NOTGRAB")){
				dograb(self, target, 0);
			}
		}
	}
}

void grabIdle()
{//Set to idle if no entity is being grabbed (FOG GRAB)
	void self 	= getlocalvar("self");
	void target = getentityproperty(self, "grabbing");

	if(target == NULL()){
		setidle(self);
	}
}