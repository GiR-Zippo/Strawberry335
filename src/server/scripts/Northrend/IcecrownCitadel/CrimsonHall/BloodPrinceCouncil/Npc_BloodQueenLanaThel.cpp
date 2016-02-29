#include "Npc_BloodQueenLanaThel.h"

void NpcBloodQueenLanaThelAI::Reset()
{
    _events.Reset();
    me->SetDisableGravity(true);
    if (_instance->GetBossState(DATA_BLOOD_PRINCE_COUNCIL) == DONE)
    {
        me->SetVisible(false);
        _introDone = true;
    }
    else
        me->SetVisible(true);
}

void NpcBloodQueenLanaThelAI::MoveInLineOfSight(Unit* who)
{
    if (_introDone)
        return;

    if (!me->IsWithinDistInMap(who, 35.0f, true))
        return;

    _introDone = true;
    Talk(SAY_INTRO_1);
    _events.SetPhase(1);
    _events.ScheduleEvent(EVENT_INTRO_1, 14000);
    // summon a visual trigger
    if (Creature* summon = DoSummon(NPC_FLOATING_TRIGGER, triggerPos, 15000, TEMPSUMMON_TIMED_DESPAWN))
    {
        summon->CastSpell(summon, SPELL_OOC_INVOCATION_VISUAL, true);
        summon->SetSpeed(MOVE_FLIGHT, 0.15f, true);
        summon->GetMotionMaster()->MovePoint(0, triggerEndPos);
    }
}

void NpcBloodQueenLanaThelAI::MovementInform(uint32 type, uint32 id)
{
    if (type == POINT_MOTION_TYPE && id == POINT_INTRO_DESPAWN)
        me->SetVisible(false);
}

void NpcBloodQueenLanaThelAI::UpdateAI(const uint32 diff)
{
    if (!_events.GetPhaseMask())
        return;

    _events.Update(diff);

    if (_events.ExecuteEvent() == EVENT_INTRO_1)
    {
        Talk(SAY_INTRO_2);
        me->GetMotionMaster()->MovePoint(POINT_INTRO_DESPAWN, introFinalPos);
        _events.Reset();

        // remove Feign Death from princes
        if (Creature* keleseth = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_PRINCE_KELESETH_GUID)))
            keleseth->AI()->DoAction(ACTION_STAND_UP);

        if (Creature* taldaram = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_PRINCE_TALDARAM_GUID)))
            taldaram->AI()->DoAction(ACTION_STAND_UP);

        if (Creature* valanar = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_PRINCE_VALANAR_GUID)))
        {
            valanar->AI()->DoAction(ACTION_STAND_UP);
            valanar->SetHealth(valanar->GetMaxHealth());
        }
    }
}