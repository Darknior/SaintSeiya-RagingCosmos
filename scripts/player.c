#import "data/scripts/library/spawn.h"
#import "data/scripts/library/basic.h"
#import "data/scripts/dialogs/animation.c"
#import "data/scripts/main_grabs.c"

void keyint(void Ani, int Frame, void Key, int Hflag)
{// Change current animation if proper key is pressed or released
// Animation is changed to attack mode

    void self = getlocalvar("self");
    int Dir = getentityproperty(self, "direction");
    int iPIndex = getentityproperty(self,"playerindex"); //Get player index
    void iRKey;

      if (Key=="U"){ //Up Required?
        iRKey = playerkeys(iPIndex, 0, "moveup"); // "Up"
      } else if (Key=="D"){ //Down Required?
        iRKey = playerkeys(iPIndex, 0, "movedown"); // "Down"
      } else if (Key=="L"){ //Left Required?
        iRKey = playerkeys(iPIndex, 0, "moveleft"); // "Left"
      } else if (Key=="R"){ //Right Required?
        iRKey = playerkeys(iPIndex, 0, "moveright"); // "Right"
      } else if (Key=="F"){ //Forward Required?
        if (Dir == 0){ // Facing Left?
          iRKey = playerkeys(iPIndex, 0, "moveleft"); // "Left"
        } else { // Facing Right
          iRKey = playerkeys(iPIndex, 0, "moveright"); // "Right"
        }
      } else if (Key=="B"){ //Backward Required?
        if (Dir == 1){ // Facing Right?
          iRKey = playerkeys(iPIndex, 0, "moveleft"); // "Left"
        } else { // Facing Left
          iRKey = playerkeys(iPIndex, 0, "moveright"); // "Right"
        }
      } else if (Key=="J"){ //Jump Required?
        iRKey = playerkeys(iPIndex, 0, "jump"); // "Jump"
      } else if (Key=="A"){ //Attack Required?
        iRKey = playerkeys(iPIndex, 0, "attack"); // "Attack"
      } else if (Key=="A2"){ //Attack Required?
        iRKey = playerkeys(iPIndex, 0, "attack2"); // "Attack2"
      } else if (Key=="S"){ //Special Required?
        iRKey = playerkeys(iPIndex, 0, "special"); // "Special"
      }

      if (Hflag==1){ //Not holding the button case?
        iRKey = !iRKey; //Take the opposite condition
      }

      if (iRKey){
        if (Ani=="ANI_IDLE"){ // Going idle?
          setidle(self, openborconstant("ANI_IDLE")); //Be idle!
        } else {
          performattack(self, openborconstant(Ani)); //Change the animation
        }
        updateframe(self, Frame); //Change frame
      }
}

void keymove(float Vx)
{// Move hero if direction button is pressed
      void self = getlocalvar("self");
      int iPIndex = getentityproperty(self,"playerindex"); //Get player index
	float xdir = 0;

      if (playerkeys(iPIndex, 0, "moveleft")){// Left is pressed?
	  xdir = -Vx;
	} else if(playerkeys(iPIndex, 0, "moveright")){// Right is pressed?
	  xdir = Vx;
      }

	changeentityproperty(self, "velocity", xdir);
}

void spawnM(void Name, float dx, float dy, float dz)
{ // Spawn certain entity and matches its map with own's map
   void self = getlocalvar("self");
   int map = getentityproperty(self,"map");
   void Spawn;

   Spawn = spawn01(Name, dx, dy, dz);
   changeentityproperty(Spawn, "map", map);
}

//Direction
//  0 = No change
//  1 = Same direction as parent
// -1 = Opposite direction as parent
//  2 = Always right
// -2 = Always left

//BindFlag:
//  0 = No effect
//  1 = Keep same animation as target
//  2 = Also keep same frame as target
//  4 = Kill binded entity if animation doesn't match
//  5 = Keep same animation as target and kill binded entity if animation doesn't match 
//  6 = Keep same frame as target and kill binded entity if animation doesn't match 

//Example: @cmd spawnbind "Naruto_Rasengan" "ANI_FOLLOW10" 0 0 0 1 0
//Result: Binds a Rasengan on the caller, running its "Follow 10" animation.

void spawnbind(void ChildName, void ChildAnim, float OffsetX, float OffsetY, float OffsetZ, int Direction, int BindFlag)
{
	void self = getlocalvar("self");										//Spawnin parent entity.
	void Child = spawn01(ChildName, OffsetX, OffsetY, OffsetZ);							//Spawned child entity.
	
	changeentityproperty(Child, "parent", self);							//Set child's parent.
	performattack(Child, openborconstant(ChildAnim));
  bindentity(Child, self, OffsetX, OffsetY, OffsetZ, Direction, BindFlag);

  return Child;
}

void unbind()
{
	void self = getlocalvar("self");
	bindentity(self, NULL());
}

void tosser3(void Bomb, float dx, float dy, float dz, float Vx, float Vy, float Vz)
{// Tossing bomb with desired speed and base 0 (SIGNAL BOTTLE)
	void self 		= getlocalvar("self");
	int Direction 	= getentityproperty(self, "direction");
	int x 			= getentityproperty(self, "x");
	int y 			= getentityproperty(self, "a");
	int z 			= getentityproperty(self, "z");
	void Shot;

	if(Direction == 0){ //Is entity facing left?                  
		dx = -dx; //Reverse X direction to match facing
	}

	Shot = projectile(Bomb, x+dx, z+dz, y+dy, Direction, 0, 0, 0);
	tossentity(Shot, Vy, Vx, Vz);
	changeentityproperty(Shot, "speed", Vx);
	changeentityproperty(Shot, "projectile", 0); 
	changeentityproperty(Shot, "base", 0);
	changeentityproperty(Shot, "parent", self);
	return Shot;
}

void stop(void anim)
{// Stop!!
	void self = getlocalvar("self");

	changeentityproperty(self, "speed", 0);
	changeentityproperty(self, "velocity", 0, 0, 0);
  if(anim != NULL()){performattack(self, openborconstant(anim));}

}

void jump(int flag)
{//Turns jumping status
    void self = getlocalvar("self");

    if(flag == 1){
        changeentityproperty(self, "aiflag", "jumping", 1);
        changeentityproperty(self, "takeaction", "common_jump");
    }
    else
    {
        changeentityproperty(self, "aiflag", "jumping", 0);
    }
}

void setLayer(int layer)
{//Changes layer
	void self = getlocalvar("self");

	changeentityproperty(self, "setlayer", layer);
}

void spawnScreen(void name, float fX, float fY, float fZ, int dir)
{//Spawn entity at specific location relative to xPos
	void self	= getlocalvar("self");
	void vSpawn	= spawn01(name, 0, 0, 0);
	float xPos	= openborvariant("xpos");

	fX = xPos+fX; //CHANGES POSITION RELATIVE TO XPOS
	changeentityproperty(vSpawn, "position", fX, fZ, fY); //SET SPAWN LOCATION.
  changeentityproperty(vSpawn, "direction", dir); //Set direction.
}