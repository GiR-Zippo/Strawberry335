#include "Spell_Rotface.h"

/////////////////
// Slime Spray //
/////////////////

void SpellScriptRotfaceSlimeSpray::HandleScript()//SpellEffIndex /*effIndex*/)
{
    Creature* dummy = GetCaster()->FindNearestCreature(NPC_OOZE_SPRAY_STALKER, 80.0f, true);
    if (!dummy)
        return;
    float orient = GetCaster()->GetAngle(dummy->GetPositionX(), dummy->GetPositionY());

    GetCaster()->SetTarget(dummy->GetGUID());
    GetCaster()->SetFacingTo(orient);
    GetCaster()->UpdateOrientation(orient);
}

void SpellScriptRotfaceSlimeSpray::Register()
{
    BeforeCast += SpellCastFn(SpellScriptRotfaceSlimeSpray::HandleScript);
    OnCast     += SpellCastFn(SpellScriptRotfaceSlimeSpray::HandleScript);
    AfterCast  += SpellCastFn(SpellScriptRotfaceSlimeSpray::HandleScript);
}



//////////////////////
// Slime Spray Tick //
//////////////////////

void SpellScriptRotfaceSlimeSprayTick::HandleScript()
{
    Creature* dummy = GetCaster()->FindNearestCreature(NPC_OOZE_SPRAY_STALKER, 100.0f, true);
    if (!dummy)
        return;
    float orient = GetCaster()->GetAngle(dummy->GetPositionX(), dummy->GetPositionY());

    GetCaster()->SetTarget(dummy->GetGUID());
    GetCaster()->SetFacingTo(orient);
    GetCaster()->UpdateOrientation(orient);
}

void SpellScriptRotfaceSlimeSprayTick::Register()
{
    BeforeCast += SpellCastFn(SpellScriptRotfaceSlimeSprayTick::HandleScript);
    OnCast     += SpellCastFn(SpellScriptRotfaceSlimeSprayTick::HandleScript);
    AfterCast  += SpellCastFn(SpellScriptRotfaceSlimeSprayTick::HandleScript);
}
