/**
 * @file
 * @author TehKevin for Laenalith-WoW
 * @date 31.03.2015
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
 * @copyright Copyright 2015 Laenalith-WoW. All rights reserved.
 */

#include "ScriptPCH.h"
#include "PetAI.h"


enum eGuardian
{
    SPELL_DEATHTOUCH    = 5
};

class npc_guardian : public CreatureScript
{
    public:

    npc_guardian() : CreatureScript("npc_guardian") { }

    struct npc_guardianAI : public ScriptedAI
    {
        npc_guardianAI(Creature* pCreature) : ScriptedAI(pCreature) {}

        void Reset()
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        }

        void EnterCombat(Unit* /*who*/)
        {
        }

        void UpdateAI(uint32 const /*diff*/)
        {
            if (!UpdateVictim())
                return;

            if (me->isAttackReady())
            {
                DoCast(me->getVictim(), SPELL_DEATHTOUCH, true);
                me->ResetAttackTimer();
            }
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new npc_guardianAI(pCreature);
    }
};

/*####
## npc_snake_trap_serpents
####*/
enum SnakeTrap
{
    SPELL_MIND_NUMBING_POISON   = 25810, // Viper
    SPELL_DEADLY_POISON         = 34655, // Venomous Snake
    SPELL_CRIPPLING_POISON      = 30981, // Viper

    VENOMOUS_SNAKE_TIMER        = 1500,
    VIPER_TIMER                 = 3000,

    C_VIPER                     = 19921,
};

class npc_snake_trap : public CreatureScript
{
    public:

    npc_snake_trap() : CreatureScript("npc_snake_trap_serpents") { }

    struct npc_snake_trap_serpentsAI : public ScriptedAI
    {
        public:

            npc_snake_trap_serpentsAI(Creature* pCreature) : ScriptedAI(pCreature) {}

            void EnterCombat(Unit* /*who*/) {}

            void Reset()
            {
                SpellTimer = 0;

                CreatureTemplate const* Info = me->GetCreatureTemplate();

                IsViper = Info->Entry == C_VIPER ? true : false;

                me->SetMaxHealth(uint32(107 * (me->getLevel() - 40) * 0.025f));
                //Add delta to make them not all hit the same time
                uint32 delta = (rand() % 7) * 100;
                me->SetStatFloatValue(UNIT_FIELD_BASEATTACKTIME, float(Info->baseattacktime + delta));
                me->SetStatFloatValue(UNIT_FIELD_RANGED_ATTACK_POWER, float(Info->attackpower));

                // Start attacking attacker of owner on first ai update after spawn - move in line of sight may choose better target
                if (!me->getVictim() && me->isSummon())
                    if (Unit* Owner = me->ToTempSummon()->GetSummoner())
                        if (Owner->getAttackerForHelper())
                            AttackStart(Owner->getAttackerForHelper());
            }

            //Redefined for random target selection:
            void MoveInLineOfSight(Unit* who)
            {
                if (!me->getVictim() && me->canCreatureAttack(who))
                {
                    if (me->GetDistanceZ(who) > CREATURE_Z_ATTACK_RANGE)
                        return;

                    float attackRadius = me->GetAttackDistance(who);
                    if (me->IsWithinDistInMap(who, attackRadius) && me->IsWithinLOSInMap(who))
                    {
                        if (!(rand() % 5))
                        {
                            me->SetAttackTimer(BASE_ATTACK, (rand() % 10) * 100);
                            SpellTimer = (rand() % 10) * 100;
                            AttackStart(who);
                        }
                    }
                }
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->getVictim()->HasBreakableByDamageCrowdControlAura(me))
                {
                    me->InterruptNonMeleeSpells(false);
                    return;
                }

                if (SpellTimer <= diff)
                {
                    if (IsViper) //Viper
                    {
                        if (urand(0, 2) == 0) //33% chance to cast
                        {
                            uint32 spell;
                            if (urand(0, 1) == 0)
                                spell = SPELL_MIND_NUMBING_POISON;
                            else
                                spell = SPELL_CRIPPLING_POISON;

                            DoCast(me->getVictim(), spell);
                        }

                        SpellTimer = VIPER_TIMER;
                    }
                    else //Venomous Snake
                    {
                        if (urand(0, 2) == 0) //33% chance to cast
                            DoCast(me->getVictim(), SPELL_DEADLY_POISON);
                        SpellTimer = VENOMOUS_SNAKE_TIMER + (rand() % 5) * 100;
                    }
                }
                else
                    SpellTimer -= diff;

