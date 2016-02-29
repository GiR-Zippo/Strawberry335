/**
 * @file
 * @author TehKevin for Laenalith-WoW
 * @date 24.08.2014
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

#include "Boss_BlackKnight.h"
#include "Vehicle.h"


typedef Boss_BlackKnight::Boss_BlackKnightAI BlackKnightAI;

void BlackKnightAI::Reset()
{
    summons.DespawnAll();
    me->SetDisplayId(me->GetNativeDisplayId());

    SetEquipmentSlots(true);
    me->ClearUnitState(UNIT_STATE_ROOT | UNIT_STATE_STUNNED);

    eventMap.Reset();

    if (pInstance)
        pInstance->SetData(DATA_I_VE_HAD_WORSE, (uint32)true);
}

void BlackKnightAI::EnterCombat(Unit* /*attacker*/)
{
    Talk(SAY_AGGRO);

    me->setActive(true);
    DoZoneInCombat();

    eventMap.SetPhase(PHASE_UNDEAD);
    eventMap.ScheduleEvent(EVENT_SPELL_PLAGUE_STRIKE, urand(10 * IN_MILLISECONDS, 13 * IN_MILLISECONDS));
    eventMap.ScheduleEvent(EVENT_SPELL_ICY_TOUCH, urand(5 * IN_MILLISECONDS, 9 * IN_MILLISECONDS));
    eventMap.ScheduleEvent(EVENT_SPELL_DEATH_RESPITE, urand(15 * IN_MILLISECONDS, 16 * IN_MILLISECONDS));
    eventMap.ScheduleEvent(EVENT_SPELL_OBLITERATE, urand(17 * IN_MILLISECONDS, 19 * IN_MILLISECONDS));

    if (pInstance)
    {
        DoCast(pInstance->GetData(DATA_TEAM) == ALLIANCE ? SPELL_RAISE_ARELAS : SPELL_RAISE_JAEREN);

        if (pInstance->GetData(BOSS_BLACK_KNIGHT) != IN_PROGRESS)
            pInstance->SetData(BOSS_BLACK_KNIGHT, IN_PROGRESS);
    }
}

void BlackKnightAI::JustReachedHome()
{
    if (pInstance)
        if (pInstance->GetData(BOSS_BLACK_KNIGHT) == IN_PROGRESS)
            pInstance->SetData(BOSS_BLACK_KNIGHT, FAIL);
}

void BlackKnightAI::KilledUnit(Unit* target)
{
    if (target->GetTypeId() == TYPEID_PLAYER)
        Talk(SAY_KILL);
}

void BlackKnightAI::JustSummoned(Creature* pCreature)
{
    if (pCreature->GetEntry() == NPC_RISEN_JAEREN || pCreature->GetEntry() == NPC_RISEN_ARELAS || pCreature->GetEntry() == NPC_RISEN_CHAMPION)
    {
        summons.Summon(pCreature);
        pCreature->AI()->AttackStart(me->getVictim());
    }
}

void BlackKnightAI::SummonedCreatureDies(Creature* pCreature, Unit* /*killer*/)
{
    summons.Despawn(pCreature);
    pCreature->SetCorpseDelay(5 * IN_MILLISECONDS);
}

void BlackKnightAI::DamageTaken(Unit* /*pDoneBy*/, uint32& damage)
{
    if (damage > me->GetHealth())
    {
        if (eventMap.GetPhaseMask() != PHASE_MASK_GHOST)
        {
            damage = 0;
            me->SetHealth(0);
            me->AddUnitState(UNIT_STATE_ROOT | UNIT_STATE_STUNNED);
            me->GetMotionMaster()->MoveIdle();

            eventMap.AddPhase(PHASE_TRANSITION);
            eventMap.ScheduleEvent(EVENT_TRANSITION_RESURRECT, 4 * IN_MILLISECONDS);
        }

        explodeAliveGhouls();
    }
}

void BlackKnightAI::JustDied(Unit* /*killer*/)
{
    Talk(SAY_DEATH);

    if (pInstance)
        pInstance->SetData(BOSS_BLACK_KNIGHT, DONE);
}

void BlackKnightAI::UpdateAI(const uint32 diff)
{
    if (!UpdateVictim())
        return;

    eventMap.Update(diff);

    if (me->HasUnitState(UNIT_STATE_CASTING))
        return;

    if (uint32 eventID = eventMap.ExecuteEvent())
    {
        executeEvent(eventID);
        return;
    }

    if (!me->HasUnitState(UNIT_STATE_ROOT) && !me->HealthBelowPct(1))
        DoMeleeAttackIfReady();
}

void BlackKnightAI::explodeAliveGhouls()
{
    if (summons.empty())
        return;

    for (SummonList::iterator itr = summons.begin(); itr != summons.end(); ++itr)
        if (Creature* pCreature = me->GetCreature(*me, *itr))
            pCreature->CastSpell(pCreature, SPELL_EXPLODE);
}

