void grabStart()
{//Grab Starter for grab moves
 //Use SLAM or THROW after using this
	void self	= getlocalvar("self");
	void target	= getentityproperty(self, "grabbing");

	//IS THERE A GRABBED ENTITY?? PROCEED!!
	if(target != NULL()){
		setentityvar(self, "grabbed", target); //SAVE THE GRABBED ENTITY AS A NEW TARGET INSIDE AN ENTITYVAR
		damageentity(target, self, 0, 1, openborconstant("ATK_NORMAL7")); //PUT THE ENTITY IN A FALLING STATE
		changeentityproperty(target, "aiflag", "frozen", 1); //USED TO AVOID GRAB INTERRUPTION WHEN THE NODROPEN IS OFF AND ANY PLAYER IS RESPAWNED
	}
}

void grabStart2()
{//Grab Starter for non-grab moves
 //Use SLAM or THROW after using this
	void self	= getlocalvar("self");
	void target	= getentityproperty(self, "opponent");

	//IS THERE A DAMAGED ENTITY?? PROCEED!!
	if(target != NULL()){
		setentityvar(self, "grabbed", target); //SAVE THE GRABBED ENTITY AS A NEW TARGET INSIDE AN ENTITYVAR
		damageentity(target, self, 0, 1, openborconstant("ATK_NORMAL7")); //PUT THE ENTITY IN A FALLING STATE
		changeentityproperty(target, "aiflag", "frozen", 1); //USED TO AVOID GRAB INTERRUPTION WHEN THE NODROPEN IS OFF AND ANY PLAYER IS RESPAWNED
	}
}

void position(int tFrame, float dx, float dy, float dz, int face)
{//Modify grabbed entity's position relative to grabber
 //Use grabstart 1st before using this
	void self		= getlocalvar("self");
	void target		= getentityvar(self,"grabbed");
	int vFrame		= getentityproperty(self,"animpos");
	int dead		= getentityproperty(target,"dead");
	float weight	= getentityproperty(target,"antigravity");
	float limit		= -0.03;

	//THERE'S A NEW TARGET SAVED IN THE VARIABLE??
	if(target != NULL()){

		//USED WHEN PLAYER DIES BY TIME OVER AND THE GRABBER IS THE ENEMY
		if(dead){
			bindentity(target, NULL());
			damageentity(target, self, 0, 1, openborconstant("ATK_NORMAL"));
			damageentity(self, self, 0, 1, openborconstant("ATK_NORMAL"));
			setentityvar(self, "grabbed", NULL());
		}else
		
		//DEFAULT
		{
			updateframe(target, tFrame);
			bindentity(target, self, dx, dz, dy, face, 0);
		}
	}
}

void slam(int damage, int type, int Vx, int Vy, int Vz, int face)
{//Damage as slam finisher
	void self	= getlocalvar("self");
	void target	= getentityvar(self,"grabbed");
	int tDir	= getentityproperty(target,"direction");
	int vDir;
	
	if(face == 0){ //SAME FACING?
		vDir = tDir;
	}

	if(face == 1){ //OPPOSITE FACING?
		if(tDir == 0){ //FACING LEFT?
			vDir = 1;
		}
		else
		{
			vDir = 0;
		}
	}

	if(target != NULL()){
		void atkType;
		void eType		= getentityproperty(target,"type");
		int dir			= getentityproperty(target,"direction");
		int dropType	= 1;
		
		if(dir == 0){Vx = -Vx;}
		if(type == 1){atkType = openborconstant("ATK_NORMAL8");}
		if(type == 2){atkType = openborconstant("ATK_NORMAL9");}
		
		if(eType == openborconstant("TYPE_PLAYER") || eType == openborconstant("TYPE_NPC")){
			changeentityproperty(target, "projectilehit", "type_player", "type_npc", "type_obstacle");
		}
		else
		if(eType == openborconstant("TYPE_ENEMY")){
			changeentityproperty(target, "projectilehit", "type_enemy", "type_obstacle");
		}
		
		damageentity(target, self, damage-1, dropType, atkType); //SPLIT DAMAGE, USE "1" TO NOT PLAY THE "SLAM" SOUND WHEN HIT THE GROUND

		changeentityproperty(target, "direction", vDir);
		changeentityproperty(target, "aiflag", "projectile", 1);
		changeentityproperty(target, "damage_on_landing", 1); //RESET PROJECTILE STATUS TO 0 WHEN FALL ON THE GROUND, SPLIT DAMAGE
		finishGrab(Vx, Vy, Vz); //EXECUTE ALL NECESSARY TASKS TO END THE GRAB MOVE (ANTIWALL, UNBIND AND TOSSENTITY)
		setentityvar(self, "grabbed", NULL()); //CLEAR ENTITYVAR
	}
}

