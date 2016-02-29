/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
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

#include "ScriptPCH.h"
#include "pit_of_saron.h"
#include "Vehicle.h"

enum Spells
{
    SPELL_OVERLORD_BRAND            = 69172,
    SPELL_OVERLORD_BRAND_HEAL       = 69190,
    SPELL_OVERLORD_BRAND_DAMAGE     = 69189,
    SPELL_FORCEFUL_SMASH            = 69155,
    SPELL_UNHOLY_POWER              = 69167,
    SPELL_MARK_OF_RIMEFANG          = 69275,
    SPELL_HOARFROST                 = 69246,

    SPELL_ICY_BLAST                 = 69232,
    SPELL_ICY_BLAST_AURA            = 69238,

    SPELL_EJECT_ALL_PASSENGERS      = 50630,
    SPELL_FULL_HEAL                 = 43979,

    SPELL_ICICLE_FALL               = 62453,
    SPELL_FALL_DAMAGE               = 69425,
    SPELL_ICICLE                    = 69424,

    SPELL_SYLVANAS_CALL             = 70638,
    SPELL_SYLVANA_CALL_PORT         = 70639,

    SPELL_SINDRAGOSA_FROSTBOMB      = 70521
};

enum Events
{
    EVENT_OVERLORD_BRAND    = 1,
    EVENT_FORCEFUL_SMASH    = 2,
    EVENT_UNHOLY_POWER      = 3,
    EVENT_MARK_OF_RIMEFANG  = 4,

    // Rimefang
    EVENT_MOVE_NEXT         = 5,
    EVENT_HOARFROST         = 6,
    EVENT_ICY_BLAST         = 7,

    EVENT_INTRO_1           = 8,
    EVENT_INTRO_2           = 9,
    EVENT_INTRO_3           = 10,
    EVENT_COMBAT_START      = 11,

    // Sindragosa
    EVENT_END_KILL          = 12,
    // Gorkun
    EVENT_OUTRO_1           = 13,
    EVENT_OUTRO_2           = 14,
    // End Teamleader
    EVENT_LEADER_OUTRO_1    = 15,
    EVENT_LEADER_SAVE       = 16,
    EVENT_OUTRO_3           = 17,
    // Sindragosa kill
    EVENT_FROSTBOMB         = 18,
    EVENT_END_AFTER         = 19
};

enum Phases
{
    PHASE_NONE      = 0,
    PHASE_INTRO     = 1,
    PHASE_COMBAT    = 2,
    PHASE_OUTRO     = 3,
};

enum Actions
{
    ACTION_START_INTRO      = 1,
    ACTION_START_RIMEFANG   = 2,
    ACTION_START_OUTRO      = 3,
    ACTION_END_COMBAT       = 4,
    ACTION_SAVE_PLAYERS     = 5,
    ACTION_FROSTBOMB        = 6
};

enum Points 
{
    POINT_SINDRA_TOP        = 1,
    POINT_SINDRA_END        = 2
};

#define GUID_HOARFROST 1

static const Position rimefangPos[10] =
{
    {1017.299f, 168.9740f, 642.9259f, 0.000000f},
    {1047.868f, 126.4931f, 665.0453f, 0.000000f},
    {1069.828f, 138.3837f, 665.0453f, 0.000000f},
    {1063.042f, 164.5174f, 665.0453f, 0.000000f},
    {1031.158f, 195.1441f, 665.0453f, 0.000000f},
    {1019.087f, 197.8038f, 665.0453f, 0.000000f},
    {967.6233f, 168.9670f, 665.0453f, 0.000000f},
    {969.1198f, 140.4722f, 665.0453f, 0.000000f},
    {986.7153f, 141.6424f, 665.0453f, 0.000000f},
    {1012.601f, 142.4965f, 665.0453f, 0.000000f},
};

static const Position sindraPosEnd = {895.892761f, 164.136551f, 658.2337f, 0.0f}; // Sindragosa end pos
static const Position sindraPosEnd2 = {895.8927f, 164.1365f, 683.2337f, 0.0f}; // Sindra after killing leader
static const Position miscPos = {1018.376f, 167.2495f, 628.2811f, 0.000000f};   //tyrannus combat start position

class boss_tyrannus : public CreatureScript
{
    public:
        boss_tyrannus() : CreatureScript("boss_tyrannus") { }

        struct boss_tyrannusAI : public BossAI
        {
            boss_tyrannusAI(Creature* creature) : BossAI(creature, DATA_TYRANNUS)
            {
                _guidSylvanasJaina = 0;
            }

