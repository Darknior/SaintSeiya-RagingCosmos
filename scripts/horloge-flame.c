void main()
{
	void self = getlocalvar("self");
	float time = openborvariant("game_time")/400;
	float adjust = 9;
	float flameSize;

	if(time > 0){
		flameSize = time+adjust;
	}
	else
	{
		flameSize = 0;
	}

	changedrawmethod(self, "enabled", 1);
	changedrawmethod(self, "scalex", flameSize);
	changedrawmethod(self, "scaley", flameSize);
}