                DoMeleeAttackIfReady();
            }

        private:

            uint32 SpellTimer;
            bool IsViper;
    };

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new npc_snake_trap_serpentsAI(pCreature);
    }
};

/*####
## npc_mirror_image
####*/
enum MirrorImage
{
    TIMER_MI_CHECK_TARGET = 500
};

class npc_mirror_image : public CreatureScript
{
    public:

    npc_mirror_image() : CreatureScript("npc_mirror_image") { }

    struct npc_mirror_imageAI : CasterAI
    {
        public:

            npc_mirror_imageAI(Creature* pCreature) : CasterAI(pCreature) {}

            void InitializeAI() override
            {
                CasterAI::InitializeAI();
                Unit* owner = me->GetOwner();
                if (!owner)
                    return;

                // Inherit Master's Threat List - Thats Bullshit
                //owner->CastSpell((Unit*)NULL, 58838, true);

                // here mirror image casts on summoner spell (not present in client dbc) 49866
                // here should be auras (not present in client dbc): 35657, 35658, 35659, 35660 selfcasted by mirror images (stats related?)
                // Clone Me!
                owner->CastSpell(me, 45204, false);

                // Set timer for target check
                targetCheckTimer = TIMER_MI_CHECK_TARGET;
            }

            // Do not reload Creature templates on evade mode enter - prevent visual lost
            void EnterEvadeMode() override
            {
                if (me->IsInEvadeMode() || !me->isAlive())
                    return;

                Unit* owner = me->GetCharmerOrOwner();

                me->CombatStop(true);
                if (owner && !me->HasUnitState(UNIT_STATE_FOLLOW))
                {
                    me->GetMotionMaster()->Clear(false);
                    me->GetMotionMaster()->MoveFollow(owner, PET_FOLLOW_DIST, me->GetFollowAngle(), MOTION_SLOT_ACTIVE);
                }
            }

            void UpdateAI(uint32 diff) override
            {
                // Check target
                targetCheckTimer -= diff;
                if (targetCheckTimer <= diff)
                {
                    if (me->GetOwner())
                    {
                        Unit* pUnitTarget = me->GetOwner()->getVictim();
                        if (me->GetOwner()->isInCombat() && pUnitTarget)
                        {
                            me->DeleteThreatList();
                            me->AI()->AttackStart(pUnitTarget);
                        }
                        else if (me->isInCombat())
                            EnterEvadeMode();
                    }

                    targetCheckTimer = TIMER_MI_CHECK_TARGET;
                }

                CasterAI::UpdateAI(diff);
            }

        private:

            uint32 targetCheckTimer;
    };

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new npc_mirror_imageAI(pCreature);
    }
};

/*####
## npc_ebon_gargoyle
####*/
enum EbonGargoyle
{
    EVENT_EG_CHECK_TARGET      = 1
};

class npc_ebon_gargoyle : public CreatureScript
{
    public:

    npc_ebon_gargoyle() : CreatureScript("npc_ebon_gargoyle") { }

    struct npc_ebon_gargoyleAI : CasterAI
    {
        public:

            npc_ebon_gargoyleAI(Creature* pCreature) : CasterAI(pCreature) {}

            void Reset() override
            {
                CasterAI::Reset();

                if (!me->GetOwner())
                    return;

                eventMap.Reset();

                Unit* pUnitTarget = NULL;
                bool targetFound = false;

                // Find victim of Summon Gargoyle spell
                if (pUnitTarget = me->GetOwner()->getVictim())
                    if (pUnitTarget->HasAura(49206, me->GetOwner()->GetGUID()))
                        targetFound = true;

                if (!targetFound)
                {
                    std::list<Unit*> targets;
                    Trinity::AnyUnfriendlyUnitInObjectRangeCheck u_check(me, me, 30);
                    Trinity::UnitListSearcher<Trinity::AnyUnfriendlyUnitInObjectRangeCheck> searcher(me, targets, u_check);
                    me->VisitNearbyObject(30, searcher);

                    for (std::list<Unit*>::const_iterator iter = targets.begin(); iter != targets.end(); ++iter)
                        if ((*iter)->GetAura(49206) && me->GetOwner()->getVictim())
                            pUnitTarget = *iter;
                }

                if (pUnitTarget)
                {
                    me->DeleteThreatList();
                    me->AI()->AttackStart(pUnitTarget);
                    eventMap.ScheduleEvent(EVENT_EG_CHECK_TARGET, 2500);
                }
                else
                    flyAwayAndDespawn();
            }

