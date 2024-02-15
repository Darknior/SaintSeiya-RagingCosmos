void main()
{
	if(openborvariant("current_branch") == "temple7"){temple7();}
}

void temple7()
{//Stop elevator movement according to the current animation
	void self		= getlocalvar("self");
	void anim		= getentityproperty(self, "animationID");
	int zPos		= getentityproperty(self, "z");
	int zStart		= getlocalvar("zStart"+self); //REGISTER THE INITIAL POSITION
	int zDif		= 300; //DEFAULT DIFFERENCE BETWEEN EACH FLOOR
	int frameStop	= 2; //DEFINED FRAME THAT THE ANIMATION WILL BE CHANGED WHEN THE ELEVATOR STOPS
	int zStop;

	//START THE "ZSTART" VARIABLE
	if(zStart == NULL()){setlocalvar("zStart"+self, zPos);}

	//DEFINE THE CURRENT "ZSTOP" ACCORDING TO THE CURRENT ANIMATION
	if(anim == openborconstant("ANI_FOLLOW1")){zStop = zStart+zDif;} //FLOOR 1
	if(anim == openborconstant("ANI_FOLLOW2")){zStop = zStart+zDif*2;} //FLOOR 2
	if(anim == openborconstant("ANI_FOLLOW3")){zStop = zStart+zDif*3;} //FLOOR 3
	if(anim == openborconstant("ANI_FOLLOW4")){zStop = zStart+zDif*4;} //FLOOR 4
	if(anim == openborconstant("ANI_FOLLOW5")){zStop = zStart+zDif*5;} //FLOOR 5

	//MOVING UP?? CHECK POSITION AND STOP AT DEFINED LIMIT
	if(zPos >= zStop){
		updateframe(self, frameStop);
		changelevelproperty("vbgspeed", 0);
		changeentityproperty(self, "z", zStop);
		changeentityproperty(self, "velocity", 0, 0, 0);
	}
}