            void InitializeAI()
            {
                if (!instance || static_cast<InstanceMap*>(me->GetMap())->GetScriptId() != sObjectMgr->GetScriptId(PoSScriptName))
                    me->IsAIEnabled = false;
                else if (instance->GetBossState(DATA_TYRANNUS) != DONE)
                    Reset();
                else
                    me->DespawnOrUnsummon();
            }

            void Reset()
            {
                events.Reset();
                events.SetPhase(PHASE_NONE);
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                instance->SetBossState(DATA_TYRANNUS, NOT_STARTED);
            }

            Creature* GetRimefang()
            {
                return ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_RIMEFANG));
            }

            void EnterCombat(Unit* /*who*/)
            {
                DoScriptText(SAY_AGGRO, me);
            }

            void AttackStart(Unit* victim)
            {
                if (me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
                    return;

                if (victim && me->Attack(victim, true) && !(events.GetPhaseMask() & (1 << (PHASE_INTRO - 1))))
                    me->GetMotionMaster()->MoveChase(victim);
            }

            void JustSummoned (Creature* creature)
            {
                switch (creature->GetEntry())
                {
                    case NPC_JAINA_PART2:
                    case NPC_SYLVANAS_PART2:
                        _guidSylvanasJaina = creature->GetGUID();
                        break;
                    default: 
                        break;
                }
            }

            uint64 GetGUID(int32 data = 0)
            {
                switch (data)
                {
                    case DATA_JAINA_SYLVANAS_2:
                        return _guidSylvanasJaina;
                    default:
                        break;
                }
                return 0;
            }

            void EnterEvadeMode()
            {
                instance->SetBossState(DATA_TYRANNUS, FAIL);
                if (Creature* rimefang = GetRimefang())
                    rimefang->AI()->EnterEvadeMode();

                me->DespawnOrUnsummon();
            }

            void KilledUnit(Unit* victim)
            {
                if (victim->GetTypeId() == TYPEID_PLAYER)
                    DoScriptText(RAND(SAY_SLAY_1, SAY_SLAY_2), me);
            }

            void JustDied(Unit* /*killer*/)
            {
                DoScriptText(SAY_DEATH, me);
                instance->SetBossState(DATA_TYRANNUS, DONE);

                // Prevent corpse despawning
                if (TempSummon* summ = me->ToTempSummon())
                    summ->SetTempSummonType(TEMPSUMMON_DEAD_DESPAWN);

                // Stop combat for Rimefang
                if (Creature* rimefang = GetRimefang())
                    rimefang->AI()->DoAction(ACTION_END_COMBAT);
            }

            void DoAction(const int32 actionId)
            {
                if (actionId == ACTION_START_INTRO)
                {
                    if (events.GetPhaseMask() & (1 << (PHASE_INTRO - 1)))
                        return;
                    DoScriptText(SAY_TYRANNUS_INTRO_1, me);
                    events.SetPhase(PHASE_INTRO);
                    events.ScheduleEvent(EVENT_INTRO_1, 14000, 0, PHASE_INTRO);
                    events.ScheduleEvent(EVENT_INTRO_2, 22000, 0, PHASE_INTRO);
                    events.ScheduleEvent(EVENT_INTRO_3, 34000, 0, PHASE_INTRO);
                    events.ScheduleEvent(EVENT_COMBAT_START, 36000, 0, PHASE_INTRO);
                    instance->SetBossState(DATA_TYRANNUS, IN_PROGRESS);
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim() && !(events.GetPhaseMask() & (1 << (PHASE_INTRO - 1))))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_INTRO_1:
                            //DoScriptText(SAY_GORKUN_INTRO_2, pGorkunOrVictus);
                            break;
                        case EVENT_INTRO_2:
                            DoScriptText(SAY_TYRANNUS_INTRO_3, me);
                            break;
                        case EVENT_INTRO_3:
                            if (Creature* rimefang = me->GetCreature(*me, instance->GetData64(DATA_RIMEFANG)))
                                rimefang->AI()->DoCast(SPELL_EJECT_ALL_PASSENGERS);
                            me->GetMotionMaster()->MovePoint(0, miscPos);
                            break;
                        case EVENT_COMBAT_START:
                            if (Creature* rimefang = me->GetCreature(*me, instance->GetData64(DATA_RIMEFANG)))
                                rimefang->AI()->DoAction(ACTION_START_RIMEFANG);    //set rimefang also infight
                            events.SetPhase(PHASE_COMBAT);
                            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                            me->SetReactState(REACT_AGGRESSIVE);
                            DoCast(me, SPELL_FULL_HEAL);
                            DoZoneInCombat();
                            events.ScheduleEvent(EVENT_OVERLORD_BRAND, urand(5000, 7000));
                            events.ScheduleEvent(EVENT_FORCEFUL_SMASH, urand(14000, 16000));
                            events.ScheduleEvent(EVENT_MARK_OF_RIMEFANG, urand(25000, 27000));
                            break;
                        case EVENT_OVERLORD_BRAND:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 0.0f, true))
                                DoCast(target, SPELL_OVERLORD_BRAND);
                            events.ScheduleEvent(EVENT_OVERLORD_BRAND, urand(11000, 12000));
                            break;
                        case EVENT_FORCEFUL_SMASH:
                            DoCastVictim(SPELL_FORCEFUL_SMASH);
                            events.ScheduleEvent(EVENT_UNHOLY_POWER, 1000);
                            break;
                        case EVENT_UNHOLY_POWER:
                            DoScriptText(SAY_DARK_MIGHT_1, me);
                            DoScriptText(SAY_DARK_MIGHT_2, me);
                            DoCast(me, SPELL_UNHOLY_POWER);
                            events.ScheduleEvent(EVENT_FORCEFUL_SMASH, urand(40000, 48000));
                            break;
                        case EVENT_MARK_OF_RIMEFANG:
                            DoScriptText(SAY_MARK_RIMEFANG_1, me);
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 0.0f, true))
                            {
                                DoScriptText(SAY_MARK_RIMEFANG_2, me, target);
                                DoCast(target, SPELL_MARK_OF_RIMEFANG);
                            }
                            events.ScheduleEvent(EVENT_MARK_OF_RIMEFANG, urand(24000, 26000));
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
            private:
                uint64 _guidSylvanasJaina;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_tyrannusAI(creature);
        }
};

