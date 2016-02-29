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

#include "Boss_GrandChampions.h"


typedef Boss_GrandChampionWarrior::Boss_GrandChampionWarriorAI  GrandChampionWarriorAI;
typedef Boss_GrandChampionMage::Boss_GrandChampionMageAI        GrandChampionMageAI;
typedef Boss_GrandChampionShaman::Boss_GrandChampionShamanAI    GrandChampionShamanAI;
typedef Boss_GrandChampionHunter::Boss_GrandChampionHunterAI    GrandChampionHunterAI;
typedef Boss_GrandChampionRouge::Boss_GrandChampionRougeAI      GrandChampionRougeAI;

void GrandChampionWarriorAI::Reset()
{
    if (isInMountedGauntlet())
    {
        MountedChampionAI::Reset();
        return;
    }

    defeated = false;
    eventMap.Reset();

    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);
    me->RemoveUnitMovementFlag(MOVEMENTFLAG_WALKING);
    me->SetReactState(REACT_AGGRESSIVE);
}

void GrandChampionWarriorAI::EnterCombat(Unit* pUnit)
{
    defeated = false;

    if (isInMountedGauntlet())
    {
        MountedChampionAI::EnterCombat(pUnit);
        return;
    }

    eventMap.ScheduleEvent(EVENT_SPELL_MORTAL_STRIKE, urand(8 * IN_MILLISECONDS, 12 * IN_MILLISECONDS));
    eventMap.ScheduleEvent(EVENT_SPELL_BLADESTORM, urand(15 * IN_MILLISECONDS, 20 * IN_MILLISECONDS));
    eventMap.ScheduleEvent(EVENT_SPELL_INTERCEPT, 7 * IN_MILLISECONDS);

    if (pInstance)
        if (pInstance->GetData(BOSS_GRAND_CHAMPIONS) != SPECIAL)
            pInstance->SetData(BOSS_GRAND_CHAMPIONS, SPECIAL);
};

void GrandChampionWarriorAI::DamageTaken(Unit* /*attacker*/, uint32& damage)
{
    if (isInMountedGauntlet())
    {
        MountedChampionAI::DamageTaken(NULL, damage);
        return;
    }

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
        me->GetMotionMaster()->MoveIdle();
        me->CastSpell(me, SPELL_KNEE, true);
        me->SetTarget(0);

        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);
        me->SetReactState(REACT_PASSIVE);

        if (pInstance)
            pInstance->SetData(ENCOUNTER_DATA_GC_BOSS_DEFEATED, 1);
    }
}

void GrandChampionWarriorAI::JustReachedHome()
{
    if (isInMountedGauntlet())
    {
        MountedChampionAI::JustReachedHome();
        return;
    }
}

void GrandChampionWarriorAI::MovementInform(uint32 type, uint32 id)
{
    if (type != POINT_MOTION_TYPE)
        return;

    if (id == ENCOUNTER_WP_DESPAWN_POSITION)
        me->DespawnOrUnsummon();
}

void GrandChampionWarriorAI::UpdateAI(const uint32 diff)
{
    if (isInMountedGauntlet())
    {
        MountedChampionAI::UpdateAI(diff);
        return;
    }

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

void GrandChampionWarriorAI::executeEvent(uint32 eventID)
{
    uint32 timer = 0;

    switch (eventID)
    {
        case EVENT_SPELL_MORTAL_STRIKE:
        {
            DoCastVictim(SPELL_MORTAL_STRIKE);
            timer = urand(8 * IN_MILLISECONDS, 12 * IN_MILLISECONDS);
        }break;

        case EVENT_SPELL_BLADESTORM:
        {
            DoCastVictim(SPELL_BLADESTORM);
            timer = urand(15 * IN_MILLISECONDS, 20 * IN_MILLISECONDS);
        }break;

        case EVENT_SPELL_INTERCEPT:
        {
            Map::PlayerList const& playerList = me->GetMap()->GetPlayers();
            if (me->GetMap()->IsDungeon() && !playerList.isEmpty())
            {
                for (Map::PlayerList::const_iterator itr = playerList.begin(); itr != playerList.end(); ++itr)
                {
                    Player* pPlayer = itr->getSource();
                    if (pPlayer && !pPlayer->isGameMaster() && me->IsInRange(pPlayer, 8.0f, 25.0f, false))
                    {
                        DoResetThreat();
                        me->AddThreat(pPlayer, 5.0f);
                        DoCast(pPlayer, SPELL_INTERCEPT);
                        break;
                    }
                }
            }

            timer = 7 * IN_MILLISECONDS;
        }break;
    }

    if (eventID && timer)
        eventMap.ScheduleEvent(eventID, timer);
}


void GrandChampionMageAI::Reset()
{
    if (isInMountedGauntlet())
    {
        MountedChampionAI::Reset();
        return;
    }

    defeated = false;
    eventMap.Reset();

    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);
    me->RemoveUnitMovementFlag(MOVEMENTFLAG_WALKING);
    me->SetReactState(REACT_AGGRESSIVE);
}

