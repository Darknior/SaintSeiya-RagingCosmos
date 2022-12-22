#ifndef EFFECT_TINT_CON

    // These are core functionality constants. Unless
    // you know what you're doing, leave them alone.
    #define     EFFECT_TINT_CON 1
    #define     EFFECT_TINT_VAR_KEY "effect_tint"

    // The various settings go here so we can keep them
    // all in one place. Feel free to play around with them.
    #define     EFFECT_TINT_BURN           rgbcolor(255, 102, 0)    // Burn color.
    #define     EFFECT_TINT_BURN_MODE      1                        // Burn tint alpha mode

    #define     EFFECT_TINT_FREEZE         255                      // Freze color (maximum blue intensity).
    #define     EFFECT_TINT_FREEZE_MODE    1                        // Freeze tint alpha mode.

#endif

void effect_tint(void ent)
{
    // effect_tint
    // License - http://www.caskeys.com/dc/?p=5067
    // Caskey, Damon V.
    // 2015-05-04
    //
    // Apply color tint for hit effects.

    float time_percentage   = 0.0;  // Calculated time percentage.
    int time_effect         = 0;    // The normal set time for an effect to expire.
    float time_remaining    = 0.0;  // Actuall time in seconds before an effect will expire.
    float time_current      = 0.0;  // The current engine time.
    float time_initial      = 0.0;  // Time an effect was applied.
    int   map_default       = 0;    // Remap entity was spawned into play with.
    int   animation         = 0;    // Animation ID currently in use.

    int rgb_r = 0,  // RGB color settings.
        rgb_g = 0,
        rgb_b = 0;

    // Verify entity or exit.
    if(typeof(ent) != openborconstant("VT_PTR")) return;
    if(!getentityproperty(ent, "exists")) return;

    // First lets turn drawmethod on. If we don't, nothing below will
    // work at all.
    changedrawmethod(ent, "enabled", 1);

    // For time controlled effects, we need to know
    // current time and the time an effect was first
    // applied.
    time_current    += openborvariant("elapsed_time");
    time_initial    += getentityvar(ent, EFFECT_TINT_VAR_KEY + "_initial");

    // If entity is frozen, apply a freeze tint.
    if(getentityproperty(ent, "aiflag", "frozen") == 1)
    {
        // First we need to know how long the entity is meant to be frozen.
        time_effect = getentityproperty(ent, "freezetime");

        // Now let's see if we have a time when the freeze took place.
        // If not, this, entity was just frozen, so let's
        // record the time to an entity var.
        if(!time_initial)
        {
            time_initial = time_effect - time_current;
            setentityvar(ent, EFFECT_TINT_VAR_KEY + "_initial", time_initial);
        }

        // How much time is left before freeze effect is set to expire?
        time_remaining  = time_effect - time_current;

        // Now let's find out want to know what % of the original
        // freeze time is left. Then we'll multiply that by
        // maximum intensity of 255 and apply it to blue color setting.
        time_percentage = (time_remaining) / (time_initial);
        rgb_b = EFFECT_TINT_FREEZE * time_percentage;

        // Make green sam intensity as blue - this makes a much
        // stronger looking freeze effect than blue alone.
        rgb_g = rgb_b;

        // Unless fmap is set to -1, OpenBOR ALWAYS applies an fmap on frozen entities.
        // Since the default fmap is 0, that means entities without an fmap revert
        // back to their default palette. Obviously we don't we don't want that since
        // the whole point is to use dynamic tinting instead of a remap. To fix the
        // problem we'll get the map that entity was spawned into play with (ie. was
        // using before being set to an fmap), and force the engine to use it here.
        map_default = getentityproperty(ent, "mapdefault");
        changedrawmethod(ent, "remap", map_default);

        // Set transparency mode for tint, and apply blue intensity based
        // on % of initial freeze time is left.
        changedrawmethod(ent, "tintmode", EFFECT_TINT_FREEZE_MODE);
        changedrawmethod(ent, "tintcolor", rgbcolor(0, rgb_g, rgb_b));

        // No reason to do anything else, so exit the function.
        return;
    }


    // For most effects checking the animations will work, so
    // let's grab the current animation here.
    animation = getentityproperty(ent, "animationid");

    // In a burning animation? Then we'll apply burn tint.
    if(animation == openborconstant("ANI_BURN") || animation == openborconstant("ANI_BURNPAIN"))
    {
        // Set transparency mode for tint, and apply tint color.
        changedrawmethod(ent, "tintmode", EFFECT_TINT_BURN_MODE);
        changedrawmethod(ent, "tintcolor", EFFECT_TINT_BURN);

        // Nothing else to do, so exit the function.
        return;
    }

    ///////

    // If you want to add other tint effects, like shock, poison,
    // and so on, put them here. Use the burn example as a
    // template.

    ///////

    // If we got this far without exiting, there was no effect to apply.
    // Clear any previous tints and entity vars.
    changedrawmethod(ent, "tintmode", 0);
    changedrawmethod(ent, "tintcolor", 0);

    setentityvar(ent, EFFECT_TINT_VAR_KEY + "_initial", NULL());
}
