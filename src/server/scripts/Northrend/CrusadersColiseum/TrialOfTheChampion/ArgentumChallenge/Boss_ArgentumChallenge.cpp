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

#include "Boss_ArgentumChallenge.h"
#include "Data_ArgentumChallenge.h"


typedef Boss_Eadric::Boss_EadricAI          EadricAI;
typedef Boss_Paletress::Boss_PaletressAI    PaletressAI;

void EadricAI::Reset()
{
    defeated = false;
    eventMap.Reset();

    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);
    me->RemoveUnitMovementFlag(MOVEMENTFLAG_WALKING);
    me->SetReactState(REACT_AGGRESSIVE);
}

void EadricAI::SpellHit(Unit* /*caster*/, const SpellInfo *spell)
{
    // Faceroller achievement
    if (IsHeroic())
        if (spell->Id == SPELL_HAMMER_RIGHTEOUS_RET)
            if (me->GetHealth() <= (uint32)spell->Effects[0].BasePoints)
                if (pInstance)
                    pInstance->DoUpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, SPELL_EADRIC_ACHIEVEMENT);
}

void EadricAI::EnterCombat(Unit* /*attacker*/)
{
    Talk(SAY_COMMON_AGGRO);

    me->setActive(true);
    DoZoneInCombat();

    eventMap.ScheduleEvent(EVENT_SPELL_VENGANCE, 10 * IN_MILLISECONDS);
    eventMap.ScheduleEvent(EVENT_SPELL_VENGANCE, 16 * IN_MILLISECONDS);
    eventMap.ScheduleEvent(EVENT_SPELL_VENGANCE, 25 * IN_MILLISECONDS);

    if (pInstance)
        if (pInstance->GetData(BOSS_ARGENT_CHALLENGE) != IN_PROGRESS)
            pInstance->SetData(BOSS_ARGENT_CHALLENGE, IN_PROGRESS);
}

void EadricAI::JustReachedHome()
{
    if (defeated)
    {
        me->DespawnOrUnsummon();
        return;
    }

    if (pInstance)
        if (pInstance->GetData(BOSS_ARGENT_CHALLENGE) == IN_PROGRESS)
            pInstance->SetData(BOSS_ARGENT_CHALLENGE, FAIL);
}

void EadricAI::KilledUnit(Unit* /*target*/)
{
    Talk(SAY_COMMON_KILL);
}

void EadricAI::DamageTaken(Unit* /*attacker*/, uint32& damage)
{
    if (defeated)
    {
        damage = 0;
        return;
    }

    // Prevent damage from finishing hit and mark creature as defeated
    if (damage >= me->GetHealth())
    {
        damage = 0;
        defeated = true;

        me->InterruptNonMeleeSpells(true);

        me->SetTarget(0);
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        me->SetReactState(REACT_PASSIVE);
        me->GetMotionMaster()->MoveTargetedHome();

        Talk(SAY_COMMON_DEATH);

        if (pInstance)
        {
            // Set encounter done
            pInstance->SetData(BOSS_ARGENT_CHALLENGE, DONE);

            // Inform instance that encounter done
            pInstance->DoAction(INSTANCE_ACTION_ENCOUNTER_DONE);
        }
    }
}

void EadricAI::MovementInform(uint32 type, uint32 id)
{
    if (type != POINT_MOTION_TYPE)
        return;

    switch (id)
    {
        case 1: // After adds has been defeated
        {
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);
            me->SetReactState(REACT_AGGRESSIVE);
            me->setFaction(16);
        }break;

        case ENCOUNTER_WP_DESPAWN_POSITION: // After boss has finished and went to door
        {
            me->DespawnOrUnsummon();
        }break;

    }
}

void EadricAI::UpdateAI(const uint32 diff)
{
    if (!UpdateVictim() || defeated)
        return;

    eventMap.Update(diff);

    if (me->HasUnitState(UNIT_STATE_CASTING))
        return;

    if (uint32 eventID = eventMap.ExecuteEvent())
    {
        executeEvent(eventID);
        return;
    }

    //DoMeleeAttackIfReady();
    if (me->isAttackReady() && me->IsWithinMeleeRange(me->getVictim()))
    {
        me->AttackerStateUpdate(me->getVictim());
        me->ResetAttackTimer();

        if (Aura* pAura = me->GetAura(SPELL_VENGANCE_AURA))
            if (pAura->GetDuration() < 4900)
                me->RemoveAurasDueToSpell(SPELL_VENGANCE_AURA);
    }
}

