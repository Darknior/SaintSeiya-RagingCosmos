//Library scripts for spawning

void spawn01(void vName, float fX, float fY, float fZ)
{
	//spawn01 (Generic spawner)
	//Damon Vaughn Caskey
	//07/06/2007
	//
	//Spawns entity next to caller.
	//
	//vName: Model name of entity to be spawned in.
	//fX: X location adjustment.
	//fY: Y location adjustment.
      //fZ: Z location adjustment.

	void self = getlocalvar("self"); //Get calling entity.
	void vSpawn; //Spawn object.
	int  iDirection = getentityproperty(self, "direction");

	clearspawnentry(); //Clear current spawn entry.
      setspawnentry("name", vName); //Acquire spawn entity by name.

	if (iDirection == 0){ //Is entity facing left?                  
          fX = -fX; //Reverse X direction to match facing.
	}

      fX = fX + getentityproperty(self, "x"); //Get X location and add adjustment.
      fY = fY + getentityproperty(self, "a"); //Get Y location and add adjustment.
      fZ = fZ + getentityproperty(self, "z"); //Get Z location and add adjustment.
	
	vSpawn = spawn(); //Spawn in entity.

	changeentityproperty(vSpawn, "position", fX, fZ, fY); //Set spawn location.
	changeentityproperty(vSpawn, "direction", iDirection); //Set direction.
    
	return vSpawn; //Return spawn.
}

void spawn02(void vName, float fX, float fY, float fZ)
{
	//Spawns entity based on left screen edge
	//
	//vName: Model name of entity to be spawned in.
	//fX: X distance relative to left edge
	//fY: Y height from ground
      //fZ: Z location adjustment

	void self = getlocalvar("self"); //Get calling entity.
	void vSpawn; //Spawn object.
        int XPos = openborvariant("xpos"); //Get screen edge's position

	clearspawnentry(); //Clear current spawn entry.
      setspawnentry("name", vName); //Acquire spawn entity by name.

      fZ = fZ + getentityproperty(self, "z"); //Get Z location and add adjustment.

	vSpawn = spawn(); //Spawn in entity.

	changeentityproperty(vSpawn, "position", fX + XPos, fZ, fY); //Set spawn location.
	return vSpawn; //Return spawn.
}

void spawner(void vName, float fX, float fY, float fZ)
{	//Spawns entity next to caller and set them as child.
	//
	//vName: Model name of entity to be spawned in.
	//fX: X location adjustment.
	//fY: Y location adjustment.
      //fZ: Z location adjustment.

	void self = getlocalvar("self"); //Get calling entity.
	void vSpawn; //Spawn object.
	
	vSpawn = spawn01(vName, fX, fY, fZ); //Spawn in entity.

	changeentityproperty(vSpawn, "parent", self); //Set caller as parent.
    
	return vSpawn; //Return spawn.
}

void spawner2(void vName, float fX, float fY, float fZ)
{//Spawns entity next to caller with same remap as spawner's.
	//
	//vName: Model name of entity to be spawned in.
	//fX: X location adjustment.
	//fY: Z location adjustment.
        //fZ: Y location adjustment.

	void self = getlocalvar("self"); //Get calling entity.
	void vSpawn; //Spawn object.
	int  iMap = getentityproperty(self, "map");
	
	vSpawn = spawn01(vName, fX, fY, fZ); //Spawn in entity.

	changeentityproperty(vSpawn, "map", iMap); //Use same remap as spawner's.
	return vSpawn; //Return spawn.
}

void spawner3(void vName, float fX, float fY, float fZ)
{//Spawns entity based on left screen edge and sets it ownership
	//
	//vName: Model name of entity to be spawned in.
	//fX: X location adjustment.
	//fY: Z location adjustment.
        //fZ: Y location adjustment.

	void self = getlocalvar("self"); //Get calling entity.
	void vSpawn; //Spawn object.
	int  iMap = getentityproperty(self, "map");
	
	vSpawn = spawn02(vName, fX, fY, fZ); //Spawn in entity

        changeentityproperty(vSpawn, "owner", self);
}

void shooter2(void Shot, float dx, float dy, float Vx, float Vy)
{ // Shooting special projectile with speed control
   void self = getlocalvar("self");
   int Direction = getentityproperty(self, "direction");
   void vShot;

   if (Direction == 0){ //Is entity facing left?                  
      Vx = -Vx; //Reverse Vx direction to match facing
   }

   vShot = spawn01(Shot, dx, dy, 0);
   changeentityproperty(vShot, "velocity", Vx, 0, Vy);
   changeentityproperty(vShot, "owner", self);
   return vShot;
}