void GrandChampionMageAI::EnterCombat(Unit* who)
{
    defeated = false;

    if (isInMountedGauntlet())
    {
        MountedChampionAI::EnterCombat(who);
        return;
    }

    eventMap.ScheduleEvent(EVENT_SPELL_FIREBALL, 2 * IN_MILLISECONDS);
    eventMap.ScheduleEvent(EVENT_SPELL_BLAST_WAVE, 8 * IN_MILLISECONDS);
    eventMap.ScheduleEvent(EVENT_SPELL_HASTE, 12 * IN_MILLISECONDS);
    eventMap.ScheduleEvent(EVENT_SPELL_POLYMORPH, 22 * IN_MILLISECONDS);

    if (pInstance)
        if (pInstance->GetData(BOSS_GRAND_CHAMPIONS) != SPECIAL)
            pInstance->SetData(BOSS_GRAND_CHAMPIONS, SPECIAL);
};

void GrandChampionMageAI::DamageTaken(Unit* /*attacker*/, uint32& damage)
{
    if (isInMountedGauntlet())
    {
        MountedChampionAI::DamageTaken(NULL, damage);
        return;
    }

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
        me->GetMotionMaster()->MoveIdle();
        me->CastSpell(me, SPELL_KNEE, true);
        me->SetTarget(0);

        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);
        me->SetReactState(REACT_PASSIVE);

        if (pInstance)
            pInstance->SetData(ENCOUNTER_DATA_GC_BOSS_DEFEATED, 1);
    }
}

void GrandChampionMageAI::JustReachedHome()
{
    if (isInMountedGauntlet())
    {
        MountedChampionAI::JustReachedHome();
        return;
    }
}

void GrandChampionMageAI::MovementInform(uint32 type, uint32 id)
{
    if (type != POINT_MOTION_TYPE)
        return;

    if (id == ENCOUNTER_WP_DESPAWN_POSITION)
        me->DespawnOrUnsummon();
}

void GrandChampionMageAI::UpdateAI(const uint32 diff)
{
    if (isInMountedGauntlet())
    {
        MountedChampionAI::UpdateAI(diff);
        return;
    }

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

void GrandChampionMageAI::executeEvent(uint32 eventID)
{
    uint32 timer = 0;

    switch (eventID)
    {
        case EVENT_SPELL_FIREBALL:
        {
            DoCastVictim(SPELL_FIREBALL);
            timer = 2.6 * IN_MILLISECONDS;
        }break;

        case EVENT_SPELL_BLAST_WAVE:
        {
            DoCastAOE(SPELL_BLAST_WAVE, false);
            timer = 13 * IN_MILLISECONDS;
        }break;

        case EVENT_SPELL_HASTE:
        {
            me->InterruptNonMeleeSpells(true);
            DoCast(me, SPELL_HASTE);

            timer = 22 * IN_MILLISECONDS;
        }break;

        case EVENT_SPELL_POLYMORPH:
        {
            if (Unit* pUnit = SelectTarget(SELECT_TARGET_RANDOM, 0))
                DoCast(pUnit, SPELL_POLYMORPH);

            timer = 8 * IN_MILLISECONDS;
        }break;
    }

    if (eventID && timer)
        eventMap.ScheduleEvent(eventID, timer);
}


void GrandChampionShamanAI::Reset()
{
    if (isInMountedGauntlet())
    {
        MountedChampionAI::Reset();
        return;
    }

    defeated = false;
    eventMap.Reset();

    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);
    me->RemoveUnitMovementFlag(MOVEMENTFLAG_WALKING);
    me->SetReactState(REACT_AGGRESSIVE);
}

