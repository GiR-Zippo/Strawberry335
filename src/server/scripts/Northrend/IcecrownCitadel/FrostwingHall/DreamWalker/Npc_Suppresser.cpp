#include "Npc_Suppresser.h"

void NpcSuppresserAI::Reset()
{
    _events.Reset();
    _events.ScheduleEvent(EVENT_SUPPRESSION, urand(10000, 15000));
    me->SetReactState(REACT_PASSIVE);
}

void NpcSuppresserAI::IsSummonedBy(Unit* )
{
    if (Creature* valithria = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_VALITHRIA_DREAMWALKER)))
        AttackStart(valithria);
}

void NpcSuppresserAI::UpdateAI(const uint32 diff)
{
    if (!UpdateVictim())
        return;

    _events.Update(diff);

    if (me->HasUnitState(UNIT_STATE_CASTING))
        return;
    
    // this code will never be reached while channeling
    while (uint32 eventId = _events.ExecuteEvent())
    {
        switch (eventId)
        {
            case EVENT_SUPPRESSION:
                if (me->IsWithinMeleeRange(ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_VALITHRIA_DREAMWALKER))))
                    DoCastAOE(SPELL_SUPPRESSION);
                _events.ScheduleEvent(EVENT_SUPPRESSION, 3000);
                break;
            default:
                break;
        }
    }

    // this creature has REACT_PASSIVE so it does not always have victim here
    if (Unit* victim = me->getVictim())
        if (victim->GetEntry() != NPC_VALITHRIA_DREAMWALKER)
            DoMeleeAttackIfReady();
}