            void JustDied(Unit* /*killer*/) override
            {
                // Stop Feeding Gargoyle when it dies
                if (Unit* owner = me->GetOwner())
                    owner->RemoveAurasDueToSpell(50514);
            }

            // Fly away when dismissed
            void SpellHit(Unit* source, SpellInfo const* spell) override
            {
                if (spell->Id != 50515 || !me->isAlive())
                    return;

                Unit* owner = me->GetOwner();

                if (!owner || owner != source)
                    return;

                flyAwayAndDespawn();
            }

            void UpdateAI(const uint32 diff) override
            {
                eventMap.Update(diff);

                if (uint32 eventID = eventMap.ExecuteEvent())
                {
                    executeEvent(eventID);
                    return;
                }

                CasterAI::UpdateAI(diff);
            }

        private:

            void executeEvent(uint32 eventID)
            {
                switch (eventID)
                {
                    case EVENT_EG_CHECK_TARGET:
                    {
                        if (me->GetReactState() != REACT_PASSIVE)
                        {
                            if ((!me->getVictim() || !me->getVictim()->HasAura(49206)))
                                flyAwayAndDespawn();
                            else
                                eventMap.ScheduleEvent(EVENT_EG_CHECK_TARGET, 500);
                        }
                    }break;
                }
            }

            void flyAwayAndDespawn()
            {
                // Stop Fighting
                me->ApplyModFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE, true);
                // Sanctuary
                me->CastSpell(me, 54661, true);
                me->SetReactState(REACT_PASSIVE);

                //! HACK: Creature's can't have MOVEMENTFLAG_FLYING
                // Fly Away
                me->AddUnitMovementFlag(MOVEMENTFLAG_CAN_FLY | MOVEMENTFLAG_ASCENDING | MOVEMENTFLAG_FLYING);
                me->SetSpeed(MOVE_FLIGHT, 0.75f, true);
                me->SetSpeed(MOVE_RUN, 0.75f, true);
                float x = me->GetPositionX() + 20 * cos(me->GetOrientation());
                float y = me->GetPositionY() + 20 * sin(me->GetOrientation());
                float z = me->GetPositionZ() + 40;
                me->GetMotionMaster()->Clear(false);
                me->GetMotionMaster()->MovePoint(0, x, y, z);

                // Despawn as soon as possible
                me->DespawnOrUnsummon(4 * IN_MILLISECONDS);
            }

            EventMap eventMap;
    };

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new npc_ebon_gargoyleAI(pCreature);
    }
};

/*####
## npc_lightwell
####*/
class npc_lightwell : public CreatureScript
{
    public:
        npc_lightwell() : CreatureScript("npc_lightwell") { }

        struct npc_lightwellAI : public PassiveAI
        {
            npc_lightwellAI(Creature* pCreature) : PassiveAI(pCreature)
            {
                DoCast(me, 59907, false);
            }

            void EnterEvadeMode()
            {
                if (!me->isAlive())
                    return;

                me->DeleteThreatList();
                me->CombatStop(true);
                me->ResetPlayerDamageReq();
            }
        };

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_lightwellAI(pCreature);
        }
};


/*######
# npc_shadowfiend
######*/
enum Shadowfiend
{
    GLYPH_OF_SHADOWFIEND_MANA   = 58227,
    GLYPH_OF_SHADOWFIEND        = 58228
};

class npc_shadowfiend : public CreatureScript
{
    public:
        npc_shadowfiend() : CreatureScript("npc_shadowfiend") { }

        struct npc_shadowfiendAI : public PetAI
        {
            npc_shadowfiendAI(Creature* pCreature) : PetAI(pCreature) {}

            void JustDied(Unit* killer)
            {
                if (me->isSummon())
                    if (Unit* owner = me->ToTempSummon()->GetSummoner())
                        if (owner->HasAura(GLYPH_OF_SHADOWFIEND))
                            owner->CastSpell(owner, GLYPH_OF_SHADOWFIEND_MANA, true);
                PetAI::JustDied(killer);
            }
        };

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_shadowfiendAI(pCreature);
        }
};