void EadricAI::executeEvent(uint32 eventID)
{
    uint32 timer = 0;

    switch (eventID)
    {
        case EVENT_SPELL_VENGANCE:
        {
            DoCast(me, SPELL_VENGEANCE);
            timer = 10 * IN_MILLISECONDS;
        }break;

        case EVENT_SPELL_RADIANCE:
        {
            Talk(SAY_EADRIC_RADIANCE_WARNING);
            DoCastAOE(SPELL_RADIANCE);
            timer = 16 * IN_MILLISECONDS;
        }break;

        case EVENT_SPELL_HAMMER_OF_JUSTICE:
        {
            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 250, true))
            {
                if (target && target->isAlive())
                {
                    me->InterruptNonMeleeSpells(true);

                    Talk(SAY_EADRIC_HAMMER);
                    DoCast(target, SPELL_HAMMER_JUSTICE);
                    DoScriptText(SAY_EADRIC_HAMMER_WARNING, me, target);
                    DoCast(target, SPELL_HAMMER_RIGHTEOUS);
                }
            }

            timer = 25 * IN_MILLISECONDS;
        }break;
    }

    if (eventID && timer)
        eventMap.ScheduleEvent(eventID, timer);
}


void PaletressAI::Reset()
{
    me->RemoveAllAuras();

    if (memoryGUID != 0)
        if (Creature* pCreature = sObjectAccessor->GetCreature(*me, memoryGUID))
            pCreature->DespawnOrUnsummon(1000);

    shielded = false;
    defeated = false;

    memoryGUID = 0;

    eventMap.Reset();

    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);
    me->RemoveUnitMovementFlag(MOVEMENTFLAG_WALKING);
    me->SetReactState(REACT_AGGRESSIVE);
}

void PaletressAI::JustSummoned(Creature* pCreature)
{
    memoryGUID = pCreature->GetGUID();
}

void PaletressAI::DoAction(const int32 action)
{
    if (action == 1)
    {
        if (Creature* pCreature = sObjectAccessor->GetCreature(*me, memoryGUID))
            pCreature->DespawnOrUnsummon();

        Talk(SAY_PALETRESS_MEMORY_DIES);
        me->RemoveAura(SPELL_HOLY_SHIELD);
    }
}

void PaletressAI::EnterCombat(Unit* /*attacker*/)
{
    Talk(SAY_COMMON_AGGRO);

    me->setActive(true);
    DoZoneInCombat();

    eventMap.ScheduleEvent(EVENT_SPELL_HOLY_FIRE, urand(9 * IN_MILLISECONDS, 12 * IN_MILLISECONDS));
    eventMap.ScheduleEvent(EVENT_SPELL_HOLY_SMITE, urand(5 * IN_MILLISECONDS, 7 * IN_MILLISECONDS));
    eventMap.ScheduleEvent(EVENT_SPELL_RENEW, urand(2 * IN_MILLISECONDS, 5 * IN_MILLISECONDS));

    if (pInstance)
        if (pInstance->GetData(BOSS_ARGENT_CHALLENGE) != IN_PROGRESS)
            pInstance->SetData(BOSS_ARGENT_CHALLENGE, IN_PROGRESS);
}

void PaletressAI::JustReachedHome()
{
    if (defeated)
    {
        me->DespawnOrUnsummon();
        return;
    }

    if (pInstance)
        if (pInstance->GetData(BOSS_ARGENT_CHALLENGE) == IN_PROGRESS)
            pInstance->SetData(BOSS_ARGENT_CHALLENGE, FAIL);
}

void PaletressAI::KilledUnit(Unit* /*target*/)
{
    Talk(SAY_COMMON_KILL);
}

