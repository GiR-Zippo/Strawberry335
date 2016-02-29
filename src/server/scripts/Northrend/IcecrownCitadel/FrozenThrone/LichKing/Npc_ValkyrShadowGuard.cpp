/**
 * @file
 * @author TehKevin for Laenalith-WoW
 * @date 10.08.2014
 *
 * Contact kevin.unegg@ainet.at
 * Website: http://www.laenalith-wow.com/
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @copyright Copyright 2014 Laenalith-WoW. All rights reserved.
 */

#include "Npc_ValkyrShadowGuard.h"

void NpcValkyrShadowguardAI::InitializeAI()
{
    me->SetReactState(REACT_PASSIVE);
    me->SetDisableGravity(true);

    me->SetSpeed(MOVE_WALK, 1.0f, true);
    me->SetSpeed(MOVE_RUN, 1.0f, true);
    me->SetSpeed(MOVE_FLIGHT, 1.0f, true);

    ScriptedAI::InitializeAI();
}

void NpcValkyrShadowguardAI::IsSummonedBy(Unit* )
{
    events.Reset();
    events.ScheduleEvent(EVENT_GRAB_PLAYER, 2500);
}

void NpcValkyrShadowguardAI::DamageTaken(Unit* , uint32& damage)
{
    if (!IsHeroic())
        return;

    if (!me->HasAuraType(SPELL_AURA_CONTROL_VEHICLE))
        return;

    if (me->HealthBelowPctDamaged(50, damage))
    {
        events.Reset();
        dropPlayer(false);

        me->SetSpeed(MOVE_WALK, 1.0f, true);
        me->SetSpeed(MOVE_RUN, 1.0f, true);
        me->SetSpeed(MOVE_FLIGHT, 1.0f, true);

        calcAndSetNewHomePosition();
        me->GetMotionMaster()->MoveTargetedHome();

        me->ClearUnitState(UNIT_STATE_EVADE);
    }
}

void NpcValkyrShadowguardAI::JustReachedHome()
{
    // schedule siphon life event (heroic only)
    if (grabbedPlayer)
    {
        me->SetDisplayId(VALKYR_REACHED_HOME_DISPLAYID);
        DoZoneInCombat();
        events.Reset();
        events.ScheduleEvent(EVENT_LIFE_SIPHON, 2000);
    }
}

void NpcValkyrShadowguardAI::AttackStart(Unit* )
{
}

void NpcValkyrShadowguardAI::MovementInform(uint32 type, uint32 id)
{
    if (type != POINT_MOTION_TYPE)
        return;

    switch (id)
    {
        case POINT_VALKYR:
        {
            events.ScheduleEvent(EVENT_VALKYR_NEXT_STEP, 1);
        }break;

        case POINT_CHARGE:
        {
            if (Player* target = ObjectAccessor::GetPlayer(*me, grabbedPlayer))
            {
                if (GameObject* platform = ObjectAccessor::GetGameObject(*me, instance->GetData64(DATA_ARTHAS_PLATFORM)))
                {
                    std::list<Creature*> triggers;
                    GetCreatureListWithEntryInGrid(triggers, me, NPC_WORLD_TRIGGER, 150.0f);
                    triggers.remove_if(HeightDifferenceCheck(platform, 5.0f, true));
                    if (triggers.empty())
                        return;

                    triggers.sort(Trinity::ObjectDistanceOrderPred(me));
                    DoCast(target, SPELL_VALKYR_CARRY);
                    dropPointTriggerGUID = triggers.front()->GetGUID();

                    calcPathToDropPoint();
                    events.ScheduleEvent(EVENT_MOVE_TO_DROP_POS, 1000);
                }
            }
            else
                me->DespawnOrUnsummon();
        }break;

        default:
            break;
    }
}

void NpcValkyrShadowguardAI::SpellHit(Unit* /*caster*/, SpellInfo const* spell)
{
    // Start the check movement event
    if (!events.IsEventActive(EVENT_CHECK_MOVEMENT) && me->HasAuraType(SPELL_AURA_CONTROL_VEHICLE) && !me->CanFreeMove())
        events.ScheduleEvent(EVENT_CHECK_MOVEMENT, 250);
}

void NpcValkyrShadowguardAI::SetGUID(uint64 guid, int32)
{
    grabbedPlayer = guid;
}

