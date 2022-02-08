void main()
{//Turn on/off some effects according to global variables, used to cursor blink effect at the select screen
	void self	= getlocalvar("self");
	void ani	= getentityproperty(self, "animationID");
	float time	= openborvariant("elapsed_time");
	
	//PLAYER CURSOR HIGHLIGHT
	if(	ani == openborconstant("ANI_FOLLOW1")|| //1P
		ani == openborconstant("ANI_FOLLOW2")|| //2P
		ani == openborconstant("ANI_FOLLOW3")|| //3P
		ani == openborconstant("ANI_FOLLOW4")){ //4P
		
		if(time%2 == 0){ //USED TO SAVE CPU POWER
			if(getlocalvar("drawmethod"+self) == NULL()){setlocalvar("drawmethod"+self, 1);}
			if(getlocalvar("color") == NULL()){setlocalvar("color", 0xFF);}
			
			void operation	= getlocalvar("operation");
			float color		= getlocalvar("color");
			float rate		= 4;
			int tintMode	= 3;
			int maxLimit	= 200;
			int minLimit	= 20;
			
			if(color != NULL()){
				changedrawmethod(self, "enabled", 1);
				changedrawmethod(self, "tintmode", tintMode);
				changedrawmethod(self, "tintcolor", rgbcolor(color,color,color));
				
				//CHANGE OPERATION WHEN REACHED THE MIN/MAX LIMITS
				if(color >= maxLimit){setlocalvar("operation", "decrease");}else
				if(color <= minLimit){setlocalvar("operation", "increase");}
				
				//INCREASE OR DECREASE COLOR VALUE
				if(operation == "increase"){setlocalvar("color", color+rate);}else
				if(operation == "decrease"){setlocalvar("color", color-rate);}
			}
		}
	}
	
	//PLAYER CURSOR SELECTED
	if(	ani == openborconstant("ANI_FOLLOW5")|| //1P
		ani == openborconstant("ANI_FOLLOW6")|| //2P
		ani == openborconstant("ANI_FOLLOW7")|| //3P
		ani == openborconstant("ANI_FOLLOW8")){ //4P
		
		if(getlocalvar("drawmethod"+self) != NULL()){
			changedrawmethod(self, "enabled", 0);
			setlocalvar("drawmethod"+self, NULL());
		}
	}
	
	//CPU PARTNER CURSOR HIGHLIGHT
	if(ani == openborconstant("ANI_FOLLOW17")){ //PARTNER
	
		if(time%2 == 0){ //USED TO SAVE CPU POWER
			if(getlocalvar("drawmethod"+self) == NULL()){setlocalvar("drawmethod"+self, 1);}
			if(getlocalvar("color") == NULL()){setlocalvar("color", 0xFF);}
			
			void operation	= getlocalvar("operation");
			float color		= getlocalvar("color");
			float rate		= 4;
			int tintMode	= 6;
			int maxLimit	= 200;
			int minLimit	= 20;
			
			if(color != NULL()){
				changedrawmethod(self, "enabled", 1);
				changedrawmethod(self, "tintmode", tintMode);
				changedrawmethod(self, "tintcolor", rgbcolor(color,color,color));
				
				//CHANGE OPERATION WHEN REACHED THE MIN/MAX LIMITS
				if(color >= maxLimit){setlocalvar("operation", "decrease");}else
				if(color <= minLimit){setlocalvar("operation", "increase");}
				
				//INCREASE OR DECREASE COLOR VALUE
				if(operation == "increase"){setlocalvar("color", color+rate);}else
				if(operation == "decrease"){setlocalvar("color", color-rate);}
			}
		}
	}
	
	//CPU PARTNER CURSOR SELECTED
	if(ani == openborconstant("ANI_FOLLOW18")){ //PARTNER
		if(getlocalvar("drawmethod"+self) != NULL()){
			changedrawmethod(self, "enabled", 0);
			setlocalvar("drawmethod"+self, NULL());
		}
	}
}