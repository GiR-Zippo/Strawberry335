/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2006-2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/* ScriptData
SDName: trial_of_the_crusader
SD%Complete: ??%
SDComment: based on /dev/rsa
SDCategory: Crusader Coliseum
EndScriptData */

// Known bugs:
// Some visuals aren't appearing right sometimes
//
// TODO:
// Redone summon's scripts in SAI
// Add immunities to the boss and summons

#include "LordJaraxxus.h"

enum Yells
{
    SAY_INTRO               = -1649030,
    SAY_AGGRO               = -1649031,
    SAY_DEATH               = -1649032,
    EMOTE_INCINERATE        = -1649033,
    SAY_INCINERATE          = -1649034,
    EMOTE_LEGION_FLAME      = -1649035,
    EMOTE_NETHER_PORTAL     = -1649036,
    SAY_NETHER_PORTAL       = -1649037,
    EMOTE_INFERNAL_ERUPTION = -1649038,
    SAY_INFERNAL_ERUPTION   = -1649039,
};

enum Equipment
{
    EQUIP_MAIN           = 47266,
    EQUIP_OFFHAND        = 46996,
    EQUIP_RANGED         = 47267,
    EQUIP_DONE           = EQUIP_NO_CHANGE,
};

enum Summons
{
    NPC_LEGION_FLAME     = 34784,
    NPC_INFERNAL_VOLCANO = 34813,
    NPC_FEL_INFERNAL     = 34815, // immune to all CC on Heroic (stuns, banish, interrupt, etc)
    NPC_NETHER_PORTAL    = 34825,
};

enum BossSpells
{
    SPELL_LEGION_FLAME                = 66197, // player should run away from raid because he triggers Legion Flame
    SPELL_LEGION_FLAME_EFFECT         = 66201, // used by trigger npc
    SPELL_NETHER_POWER                = 66228, // +20% of spell damage per stack, stackable up to 5/10 times, must be dispelled/stealed
    SPELL_FEL_LIGHTING                = 66528, // jumps to nearby targets
    SPELL_FEL_FIREBALL                = 66532, // does heavy damage to the tank, interruptable
    SPELL_INCINERATE_FLESH            = 66237, // target must be healed or will trigger Burning Inferno
    SPELL_BURNING_INFERNO             = 66242, // triggered by Incinerate Flesh
    SPELL_INFERNAL_ERUPTION           = 66258, // summons Infernal Volcano
    SPELL_INFERNAL_ERUPTION_EFFECT    = 66252, // summons Felflame Infernal (3 at Normal and inifinity at Heroic)
    SPELL_NETHER_PORTAL               = 66269, // summons Nether Portal
    SPELL_NETHER_PORTAL_EFFECT        = 66263, // summons Mistress of Pain (1 at Normal and infinity at Heroic)

    SPELL_BERSERK                     = 64238, // unused

    // Mistress of Pain spells
    SPELL_SHIVAN_SLASH                = 67098,
    SPELL_SPINNING_STRIKE             = 66283,
    SPELL_MISTRESS_KISS               = 67077,
    SPELL_FEL_INFERNO                 = 67047,
    SPELL_FEL_STREAK                  = 66494,
};

enum BossEvents
{
    EVENT_SPELL_FELFIREBALL           = 0,
    EVENT_SPELL_FELLIGHTNING,
    EVENT_SPELL_INCINERATEFLESH,
    EVENT_SPELL_NETHERPOWER,
    EVENT_SPELL_LEGIONFLAME,
    EVENT_SUMMON_NETHERPORTAL,
    EVENT_SUMMON_INFERNALERUPTION
};

/*######
## boss_jaraxxus
######*/

