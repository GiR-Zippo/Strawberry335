#include "Npc_RagingSpirit.h"

void NpcRagingSpiritAI::Reset()
{
    me->SetInCombatWithZone();
    me->SetReactState(REACT_PASSIVE);
    me->AttackStop();
    me->GetMotionMaster()->MovementExpired(true);

    events.Reset();
    events.ScheduleEvent(EVENT_SOUL_SHRIEK, urand(12000, 15000));

    DoCast(me, SPELL_PLAGUE_AVOIDANCE, true);
    DoCast(me, SPELL_RAGING_SPIRIT_VISUAL, true);

    if (TempSummon* summon = me->ToTempSummon())
        if (Unit* summoner = summon->GetSummoner())
            summoner->CastSpell(me, SPELL_RAGING_SPIRIT_VISUAL_CLONE, true);

    DoCast(me, SPELL_BOSS_HITTIN_YA, true);
    me->m_Events.AddEvent(new SetAggressiveEvent(me), me->m_Events.CalculateTime(2500));}

void NpcRagingSpiritAI::EnterCombat(Unit* )
{
    me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, false);
    me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_ATTACK_ME, false);
}

void NpcRagingSpiritAI::IsSummonedBy(Unit* )
{
    // player is the spellcaster so register summon manually
    if (Creature* lichKing = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_THE_LICH_KING)))
        lichKing->AI()->JustSummoned(me);
}

void NpcRagingSpiritAI::JustDied(Unit* )
{
    if (Creature* lichKing = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_THE_LICH_KING)))
        lichKing->AI()->SummonedCreatureDespawn(me);
    if (TempSummon* summon = me->ToTempSummon())
        summon->SetTempSummonType(TEMPSUMMON_CORPSE_DESPAWN);
}

void NpcRagingSpiritAI::DoAction(const int32 action)
{
    switch (action)
    {
        case ACTION_RAGINGSPIRIT_FROSTMOURNE:
        {
            if (events.IsInPhase(RAGINGSPIRIT_PHASE_FROSTMOURNE))
            {
                events.RemovePhase(RAGINGSPIRIT_PHASE_FROSTMOURNE);
                me->SetReactState(REACT_AGGRESSIVE);
            }
            else
            {
                me->SetReactState(REACT_PASSIVE);
                me->AttackStop();
                events.SetPhase(RAGINGSPIRIT_PHASE_FROSTMOURNE);
            }
        }break;

        default:
            return;
    }
}

void NpcRagingSpiritAI::UpdateAI(const uint32 diff)
{
    if (!UpdateVictim())
        return;

    if (events.IsInPhase(RAGINGSPIRIT_PHASE_FROSTMOURNE))
        return;

    events.Update(diff);

    if (me->HasUnitState(UNIT_STATE_CASTING))
        return;

    while (uint32 eventId = events.ExecuteEvent())
    {
        switch (eventId)
        {
            case EVENT_SOUL_SHRIEK:
                DoCastAOE(SPELL_SOUL_SHRIEK);
                events.ScheduleEvent(EVENT_SOUL_SHRIEK, urand(12000, 15000));
                break;

            default:
                break;
        }
    }

    DoMeleeAttackIfReady();
}