void GrandChampionShamanAI::EnterCombat(Unit* who)
{
    defeated = false;

    if (isInMountedGauntlet())
    {
        MountedChampionAI::EnterCombat(who);
        return;
    }

    eventMap.ScheduleEvent(EVENT_SPELL_CHAIN_LIGHTNING, 5 * IN_MILLISECONDS);
    eventMap.ScheduleEvent(EVENT_SPELL_EARTH_SHIELD, urand(30 * IN_MILLISECONDS, 35 * IN_MILLISECONDS));
    eventMap.ScheduleEvent(EVENT_SPELL_HEALING_WAVE, 12 * IN_MILLISECONDS);
    eventMap.ScheduleEvent(EVENT_SPELL_HEX_OF_MENDING, urand(1 * IN_MILLISECONDS, 5 * IN_MILLISECONDS));

    if (pInstance)
        if (pInstance->GetData(BOSS_GRAND_CHAMPIONS) != SPECIAL)
            pInstance->SetData(BOSS_GRAND_CHAMPIONS, SPECIAL);

    DoCast(me, SPELL_EARTH_SHIELD);
};

void GrandChampionShamanAI::JustReachedHome()
{
    if (isInMountedGauntlet())
    {
        MountedChampionAI::JustReachedHome();
        return;
    }
}

void GrandChampionShamanAI::DamageTaken(Unit* /*attacker*/, uint32& damage)
{
    if (isInMountedGauntlet())
    {
        MountedChampionAI::DamageTaken(NULL, damage);
        return;
    }

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
        me->GetMotionMaster()->MoveIdle();
        me->CastSpell(me, SPELL_KNEE, true);
        me->SetTarget(0);

        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);
        me->SetReactState(REACT_PASSIVE);

        if (pInstance)
            pInstance->SetData(ENCOUNTER_DATA_GC_BOSS_DEFEATED, 1);
    }
}

void GrandChampionShamanAI::MovementInform(uint32 type, uint32 id)
{
    if (type != POINT_MOTION_TYPE)
        return;

    if (id == ENCOUNTER_WP_DESPAWN_POSITION)
        me->DespawnOrUnsummon();
}

void GrandChampionShamanAI::UpdateAI(const uint32 diff)
{
    if (isInMountedGauntlet())
    {
        MountedChampionAI::UpdateAI(diff);
        return;
    }

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

void GrandChampionShamanAI::executeEvent(uint32 eventID)
{
    uint32 timer = 0;

    switch (eventID)
    {
        case EVENT_SPELL_CHAIN_LIGHTNING:
        {
            if (Unit* pUnit = SelectTarget(SELECT_TARGET_RANDOM, 0))
                DoCast(pUnit, SPELL_CHAIN_LIGHTNING);

            timer = 8 * IN_MILLISECONDS;
        }break;

        case EVENT_SPELL_EARTH_SHIELD:
        {
            Unit* pUnit = DoSelectLowestHpFriendly(40);

            if (pUnit && pUnit->HealthBelowPct(60) && !pUnit->HasAura(SPELL_EARTH_SHIELD))
            {
                DoCast(pUnit, SPELL_EARTH_SHIELD);
                timer = urand(30 * IN_MILLISECONDS, 35 * IN_MILLISECONDS);
            }
            else if (!me->HasAura(SPELL_EARTH_SHIELD))
            {
                DoCast(me, SPELL_EARTH_SHIELD);
                timer = urand(30 * IN_MILLISECONDS, 35 * IN_MILLISECONDS);
            }
            else
                timer = urand(1.5 * IN_MILLISECONDS, 3.5 * IN_MILLISECONDS);
        }break;

        case EVENT_SPELL_HEALING_WAVE:
        {
            bool chance = urand(0, 1);

            if (!chance)
            {
                if (Unit* pUnit = DoSelectLowestHpFriendly(40))
                {
                    DoCast(pUnit, SPELL_HEALING_WAVE);
                    timer = 5 * IN_MILLISECONDS;
                }
            }
            else
            {
                DoCast(me, SPELL_HEALING_WAVE);
                timer = 8 * IN_MILLISECONDS;
            }
        }break;

        case EVENT_SPELL_HEX_OF_MENDING:
        {
            DoCastVictim(SPELL_HEX_OF_MENDING, true);
            timer = urand(12 * IN_MILLISECONDS, 15 * IN_MILLISECONDS);
        }break;
    }

    if (eventID && timer)
        eventMap.ScheduleEvent(eventID, timer);
}


void GrandChampionHunterAI::Reset()
{
    if (isInMountedGauntlet())
    {
        MountedChampionAI::Reset();
        return;
    }

    defeated = false;
    ligthingArrows = false;
    eventMap.Reset();

    me->SetSheath(SHEATH_STATE_RANGED);
    me->SetStatFloatValue(UNIT_FIELD_MINRANGEDDAMAGE, IsHeroic() ? 5000.0f : 3000.0f);
    me->SetStatFloatValue(UNIT_FIELD_MAXRANGEDDAMAGE, IsHeroic() ? 6000.0f : 4000.0f);

    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);
    me->RemoveUnitMovementFlag(MOVEMENTFLAG_WALKING);
    me->SetReactState(REACT_AGGRESSIVE);
}