class boss_jaraxxus : public CreatureScript
{
public:
    boss_jaraxxus() : CreatureScript("boss_jaraxxus") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_jaraxxusAI(creature);
    }

    struct boss_jaraxxusAI : public ScriptedAI
    {
        boss_jaraxxusAI(Creature* creature) : ScriptedAI(creature), Summons(me)
        {
            instance = creature->GetInstanceScript();
            Reset();
        }

        void Reset()
        {
            SetEquipmentSlots(false, EQUIP_MAIN, EQUIP_OFFHAND, EQUIP_RANGED);
            Summons.DespawnAll();
            m_EventMap.Reset();
        }

        void EnterCombat(Unit* /*victim*/)
        {
            me->SetInCombatWithZone();
            DoScriptText(SAY_AGGRO, me);

            m_EventMap.ScheduleEvent(EVENT_SPELL_FELFIREBALL, 5 * IN_MILLISECONDS);
            m_EventMap.ScheduleEvent(EVENT_SPELL_FELLIGHTNING, urand(10 * IN_MILLISECONDS, 15 * IN_MILLISECONDS));
            m_EventMap.ScheduleEvent(EVENT_SPELL_INCINERATEFLESH, urand(20 * IN_MILLISECONDS, 25 * IN_MILLISECONDS));
            m_EventMap.ScheduleEvent(EVENT_SPELL_NETHERPOWER, 40 * IN_MILLISECONDS);
            m_EventMap.ScheduleEvent(EVENT_SPELL_LEGIONFLAME, 30 * IN_MILLISECONDS);
            m_EventMap.ScheduleEvent(EVENT_SUMMON_NETHERPORTAL, 1 * MINUTE*IN_MILLISECONDS);
            m_EventMap.ScheduleEvent(EVENT_SUMMON_INFERNALERUPTION, 2 * MINUTE*IN_MILLISECONDS);

            if (instance)
                instance->SetData(TYPE_ENCOUNTER_STATUS, IN_PROGRESS);
        }

        void JustReachedHome()
        {
            DoCast(me, SPELL_JARAXXUS_CHAINS);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            me->SetReactState(REACT_PASSIVE);
        }

        void JustDied(Unit* /*killer*/)
        {
            Summons.DespawnAll();
            DoScriptText(SAY_DEATH, me);
            if (instance)
                instance->SetData(TYPE_ENCOUNTER_STATUS, DONE);
        }

        void JustSummoned(Creature* summoned)
        {
            Summons.Summon(summoned);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            m_EventMap.Update(diff);
            ExecuteEvents();

            DoMeleeAttackIfReady();
        }

        void ExecuteEvents()
        {
            while (uint32 eventId = m_EventMap.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_SPELL_FELFIREBALL:
                    {
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            DoCast(target, SPELL_FEL_LIGHTING);
                        m_EventMap.ScheduleEvent(EVENT_SPELL_FELFIREBALL, urand(5 * IN_MILLISECONDS, 10 * IN_MILLISECONDS));
                    }break;

                    case EVENT_SPELL_FELLIGHTNING:
                    {
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            DoCast(target, SPELL_FEL_LIGHTING);
                        m_EventMap.ScheduleEvent(EVENT_SPELL_FELLIGHTNING, urand(5 * IN_MILLISECONDS, 10 * IN_MILLISECONDS));
                    }break;

                    case EVENT_SPELL_INCINERATEFLESH:
                    {
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 0, true))
                        {
                            DoScriptText(EMOTE_INCINERATE, me, target);
                            DoScriptText(SAY_INCINERATE, me);
                            DoCast(target, SPELL_INCINERATE_FLESH);
                        }
                        m_EventMap.ScheduleEvent(EVENT_SPELL_INCINERATEFLESH, 40 * IN_MILLISECONDS);
                    }break;

                    case EVENT_SPELL_NETHERPOWER:
                    {
                        me->CastCustomSpell(SPELL_NETHER_POWER, SPELLVALUE_AURA_STACK, RAID_MODE<uint32>(5, 10, 5, 10), me, TRIGGERED_FULL_MASK);
                        m_EventMap.ScheduleEvent(EVENT_SPELL_NETHERPOWER, 40 * IN_MILLISECONDS);
                    }break;

                    case EVENT_SPELL_LEGIONFLAME:
                    {
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 0, true))
                        {
                            DoScriptText(EMOTE_LEGION_FLAME, me, target);
                            DoCast(target, SPELL_LEGION_FLAME);
                        }
                        m_EventMap.ScheduleEvent(EVENT_SPELL_LEGIONFLAME, 30 * IN_MILLISECONDS);
                    }break;

                    case EVENT_SUMMON_NETHERPORTAL:
                    {
                        DoScriptText(EMOTE_NETHER_PORTAL, me);
                        DoScriptText(SAY_NETHER_PORTAL, me);
                        DoCast(SPELL_NETHER_PORTAL);
                        m_EventMap.ScheduleEvent(EVENT_SUMMON_NETHERPORTAL, 2 * MINUTE*IN_MILLISECONDS);
                    }break;

                    case EVENT_SUMMON_INFERNALERUPTION:
                    {
                        DoScriptText(EMOTE_INFERNAL_ERUPTION, me);
                        DoScriptText(SAY_INFERNAL_ERUPTION, me);
                        DoCast(SPELL_INFERNAL_ERUPTION);
                        m_EventMap.ScheduleEvent(EVENT_SUMMON_INFERNALERUPTION, 2 * MINUTE*IN_MILLISECONDS);
                    }break;
                }
            }
        }

        private:
            InstanceScript* instance;
            EventMap m_EventMap;
            SummonList Summons;
    };

};

