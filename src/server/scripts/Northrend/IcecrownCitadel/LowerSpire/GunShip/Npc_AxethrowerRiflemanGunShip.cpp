#include "Npc_AxethrowerRiflemanGunShip.h"

void NpcAxethrowerRiflemanGunshipAI::Reset()
{
    ScriptedAI::Reset();
    DesperateResolve = RAID_MODE( SPELL_DESPERATE_RESOLVE_10_NM, SPELL_DESPERATE_RESOLVE_25_NM, SPELL_DESPERATE_RESOLVE_10_HM, SPELL_DESPERATE_RESOLVE_25_HM);
    desperated = false;
    me->RemoveAurasDueToSpell(SPELL_EXPERIENCED);
    me->RemoveAurasDueToSpell(SPELL_ELITE);
    me->RemoveAurasDueToSpell(SPELL_VETERAN);
    events.ScheduleEvent(EVENT_EXPERIENCED, urand(19000, 21000)); // ~20 sec
    events.ScheduleEvent(EVENT_VETERAN, urand(39000, 41000)); // ~40 sec
    events.ScheduleEvent(EVENT_ELITE, urand(59000, 61000)); // ~60 sec
    attacktimer = urand(6000, 15000);
    if (me->GetEntry() == NPC_GB_SKYBREAKER_RIFLEMAN)
        SetEquipmentSlots(false, EQUIP_NO_CHANGE, EQUIP_NO_CHANGE, 47267);
    else
        SetEquipmentSlots(false, 49691, EQUIP_NO_CHANGE, EQUIP_NO_CHANGE);
}

void NpcAxethrowerRiflemanGunshipAI::DamageTaken(Unit* , uint32& )
{
    if (_instance->GetData(DATA_TEAM_IN_INSTANCE) == ALLIANCE && me->GetHealthPct() < 20.0f && !desperated)
    {
        desperated = true;
        DoCast(me, DesperateResolve);
    }
}

void NpcAxethrowerRiflemanGunshipAI::JustDied(Unit* killer)
{
    if (_instance->GetData(DATA_TEAM_IN_INSTANCE) == ALLIANCE)
    {
        if (Creature* pSaurfangBoss = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_GB_HIGH_OVERLORD_SAURFANG)))
            pSaurfangBoss->AI()->DoAction(ACTION_AXES_RIFL_DIE);
    }
    
    if (_instance->GetData(DATA_TEAM_IN_INSTANCE) == HORDE)
    {
        if (Creature* pMuradin = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_GB_MURADIN_BRONZEBEARD)))
            pMuradin->AI()->DoAction(ACTION_AXES_RIFL_DIE);
    }
}

void NpcAxethrowerRiflemanGunshipAI::UpdateAI(const uint32 diff)
{
    if (_instance->GetBossState(DATA_GUNSHIP_EVENT) != IN_PROGRESS)
        return;

    if (me->HasUnitState(UNIT_STATE_CASTING))
        return;

    //me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE); // Dodato
    me->AI()->AttackStart(SelectRandomPlayerInTheMaps(me->GetMap()));

    if (!UpdateVictim())
        return;

    events.Update(diff);

    if (attacktimer <= diff)
    {
        if (me->GetEntry() == NPC_GB_KORKRON_AXETHROWER)
            DoCast(me->getVictim(), SPELL_HURL_AXE);
        else if (me->GetEntry() == NPC_GB_SKYBREAKER_RIFLEMAN)
            DoCast(me->getVictim(), SPELL_SHOOT);
        attacktimer = urand(6000, 15000);
    } else attacktimer -= diff;

    while (uint32 eventId = events.ExecuteEvent())
    {
        switch (eventId)
        {
            case EVENT_EXPERIENCED:
                DoCast(me, SPELL_EXPERIENCED);
                break;
            case EVENT_VETERAN:
                me->RemoveAurasDueToSpell(SPELL_EXPERIENCED);
                DoCast(me, SPELL_VETERAN);
                break;
            case EVENT_ELITE:
                me->RemoveAurasDueToSpell(SPELL_VETERAN);
                DoCast(me, SPELL_ELITE);
                break;
            default:
                break;
        }
    }
}