void BlackKnightAI::executeEvent(uint32 eventID)
{
    if (eventMap.GetPhaseMask() & PHASE_MASK_TRANSITION)
    {
        if (eventID == EVENT_TRANSITION_RESURRECT)
        {
            me->SetFullHealth();
            me->RemoveAllAuras();

            DoCast(me, SPELL_BLACK_KNIGHT_RES, true);

            uint32 phaseMask = eventMap.GetPhaseMask();
            eventMap.Reset();

            switch (phaseMask)
            {
                case PHASE_MASK_FIRST_TRANSITION:
                {
                    eventMap.SetPhase(PHASE_SKELETON);
                    eventMap.ScheduleEvent(EVENT_SPELL_PLAGUE_STRIKE, urand(10 * IN_MILLISECONDS, 13 * IN_MILLISECONDS));
                    eventMap.ScheduleEvent(EVENT_SPELL_ICY_TOUCH, urand(5 * IN_MILLISECONDS, 9 * IN_MILLISECONDS));
                    eventMap.ScheduleEvent(EVENT_SPELL_DEATH_RESPITE, urand(14 * IN_MILLISECONDS, 15 * IN_MILLISECONDS));
                    eventMap.ScheduleEvent(EVENT_SPELL_DESECRATION, urand(15 * IN_MILLISECONDS, 16 * IN_MILLISECONDS));
                    eventMap.ScheduleEvent(EVENT_SPELL_ARMY_DEAD, 0.5 * IN_MILLISECONDS);

                    Talk(SAY_SKELETON);
                    me->SetDisplayId(MODEL_SKELETON);
                }break;

                case PHASE_MASK_SECOND_TRANSITION:
                {
                    eventMap.SetPhase(PHASE_GHOST);
                    eventMap.ScheduleEvent(EVENT_SPELL_DEATH_BITE, urand(2 * IN_MILLISECONDS, 4 * IN_MILLISECONDS));
                    eventMap.ScheduleEvent(EVENT_SPELL_MARKED_DEATH, urand(5 * IN_MILLISECONDS, 7 * IN_MILLISECONDS));
                    eventMap.ScheduleEvent(EVENT_SPELL_GHOUL_EXPLODE, 8 * IN_MILLISECONDS);

                    Talk(SAY_GHOST);
                    me->SetDisplayId(MODEL_GHOST);
                    SetEquipmentSlots(false, EQUIP_UNEQUIP);

                    me->ClearUnitState(UNIT_STATE_ROOT | UNIT_STATE_STUNNED);
                    me->GetMotionMaster()->MoveChase(me->getVictim());
                }break;
            }
        }
    }
    else
    {
        uint32 timer = 0;

        switch (eventID)
        {
            case EVENT_SPELL_PLAGUE_STRIKE:
            {
                DoCastVictim(SPELL_PLAGUE_STRIKE);
                timer = urand(12 * IN_MILLISECONDS, 15 * IN_MILLISECONDS);
            }break;

            case EVENT_SPELL_ICY_TOUCH:
            {
                DoCastVictim(SPELL_ICY_TOUCH);
                timer = urand(5 * IN_MILLISECONDS, 7 * IN_MILLISECONDS);
            }break;

            case EVENT_SPELL_DEATH_RESPITE:
            {
                if (Unit* pUnit = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                    DoCast(pUnit, IsHeroic() ? SPELL_DEATH_RESPITE_H : SPELL_DEATH_RESPITE); // No spell difficult entry

                timer = urand(15 * IN_MILLISECONDS, 16 * IN_MILLISECONDS);
            }break;

            case EVENT_SPELL_OBLITERATE:
            {
                DoCastVictim(SPELL_OBLITERATE);
                timer = urand(17 * IN_MILLISECONDS, 19 * IN_MILLISECONDS);
            }break;

            case EVENT_SPELL_ARMY_DEAD:
            {
                DoCast(me, SPELL_ARMY_DEAD);

                eventID = EVENT_RESTORE_FROM_ARMY_DEAD;
                timer = 1 * IN_MILLISECONDS;
            }break;

            case EVENT_RESTORE_FROM_ARMY_DEAD:
            {
                me->ClearUnitState(UNIT_STATE_ROOT | UNIT_STATE_STUNNED);
                me->GetMotionMaster()->MoveChase(me->getVictim());
                // No ReSchedule
            }break;

            case EVENT_SPELL_DESECRATION:
            {
                if (Unit* pUnit = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                    DoCast(pUnit, SPELL_DESECRATION);

                timer = urand(15 * IN_MILLISECONDS, 16 * IN_MILLISECONDS);
            }break;

            case EVENT_SPELL_GHOUL_EXPLODE:
            {
                if (!summons.empty())
                    DoCast(me, SPELL_GHOUL_EXPLODE);

                timer = 8 * IN_MILLISECONDS;
            }break;

            case EVENT_SPELL_DEATH_BITE:
            {
                DoCastAOE(SPELL_DEATH_BITE);
                timer = urand(1 * IN_MILLISECONDS, 2 * IN_MILLISECONDS);
            }break;

            case EVENT_SPELL_MARKED_DEATH:
            {
                if (Unit* pUnit = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                    DoCast(pUnit, SPELL_MARKED_DEATH);

                timer = urand(5 * IN_MILLISECONDS, 7 * IN_MILLISECONDS);
            }break;
        }

        if (eventID && timer)
            eventMap.ScheduleEvent(eventID, timer);
    }
}
