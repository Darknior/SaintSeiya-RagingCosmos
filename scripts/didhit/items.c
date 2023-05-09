#import "data/scripts/didhit/main.h"

void main()
{
	tintEffect(NULL(), "rainbow", 1.2, 1);
	showText();
}

void showText()
{
	void vSpawn;
	void self		= getlocalvar("self");
	void target		= getlocalvar("damagetaker");
	float x			= getentityproperty(target, "x");
	float y			= getentityproperty(target, "y");
	float z			= getentityproperty(target, "z");
	float xVel		= 0;
	float yVel		= getentityvar(self, "textVel");
	float zVel		= 0;
	float height	= 100;
	float duration	= getentityvar(self, "textTime");

	clearspawnentry();
	setspawnentry("name", "text");
	vSpawn = spawn();
	changeentityproperty(vSpawn, "position", x, z, y+height);
	changeentityproperty(vSpawn, "lifespancountdown", duration*200);
	tossentity(vSpawn, yVel, xVel, zVel);
	setentityvar(vSpawn, "text", getentityvar(self, "text"));
	setentityvar(vSpawn, "font", getentityvar(self, "font"));
}