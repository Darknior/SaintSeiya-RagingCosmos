// Compare handles against OpenBOR's current entity pool before dereferencing.
void liveEntity(void entity)
{
    if(typeof(entity)!=openborconstant("VT_PTR") || entity==NULL())return NULL();
    int count=openborvariant("ent_max");
    int i;
    for(i=0;i<count;i++){
        if(getentity(i)==entity && getentityproperty(entity,"exists"))return entity;
    }
    return NULL();
}
