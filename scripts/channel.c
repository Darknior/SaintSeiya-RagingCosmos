void main()
{//Set gradual channel reduction effect in each spawned entity
	void self		= getlocalvar("self");
	void parent		= getentityproperty(self, "parent");
	void vAnim		= getentityproperty(self, "animationID");
	void pAnim		= getentityproperty(parent, "animationID");
	int alpha		= 6;
	int chStart		= 160;
	float channel	= getlocalvar("channel"+self);
	float reduce	= 3;

	if(getlocalvar("channel"+self) == NULL()){setlocalvar("channel"+self, chStart);}
	
	if(channel >= chStart){
		changedrawmethod(self, "enabled", 1);
		changedrawmethod(self, "alpha", alpha);
		changedrawmethod(self, "channelr", channel);
		changedrawmethod(self, "channelg", channel);
		changedrawmethod(self, "channelb", channel);
		setlocalvar("channel"+self, channel-reduce);
	}
	if(channel < chStart){
		changedrawmethod(self, "channelr", channel);
		changedrawmethod(self, "channelg", channel);
		changedrawmethod(self, "channelb", channel);
		setlocalvar("channel"+self, channel-reduce);
	}
	
	if(getlocalvar("channel"+self) <= 0){killentity(self);}

	//REMOVE ALL SHADOW TRAILS WHEN THE ANIMATION ENDS
	if(vAnim != pAnim){killentity(self);}
}