void main()
{//Turn on/off some effects according to global variables, used to cursor blink effect at the select screen
	void self		= getlocalvar("self");
	void operation	= getglobalvar("operation"+self);
	float time		= openborvariant("elapsed_time");
	float channel	= getglobalvar("channel"+self);
	float rate		= getglobalvar("rate"+self);
	float minLimit	= 0;
	float maxLimit	= 254;
	int frame		= getentityproperty(self, "animpos");
	
	if(operation != NULL()){
		
		//INCREASE CHANNEL VALUE
		if(operation == "out"){
			setglobalvar("channel"+self, channel+rate);

			//KILL ENTITY WHEN REACHED THE MIN/MAX LIMITS
			if(channel >= maxLimit){killentity(self);}
		}

		//DECREASE CHANNEL VALUE
		if(operation == "in"){
			setglobalvar("channel"+self, channel-rate);

			//KILL ENTITY WHEN REACHED THE MIN/MAX LIMITS
			if(channel <= minLimit){killentity(self);}
		}

		//CHANGE DRAWMETHOD PROPERTIES
		changedrawmethod(self, "enabled", 1);
		changedrawmethod(self, "alpha", 6);
		changedrawmethod(self, "channelr", channel);
		changedrawmethod(self, "channelg", channel);
		changedrawmethod(self, "channelb", channel);
		if(operation == "out" && frame == 0){updateframe(self, 1);}
		//changedrawmethod(self, "fillcolor", 255); //USED FOR DEBUG ONLY
	}
}