void tosser2(void Bomb, float dx, float dy, float Vx, float Vy)
{ // Tossing special bomb with desired speed
   void self = getlocalvar("self");
   int Direction = getentityproperty(self, "direction");
   void Shot;

   if (Direction == 0){ //Is entity facing left?                  
      Vx = -Vx; //Reverse Vx direction to match facing
   }

   Shot = spawn01(Bomb, dx, dy, 0);
   tossentity(Shot, Vy, Vx);
   changeentityproperty(Shot, "owner", self);
   return Shot;
}

void shoot(void Shot, float dx, float dy, float dz)
{ // Shooting projectile
   void self = getlocalvar("self");
   int Direction = getentityproperty(self, "direction");
   int x = getentityproperty(self, "x");
   int y = getentityproperty(self, "a");
   int z = getentityproperty(self, "z");

   if (Direction == 0){ //Is entity facing left?                  
      dx = -dx; //Reverse X direction to match facing
   }

   projectile(Shot, x+dx, z+dz, y+dy, Direction, 0, 0, 0);
}

void shooter(void Shot, float dx, float dy, float dz, float Vx, float Vy, float Vz)
{ // Shooting projectile with speed control
   void self = getlocalvar("self");
   int Direction = getentityproperty(self, "direction");
   int x = getentityproperty(self, "x");
   int y = getentityproperty(self, "a");
   int z = getentityproperty(self, "z");
   void vShot;

   if (Direction == 0){ //Is entity facing left?                  
      dx = -dx; //Reverse X direction to match facing
   }

   vShot = projectile(Shot, x+dx, z+dz, y+dy, Direction, 1, 0, 0);
   changeentityproperty(vShot, "velocity", Vx, Vz, Vy);
   changeentityproperty(vShot, "speed", Vx);
}

void toss(void Bomb, float dx, float dy, float dz)
{ // Tossing bomb
   void self = getlocalvar("self");
   int Direction = getentityproperty(self, "direction");
   int x = getentityproperty(self, "x");
   int y = getentityproperty(self, "a");
   int z = getentityproperty(self, "z");

   if (Direction == 0){ //Is entity facing left?                  
      dx = -dx; //Reverse X direction to match facing
   }

   projectile(Bomb, x+dx, z+dz, y+dy, Direction, 0, 1, 0);
}

void tosser(void Bomb, float dx, float dy, float dz, float Vx, float Vy, float Vz)
{ // Tossing bomb with desired speed
   void self = getlocalvar("self");
   int Direction = getentityproperty(self, "direction");
   int x = getentityproperty(self, "x");
   int y = getentityproperty(self, "a");
   int z = getentityproperty(self, "z");
   void Shot;

   if (Direction == 0){ //Is entity facing left?                  
      dx = -dx; //Reverse X direction to match facing
   }

   Shot = projectile(Bomb, x+dx, z+dz, y+dy, Direction, 0, 1, 0);
   tossentity(Shot, Vy, Vx, Vz);
   changeentityproperty(Shot, "speed", Vx); 
}

void tosserM(void Bomb, float dx, float dy, float Vx, float Vy)
{ // Tossing special bomb with speed control and same remap
   void self = getlocalvar("self");
   int  iMap = getentityproperty(self, "map");
   void vShot;

   vShot = tosser2(Bomb, dx, dy, Vx, Vy);
   changeentityproperty(vShot, "map", iMap);
}

void shootM(void Shot, float dx, float dy, float dz)
{ // Shooting projectile with same palette
   void self = getlocalvar("self");
   int Direction = getentityproperty(self, "direction");
   int Map = getentityproperty(self, "map");
   int x = getentityproperty(self, "x");
   int y = getentityproperty(self, "a");
   int z = getentityproperty(self, "z");
   void vShot;

   if (Direction == 0){ //Is entity facing left?                  
      dx = -dx; //Reverse X direction to match facing
   }

   vShot = projectile(Shot, x+dx, z+dz, y+dy, Direction, 0, 0, Map);
   return vShot;
}

int check_inscreen(void self)
{
    if(getentityproperty(self, "animnum")!=openborconstant("ANI_SPAWN") && getentityproperty(self, "x")>0 && getentityproperty(self, "x")<getindexedvar(11))
    {
	    changeentityproperty(self, "invincible", 0);
	    changeentityproperty(self, "invinctime", 0);
	    changeentityproperty(self, "velocity", 0, 0, 0);
	    changeentityproperty(self, "subject_to_screen", 1);
        changeentityproperty(self, "animation", openborconstant("ANI_SPAWN"));
        return 1;
    }
    else
    {
        return 0;
    }
}