class mob_legion_flame : public CreatureScript
{
public:
    mob_legion_flame() : CreatureScript("mob_legion_flame") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_legion_flameAI(creature);
    }

    struct mob_legion_flameAI : public Scripted_NoMovementAI
    {
        mob_legion_flameAI(Creature* creature) : Scripted_NoMovementAI(creature)
        {
            Reset();
        }

        void Reset()
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
            me->SetInCombatWithZone();
            DoCast(SPELL_LEGION_FLAME_EFFECT);
        }

        void UpdateAI(const uint32 /*uiDiff*/)
        {
            UpdateVictim();
        }
    };

};

class mob_infernal_volcano : public CreatureScript
{
public:
    mob_infernal_volcano() : CreatureScript("mob_infernal_volcano") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_infernal_volcanoAI(creature);
    }

    struct mob_infernal_volcanoAI : public Scripted_NoMovementAI
    {
        mob_infernal_volcanoAI(Creature* creature) : Scripted_NoMovementAI(creature), Summons(me)
        {
            instance = creature->GetInstanceScript();
            Reset();
        }

        InstanceScript* instance;

        SummonList Summons;

        void Reset()
        {
            me->SetReactState(REACT_PASSIVE);

            if (!IsHeroic())
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_PACIFIED);
            else
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_PACIFIED);

            Summons.DespawnAll();
        }

        void IsSummonedBy(Unit* /*summoner*/)
        {
            DoCast(SPELL_INFERNAL_ERUPTION_EFFECT);
        }

        void JustSummoned(Creature* summoned)
        {
            Summons.Summon(summoned);
            // makes immediate corpse despawn of summoned Felflame Infernals
            summoned->SetCorpseDelay(0);
        }

        void JustDied(Unit* /*killer*/)
        {
            // used to despawn corpse immediately
            me->DespawnOrUnsummon();
        }
    };

};

class mob_fel_infernal : public CreatureScript
{
public:
    mob_fel_infernal() : CreatureScript("mob_fel_infernal") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_fel_infernalAI(creature);
    }

    struct mob_fel_infernalAI : public ScriptedAI
    {
        mob_fel_infernalAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
            Reset();
        }

        InstanceScript* instance;
        uint32 m_uiFelStreakTimer;

        void Reset()
        {
            m_uiFelStreakTimer = 30*IN_MILLISECONDS;
            me->SetInCombatWithZone();
        }

        /*void SpellHitTarget(Unit* target, const SpellInfo* pSpell)
        {
            if (pSpell->Id == SPELL_FEL_STREAK)
                DoCastAOE(SPELL_FEL_INFERNO); //66517
        }*/

        void UpdateAI(const uint32 uiDiff)
        {
            if (!UpdateVictim())
                return;

            if (instance && instance->GetData(TYPE_ENCOUNTER_STATUS) != IN_PROGRESS)
                me->DespawnOrUnsummon();

            if (m_uiFelStreakTimer <= uiDiff)
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                    DoCast(target, SPELL_FEL_STREAK);
                m_uiFelStreakTimer = 30*IN_MILLISECONDS;
            } else m_uiFelStreakTimer -= uiDiff;

            DoMeleeAttackIfReady();
        }
    };

};

