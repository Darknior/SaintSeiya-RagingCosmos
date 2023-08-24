//Library scripts for basic functions

void platform(int Flag)
{// Turns subject's platform status
    void self = getlocalvar("self");
    changeentityproperty(self, "Subject_to_Platform", Flag);
}

void offscreenkill( float dx, int E )
{// Check position relative to screen. If enemy is offscreen, suicide!
//  dx : Distance to screen edge
//  E  : Edge selection, 0 = left, 1 = right

    void self = getlocalvar("self");
    int x = getentityproperty(self,"x"); //Get character's x coordinate
    int XPos = openborvariant("xpos"); //Get screen edge's position

    if( (x > XPos + 320 + dx) && (E == 1) ){ // Offscreen to the right?
      killentity(self); //Suicide!
    } else if( (x < XPos - dx) && (E == 0) ){ // Offscreen to the left?
      killentity(self); //Suicide!
    }
}

void beidle()
{// Go to IDLE animation!
    void self = getlocalvar("self");

    setidle(self, openborconstant("ANI_IDLE"));
}

void suicide()
{ // Suicide!!
    void self = getlocalvar("self");

    killentity(self); //Suicide!
}

void move(int dx, int dz, int da)
{ // Moves entity freely or ignores obstacles
    void self = getlocalvar("self");
    int x = getentityproperty(self,"x"); //Get character's x coordinate
    int z = getentityproperty(self,"z"); //Get character's z coordinate
    int a = getentityproperty(self,"a"); //Get character's a coordinate
    int dir = getentityproperty(self,"direction"); //Get character's facing direction

     if(dir==0){ // Facing left?
      changeentityproperty(self, "position", x-dx, z+dz, a+da); //Move
     }
     else if(dir==1){ // Facing right?
      changeentityproperty(self, "position", x+dx, z+dz, a+da); //Move
     }
}

void flip()
{// Flip to opposite direction
    void self = getlocalvar("self");
    int dir = getentityproperty(self,"direction");

    if(dir==0){ // Facing left?
      changeentityproperty(self, "direction", 1);
    } else {
      changeentityproperty(self, "direction", 0);
    }
}

void mpcost( int Cost)
{// Spend some MP
    void self = getlocalvar("self");
    int MP = getentityproperty(self,"mp");

    changeentityproperty(self, "mp", MP-Cost); //Spend!
}

void keyLoop(void key, int frame)
{//Loops defined frame if defined key is held
	void self	= getlocalvar("self");
	int iPIndex	= getentityproperty(self,"playerindex");

	if(playerkeys(iPIndex, 0, key)){updateframe(self, frame);}
}

void addMp(int add)
{//Add mp according to the defined amount
	void self = getlocalvar("self");
  int maxMp = getentityproperty(self, "maxmp");
  int mp    = getentityproperty(self, "mp");

	if(mp < maxMp){changeentityproperty(self, "mp", mp+add);}
}

void aniLimit(void Ani, int Frame, int Limit)
{// Change current animation if MP falls below limit
    void self = getlocalvar("self");
    void MP = getentityproperty(self,"mp");    

    if(MP < Limit){
      if(Ani == openborconstant("ANI_IDLE")){
        setidle(self);
      } else {
        changeentityproperty(self, "animation", openborconstant(Ani),2);
        updateframe(self, Frame);
      }
    }
}

void limiter(int Limit)
{// Prevents hero from performing the animation if his/her MP is less than Limit
    void self = getlocalvar("self");
    int MP = getentityproperty(self,"mp"); //Get entity's MP
    int y = getentityproperty(self,"a"); //Get entity's altitude
    int b = getentityproperty(self, "base"); //Get entity's base

   if(MP<=Limit) // Don't have enough MP?
   {
     if(y > b) // Mid air?
     {
       changeentityproperty(self, "animation", openborconstant("ANI_JUMP") );
     } else{
       setidle(self); //Don't play the animation
     }
   }
}

void floater( int Time )
{// Floats in Time centiseconds
    void self = getlocalvar("self");
    int eTime = openborvariant("elapsed_time");

    changeentityproperty(self, "tosstime", eTime + 2*Time);
}

void anichange(void Ani)
{ // Animation changer
    void self = getlocalvar("self");

    changeentityproperty(self, "animation", openborconstant(Ani)); //Change the animation
}

void aniexec(void Ani)
{// Animation executor
	void self = getlocalvar("self");

    executeanimation(self, openborconstant(Ani), 1);
}

void looper(int Frame, int Limit)
{// Loops current animation
    void self = getlocalvar("self");
    void loop = getlocalvar("Loop" + self);

    if(loop==NULL()){ // Localvar empty?
      setlocalvar("Loop" + self, 0);
      loop = 0;
    } 
   
    if(loop < Limit){ // loops reach limit?
      updateframe(self, Frame); //Change frame
      setlocalvar("Loop" + self, loop+1); // Increment number of loops
    } else if(loop==Limit){ // loops reach limit?
      setlocalvar("Loop" + self, NULL());
    }
}

void degravity(int Ratio)
{// Changes antigravity effect
    void self = getlocalvar("self");
    changeentityproperty(self, "antigravity", Ratio);
}

void clearL()
{ // Clears all local variables
     clearlocalvar();
}

void dasher( float Vx, float Vy, float Vz, int Flip)
{// Dash with desired speed!
    void self = getlocalvar("self");
    int dir = getentityproperty(self,"direction");

    if(dir==0 && Flip==1){ // Facing left?
      Vx = -Vx ;
    }

    changeentityproperty(self, "velocity", Vx, Vz, Vy); //Move!
}

void leaper( float Vx, float Vy, int Flip)
{// Leap with desired speed!
    void self = getlocalvar("self");
    int dir = getentityproperty(self,"direction");

    if(dir==0 && Flip==1){ // Facing left?
      Vx = -Vx ;
    }

    tossentity(self, Vy, Vx); //Leap!
}

void stop()
{// Stop enemy's movement!
    void self = getlocalvar("self");
    changeentityproperty(self, "velocity", 0, 0, 0); //Stop moving!
}

void block(int Flag)
{// Turns blocking status
    void self = getlocalvar("self");
    changeentityproperty(self, "aiflag", "blocking", Flag);
}