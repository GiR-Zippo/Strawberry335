#include "Npc_SindragosaWard.h"

/*
class frostwinghall_vrykul_frostbinderAI : public ScriptedAI
{
public:
    frostwinghall_vrykul_frostbinderAI(Creature* creature) : ScriptedAI(creature)
    {
    }

    void Reset()
    {
        events.Reset();
        events.ScheduleEvent(EVENT_ARCTIC_CHILL, 1000);
        events.ScheduleEvent(EVENT_FROZEN_ORB, 1000);
    }

    bool CanAIAttack(Unit const* target) const
    {
        // do not see targets inside Frostwing Halls when we are not there
        return (me->GetPositionY() > 2660.0f) == (target->GetPositionY() > 2660.0f) && ScriptedAI::CanAIAttack(target);
    }

    void UpdateAI(const uint32 diff)
    {
        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            switch (eventId)
            {
            case EVENT_ARCTIC_CHILL:
                DoCastAOE(SPELL_ARCTIC_CHILL);
                break;
            case EVENT_FROZEN_ORB:
                DoCastAOE(SPELL_FROZEN_ORB);
                events.ScheduleEvent(EVENT_FROZEN_ORB, 10000);
                break;
            default:
                break;
            }
        }

        DoMeleeAttackIfReady();
    }

    void DoAction(const int32 action)
    {
        switch (action)
        {
        default:
            break;
        }
    }

private:
    EventMap events;
};

class frostwinghall_vrykul_frostbinder : public CreatureScript
{
public:
    frostwinghall_vrykul_frostbinder() : CreatureScript("frostwinghall_vrykul_frostbinder") {}

    CreatureAI* GetAI(Creature* creature) const
    {
        return new frostwinghall_vrykul_frostbinderAI(creature);
    };
};
*/
void LoadTrashFrostwingHall()
{
    new NpcSindragosaWard();
    new NpcSindragosaWardSpider();
    new NpcSindragosaWardFrostwarden();

    new AtStartSindragosaGauntlet();
}