class boss_rimefang : public CreatureScript
{
    public:
        boss_rimefang() : CreatureScript("boss_rimefang") { }

        struct boss_rimefangAI : public ScriptedAI
        {
            boss_rimefangAI(Creature* creature) : ScriptedAI(creature), _vehicle(creature->GetVehicleKit())
            {
                ASSERT(_vehicle);
            }

            void Reset()
            {
                _events.Reset();
                _events.SetPhase(PHASE_NONE);
                _currentWaypoint = 0;
                _hoarfrostTargetGUID = 0;
                me->SetCanFly(true);
                me->SetHover(true);
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            }

            void JustReachedHome()
            {
                _vehicle->InstallAllAccessories(false);
            }

            void DoAction(const int32 actionId)
            {
                if (actionId == ACTION_START_RIMEFANG)
                {
                    _events.SetPhase(PHASE_COMBAT);
                    DoZoneInCombat();
                    _events.ScheduleEvent(EVENT_MOVE_NEXT, 500, 0, PHASE_COMBAT);
                    _events.ScheduleEvent(EVENT_ICY_BLAST, 15000, 0, PHASE_COMBAT);
                }
                else if (actionId == ACTION_END_COMBAT)
                {
                    _EnterEvadeMode();
                    me->DespawnOrUnsummon(1000);
                }                  
            }

            void SetGUID(uint64 guid, int32 type)
            {
                if (type == GUID_HOARFROST)
                {
                    _hoarfrostTargetGUID = guid;
                    _events.ScheduleEvent(EVENT_HOARFROST, 1000);
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim() && !(_events.GetPhaseMask() & (1 << (PHASE_COMBAT - 1))))
                    return;

                _events.Update(diff);

                while (uint32 eventId = _events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_MOVE_NEXT:
                            if (_currentWaypoint >= 10 || _currentWaypoint == 0)
                                _currentWaypoint = 1;
                            me->GetMotionMaster()->MovePoint(0, rimefangPos[_currentWaypoint]);
                            ++_currentWaypoint;
                            _events.ScheduleEvent(EVENT_MOVE_NEXT, 2000, 0, PHASE_COMBAT);
                            break;
                        case EVENT_ICY_BLAST:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                                DoCast(target, SPELL_ICY_BLAST);
                            _events.ScheduleEvent(EVENT_ICY_BLAST, 15000, 0, PHASE_COMBAT);
                            break;
                        case EVENT_HOARFROST:
                            if (Unit* target = me->GetUnit(*me, _hoarfrostTargetGUID))
                            {
                                DoCast(target, SPELL_HOARFROST);
                                _hoarfrostTargetGUID = 0;
                            }
                            break;
                        default:
                            break;
                    }
                }
            }