void throw(int damage, int type, int Vx, int Vy, int Vz, int face)
{//Damage as throw finisher
	void self	= getlocalvar("self");
	void target	= getentityvar(self,"grabbed");
	int z		= getentityproperty(self,"z");
	int tDir	= getentityproperty(target,"direction");
	int vDir;
	
	if(face == 0){ //SAME FACING?
		vDir = tDir;
	}

	if(face == 1){ //OPPOSITE FACING?
		if(tDir == 0){ //FACING LEFT?
			vDir = 1;
		}
		else
		{
			vDir = 0;
		}
	}

	if(target != NULL()){
		void atkType;
		void eType		= getentityproperty(target,"type");
		int dir			= getentityproperty(target,"direction");
		int dropType	= 1;
		
		if(dir == 0){Vx = -Vx;}
		if(type == 1){atkType = openborconstant("ATK_NORMAL8");}
		if(type == 2){atkType = openborconstant("ATK_NORMAL9");}
		if(z > (openborconstant("PLAYER_MIN_Z")+openborconstant("PLAYER_MAX_Z"))/2){Vz = -Vz ;}
		
		if(eType == openborconstant("TYPE_PLAYER") || eType == openborconstant("TYPE_NPC")){
			changeentityproperty(target, "projectilehit", "type_player", "type_npc", "type_obstacle");
		}
		else
		if(eType == openborconstant("TYPE_ENEMY")){
			changeentityproperty(target, "projectilehit", "type_enemy", "type_obstacle");
		}
		
		damageentity(target, self, 0, dropType, atkType);

		changeentityproperty(target, "direction", vDir);
		changeentityproperty(target, "aiflag", "projectile", 1);
		changeentityproperty(target, "damage_on_landing", damage); //RESET PROJECTILE STATUS TO 0 WHEN FALL ON THE GROUND, TOTAL DAMAGE
		finishGrab(Vx, Vy, Vz); //EXECUTE ALL NECESSARY TASKS TO END THE GRAB MOVE (ANTIWALL, UNBIND AND TOSSENTITY)
		setentityvar(self, "grabbed", NULL()); //CLEAR ENTITYVAR
	}
}

void finishGrab(int Vx, int Vy, int Vz)
{//This new function was created by mixing the "antiwall", "depost" and "tossentity" into a unique function
	void self	= getlocalvar("self");
	void target	= getentityvar(self, "grabbed");
	
	if(target != NULL()){
		int x		= getentityproperty(self, "x");
		int Tx		= getentityproperty(target, "x");
		int z		= getentityproperty(self, "z");
		int Tz		= getentityproperty(target, "z");
		int wall	= checkwall(Tx, Tz);

		if(wall){ //WAS DETECTED ANY WALL IN THE TARGET'S POSITION?? RUN ALL TASKS BELOW!!
			changeentityproperty(target, "position", x, z); //FIX THE GRABBED ENTITY'S POSITION ACCORDING TO GRABBER CORRDINATES BEFORE THE OPPONENT IS RELEASED
			bindentity(target, NULL()); //RELEASE GRABBED ENTITY, NOW THE OLD FUNCTION "DEPOST" WORKS HERE
			tossentity(target, Vy); //TOSS OPPONENT WITH Y VELOCITY ONLY, THIS WAY THE RELEASED OPPONENT WILL NOT BE MOVED AGAINST A WALL AGAIN
		}
		else //NO WALLS DETECTED?? ONLY THE DEFAULT TASKS WILL RUN
		{
			bindentity(target, NULL()); //RELEASE GRABBED ENTITY, NOW THE OLD FUNCTION "DEPOST" WORKS HERE
			tossentity(target, Vy, Vx, Vz); //TOSS OPPONENT WITH NORMAL VELOCITY, X, Y AND Z
		}
	}
}