void main()
{
	adjustLayer();
}

void adjustLayer()
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

		if(getentityproperty(entity_cursor, "y") < -10){
			changeentityproperty(entity_cursor, "setlayer", -10);
		}
	}
}