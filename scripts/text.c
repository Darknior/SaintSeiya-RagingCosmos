void main()
{
	void self		= getlocalvar("self");
	void text		= getentityvar(self, "text");
	float x			= getentityproperty(self, "x");
	float y			= getentityproperty(self, "y");
	float z			= getentityproperty(self, "z");
	float xPos		= openborvariant("xpos");
	int font		= 8;

	if(text == NULL()){text = "";}
	drawstring(x-xPos-(strwidth(text, font)/2), z-y, font, text, z);
}