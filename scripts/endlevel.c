void main()
{//Remove remaining characters after a reset timer event
	void player1	= getplayerproperty(0, "entity");
	void player2	= getplayerproperty(1, "entity");
	void player3	= getplayerproperty(2, "entity");
	void player4	= getplayerproperty(3, "entity");
	void lives1		= getplayerproperty(0, "lives");
	void lives2		= getplayerproperty(1, "lives");
	void lives3		= getplayerproperty(2, "lives");
	void lives4		= getplayerproperty(3, "lives");
	int exists1		= getentityproperty(player1, "exists");
	int exists2		= getentityproperty(player2, "exists");
	int exists3		= getentityproperty(player3, "exists");
	int exists4		= getentityproperty(player4, "exists");

	if(exists1 && lives1 <= 0){killentity(player1);}
	if(exists2 && lives2 <= 0){killentity(player2);}
	if(exists3 && lives3 <= 0){killentity(player3);}
	if(exists4 && lives4 <= 0){killentity(player4);}
}