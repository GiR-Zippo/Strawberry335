#include "Npc_MarineReaverGunShip.h"

void NpcMarineReaverGunshipAI::Reset()
{
    ScriptedAI::Reset();
    desperated = false;
    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
    me->SetReactState(REACT_PASSIVE);
    events.ScheduleEvent(EVENT_WALK_MOBS, 1500);
    me->RemoveAurasDueToSpell(SPELL_EXPERIENCED);
    me->RemoveAurasDueToSpell(SPELL_ELITE);
    DesperateResolve = RAID_MODE( SPELL_DESPERATE_RESOLVE_10_NM, SPELL_DESPERATE_RESOLVE_25_NM, SPELL_DESPERATE_RESOLVE_10_HM, SPELL_DESPERATE_RESOLVE_25_HM);
    events.ScheduleEvent(EVENT_EXPERIENCED, urand(19000, 21000)); // ~20 sec
    events.ScheduleEvent(EVENT_VETERAN, urand(39000, 41000)); // ~40 sec
    events.ScheduleEvent(EVENT_BURNING_PITCH, urand(60000, 62000));// ~61 sec
}

void NpcMarineReaverGunshipAI::DamageTaken(Unit* , uint32& )
{
    if (me->GetHealthPct() < 20.0f && !desperated)
    {
        desperated = true;
        DoCast(me, DesperateResolve);
    }
}

bool NpcMarineReaverGunshipAI::CanAIAttack(const Unit* target) const
{
    if (target->GetEntry() == NPC_GB_GUNSHIP_HULL)
        return false;

    return true;
}

void NpcMarineReaverGunshipAI::UpdateAI(const uint32 diff)
{
    if (_instance->GetBossState(DATA_GUNSHIP_EVENT) != IN_PROGRESS)
        return;
    
    if (me->HasUnitState(UNIT_STATE_CASTING))
        return;
    
    events.Update(diff);
    
    while (uint32 eventId = events.ExecuteEvent())
    {
        switch (eventId)
        {
            case EVENT_WALK_MOBS:
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                me->SetReactState(REACT_AGGRESSIVE);
                switch (me->GetEntry())
                {
                    case NPC_GB_KORKRON_REAVERS:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 0.0f, true, SPELL_ON_SKYBREAKERS_DECK))
                            me->AI()->AttackStart(target);
                        break;
                    case NPC_GB_SKYBREAKER_MARINE:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 0.0f, true, SPELL_ON_ORGRIMS_HAMMERS_DECK))
                            me->AI()->AttackStart(target);
                        break;
                }
                break;
                    case EVENT_BURNING_PITCH:
                        if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                        {
                            if (me->GetEntry() == NPC_GB_KORKRON_REAVERS)
                                DoCast(pTarget, SPELL_BURNING_PITCH_H);
                            else
                                DoCast(pTarget, SPELL_BURNING_PITCH_A);
                        }
                        events.ScheduleEvent(EVENT_BURNING_PITCH, urand(60000, 62000));// ~61 sec
                        break;
                    case EVENT_EXPERIENCED:
                        DoCast(me, SPELL_EXPERIENCED);
                        break;
                    case EVENT_VETERAN:
                        me->RemoveAurasDueToSpell(SPELL_EXPERIENCED);
                        DoCast(me, SPELL_VETERAN);
                        break;
        }
    }

    DoMeleeAttackIfReady();
}