/*######
# npc_fire_elemental
######*/
enum FireElemental
{
    EVENT_SPELL_FIRENOVA        = 1,
    EVENT_SPELL_FIRESHIELD      = 2,
    EVENT_SPELL_FIREBLAST       = 3,
    EVENT_FE_CHECK_TARGET_RANGE = 4,

    SPELL_FIRENOVA              = 12470,
    SPELL_FIRESHIELD            = 13376,
    SPELL_FIREBLAST             = 57984,

    CREATURE_FIRETOTEM          = 15439
};

class npc_fire_elemental : public CreatureScript
{
    public:

    npc_fire_elemental() : CreatureScript("npc_fire_elemental") { }

    struct npc_fire_elementalAI : public ScriptedAI
    {
        public:

            npc_fire_elementalAI(Creature* pCreature) : ScriptedAI(pCreature) {}

            void InitializeAI() override
            {
                me->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_FIRE, true);
                rangeCheckTimer = 5 * IN_MILLISECONDS;
            }

            void Reset() override
            {
                eventMap.Reset();
                eventMap.ScheduleEvent(EVENT_FE_CHECK_TARGET_RANGE, 1 * IN_MILLISECONDS);
            }

            void EnterCombat(Unit* who) override
            {
                eventMap.ScheduleEvent(EVENT_SPELL_FIRENOVA, urand(5 * IN_MILLISECONDS, 20 * IN_MILLISECONDS));
                eventMap.ScheduleEvent(EVENT_SPELL_FIRESHIELD, 500);
                eventMap.ScheduleEvent(EVENT_SPELL_FIREBLAST, urand(5 * IN_MILLISECONDS, 20 * IN_MILLISECONDS));
            };

            void JustDied(Unit* killer) override
            {
                Unit* pTotem = NULL;
                if (pTotem = sObjectAccessor->GetCreature((*me), me->GetOwner()->m_SummonSlot[SUMMON_SLOT_TOTEM])) // Fire totem
                    pTotem->Kill(pTotem);
            };

            void UpdateAI(const uint32 diff) override
            {
                if (!me->GetOwner())
                {
                    me->DespawnOrUnsummon();
                    return;
                }

                rangeCheckTimer -= diff;
                if (rangeCheckTimer < diff)
                {
                    if (Creature* pCreatureTotem = sObjectAccessor->GetCreature((*me), me->GetOwner()->m_SummonSlot[SUMMON_SLOT_TOTEM])) // Fire totem
                    {
                        // Check range to totem
                        if (me->GetDistance(pCreatureTotem) > 30.0f && me->GetOwner()->GetDistance(pCreatureTotem) > 30.0f)
                        {
                            if (!me->HasUnitState(UNIT_STATE_ROOT | UNIT_STATE_STUNNED))
                            {
                                me->GetMotionMaster()->MovementExpired();
                                me->AddUnitState(UNIT_STATE_ROOT | UNIT_STATE_STUNNED);
                            }
                        }
                        else if (me->HasUnitState(UNIT_STATE_ROOT | UNIT_STATE_STUNNED))
                        {
                            me->ClearUnitState(UNIT_STATE_ROOT | UNIT_STATE_STUNNED);
                            EnterEvadeMode();
                        }
                    }
                    else
                        me->DespawnOrUnsummon();

                    rangeCheckTimer = 500;
                }

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

                DoMeleeAttackIfReady();
            };

        private:

