#include "Npc_RisenArchMage.h"

bool NpcRisenArchmageAI::CanAIAttack(const Unit* target) const
{
    return target->GetEntry() != NPC_VALITHRIA_DREAMWALKER;
}

void NpcRisenArchmageAI::Reset()
{
    _events.Reset();
    _events.ScheduleEvent(EVENT_FROSTBOLT_VOLLEY, urand(5000, 15000));
    _events.ScheduleEvent(EVENT_MANA_VOID, urand(20000, 25000));
    _events.ScheduleEvent(EVENT_COLUMN_OF_FROST, urand(10000, 20000));
    _canCallEnterCombat = true;
}

void NpcRisenArchmageAI::EnterCombat(Unit* )
{
    me->FinishSpell(CURRENT_CHANNELED_SPELL, false);
    if (me->GetDBTableGUIDLow() && _canCallEnterCombat)
    {
        std::list<Creature*> archmages;
        RisenArchmageCheck check;
        Trinity::CreatureListSearcher<RisenArchmageCheck> searcher(me, archmages, check);
        me->VisitNearbyGridObject(100.0f, searcher);
        for (std::list<Creature*>::iterator itr = archmages.begin(); itr != archmages.end(); ++itr)
            (*itr)->AI()->DoAction(ACTION_ENTER_COMBAT);

        if (Creature* lichKing = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_VALITHRIA_LICH_KING)))
            lichKing->AI()->DoZoneInCombat();

        if (Creature* trigger = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_VALITHRIA_TRIGGER)))
            trigger->AI()->DoZoneInCombat();
    }
}

void NpcRisenArchmageAI::DoAction(const int32 action)
{
    if (action != ACTION_ENTER_COMBAT)
        return;

    _canCallEnterCombat = false;
    DoZoneInCombat();
    _canCallEnterCombat = true;
}

void NpcRisenArchmageAI::JustSummoned(Creature* summon)
{
    if (summon->GetEntry() == NPC_COLUMN_OF_FROST)
        summon->m_Events.AddEvent(new DelayedCastEvent(summon, SPELL_COLUMN_OF_FROST_DAMAGE, 0, 8000), summon->m_Events.CalculateTime(2000));
    else if (summon->GetEntry() == NPC_MANA_VOID)
        summon->DespawnOrUnsummon(36000);
}

void NpcRisenArchmageAI::UpdateAI(const uint32 diff)
{
    if (!me->isInCombat())
        if (me->GetDBTableGUIDLow())
            if (!me->GetCurrentSpell(CURRENT_CHANNELED_SPELL))
                DoCast(me, SPELL_CORRUPTION);

    if (!UpdateVictim())
        return;

    _events.Update(diff);

    if (me->HasUnitState(UNIT_STATE_CASTING))
        return;

    while (uint32 eventId = _events.ExecuteEvent())
    {
        switch (eventId)
        {
            case EVENT_FROSTBOLT_VOLLEY:
                DoCast(me, SPELL_FROSTBOLT_VOLLEY);
                _events.ScheduleEvent(EVENT_FROSTBOLT_VOLLEY, urand(8000, 15000));
                break;
            case EVENT_MANA_VOID:
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, ManaVoidSelector(me)))
                    DoCast(target, SPELL_MANA_VOID);
                _events.ScheduleEvent(EVENT_MANA_VOID, urand(20000, 25000));
                break;
            case EVENT_COLUMN_OF_FROST:
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, -10.0f, true))
                    DoCast(target, SPELL_COLUMN_OF_FROST);
                _events.ScheduleEvent(EVENT_COLUMN_OF_FROST, urand(15000, 25000));
                break;
            default:
                break;
        }
    }

    DoMeleeAttackIfReady();
}