void PaletressAI::DamageTaken(Unit* /*attacker*/, uint32& damage)
{
    if (defeated)
    {
        damage = 0;
        return;
    }

    if (!shielded && HealthBelowPct(25))
    {
        shielded = true;

        me->InterruptNonMeleeSpells(true);
        DoCastAOE(SPELL_HOLY_NOVA, false);
        DoCast(me, SPELL_HOLY_SHIELD);
        DoCastAOE(SPELL_SUMMON_MEMORY, false);
        DoCastAOE(SPELL_CONFESS, false);
        Talk(SAY_PALETRESS_SUMMON_MEMORY);
    }

    // Prevent damage from finishing hit and mark creature as defeated
    if (damage >= me->GetHealth())
    {
        damage = 0;
        defeated = true;

        me->InterruptNonMeleeSpells(true);

        me->SetTarget(0);
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        me->SetReactState(REACT_PASSIVE);
        me->GetMotionMaster()->MoveTargetedHome();

        Talk(SAY_COMMON_DEATH);

        if (Creature* pCreature = sObjectAccessor->GetCreature(*me, memoryGUID))
            pCreature->DespawnOrUnsummon();

        if (pInstance)
        {
            // Set encounter done
            pInstance->SetData(BOSS_ARGENT_CHALLENGE, DONE);

            // Inform instance that encounter done
            pInstance->DoAction(INSTANCE_ACTION_ENCOUNTER_DONE);
        }
    }
}

void PaletressAI::MovementInform(uint32 type, uint32 id)
{
    if (type != POINT_MOTION_TYPE)
        return;

    switch (id)
    {
        case 1: // After adds has been defeated
        {
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);
            me->SetReactState(REACT_AGGRESSIVE);
            me->setFaction(16);
        }break;

        case ENCOUNTER_WP_DESPAWN_POSITION: // After boss has finished and went to door
        {
            me->DespawnOrUnsummon();
        }break;

    }
}

void PaletressAI::UpdateAI(const uint32 diff)
{
    if (!UpdateVictim() || defeated)
        return;

    eventMap.Update(diff);

    if (me->HasUnitState(UNIT_STATE_CASTING))
        return;

    if (uint32 eventID = eventMap.ExecuteEvent())
    {
        executeEvent(eventID);
        return;
    }

    DoMeleeAttackIfReady();
}

void PaletressAI::executeEvent(uint32 eventID)
{
    uint32 timer = 0;

    switch (eventID)
    {
        case EVENT_SPELL_HOLY_FIRE:
        {
            if (Unit* pUnit = SelectTarget(SELECT_TARGET_RANDOM, 0, 250, true))
            {
                if (pUnit && pUnit->isAlive())
                    DoCast(pUnit, SPELL_HOLY_FIRE);
            }

            if (me->HasAura(SPELL_HOLY_SHIELD))
                timer = 13 * IN_MILLISECONDS;
            else
                timer = urand(9 * IN_MILLISECONDS, 12 * IN_MILLISECONDS);
        }break;

        case EVENT_SPELL_HOLY_SMITE:
        {
            if (Unit* pUnit = SelectTarget(SELECT_TARGET_RANDOM, 0, 250, true))
            {
                if (pUnit && pUnit->isAlive())
                    DoCast(pUnit, SPELL_SMITE);
            }

            if (me->HasAura(SPELL_HOLY_SHIELD))
                timer = 9 * IN_MILLISECONDS;
            else
                timer = urand(5 * IN_MILLISECONDS, 7 * IN_MILLISECONDS);
        }break;

        case EVENT_SPELL_RENEW:
        {
            me->InterruptNonMeleeSpells(true);
            uint8 target = urand(0, 1);

            switch (target)
            {
                case 0:
                {
                    DoCast(me, SPELL_RENEW);
                }break;

                case 1:
                {
                    if (Creature* pCreature = sObjectAccessor->GetCreature(*me, memoryGUID))
                    {
                        if (pCreature->isAlive())
                        {
                            DoCast(pCreature, SPELL_RENEW);
                            break;
                        }
                    }

                    DoCast(me, SPELL_RENEW);
                }break;
            }

            timer = urand(15 * IN_MILLISECONDS, 17 * IN_MILLISECONDS);
        }break;
    }

    if (eventID && timer)
        eventMap.ScheduleEvent(eventID, timer);
}
