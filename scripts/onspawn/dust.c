#import "data/scripts/updatelevel/temple11_water.c"

void main()
{
	void self	= getlocalvar("self");
	void branch	= openborvariant("current_branch");
	void name	= getentityproperty(self, "defaultname");
	
	//WORKS ONLY IN THE BRANCHES BELOW
	if(branch == "temple11"){

		//THESE ENTITIES WILL BE ERASED COMPLETELY
		if(	name == "dash_dust"		||
			name == "dash_dustB"	||
			name == "kick_dust"		||
			name == "jump_dust"		||
			name == "round_dust"	){
			killentity(self);
		}

		//THESE ENTITIES WILL BE ERASED BUT REPLACED WITH SPLASH ENTITY
		if(	name == "round_dustB"	||
			name == "fall_dust"		){
			killentity(self);
			spawnStrongSplash(self);
		}
	}
}