#include "Npc_SkybreakerGunShip.h"

void NpcSkybreakerGunshipAI::Reset()
{
    ScriptedAI::Reset();
    me->SetReactState(REACT_PASSIVE);
    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
    me->SetDisplayId(11686);
}

void NpcSkybreakerGunshipAI::EnterCombat(Unit* )
{
    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
    SetCombatMovement(false);
    _instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
}

void NpcSkybreakerGunshipAI::JustDied(Unit* killer)
{
    if (Transport* t = me->GetTransport())
        DoShipExplosion(t);

    if (_instance->GetData(DATA_TEAM_IN_INSTANCE) == ALLIANCE)
    {
        if (Creature* pMuradin = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_GB_MURADIN_BRONZEBEARD)))
            pMuradin->AI()->DoAction(ACTION_FAIL);
    }

    else if (_instance->GetData(DATA_TEAM_IN_INSTANCE) == HORDE)
    {
        if (Creature* pSaurfang = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_GB_HIGH_OVERLORD_SAURFANG)))
            pSaurfang->AI()->DoAction(ACTION_DONE);
    }
}