void GrandChampionHunterAI::EnterCombat(Unit* who)
{
    defeated = false;

    if (isInMountedGauntlet())
    {
        MountedChampionAI::EnterCombat(who);
        return;
    }

    eventMap.ScheduleEvent(EVENT_SPELL_DISENGAGE, 3 * IN_MILLISECONDS);
    eventMap.ScheduleEvent(EVENT_SPELL_LIGHTNING_ARROWS, 7 * IN_MILLISECONDS);
    eventMap.ScheduleEvent(EVENT_SPELL_MULTI_SHOT, 2 * IN_MILLISECONDS);

    if (pInstance)
        if (pInstance->GetData(BOSS_GRAND_CHAMPIONS) != SPECIAL)
            pInstance->SetData(BOSS_GRAND_CHAMPIONS, SPECIAL);
}

void GrandChampionHunterAI::DamageTaken(Unit* /*attacker*/, uint32& damage)
{
    if (isInMountedGauntlet())
    {
        MountedChampionAI::DamageTaken(NULL, damage);
        return;
    }

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
        me->GetMotionMaster()->MoveIdle();
        me->CastSpell(me, SPELL_KNEE, true);
        me->SetTarget(0);

        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);
        me->SetReactState(REACT_PASSIVE);

        if (pInstance)
            pInstance->SetData(ENCOUNTER_DATA_GC_BOSS_DEFEATED, 1);
    }
}

void GrandChampionHunterAI::JustReachedHome()
{
    if (isInMountedGauntlet())
    {
        MountedChampionAI::JustReachedHome();
        return;
    }
}

void GrandChampionHunterAI::MovementInform(uint32 type, uint32 id)
{
    if (type != POINT_MOTION_TYPE)
        return;

    if (id == ENCOUNTER_WP_DESPAWN_POSITION)
        me->DespawnOrUnsummon();
}

void GrandChampionHunterAI::UpdateAI(const uint32 diff)
{
    if (isInMountedGauntlet())
    {
        MountedChampionAI::UpdateAI(diff);
        return;
    }

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

    // Increase distance to target
    if (me->GetDistance(me->getVictim()) >= 30.0f && !me->isMoving())
        me->GetMotionMaster()->MoveChase(me->getVictim(), 25.0f);

    // Trigger the aura after the visual storm
    if (ligthingArrows && !me->HasAura(SPELL_LIGHTNING_ARROWS_VISUAL))
    {
        DoCast(SPELL_LIGHTNING_ARROWS);
        ligthingArrows = false;
    }

    // Shoot instead of meele swing
    if (me->isAttackReady())
    {
        if (Aura* pAura = me->GetAura(SPELL_LIGHTNING_ARROWS))
        {
            DoCast(me->getVictim(), SPELL_LIGHTNING_ARROWS_DAMAGE, true);
            pAura->ModCharges(-1);
        }

        DoCast(me->getVictim(), SPELL_SHOOT, true);
        me->ResetAttackTimer();
    }
}

void GrandChampionHunterAI::executeEvent(uint32 eventID)
{
    uint32 timer = 0;

    switch (eventID)
    {
        case EVENT_SPELL_DISENGAGE:
        {
            if (me->GetDistance(me->getVictim()) <= 3.0f)
            {
                DoCast(SPELL_DISENGAGE);
                timer = 7 * IN_MILLISECONDS;
            }
            else
                timer = 1 * IN_MILLISECONDS;
        }break;

        case EVENT_SPELL_LIGHTNING_ARROWS:
        {
            if (!me->HasAura(SPELL_LIGHTNING_ARROWS))
            {
                DoCastAOE(SPELL_LIGHTNING_ARROWS_VISUAL, false);
                ligthingArrows = true;

                timer = 14 * IN_MILLISECONDS;
                break;
            }

            timer = 1 * IN_MILLISECONDS;
        }break;

        case EVENT_SPELL_MULTI_SHOT:
        {
            if (Unit* pUnit = SelectTarget(SELECT_TARGET_FARTHEST))
            {
                if (pUnit->IsInRange(me, 5.0f, 35.0f))
                {
                    DoCast(pUnit, SPELL_MULTI_SHOT);
                    timer = 6 * IN_MILLISECONDS;
                    break;
                }
            }

            timer = 1 * IN_MILLISECONDS;
        }break;
    }

    if (eventID && timer)
        eventMap.ScheduleEvent(eventID, timer);
}