        private:
            Vehicle* _vehicle;
            uint64 _hoarfrostTargetGUID;
            EventMap _events;
            uint8 _currentWaypoint;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_rimefangAI(creature);
        }
};

class player_overlord_brandAI : public PlayerAI
{
    public:
        player_overlord_brandAI(Player* player) : PlayerAI(player)
        {
            tyrannus = NULL;
        }

        void SetGUID(uint64 guid, int32 /*type*/)
        {
            tyrannus = ObjectAccessor::GetCreature(*me, guid);
            me->IsAIEnabled = tyrannus != NULL;
        }

        void DamageDealt(Unit* /*victim*/, uint32& damage, DamageEffectType /*damageType*/)
        {
            if (tyrannus->getVictim())
                me->CastCustomSpell(SPELL_OVERLORD_BRAND_DAMAGE, SPELLVALUE_BASE_POINT0, damage, tyrannus->getVictim(), TRIGGERED_FULL_MASK, NULL, NULL, tyrannus->GetGUID());
        }

        void HealDone(Unit* /*target*/, uint32& addHealth)
        {
            me->CastCustomSpell(SPELL_OVERLORD_BRAND_HEAL, SPELLVALUE_BASE_POINT0, int32(addHealth*5.5f), tyrannus, TRIGGERED_FULL_MASK, NULL, NULL, tyrannus->GetGUID());
        }

        void UpdateAI(const uint32 /*diff*/) { }

    private:
        Creature* tyrannus;
};

class spell_tyrannus_overlord_brand : public SpellScriptLoader
{
    public:
        spell_tyrannus_overlord_brand() : SpellScriptLoader("spell_tyrannus_overlord_brand") { }

        class spell_tyrannus_overlord_brand_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_tyrannus_overlord_brand_AuraScript);

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (GetTarget()->GetTypeId() != TYPEID_PLAYER)
                    return;

                oldAI = GetTarget()->GetAI();
                oldAIState = GetTarget()->IsAIEnabled;
                GetTarget()->SetAI(new player_overlord_brandAI(GetTarget()->ToPlayer()));
                GetTarget()->GetAI()->SetGUID(GetCasterGUID());
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (GetTarget()->GetTypeId() != TYPEID_PLAYER)
                    return;

                delete GetTarget()->GetAI();
                GetTarget()->SetAI(oldAI);
                GetTarget()->IsAIEnabled = oldAIState;
            }

            void Register()
            {
                AfterEffectApply += AuraEffectApplyFn(spell_tyrannus_overlord_brand_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_tyrannus_overlord_brand_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }

            UnitAI* oldAI;
            bool oldAIState;
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_tyrannus_overlord_brand_AuraScript();
        }
};

class spell_tyrannus_mark_of_rimefang : public SpellScriptLoader
{
    public:
        spell_tyrannus_mark_of_rimefang() : SpellScriptLoader("spell_tyrannus_mark_of_rimefang") { }

        class spell_tyrannus_mark_of_rimefang_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_tyrannus_mark_of_rimefang_AuraScript);

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* caster = GetCaster();
                if (!caster || caster->GetTypeId() != TYPEID_UNIT)
                    return;

                if (InstanceScript* instance = caster->GetInstanceScript())
                    if (Creature* rimefang = ObjectAccessor::GetCreature(*caster, instance->GetData64(DATA_RIMEFANG)))
                        rimefang->AI()->SetGUID(GetTarget()->GetGUID(), GUID_HOARFROST);
            }

            void Register()
            {
                AfterEffectApply += AuraEffectApplyFn(spell_tyrannus_mark_of_rimefang_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_tyrannus_mark_of_rimefang_AuraScript();
        }
};



class npc_tyrannus_icicle_spawner : public CreatureScript
{
public:
    npc_tyrannus_icicle_spawner() : CreatureScript("npc_tyrannus_icicle_spawner") { }

    struct npc_tyrannus_icicle_spawnerAI : public ScriptedAI
    {
        npc_tyrannus_icicle_spawnerAI(Creature* c) : ScriptedAI(c)
        {
            pInstance = c->GetInstanceScript();
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_PACIFIED | UNIT_FLAG_NOT_SELECTABLE);
            me->SetReactState(REACT_PASSIVE);
        }