            void executeEvent(uint32 eventID)
            {
                switch (eventID)
                {
                    case EVENT_SPELL_FIRENOVA:
                    {
                        DoCastAOE(SPELL_FIRENOVA);
                        eventMap.ScheduleEvent(EVENT_SPELL_FIRENOVA, urand(5 * IN_MILLISECONDS, 20 * IN_MILLISECONDS));
                    }break;

                    case EVENT_SPELL_FIRESHIELD:
                    {
                        DoCastAOE(SPELL_FIRESHIELD);
                        eventMap.ScheduleEvent(EVENT_SPELL_FIRESHIELD, 2 * IN_MILLISECONDS);
                    }break;

                    case EVENT_SPELL_FIREBLAST:
                    {
                        DoCastVictim(SPELL_FIREBLAST);
                        eventMap.ScheduleEvent(EVENT_SPELL_FIREBLAST, urand(5 * IN_MILLISECONDS, 20 * IN_MILLISECONDS));
                    }break;

                    case EVENT_FE_CHECK_TARGET_RANGE:
                    {
                        Unit* pUnitTarget = me->GetOwner()->getVictim();
                        if (me->GetOwner()->isInCombat() && pUnitTarget)
                        {
                            me->DeleteThreatList();
                            me->AI()->AttackStart(pUnitTarget);
                        }
                        else if (me->isInCombat())
                            EnterEvadeMode();

                        eventMap.ScheduleEvent(EVENT_FE_CHECK_TARGET_RANGE, 500);
                    }break;
                }
            }

            EventMap eventMap;
            uint32 rangeCheckTimer;
    };

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new npc_fire_elementalAI(pCreature);
    };
};

/*######
# npc_earth_elemental
######*/
enum EarthElemental
{
    EVENT_SPELL_ANGEREDEARTH        = 1,
    EVENT_EE_CHECK_TARGET_RANGE     = 2,

    SPELL_ANGEREDEARTH              = 36213,

    CREATURE_EARTHTOTEM             = 15430
};

class npc_earth_elemental : public CreatureScript
{
    public:

    npc_earth_elemental() : CreatureScript("npc_earth_elemental") { }

    struct npc_earth_elementalAI : public ScriptedAI
    {
        public:

            npc_earth_elementalAI(Creature* pCreature) : ScriptedAI(pCreature) {}

            void InitializeAI() override
            {
                me->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_NATURE, true);
                rangeCheckTimer = 5 * IN_MILLISECONDS;
            }

            void Reset() override
            {
                eventMap.Reset();
                eventMap.ScheduleEvent(EVENT_EE_CHECK_TARGET_RANGE, 1 * IN_MILLISECONDS);
            }

            void EnterCombat(Unit* who) override
            {
                eventMap.ScheduleEvent(EVENT_SPELL_ANGEREDEARTH, 500);
            };

            void JustDied(Unit* killer) override
            {
                Unit* pTotem = NULL;
                if (pTotem = sObjectAccessor->GetCreature((*me), me->GetOwner()->m_SummonSlot[TC_EARTH_TOTEM])) // Earth totem
                    pTotem->Kill(pTotem);
            };

            void UpdateAI(const uint32 diff) override
            {
                if (!me->GetOwner())
                {
                    me->DespawnOrUnsummon();
                    return;
                }

                rangeCheckTimer -= diff;
                if (rangeCheckTimer < diff)
                {
                    if (Creature* pCreatureTotem = sObjectAccessor->GetCreature((*me), me->GetOwner()->m_SummonSlot[TC_EARTH_TOTEM])) // Earth totem
                    {
                        // Check range to totem
                        if (me->GetDistance(pCreatureTotem) > 30.0f && me->GetOwner()->GetDistance(pCreatureTotem) > 30.0f)
                        {
                            if (!me->HasUnitState(UNIT_STATE_ROOT | UNIT_STATE_STUNNED))
                            {
                                me->GetMotionMaster()->MovementExpired();
                                me->AddUnitState(UNIT_STATE_ROOT | UNIT_STATE_STUNNED);
                            }
                        }
                        else if (me->HasUnitState(UNIT_STATE_ROOT | UNIT_STATE_STUNNED))
                        {
                            me->ClearUnitState(UNIT_STATE_ROOT | UNIT_STATE_STUNNED);
                            EnterEvadeMode();
                        }
                    }
                    else
                        me->DespawnOrUnsummon();

                    rangeCheckTimer = 500;
                }

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

                DoMeleeAttackIfReady();
            };

        private:

            void executeEvent(uint32 eventID)
            {
                switch (eventID)
                {
                    case EVENT_SPELL_ANGEREDEARTH:
                    {
                        DoCastVictim(SPELL_ANGEREDEARTH);
                        eventMap.ScheduleEvent(EVENT_SPELL_ANGEREDEARTH, urand(5 * IN_MILLISECONDS, 20 * IN_MILLISECONDS));
                    }break;

                    case EVENT_EE_CHECK_TARGET_RANGE:
                    {
                        Unit* pUnitTarget = me->GetOwner()->getVictim();
                        if (me->GetOwner()->isInCombat() && pUnitTarget)
                        {
                            me->DeleteThreatList();
                            me->AI()->AttackStart(pUnitTarget);
                        }
                        else if (me->isInCombat())
                            EnterEvadeMode();

                        eventMap.ScheduleEvent(EVENT_EE_CHECK_TARGET_RANGE, 500);
                    }break;
                }
            }

