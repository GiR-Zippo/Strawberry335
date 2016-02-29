#include "Npc_Darnavan.h"

/////////////////////////
// Darnavan Move Event //
/////////////////////////

bool DarnavanMoveEvent::Execute(uint64 /*time*/, uint32 /*diff*/)
{
    _darnavan.GetMotionMaster()->MovePoint(POINT_DESPAWN, SummonPositions[6]);
    return true;
}



/////////////////
// Darnavan AI //
/////////////////

void DarnavanAI::Reset()
{
    _events.Reset();
    _events.ScheduleEvent(EVENT_DARNAVAN_BLADESTORM, 10000);
    _events.ScheduleEvent(EVENT_DARNAVAN_INTIMIDATING_SHOUT, urand(20000, 25000));
    _events.ScheduleEvent(EVENT_DARNAVAN_MORTAL_STRIKE, urand(25000, 30000));
    _events.ScheduleEvent(EVENT_DARNAVAN_SUNDER_ARMOR, urand(5000, 8000));
    _canCharge = true;
    _canShatter = true;
}

void DarnavanAI::JustDied(Unit* killer)
{
    _events.Reset();
    if (Player* owner = killer->GetCharmerOrOwnerPlayerOrPlayerItself())
    {
        if (Group* group = owner->GetGroup())
        {
            for (GroupReference* itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
                if (Player* member = itr->getSource())
                    member->FailQuest(QUEST_DEPROGRAMMING);
        }
        else
            owner->FailQuest(QUEST_DEPROGRAMMING);
    }
}

void DarnavanAI::MovementInform(uint32 type, uint32 id)
{
    if (type != POINT_MOTION_TYPE || id != POINT_DESPAWN)
        return;

    me->DespawnOrUnsummon();
}

void DarnavanAI::EnterCombat(Unit* /*victim*/)
{
    Talk(SAY_DARNAVAN_AGGRO);
}

void DarnavanAI::UpdateAI(const uint32 diff)
{
    if (!UpdateVictim())
        return;

    _events.Update(diff);

    if (me->HasUnitState(UNIT_STATE_CASTING))
        return;

    if (_canShatter && me->getVictim() && me->getVictim()->IsImmunedToDamage(SPELL_SCHOOL_MASK_NORMAL))
    {
        DoCastVictim(SPELL_SHATTERING_THROW);
        _canShatter = false;
        _events.ScheduleEvent(EVENT_DARNAVAN_SHATTERING_THROW, 30000);
        return;
    }

    if (_canCharge && !me->IsWithinMeleeRange(me->getVictim()))
    {
        DoCastVictim(SPELL_CHARGE);
        _canCharge = false;
        _events.ScheduleEvent(EVENT_DARNAVAN_CHARGE, 20000);
        return;
    }

    while (uint32 eventId = _events.ExecuteEvent())
    {
        switch (eventId)
        {
        case EVENT_DARNAVAN_BLADESTORM:
            DoCast(SPELL_BLADESTORM);
            _events.ScheduleEvent(EVENT_DARNAVAN_BLADESTORM, urand(90000, 100000));
            break;
        case EVENT_DARNAVAN_CHARGE:
            _canCharge = true;
            break;
        case EVENT_DARNAVAN_INTIMIDATING_SHOUT:
            DoCast(SPELL_INTIMIDATING_SHOUT);
            _events.ScheduleEvent(EVENT_DARNAVAN_INTIMIDATING_SHOUT, urand(90000, 120000));
            break;
        case EVENT_DARNAVAN_MORTAL_STRIKE:
            DoCastVictim(MORTAL_STRIKE_HELPER);
            _events.ScheduleEvent(EVENT_DARNAVAN_MORTAL_STRIKE, urand(15000, 30000));
            break;
        case EVENT_DARNAVAN_SHATTERING_THROW:
            _canShatter = true;
            break;
        case EVENT_DARNAVAN_SUNDER_ARMOR:
            DoCastVictim(SPELL_SUNDER_ARMOR);
            _events.ScheduleEvent(EVENT_DARNAVAN_SUNDER_ARMOR, urand(3000, 7000));
            break;
        }
    }

    DoMeleeAttackIfReady();
}