        void Reset()
        {
            IcicleTimer = urand(2000, 10000);
        }

        void UpdateAI(const uint32 diff)
        {
            if (pInstance->GetData(DATA_TYRANNUS_EVENT) == NOT_STARTED)
            {
                if (IcicleTimer <= uint32(diff))
                {
                    std::list<Creature*> targets;
                    me->GetCreatureListWithEntryInGrid(targets, NPC_ICICLE_TRIGGER, 100.f);
                    Trinity::Containers::RandomResizeList(targets, 1);
                    targets.front()->CastSpell(targets.front(), SPELL_ICICLE);
                    IcicleTimer = urand(2000, 10000);
                }
                else
                    IcicleTimer -= diff;
            }
        }

    private:
        InstanceScript* pInstance;
        uint32 IcicleTimer;
    };

    ScriptedAI* GetAI(Creature* pCreature) const
    {
        return new npc_tyrannus_icicle_spawnerAI(pCreature);
    }
};

class npc_tyrannus_icicle : public CreatureScript
{
public:
    npc_tyrannus_icicle() : CreatureScript("npc_tyrannus_icicle") { }
    
    struct npc_tyrannus_icicleAI : public ScriptedAI
    {
        npc_tyrannus_icicleAI(Creature* creature) : ScriptedAI(creature)
        {
            me->SetDisplayId(me->GetCreatureTemplate()->Modelid1);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_PACIFIED | UNIT_FLAG_NOT_SELECTABLE);
            me->SetReactState(REACT_PASSIVE);
        }

        uint32 timer;

        void Reset()
        {
            timer = 4500;
        }

        void UpdateAI(const uint32 diff)
        {
            if (timer <= diff)
            {
                me->CastSpell(me, SPELL_ICICLE_FALL);
                me->CastSpell(me, SPELL_FALL_DAMAGE, true);
                timer = 10000;
            }
            else
                timer -= diff;
        }
    };
    
    ScriptedAI* GetAI(Creature* creature) const
    {
        return new npc_tyrannus_icicleAI(creature);
    };
};

class at_tyrannus_event_starter : public AreaTriggerScript
{
    public:
        at_tyrannus_event_starter() : AreaTriggerScript("at_tyrannus_event_starter") { }

        bool OnTrigger(Player* player, const AreaTriggerEntry* /*at*/)
        {
            InstanceScript* instance = player->GetInstanceScript();
            if (player->isGameMaster() || !instance)
                return false;

            if (instance->GetBossState(DATA_TYRANNUS) != IN_PROGRESS && instance->GetBossState(DATA_TYRANNUS) != DONE)
                if (Creature* tyrannus = ObjectAccessor::GetCreature(*player, instance->GetData64(DATA_TYRANNUS)))
                {
                    instance->SetData(DATA_TYRANNUS_EVENT, IN_PROGRESS);
                    tyrannus->AI()->DoAction(ACTION_START_INTRO);
                    return true;
                }

            return false;
        }
};

class npc_sindragosa_pit : public CreatureScript
{
    public:
        npc_sindragosa_pit() : CreatureScript("npc_sindragosa_pit") { }

        struct npc_sindragosa_pitAI : public ScriptedAI
        {
            npc_sindragosa_pitAI(Creature* creature) : ScriptedAI(creature) { }
           
            void Reset()
            {
                _events.Reset();
                me->SetCanFly(true);
                me->SetHover(true);
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                _events.ScheduleEvent(EVENT_END_KILL, 20000);
            }

            void UpdateAI(const uint32 diff)
            {
                _events.Update(diff);

                while (uint32 eventId = _events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_END_KILL:
                            me->GetMotionMaster()->MovePoint(POINT_SINDRA_TOP, sindraPosEnd);
                            _events.ScheduleEvent(EVENT_END_AFTER, 30000);
                            break;
                        case EVENT_END_AFTER:
                            me->GetMotionMaster()->MovePoint(POINT_SINDRA_END, sindraPosEnd2);
                            me->DespawnOrUnsummon(4000);
                            break;
                        default:
                            break;
                    }
                }
            }
        private:
            EventMap _events;
        };

        ScriptedAI* GetAI(Creature* creature) const
        {
            return new npc_sindragosa_pitAI(creature);
        }
};

