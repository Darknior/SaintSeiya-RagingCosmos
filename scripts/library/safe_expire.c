#ifndef SAFE_EXPIRE_LOADED
#define SAFE_EXPIRE_LOADED 1
// Build 7533 kill_entity dereferences native self even from draw callbacks.
// Expire on the next native entity update, where that context is valid.
void expireEntity(void entity)
{
    if(typeof(entity)!=openborconstant("VT_PTR")) return;
    if(entity==NULL()) return;
    if(!getentityproperty(entity,"exists")) return;
    changeentityproperty(entity,"lifespancountdown",-1);
}
#endif