void NpcValkyrShadowguardAI::UpdateAI(const uint32 diff)
{
    events.Update(diff);

    if (me->HasUnitState(UNIT_STATE_CASTING))
        return;

    while (uint32 eventId = events.ExecuteEvent())
    {
        switch (eventId)
        {
            case EVENT_GRAB_PLAYER:
            {
                if (!grabbedPlayer)
                {
                    DoCastAOE(SPELL_VALKYR_TARGET_SEARCH);
                    events.ScheduleEvent(EVENT_GRAB_PLAYER, 1500);
                }
            }break;

            case EVENT_MOVE_TO_DROP_POS:
            {
                if (pathVector.empty())
                    dropPlayer(true);
                else
                {
                    // Start moving
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    me->SetSpeed(MOVE_WALK, 0.5f, true);
                    me->SetSpeed(MOVE_RUN, 0.5f, true);
                    me->SetSpeed(MOVE_FLIGHT, 0.5f, true);
                    DoCast(me, SPELL_WINGS_OF_THE_DAMNED, false);

                    events.ScheduleEvent(EVENT_VALKYR_NEXT_STEP, 1);
                }
            }break;

            case EVENT_LIFE_SIPHON:
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1))
                    DoCast(target, SPELL_LIFE_SIPHON);
                events.ScheduleEvent(EVENT_LIFE_SIPHON, 2500);
            }break;

            case EVENT_VALKYR_NEXT_STEP:
            {
                if (pathVector.empty())
                    dropPlayer(true);
                else
                {
                    pathVector.erase(pathVector.begin());

                    if (pathVector.empty())
                        dropPlayer(true);
                    else
                        me->GetMotionMaster()->MovePoint(POINT_VALKYR, *pathVector.begin());
                }
            }break;

            case EVENT_CHECK_MOVEMENT:
            {
                if (me->CanFreeMove())
                {
                    if (pathVector.empty())
                        dropPlayer(true);
                    else
                        events.ScheduleEvent(EVENT_VALKYR_NEXT_STEP, 0);
                }
                else if (me->HasAuraType(SPELL_AURA_CONTROL_VEHICLE))
                    events.ScheduleEvent(EVENT_CHECK_MOVEMENT, 250);
            }break;

            default:
                break;
        }
    }

    // no melee attacks
}

void NpcValkyrShadowguardAI::calcAndSetNewHomePosition()
{
    // Change homeposition to center +- CALC_X/CALC_Y
    Position newHome = me->GetHomePosition();
    newHome.m_positionX = CenterPosition.GetPositionX() + ((rand() % (VALKYR_HOME_POSITION_CALC * 2)) - VALKYR_HOME_POSITION_CALC);
    newHome.m_positionY = CenterPosition.GetPositionY() + ((rand() % (VALKYR_HOME_POSITION_CALC * 2)) - VALKYR_HOME_POSITION_CALC);
    me->SetHomePosition(newHome);
}

void NpcValkyrShadowguardAI::calcPathToDropPoint()
{
    pathVector.clear();

    Player* pPlayer = ObjectAccessor::GetPlayer(*me, grabbedPlayer);
    if (!pPlayer)
    {
        me->DespawnOrUnsummon();
        return;
    }

    Position currentPos, dropPos;
    pPlayer->GetPosition(&currentPos);
    float orientation = 0.0f;

    if (Creature* pCreature = sObjectAccessor->GetCreature(*me, dropPointTriggerGUID))
    {
        pCreature->GetPosition(&dropPos);
        pathSteps = float(me->GetDistance2d(pCreature));
        orientation = me->GetAngle(&dropPos);
    }
    else
    {
        me->DespawnOrUnsummon();
        return;
    }

    float posXdiff = ((dropPos.GetPositionX() - currentPos.GetPositionX()) / (float)pathSteps);
    float posYdiff = ((dropPos.GetPositionY() - currentPos.GetPositionY()) / (float)pathSteps);

    for (uint8 i = 1; i <= (pathSteps - 5); ++i) // Stop 5 points (yd) before trigger
    {
        Position pointPos;
        pointPos.m_positionX = ((posXdiff * i) + currentPos.m_positionX);
        pointPos.m_positionY = ((posYdiff * i) + currentPos.m_positionY);
        pointPos.m_positionZ = (currentPos.m_positionZ + 1.5f);
        pointPos.m_orientation = orientation;

        pathVector.push_back(pointPos);
    }
}

void NpcValkyrShadowguardAI::dropPlayer(bool despawn)
{
    DoCastAOE(SPELL_EJECT_ALL_PASSENGERS);

    if (despawn)
        me->DespawnOrUnsummon(1000);
}