class mob_nether_portal : public CreatureScript
{
public:
    mob_nether_portal() : CreatureScript("mob_nether_portal") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_nether_portalAI(creature);
    }

    struct mob_nether_portalAI : public ScriptedAI
    {
        mob_nether_portalAI(Creature* creature) : ScriptedAI(creature), Summons(me)
        {
            instance = creature->GetInstanceScript();
            Reset();
        }

        InstanceScript* instance;

        SummonList Summons;

        void Reset()
        {
            me->SetReactState(REACT_PASSIVE);

            if (!IsHeroic())
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_PACIFIED);
            else
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_PACIFIED);

            Summons.DespawnAll();
        }

        void IsSummonedBy(Unit* /*summoner*/)
        {
            DoCast(SPELL_NETHER_PORTAL_EFFECT);
        }

        void JustSummoned(Creature* summoned)
        {
            Summons.Summon(summoned);
            // makes immediate corpse despawn of summoned Mistress of Pain
            summoned->SetCorpseDelay(0);
        }

        void JustDied(Unit* /*killer*/)
        {
            // used to despawn corpse immediately
            me->DespawnOrUnsummon();
        }
    };

};

enum MistressEvents
{
    EVENT_SPELL_SHIVANSLASH   = 0,
    EVENT_SPELL_SPINNINGSTRIKE,
    EVENT_SPELL_MISTRESSKISS
};

class mob_mistress_of_pain : public CreatureScript
{
public:
    mob_mistress_of_pain() : CreatureScript("mob_mistress_of_pain") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_mistress_of_painAI(creature);
    }

    struct mob_mistress_of_painAI : public ScriptedAI
    {
        mob_mistress_of_painAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
            Reset();
        }

        void Reset()
        {
            m_EventMap.Reset();
            me->SetInCombatWithZone();
        }

        void EnterCombat(Unit* /*victim*/)
        {
            m_EventMap.ScheduleEvent(EVENT_SPELL_SHIVANSLASH, 30 * IN_MILLISECONDS);
            m_EventMap.ScheduleEvent(EVENT_SPELL_SPINNINGSTRIKE, 30 * IN_MILLISECONDS);
            if (IsHeroic())
                m_EventMap.ScheduleEvent(EVENT_SPELL_MISTRESSKISS, 15 * IN_MILLISECONDS);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            m_EventMap.Update(diff);
            ExecuteEvents();

            DoMeleeAttackIfReady();
        }

        void ExecuteEvents()
        {
            while (uint32 eventId = m_EventMap.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_SPELL_SHIVANSLASH:
                    {
                        DoCastVictim(SPELL_SHIVAN_SLASH);
                        m_EventMap.ScheduleEvent(EVENT_SPELL_SHIVANSLASH, 30 * IN_MILLISECONDS);
                    }break;

                    case EVENT_SPELL_SPINNINGSTRIKE:
                    {
                        DoCastVictim(SPELL_SHIVAN_SLASH);
                        m_EventMap.ScheduleEvent(EVENT_SPELL_SPINNINGSTRIKE, 30 * IN_MILLISECONDS);
                    }break;

                    case EVENT_SPELL_MISTRESSKISS:
                    {
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0, true))
                            DoCast(target, SPELL_SPINNING_STRIKE);
                        m_EventMap.ScheduleEvent(EVENT_SPELL_MISTRESSKISS, 15 * IN_MILLISECONDS);
                    }break;
                }
            }
        }

    private:
        InstanceScript* instance;
        EventMap m_EventMap;
    };

};

void AddBossLordJaraxus()
{
    new boss_jaraxxus();
    new mob_legion_flame();
    new mob_infernal_volcano();
    new mob_fel_infernal();
    new mob_nether_portal();
    new mob_mistress_of_pain();
}
