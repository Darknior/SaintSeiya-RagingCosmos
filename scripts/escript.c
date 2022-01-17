#import "data/scripts/library/basic.h"
#import "data/scripts/library/spawn.h"


void faceOpp(int Flag)
{// Faces Opponent
    void self = getlocalvar("self");
    void Opp = getentityproperty(self, "opponent");

    if(Opp){
      int x = getentityproperty(self, "x");
      int Ox = getentityproperty(Opp, "x");

      if(Flag==1){
        if(Ox > x){
          changeentityproperty(self, "direction", 0);
        } else {
          changeentityproperty(self, "direction", 1);
        }
      } else {
        if(Ox > x){
          changeentityproperty(self, "direction", 1);
        } else {
          changeentityproperty(self, "direction", 0);
        }
      }
    }
}

void aniLimit(void Ani, int Frame, int Limit)
{// Change current animation if MP falls below limit
    void self = getlocalvar("self");
    void MP = getentityproperty(self,"mp");    

    if (MP < Limit){
      changeentityproperty(self, "animation", openborconstant(Ani),2);
//      changeentityproperty(self, "animpos", Frame);
      updateframe(self, Frame);
    }
}

void attack0(void Ani)
{// Attack interruption
    void self = getlocalvar("self");

    performattack(self, openborconstant(Ani)); //Attack!
}

void attack1(int RxMin, int RxMax, int Rz, void Ani)
{// Attack interruption with range check
    void self = getlocalvar("self");
    void target = findtarget(self); //Get nearest player
    float x = getentityproperty(self, "x");
    float z = getentityproperty(self, "z");
    int dir = getentityproperty(self, "direction");

    if(target!=NULL()){
      float Tx = getentityproperty(target, "x");
      float Tz = getentityproperty(target, "z");
      float Disx = Tx - x;
      float Disz = Tz - z;

      if(Disz < 0){
        Disz = -Disz;
      }

      if( Disx >= RxMin && Disx <= RxMax && Disz <= Rz && dir == 1) // Target within range on right facing?
      {
        performattack(self, openborconstant(Ani)); //Change the animation
      } else if( Disx >= -RxMax && Disx <= -RxMin && Disz <= Rz && dir == 0) // Target within range on left facing?
      {
        performattack(self, openborconstant(Ani)); //Change the animation
      }
    }
}

void anichange(void Ani)
{// Animation changer
	void self = getlocalvar("self");

    changeentityproperty(self, "animation", openborconstant(Ani)); //Change the animation
}

void aniexec(void Ani)
{// Animation executor
	void self = getlocalvar("self");

    executeanimation(self, openborconstant(Ani), 1);
}

void cancel(int RxMin, int RxMax, int RyMin, int RyMax, int RzMin, int RzMax, void Ani)
{// Attack interruption with range check
	void self 	= getlocalvar("self");
	void target = findtarget(self); //Get nearest player
	float x 	= getentityproperty(self, "x");
	float y 	= getentityproperty(self, "y");
	float z 	= getentityproperty(self, "z");
	int dir 	= getentityproperty(self, "direction");

	if(target!=NULL()){
		float Tx = getentityproperty(target, "x");
		float Ty = getentityproperty(target, "y");
		float Tz = getentityproperty(target, "z");
		float Disx = Tx - x;
		float Disy = Ty - y;
		float Disz = Tz - z;

		if(Disx >= RxMin && Disx <= RxMax && Disy >= RyMin && Disy <= RyMax && Disz >= RzMin && Disz <= RzMax && dir == 1) // Target within range on right facing?
		{
			executeanimation(self, openborconstant(Ani), 1); //Change the animation
		} else if(Disx >= -RxMax && Disx <= -RxMin && Disy >= RyMin && Disy <= RyMax && Disz >= -RzMax && Disz <= -RzMin && dir == 0) // Target within range on left facing?
		{
			executeanimation(self, openborconstant(Ani), 1); //Change the animation
		}
	}
}

