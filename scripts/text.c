void main()
{
	void self		= getlocalvar("self");
	void text		= getentityvar(self, "text");
	void font		= getentityvar(self, "font");
	float x			= getentityproperty(self, "x");
	float y			= getentityproperty(self, "y");
	float z			= getentityproperty(self, "z");
	float xPos		= openborvariant("xpos");

	if(text == NULL()){text = "";}
	if(font == NULL()){font = 8;}
	drawstring(x-xPos-(strwidth(text, font)/2), z-y, font, text, z);
}