class npc_gorkun_pit : public CreatureScript
{
    public:
        npc_gorkun_pit() : CreatureScript("npc_gorkun_pit") { }

        struct npc_gorkun_pitAI : public ScriptedAI
        {
            npc_gorkun_pitAI(Creature* creature) : ScriptedAI(creature)
            {
                instance = creature->GetInstanceScript();
            }
           
            void Reset()
            {
                _events.Reset();
                _events.ScheduleEvent(EVENT_OUTRO_1, 10000);
            }

            void UpdateAI(const uint32 diff)
            {
                _events.Update(diff);

                while (uint32 eventId = _events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_OUTRO_1:
                            DoScriptText(SAY_GORKUN_OUTRO_1, me);
                            _events.ScheduleEvent(EVENT_OUTRO_2, 18000);
                            break;
                        case EVENT_OUTRO_2:
                            DoScriptText(SAY_GORKUN_OUTRO_2, me);
                            _events.ScheduleEvent(EVENT_OUTRO_3, 10000);
                            break;
                        case EVENT_OUTRO_3:
                            if (Creature* tyrannus = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_TYRANNUS)))
                            {
                                if (Creature* jainasylvana = ObjectAccessor::GetCreature(*me, tyrannus->AI()->GetGUID(DATA_JAINA_SYLVANAS_2)))
                                    jainasylvana->AI()->DoAction(ACTION_SAVE_PLAYERS);
                            }
                            _events.ScheduleEvent(EVENT_FROSTBOMB, 5000);
                            break;
                        case EVENT_FROSTBOMB:
                            me->CastSpell(me, SPELL_SINDRAGOSA_FROSTBOMB);
                            break;
                        default:
                            break;
                    }
                }
            }
        private:
            EventMap _events;
            InstanceScript* instance;
        };

        ScriptedAI* GetAI(Creature* creature) const
        {
            return new npc_gorkun_pitAI(creature);
        }
};

class npc_teamleader_end_pit : public CreatureScript
{
    public:
        npc_teamleader_end_pit() : CreatureScript("npc_teamleader_end_pit") { }

        struct npc_teamleader_end_pitAI : public ScriptedAI
        {
            npc_teamleader_end_pitAI(Creature* creature) : ScriptedAI(creature) 
            {
            }

            void DoAction(int32 const actionId)
            {
                if (actionId == ACTION_SAVE_PLAYERS)
                {
                    _events.Reset();
                    _events.ScheduleEvent(EVENT_LEADER_OUTRO_1, 1000);
                }
            }

            void UpdateAI(const uint32 diff)
            {
                _events.Update(diff);

                while (uint32 eventId = _events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_LEADER_OUTRO_1:
                            _teamInInstance = me->GetInstanceScript()->GetData(DATA_TEAM_IN_INSTANCE);
                            if (_teamInInstance == HORDE)
                                DoScriptText(SAY_SYLVANAS_OUTRO_3, me);
                            else
                                DoScriptText(SAY_JAYNA_OUTRO_3, me);
                            DoCastAOE(SPELL_SYLVANAS_CALL, true); // LoS check
                            _events.ScheduleEvent(EVENT_LEADER_SAVE, 1000);
                            break;
                        case EVENT_LEADER_SAVE:
                            break;
                        default:
                            break;
                    }
                }
            }
        private:
            EventMap _events;
            uint32 _teamInInstance;
        };
        
        ScriptedAI* GetAI(Creature* creature) const
        {
            return new npc_teamleader_end_pitAI(creature);
        }
};

class spell_call_of_sylvanas : public SpellScriptLoader
{
    public:
        spell_call_of_sylvanas() : SpellScriptLoader("spell_call_of_sylvanas") { }

        class spell_call_of_sylvanas_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_call_of_sylvanas_SpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                if (Unit* hitUnit = GetHitUnit())
                {
                    caster->CastSpell(hitUnit, SPELL_SYLVANA_CALL_PORT);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_call_of_sylvanas_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_call_of_sylvanas_SpellScript();
        }
};

void AddSC_boss_tyrannus()
{
    new boss_tyrannus();
    new boss_rimefang();
    new spell_tyrannus_overlord_brand();
    new spell_tyrannus_mark_of_rimefang();
    new at_tyrannus_event_starter();
    new npc_tyrannus_icicle_spawner();
    new npc_tyrannus_icicle();
    new npc_sindragosa_pit();
    new npc_gorkun_pit();
    new npc_teamleader_end_pit();
    new spell_call_of_sylvanas();
}
