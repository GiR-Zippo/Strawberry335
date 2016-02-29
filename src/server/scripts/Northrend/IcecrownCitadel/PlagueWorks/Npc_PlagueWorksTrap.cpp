#include "Npc_PlagueWorksTrap.h"

/////////////////////////
// PlagueWorks Trap AI //
/////////////////////////

void PlagueWorksTrapAI::Reset()
{
    events.Reset();
    running = false;
}

void PlagueWorksTrapAI::JustDied(Unit* /*killer*/)
{
    events.Reset();
}

void PlagueWorksTrapAI::DoAction(const int32 action)
{
    switch (action)
    {
    case ACTION_START_EVENT:
        if (running)
            return;
        if (!instance)
            return;

        running=true;
        instance->HandleGameObject(instance->GetData64(DATA_PUTRICIDE_GATE_1), false);
        instance->HandleGameObject(instance->GetData64(DATA_PUTRICIDE_GATE_2), false);
        instance->HandleGameObject(instance->GetData64(DATA_PUTRICIDE_GATE_3), false);
        instance->HandleGameObject(instance->GetData64(DATA_PUTRICIDE_ENTRANCE), false);
        events.Reset();
        events.ScheduleEvent(EVENT_SPAWN_ADD_WAVE, 5000);
        events.ScheduleEvent(EVENT_GIANT_INSECT_SWARM, 10000);
        events.ScheduleEvent(EVENT_STOP_EVENT, 90000);
        if (Creature* professor = Unit::GetCreature(*me, instance->GetData64(DATA_PROFESSOR_PUTRICIDE)))
            if (professor->AI())
                professor->AI()->Talk(SAY_AIRLOCK);
        break;
    default:
        break;
    }
}

void PlagueWorksTrapAI::UpdateAI(const uint32 diff)
{
    events.Update(diff);

    while (uint32 eventId = events.ExecuteEvent())
    {
        switch (eventId)
        {
        case EVENT_SPAWN_ADD_WAVE: // 150 adds in 90 sec ~4/2,5sec
            for (uint8 i = 0; i < 4; ++i)
            {
                me->SummonCreature(NPC_FLESH_EATING_INSECTS, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 0, TEMPSUMMON_DEAD_DESPAWN, 0);
            }
            events.ScheduleEvent(EVENT_SPAWN_ADD_WAVE, 2500);
            break;
        case EVENT_GIANT_INSECT_SWARM:
            DoCast(SPELL_GIANT_INSECT_SWARM);
            break;
        case EVENT_STOP_EVENT:
            events.Reset();
            me->RemoveAurasDueToSpell(SPELL_GIANT_INSECT_SWARM);
            // Open doors
            if (GameObject* go = GameObject::GetGameObject(*me, instance->GetData64(DATA_PUTRICIDE_GATE_1)))
                go->SetGoState(GO_STATE_ACTIVE_ALTERNATIVE);
            if (GameObject* go = GameObject::GetGameObject(*me, instance->GetData64(DATA_PUTRICIDE_GATE_2)))
                go->SetGoState(GO_STATE_ACTIVE_ALTERNATIVE);
            instance->HandleGameObject(instance->GetData64(DATA_PUTRICIDE_GATE_3), true);
            instance->HandleGameObject(instance->GetData64(DATA_PUTRICIDE_ENTRANCE), true);
            me->DespawnOrUnsummon();
            break;
        default:
            break;
        }
    }
}
