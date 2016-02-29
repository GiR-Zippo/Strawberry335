#include "Npc_SpireFrostwyrm.h"

enum AreaTriggerFrostwyrm
{
    FROSTWYRM_TRIGGER_A = 5628,
    FROSTWYRM_TRIGGER_H = 5630
};

enum SpireFrostwyrmMovementPoints
{
    POINT_DOWN = 1,
    POINT_LAND = 2,
};

Position const FrostWyrmPosH   = {-435.429f, 2077.556f, 219.1148f, 4.767166f};
Position const FrostWyrmPosA   = {-437.409f, 2349.026f, 219.1148f, 1.483120f};
Position const FrostWyrmLandPosH   = {-437.250f, 2078.780f, 191.254f, 3.085365f};
Position const FrostWyrmLandPosA   = {-437.335f, 2344.152f, 191.2539f, 3.346516f};

void NpcSpireFrostwyrmAI::Reset()
{
    events.Reset();
    me->SetReactState(REACT_PASSIVE);
    me->SetCanFly(true);
    me->SetDisableGravity(true);
    me->SetByteFlag(UNIT_FIELD_BYTES_1, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND | UNIT_BYTE1_FLAG_HOVER);
    Position pos;
    if (instance->GetData(DATA_TEAM_IN_INSTANCE) == HORDE)
    {
        pos.Relocate(FrostWyrmLandPosH);
        pos.m_positionZ += 5.0f;
        Talk(SAY_FROSTWYRM_LAND_H_0);
    }
    else
    {
        pos.Relocate(FrostWyrmLandPosA);
        pos.m_positionZ += 5.0f;
        Talk(SAY_FROSTWYRM_LAND_A_1);
    }
    me->GetMotionMaster()->MovePoint(POINT_DOWN, pos);
    events.ScheduleEvent(EVENT_FROST_BREATH, 20000);
    events.ScheduleEvent(EVENT_BLIZZARD, 25000);
    events.ScheduleEvent(EVENT_CLEAVE, 10000);
}

void NpcSpireFrostwyrmAI::MovementInform(uint32 type, uint32 id)
{
    if (type != EFFECT_MOTION_TYPE && type != POINT_MOTION_TYPE)
        return;

    Position pos;
    switch (id)
    {
        case POINT_DOWN:
            if (me->GetMotionMaster()->GetCurrentMovementGeneratorType() == POINT_MOTION_TYPE)
                me->GetMotionMaster()->MovementExpired();
            if (instance->GetData(DATA_TEAM_IN_INSTANCE) == HORDE)
                pos.Relocate(FrostWyrmLandPosH);
            else
                pos.Relocate(FrostWyrmLandPosA);
            me->GetMotionMaster()->MoveLand(POINT_LAND, pos);
            break;
        case POINT_LAND:
            me->SetCanFly(false);
            me->SetDisableGravity(false);
            me->RemoveByteFlag(UNIT_FIELD_BYTES_1, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND | UNIT_BYTE1_FLAG_HOVER);
            if (instance->GetData(DATA_TEAM_IN_INSTANCE) == HORDE)
            {
                me->SetHomePosition(FrostWyrmLandPosH);
                me->SetFacingTo(FrostWyrmLandPosH.GetOrientation());
            }
            else
            {
                me->SetHomePosition(FrostWyrmLandPosA);
                me->SetFacingTo(FrostWyrmLandPosA.GetOrientation());
            }
            me->SetReactState(REACT_AGGRESSIVE);
            break;
        default:
            break;
    }
}

void NpcSpireFrostwyrmAI::JustDied(Unit* killer)
{
    killer->GetInstanceScript()->SetData(DATA_SPIRE_FROSTWYRM_STATE, DONE);
}

void NpcSpireFrostwyrmAI::UpdateAI(const uint32 diff)
{
    if (!UpdateVictim())
        return;
    
    if (me->HasUnitState(UNIT_STATE_CASTING))
        return;
    
    events.Update(diff);
    
    while (uint32 eventId = events.ExecuteEvent())
    {
        switch (eventId)
        {
            case EVENT_FROST_BREATH:
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 10.0f))
                    DoCast(target, SPELL_FROST_BREATH);
                events.ScheduleEvent(EVENT_FROST_BREATH, 20000);
                break;
            case EVENT_BLIZZARD:
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 10.0f))
                    DoCast(target, SPELL_BLIZZARD);
                events.ScheduleEvent(EVENT_BLIZZARD, 25000);
                break;
            case EVENT_CLEAVE:
                DoCast(me->getVictim(), SPELL_FROST_CLEAVE);
                events.ScheduleEvent(EVENT_CLEAVE, 10000);
                break;
        }
    }

    DoMeleeAttackIfReady();
}

bool AtSpireFrostwyrm::OnTrigger(Player* player, const AreaTriggerEntry* areaTrigger)
{
    if (InstanceScript* instance = player->GetInstanceScript())
    {
        if (instance->GetData(DATA_SPIRE_FROSTWYRM_STATE) == NOT_STARTED)
        {
            if (instance->GetData(DATA_TEAM_IN_INSTANCE) == ALLIANCE && areaTrigger->id == FROSTWYRM_TRIGGER_A)
                player->GetMap()->SummonCreature(NPC_SPIRE_FROSTWYRM, FrostWyrmPosA);
            else if (areaTrigger->id == FROSTWYRM_TRIGGER_H)
                player->GetMap()->SummonCreature(NPC_SPIRE_FROSTWYRM, FrostWyrmPosH);

            instance->SetData(DATA_SPIRE_FROSTWYRM_STATE, IN_PROGRESS);
        }
    }

    return true;
}