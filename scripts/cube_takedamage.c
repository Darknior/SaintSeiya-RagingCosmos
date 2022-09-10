void main()
{
	void self = getlocalvar("self");
	void target = getlocalvar("attacker");
	int damage = getlocalvar("damage");
	float x = getentityproperty(self, "x");
	float Tx = getentityproperty(target, "x");
	int mHealth = getentityproperty(self, "maxhealth");
	int health = getentityproperty(self, "health");
	int damageR = getentityvar(self, "damageRight");
	int damageL = getentityvar(self, "damageLeft");

	//RIGHT DAMAGE
	if(Tx > x){
		if(damageR == NULL())
		{
			setentityvar(self, "damageRight", damage);
		}
		else
		{
			setentityvar(self, "damageRight", damageR+damage);
		}

		if(damageR >= mHealth/2){changeentityproperty(self, "health", health+damage);}
	}

	//LEFT DAMAGE
	if(Tx < x){
		if(damageL == NULL())
		{
			setentityvar(self, "damageLeft", damage);
		}
		else
		{
			setentityvar(self, "damageLeft", damageL+damage);
		}

		if(damageL >= mHealth/2){changeentityproperty(self, "health", health+damage);}
	}
}