            EventMap eventMap;
            uint32 rangeCheckTimer;
    };

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new npc_earth_elementalAI(pCreature);
    }
};

/*######
## npc_guardian_risen_ghoul
######*/
enum RisenGhoul
{
    SPELL_GHOUL_JUMP                        = 47482,
    SPELL_GHOUL_CLAW                        = 47468,
    SPELL_GHOUL_GNAW                        = 47481,

    EVENT_GHOUL_CHECK_TARGET                = 1,
    EVENT_GHOUL_SPELL_JUMP                  = 2,
    EVENT_GHOUL_SPELL_CLAW                  = 3,
    EVENT_GHOUL_COOLDOWN_JUMP               = 4,
    EVENT_GHOUL_SPELL_GNAW                  = 5,

    EVENT_TIMER_GHOUL_CHECK_TARGET          = 500,
    EVENT_TIMER_GHOUL_COOLDOWN_SPELL_JUMP   = 20 * IN_MILLISECONDS,

    EVENT_TIMER_GHOUL_SPELL_CLAW_INIT       = 1 * IN_MILLISECONDS,
    EVENT_TIMER_GHOUL_SPELL_CLAW_MIN        = 4 * IN_MILLISECONDS,
    EVENT_TIMER_GHOUL_SPELL_CLAW_MAX        = 5 * IN_MILLISECONDS,

    EVENT_TIMER_GHOUL_SPELL_GNAW_MIN        = 20 * IN_MILLISECONDS,
    EVENT_TIMER_GHOUL_SPELL_GNAW_MAX        = 40 * IN_MILLISECONDS
};

const float MinRangeGhoulJump = 5.0f;
const float MaxRangeGhoulJump = 30.0f;

class npc_guardian_risen_ghoul : public CreatureScript
{
    public:

    npc_guardian_risen_ghoul() : CreatureScript("npc_guardian_risen_ghoul") { }

    struct npc_guardian_risen_ghoulAI : public ScriptedAI
    {
        public:

        npc_guardian_risen_ghoulAI(Creature* pCreature) : ScriptedAI(pCreature) { }

        void EnterCombat(Unit* /*who*/)
        {
            eventMap.Reset();
            eventMap.ScheduleEvent(EVENT_GHOUL_CHECK_TARGET, EVENT_TIMER_GHOUL_CHECK_TARGET);
            eventMap.ScheduleEvent(EVENT_GHOUL_SPELL_CLAW, EVENT_TIMER_GHOUL_SPELL_CLAW_INIT);
            eventMap.ScheduleEvent(EVENT_GHOUL_SPELL_GNAW, urand(EVENT_TIMER_GHOUL_SPELL_GNAW_MIN, EVENT_TIMER_GHOUL_SPELL_GNAW_MAX));
        }

        void UpdateAI(uint32 const diff) override
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

            DoMeleeAttackIfReady();
        }

        private:

            void executeEvent(uint32 eventID)
            {
                switch (eventID)
                {
                    case EVENT_GHOUL_CHECK_TARGET:
                    {
                        if (me->GetOwner())
                        {
                            Unit* pUnitTarget = me->GetOwner()->getVictim();
                            if (me->GetOwner()->isInCombat() && pUnitTarget)
                            {
                                me->DeleteThreatList();
                                me->AI()->AttackStart(pUnitTarget);

                                if (pUnitTarget->GetDistance(me) >= MinRangeGhoulJump
                                    && pUnitTarget->GetDistance(me) <= MaxRangeGhoulJump
                                    && !eventMap.IsEventActive(EVENT_GHOUL_COOLDOWN_JUMP))
                                {
                                    DoCast(pUnitTarget, SPELL_GHOUL_JUMP);
                                    eventMap.ScheduleEvent(EVENT_GHOUL_COOLDOWN_JUMP, EVENT_TIMER_GHOUL_COOLDOWN_SPELL_JUMP);
                                }
                            }
                            else if (me->isInCombat())
                                EnterEvadeMode();
                        }

                        eventMap.ScheduleEvent(EVENT_GHOUL_CHECK_TARGET, 500);
                    }break;

                    case EVENT_GHOUL_SPELL_CLAW:
                    {
                        DoCastVictim(SPELL_GHOUL_CLAW);
                        eventMap.ScheduleEvent(EVENT_GHOUL_SPELL_CLAW, urand(EVENT_TIMER_GHOUL_SPELL_CLAW_MIN, EVENT_TIMER_GHOUL_SPELL_CLAW_MAX));
                    }break;

                    case EVENT_GHOUL_SPELL_GNAW:
                    {
                        DoCastVictim(SPELL_GHOUL_GNAW);
                    }break;
                }
            }