void GrandChampionRougeAI::Reset()
{
    if (isInMountedGauntlet())
    {
        MountedChampionAI::Reset();
        return;
    }

    defeated = false;
    eventMap.Reset();

    // Settings for offhand attack
    DoCast(me, SPELL_DUAL_WIELD, true);
    me->SetBaseAttackTime(OFF_ATTACK, 1400);
    me->SetStatFloatValue(UNIT_FIELD_MINOFFHANDDAMAGE, IsHeroic() ? 5000.0f : 3000.0f);
    me->SetStatFloatValue(UNIT_FIELD_MAXOFFHANDDAMAGE, IsHeroic() ? 6000.0f : 4000.0f);

    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);
    me->RemoveUnitMovementFlag(MOVEMENTFLAG_WALKING);
    me->SetReactState(REACT_AGGRESSIVE);
}

void GrandChampionRougeAI::EnterCombat(Unit* who)
{
    defeated = false;

    if (isInMountedGauntlet())
    {
        MountedChampionAI::EnterCombat(who);
        return;
    }

    eventMap.ScheduleEvent(EVENT_SPELL_EVISCERATE, 8 * IN_MILLISECONDS);
    eventMap.ScheduleEvent(EVENT_SPELL_FAN_OF_KNIVES, 14 * IN_MILLISECONDS);
    eventMap.ScheduleEvent(EVENT_SPELL_POISON_BOTTLE, 19 * IN_MILLISECONDS);

    if (pInstance)
        if (pInstance->GetData(BOSS_GRAND_CHAMPIONS) != SPECIAL)
            pInstance->SetData(BOSS_GRAND_CHAMPIONS, SPECIAL);

    DoCast(me, SPELL_DEADLY_POISON);
};

void GrandChampionRougeAI::DamageTaken(Unit* /*attacker*/, uint32& damage)
{
    if (isInMountedGauntlet())
    {
        MountedChampionAI::DamageTaken(NULL, damage);
        return;
    }

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
        me->GetMotionMaster()->MoveIdle();
        me->CastSpell(me, SPELL_KNEE, true);
        me->SetTarget(0);

        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);
        me->SetReactState(REACT_PASSIVE);

        if (pInstance)
            pInstance->SetData(ENCOUNTER_DATA_GC_BOSS_DEFEATED, 1);
    }
}

void GrandChampionRougeAI::JustReachedHome()
{
    if (isInMountedGauntlet())
    {
        MountedChampionAI::JustReachedHome();
        return;
    }
}

void GrandChampionRougeAI::MovementInform(uint32 type, uint32 id)
{
    if (type != POINT_MOTION_TYPE)
        return;

    if (id == ENCOUNTER_WP_DESPAWN_POSITION)
        me->DespawnOrUnsummon();
}

void GrandChampionRougeAI::UpdateAI(const uint32 diff)
{
    if (isInMountedGauntlet())
    {
        MountedChampionAI::UpdateAI(diff);
        return;
    }

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

void GrandChampionRougeAI::executeEvent(uint32 eventID)
{
    uint32 timer = 0;

    switch (eventID)
    {
        case EVENT_SPELL_EVISCERATE:
        {
            DoCast(me->getVictim(), SPELL_EVISCERATE);
            timer = 8 * IN_MILLISECONDS;
        }break;

        case EVENT_SPELL_FAN_OF_KNIVES:
        {
            DoCastAOE(SPELL_FAN_OF_KNIVES, false);
            timer = 14 * IN_MILLISECONDS;
        }break;

        case EVENT_SPELL_POISON_BOTTLE:
        {
            if (Unit* pUnit = SelectTarget(SELECT_TARGET_RANDOM, 0))
                DoCast(pUnit, SPELL_POISON_BOTTLE);

            timer = 19 * IN_MILLISECONDS;
        }break;
    }

    if (eventID && timer)
        eventMap.ScheduleEvent(eventID, timer);
}
