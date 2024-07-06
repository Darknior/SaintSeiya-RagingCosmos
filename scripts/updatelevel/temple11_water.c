void main()
{
	if(!(openborvariant("game_paused"))){
		float time		= openborvariant("elapsed_time");
		float xSize		= 5000;
		float xAdd		= 4520;
		float xAdjust	= 1.65;
		float realXpos	= openborvariant("xpos");
		float xPos		= -(realXpos+xSize-xAdd);
		float ySize		= 20;
		float zPos		= 90;
		float zLayer	= 200;
		float layerAdd	= -100;
		float zAdd		= 10;
		int alpha		= 2;
		int red			= 0;
		int green		= 240;
		int blue		= 200;
		int color		= rgbcolor(red, green, blue);

		drawbox(xPos, zPos, xSize, ySize, zLayer, color, alpha);zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zLayer, color, alpha);zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zLayer, color, alpha);zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zLayer, color, alpha);zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zLayer, color, alpha);zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zLayer, color, alpha);zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zLayer, color, alpha);zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zLayer, color, alpha);zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zLayer, color, alpha);zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zLayer, color, alpha);zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zLayer, color, alpha);zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zLayer, color, alpha);zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zLayer, color, alpha);zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zLayer, color, alpha);zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zLayer, color, alpha);zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zLayer, color, alpha);zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zLayer, color, alpha);zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zLayer, color, alpha);zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zLayer, color, alpha);zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zLayer, color, alpha);zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zLayer, color, alpha);zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zLayer, color, alpha);zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zLayer, color, alpha);zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zLayer, color, alpha);zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zLayer, color, alpha);zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zLayer, color, alpha);zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zLayer, color, alpha);zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zLayer, color, alpha);zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zLayer, color, alpha);zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zLayer, color, alpha);zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zLayer, color, alpha);zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zLayer, color, alpha);zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zLayer, color, alpha);zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zLayer, color, alpha);zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zLayer, color, alpha);zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zLayer, color, alpha);zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zLayer, color, alpha);zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zLayer, color, alpha);zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zLayer, color, alpha);zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zLayer, color, alpha);zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zLayer, color, alpha);zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zLayer, color, alpha);zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zLayer, color, alpha);zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zLayer, color, alpha);zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zLayer, color, alpha);zPos = zPos+zAdd;
		
		xSize		= 250;
		xAdd		= 250;
		xPos		= -(realXpos+xSize-xAdd);
		zPos		= 320;
		zAdd		= 5;

		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;

		if(realXpos > 0){realXpos = realXpos-1;}
		xSize		= 5000;
		xAdd		= 5250;
		xPos		= -(realXpos+xSize-xAdd);
		ySize		= 20;
		zPos		= 320;
		zAdd		= 5;
		layerAdd	= 30;

		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;
		drawbox(xPos, zPos, xSize, ySize, zPos+layerAdd, color, alpha); zPos = zPos+zAdd;
	}
	waterSplash();
}

void waterSplash()
{
	void self			= getlocalvar("self");
	void entity_cursor	= NULL();
	int entity_count	= openborvariant("count_entities");
	int entity_index	= 0;
	int exists			= 0;

	for(entity_index = 0; entity_index < entity_count; entity_index++){

		entity_cursor = getentity(entity_index);

		if(!entity_cursor){
			continue;
		}

		exists = getentityproperty(entity_cursor, "exists");

		if(!exists){
			continue;
		}

		/* Take your actions here. Sucking in, doing damage, etc. */

		void type = getentityproperty(entity_cursor, "type");

		if(type == openborconstant("TYPE_PLAYER") || type == openborconstant("TYPE_ENEMY")){
			void anim	= getentityproperty(entity_cursor, "animationID");
			int height	= getentityproperty(entity_cursor, "y");
			int limit	= 10; //BELOW THIS HEIGHT LIMIT ALLOW TO SPAWN SPLASHES
			
			if(height <= limit){
				float xDir	= getentityproperty(entity_cursor, "xdir");
				float zDir	= getentityproperty(entity_cursor, "zdir");
				float yDir	= getentityproperty(entity_cursor, "tossv");
				float time	= openborvariant("elapsed_time");
				int delay	= 50; //SPLASH SPAWN RATE

				//SPAWN SPLASH WHEN MOVING THE CHARACTER
				if((xDir || zDir) && !yDir){
					if(getentityvar(entity_cursor, "timeSplash") == NULL()){
						setentityvar(entity_cursor, "timeSplash", time);
					}
					if(time > getentityvar(entity_cursor, "timeSplash")){
						spawnWeakSplash(entity_cursor);
						setentityvar(entity_cursor, "timeSplash", time+delay);
					}
				}else

				//SPAWN STRONGER SPLASH DURING JUMPDELAY/JUMPLAND
				if(anim == openborconstant("ANI_JUMPDELAY") || anim == openborconstant("ANI_JUMPLAND")){
					if(getentityvar(entity_cursor, "timeSplash") == NULL()){
						setentityvar(entity_cursor, "timeSplash", time);
					}
					if(time > getentityvar(entity_cursor, "timeSplash")){
						spawnStrongSplash(entity_cursor);
						setentityvar(entity_cursor, "timeSplash", time+delay);
					}
				}else

				//ANY OTHER SITUATION, CLEAR VARIABLES
				{
					setentityvar(entity_cursor, "timeSplash", NULL());
				}
			}
		}
	}
}

void spawnWeakSplash(void entity)
{
	void vSpawn;
	int dir	= getentityproperty(entity, "direction");
	float x	= getentityproperty(entity, "x");
	float y	= 5;
	float z	= getentityproperty(entity, "z");

	clearspawnentry();
	setspawnentry("name", "splash");
	vSpawn = spawn();
	changeentityproperty(vSpawn, "direction", dir);
	changeentityproperty(vSpawn, "setlayer", -1);
	changeentityproperty(vSpawn, "position", x, z-1, y);

	clearspawnentry();
	setspawnentry("name", "splash");
	vSpawn = spawn();
	changeentityproperty(vSpawn, "animation", openborconstant("ANI_FOLLOW1"));
	changeentityproperty(vSpawn, "direction", dir);
	changeentityproperty(vSpawn, "position", x, z+1, y);
}

void spawnStrongSplash(void entity)
{
	void vSpawn;
	int dir	= getentityproperty(entity, "direction");
	float x	= getentityproperty(entity, "x");
	float y	= 5;
	float z	= getentityproperty(entity, "z");

	clearspawnentry();
	setspawnentry("name", "splash");
	vSpawn = spawn();
	changeentityproperty(vSpawn, "animation", openborconstant("ANI_FOLLOW2"));
	changeentityproperty(vSpawn, "direction", dir);
	changeentityproperty(vSpawn, "setlayer", -1);
	changeentityproperty(vSpawn, "position", x, z-1, y);

	clearspawnentry();
	setspawnentry("name", "splash");
	vSpawn = spawn();
	changeentityproperty(vSpawn, "animation", openborconstant("ANI_FOLLOW3"));
	changeentityproperty(vSpawn, "direction", dir);
	changeentityproperty(vSpawn, "position", x, z+1, y);

	clearspawnentry();
	setspawnentry("name", "water");
	vSpawn = spawn();
	changeentityproperty(vSpawn, "direction", dir);
	changeentityproperty(vSpawn, "position", x, z+1, y);
}