            EventMap eventMap;
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_guardian_risen_ghoulAI(pCreature);
    }
};

/*######
## npc_army_of_the_dead
######*/
enum ArmyOfTheDead
{
    SPELL_AOTD_BIRTH                        = 7398,
    SPELL_AOTD_PERIODIC_TAUNT               = 43264,
};

class npc_army_of_the_dead : public CreatureScript
{
    public:

    npc_army_of_the_dead() : CreatureScript("npc_army_of_the_dead") { }

    struct npc_army_of_the_deadAI : public ScriptedAI
    {
        public:

            npc_army_of_the_deadAI(Creature* pCreature) : ScriptedAI(pCreature) { }

            void InitializeAI() override
            {
                DoCast(me, SPELL_AOTD_BIRTH, true);
                me->DelayedCastSpell(2 * IN_MILLISECONDS, me, SPELL_AOTD_PERIODIC_TAUNT);
            }

            void EnterCombat(Unit* /*who*/)
            {
                eventMap.Reset();
                eventMap.ScheduleEvent(EVENT_GHOUL_CHECK_TARGET, EVENT_TIMER_GHOUL_CHECK_TARGET);
                eventMap.ScheduleEvent(EVENT_GHOUL_SPELL_CLAW, EVENT_TIMER_GHOUL_SPELL_CLAW_INIT);
            }

            void UpdateAI(uint32 const diff) override
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

                DoMeleeAttackIfReady();
            }

        private:

            void executeEvent(uint32 eventID)
            {
                switch (eventID)
                {
                    case EVENT_GHOUL_CHECK_TARGET:
                    {
                        if (me->GetOwner()->isInCombat() && me->GetOwner()->getVictim())
                        {
                            Unit* pUnitTarget = me->GetOwner()->getVictim();
                            if (me->GetOwner()->isInCombat() && pUnitTarget)
                            {
                                me->DeleteThreatList();
                                me->AI()->AttackStart(pUnitTarget);

                                if (pUnitTarget->GetDistance(me) >= MinRangeGhoulJump
                                    && pUnitTarget->GetDistance(me) <= MaxRangeGhoulJump
                                    && !eventMap.IsEventActive(EVENT_GHOUL_COOLDOWN_JUMP))
                                {
                                    DoCast(pUnitTarget, SPELL_GHOUL_JUMP);
                                    eventMap.ScheduleEvent(EVENT_GHOUL_COOLDOWN_JUMP, EVENT_TIMER_GHOUL_COOLDOWN_SPELL_JUMP);
                                }
                            }
                        }

                        eventMap.ScheduleEvent(EVENT_GHOUL_CHECK_TARGET, 500);
                    }break;

                    case EVENT_GHOUL_SPELL_CLAW:
                    {
                        DoCastVictim(SPELL_GHOUL_CLAW);
                        eventMap.ScheduleEvent(EVENT_GHOUL_SPELL_CLAW, urand(EVENT_TIMER_GHOUL_SPELL_CLAW_MIN, EVENT_TIMER_GHOUL_SPELL_CLAW_MAX));
                    }break;
                }
            }

            EventMap eventMap;
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_army_of_the_deadAI(pCreature);
    }
};


void AddSC_npc_guardian()
{
    new npc_guardian();
    new npc_snake_trap();
    new npc_mirror_image();
    new npc_ebon_gargoyle();
    new npc_lightwell();
    new npc_shadowfiend();
    new npc_fire_elemental();
    new npc_earth_elemental();
    new npc_guardian_risen_ghoul();
    new npc_army_of_the_dead();
}
