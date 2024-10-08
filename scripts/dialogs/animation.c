void spawnFade(void operation, float fX, float fY, float fZ, int layer, float rate, int scalex, int scaley)
{//Spawn black image with alpha channel change to simulate a "Fade" effect
	void vSpawn;
	void self	= getlocalvar("self");
	float xPos	= openborvariant("xpos");
	int iDir	= 1;

	fX = xPos+fX;

	clearspawnentry();
	setspawnentry("name", "Fade");

	vSpawn = spawn();

	//SET CHANNEL PROPERTIES
	setglobalvar("operation"+vSpawn, operation);
	setglobalvar("rate"+vSpawn, rate);

	if(operation == "out"){
		setglobalvar("channel"+vSpawn, 0);
		changeentityproperty(vSpawn, "animpos", 0);
	}

	if(operation == "in"){
		setglobalvar("channel"+vSpawn, 254);
		changeentityproperty(vSpawn, "animpos", 1);
	}

	//SET ENTITY PROPERTIES
	changeentityproperty(vSpawn, "setlayer", layer);
	changeentityproperty(vSpawn, "position", fX, fZ, fY);
	changeentityproperty(vSpawn, "direction", iDir);
	if(scalex != NULL()){changedrawmethod(vSpawn, "enabled", 1);changedrawmethod(vSpawn, "scalex", scalex);}
	if(scaley != NULL()){changedrawmethod(vSpawn, "enabled", 1);changedrawmethod(vSpawn, "scaley", scaley);}
	
	return vSpawn;
}

void spawnDialog(void entity, float fX, float fY)
{//Spawn dialog entities
	void vSpawn;
	void self	= getlocalvar("self");
	float xPos	= openborvariant("xpos");
	int iDir	= 1;

	fX = xPos+fX;

	clearspawnentry();
	setspawnentry("name", entity);

	vSpawn = spawn();

	changeentityproperty(vSpawn, "position", fX, fY, 0);
	changeentityproperty(vSpawn, "direction", iDir);
	
	return vSpawn;
}

void spawnScreen(void entity, float fX, float fY, float fZ, int dir, void anim)
{//Spawn entity at specific location relative to xPos and yPos
	void vSpawn;
	void self	= getlocalvar("self");
	float z		= getentityproperty(self, "z");
	float xPos	= openborvariant("xpos");

	fX = xPos+fX;
	fZ = z+fZ;

	clearspawnentry();
	setspawnentry("name", entity);

	vSpawn = spawn();
	
	changeentityproperty(vSpawn, "position", fX, fZ, fY);
	changeentityproperty(vSpawn, "direction", dir);
	changeentityproperty(vSpawn, "parent", self);
	changeentityproperty(vSpawn, "animation", openborconstant(anim));
	setentityvar(self, entity, vSpawn);

	return vSpawn;
}

void setVar(void variable, float value)
{//Set generic global variable for further use
	
	setglobalvar(variable, value);
}

void removEntity(void entity)
{//Set generic global variable for further use
	void self = getlocalvar("self");
	void victim = getentityvar(self, entity);

	killentity(victim);
}

void suicide()
{//Set generic global variable for further use
	
	killentity(getlocalvar("self"));
}