
#include "ScriptPCH.h"
#include "ScriptedCreature.h"
#include "ScriptMgr.h"


#define MAX_EVENT_TIME  300 * IN_MILLISECONDS

enum Says
{
    HEADLESS_HORSEMAN_SAY_SPAWN                 =-10040,
    HEADLESS_HORSEMAN_SAY_START                 =-10041,
    HEADLESS_HORSEMAN_SAY_RESIZE                =-10042,
    HEADLESS_HORSEMAN_SAY_WIN                   =-10043,
    HEADLESS_HORSEMAN_SAY_FAIL                  =-10044
};

enum NPCs
{
    HEADLESS_HORSEMAN_NPC_HORSEMAN              =24700,
    HEADLESS_HORSEMAN_NPC_FIRE_TRIGGER          =23537
};

enum Items
{
    HEADLESS_HORSEMAN_ITEM_WATER                =32971
};

enum Quests
{
    HEADLESS_HORSEMAN_QUEST1                    =11131,
    HEADLESS_HORSEMAN_QUEST2                    =11219
};

enum Spells
{
    HEADLESS_HORSEMAN_SPELL_QUEST_COMPLETED     =42242,
    HEADLESS_HORSEMAN_SPELL_TALK                =48149,
    HEADLESS_HORSEMAN_SPELL_FIRE                =42074,
    HEADLESS_HORSEMAN_SPELL_FIRE_SIZE           =42091,
    HEADLESS_HORSEMAN_SPELL_FIRE_THROW          =42443
};

enum Datas
{
    HEADLESS_HORSEMAN_DATA_ZONE                 = 1
};

enum Events
{
    HEADLESS_HORSEMAN_EVENT_START_MOVE          = 1,
    HEADLESS_HORSEMAN_EVENT_MOVE_TARGET         = 2,
    HEADLESS_HORSEMAN_EVENT_SPAWN_FIRE          = 3,
    HEADLESS_HORSEMAN_EVENT_LIGHT_FIRE          = 4,
    HEADLESS_HORSEMAN_EVENT_CAST_FIRE           = 5,
    HEADLESS_HORSEMAN_EVENT_RESIZE_FIRE         = 6,
    HEADLESS_HORSEMAN_EVENT_DESPAWN             = 7,
    HEADLESS_HORSEMAN_CHECK_FIRE                = 8
};

enum Misc
{
    HEADLESS_HORSEMAN_FIRE_COUNT                = 4,
    HEADLESS_HORSEMAN_FIRE_RESIZE_COUNT         = 100
};

enum Zones
{
    HEADLESS_HORSEMAN_ZONE_BRILL                =85,
    HEADLESS_HORSEMAN_ZONE_RAZORHILL            =14,
    HEADLESS_HORSEMAN_ZONE_SILVERMOON           =3430,
    HEADLESS_HORSEMAN_ZONE_GOLDHAIN             =12,
    HEADLESS_HORSEMAN_ZONE_IRONFORGE            =1,
    HEADLESS_HORSEMAN_ZONE_EXODAR               =3524
};

enum Pos
{
    HEADLESS_HORSEMAN_POS_SPAWN                 =1,
    HEADLESS_HORSEMAN_POS_WP                    =2,
    HEADLESS_HORSEMAN_POS_TARGET                =3
};

const Position FireSpawnLocations[6][HEADLESS_HORSEMAN_FIRE_COUNT] =
{
    {
        {2239.656006f, 305.661377f, 36.665981f, 5.617225f},
        {2259.366943f, 320.522766f, 41.653355f, 5.526774f},
        {2295.410156f, 265.975159f, 50.316811f, 2.952716f},
        {2248.789551f, 252.674332f, 48.182091f, 2.907555f},
    },
    {
        {361.802673f,  -4764.839844f, 17.443691f, 2.334731f},
        {305.809052f, -4775.622070f, 9.945764f, 1.696277f},
        {273.512817f, -4707.836914f, 22.406017f, 5.671266f},
        {329.939850f, -4707.298340f, 15.528252f, 4.110722f},
    },
    {
        {9488.370117f, -6839.306152f, 31.745640f, 0.772295f},
        {9525.971680f, -6861.679199f, 32.321358f, 1.402060f},
        {9526.700195f, -6861.923340f, 18.740734f, 1.539199f},
        {9476.559570f, -6806.891602f, 22.024618f, 0.263533f},
    },
    {
        {-9459.001953f, 43.957722f, 64.224106f, 1.536983f},
        {-9460.256836f, 82.026772f, 58.278679f, 4.883872f},
        {-9464.954102f, 83.417397f, 67.053688f, 4.904817f},
        {-9495.390625f, 83.587860f, 60.462425f, 5.832982f},
    },
    {
        {-5592.073730f, -511.133972f, 404.096069f, 3.150571f},
        {-5606.240234f, -514.467346f, 413.353638f, 1.680350f},
        {-5586.816895f, -462.303162f, 414.487122f, 4.359867f},
        {-5634.472656f, -500.472992f, 401.364960f, 2.045778f},
    },
    {
        {-4214.068848f, -12515.554688f, 54.943645f, 0.431931f},
        {-4149.776367f, -12530.673828f, 54.712124f, 2.349608f},
        {-4146.563477f, -12483.081055f, 59.877800f, 4.036863f},
        {-4205.534180f, -12459.577148f, 45.622314f, 5.319854f},
    }
};

Position const WaypointLocations[6][3] =
{
    {
        {2219.148926f, 254.338745f, 34.047642f, 0.711897f},
        {2246.051758f, 272.910370f, 34.113792f, 0.887521f},
        {2262.387695f, 299.247009f, 34.113693f, 4.018643f},
    },
    {
        {260.032349f, -4641.821777f, 11.811209f, 5.233810f},
        {302.676392f, -4706.388184f, 11.162858f, 5.294023f},
        {311.648560f, -4742.500488f, 9.562349f, 6.232355f},
    },
    {
        {9465.781250f, -6780.749023f, 16.494684f, 6.186916f},
        {9490.596680f, -6794.307617f, 16.494684f, 5.459333f},
        {9504.250000f, -6823.385742f, 16.494011f, 5.248585f},
    },
    {
        {-9505.628906f, 34.242390f, 56.426086f, 1.161816f},
        {-9487.478516f, 67.329453f, 56.104767f, 6.235705f},
        {-9463.003906f, 63.226692f, 55.852245f, 6.170473f},
    },
    {
        {-5636.586426f, -483.960876f, 396.554047f, 6.255342f},
        {-5592.209473f, -481.419128f, 396.980530f, 0.052441f},
        {-5571.517090f, -479.885620f, 397.639832f, 3.462545f},
    },
    {
        {-4188.634277f, -12501.792969f, 44.362236f, 2.219295f},
        {-4203.881836f, -12499.188477f, 44.429306f, 5.987679f},
        {-4190.504395f, -12521.936523f, 44.405033f, 1.415569f},
    }
};

static bool headlessHorsemanRunning[6];

class npc_Headless_Horseman_Rotten_Hollow : public CreatureScript
{
    public:

    npc_Headless_Horseman_Rotten_Hollow() : CreatureScript("npc_Headless_Horseman_Rotten_Hollow") { }

    struct npc_Headless_Horseman_Rotten_Hollow_AI : public ScriptedAI
    {
        npc_Headless_Horseman_Rotten_Hollow_AI(Creature* creature) : ScriptedAI(creature) { }

        void Reset() override
        {
            eventFailed = false;

            fireSpawnCount = NULL;
            fireSizeCount = NULL;
            zonearray = NULL;

            me->SetWalk(false);

            me->Yell(HEADLESS_HORSEMAN_SAY_SPAWN, LANG_UNIVERSAL, 0);
            me->CastSpell(me, HEADLESS_HORSEMAN_SPELL_TALK);

            eventMap.ScheduleEvent(HEADLESS_HORSEMAN_EVENT_START_MOVE, 5000);
            eventMap.ScheduleEvent(HEADLESS_HORSEMAN_EVENT_DESPAWN, MAX_EVENT_TIME);
        }

        void SetData(uint32 id, uint32 data) override
        {
            switch (id)
            {
                case HEADLESS_HORSEMAN_DATA_ZONE:
                    zonearray = data;
                    break;
            }
        }

        void MovementInform(uint32 type, uint32 id) override
        {
            if (type != POINT_MOTION_TYPE)
                return;

            switch (id)
            {
                case HEADLESS_HORSEMAN_POS_WP:
                {
                    eventMap.ScheduleEvent(HEADLESS_HORSEMAN_EVENT_MOVE_TARGET, 100);
                }break;

                case HEADLESS_HORSEMAN_POS_TARGET:
                {
                    me->Yell(HEADLESS_HORSEMAN_SAY_START, LANG_UNIVERSAL, 0);
                    eventMap.ScheduleEvent(HEADLESS_HORSEMAN_EVENT_SPAWN_FIRE, 1000);
                }break;
            }
        }

        void UpdateAI(const uint32 diff) override
        {
            eventMap.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (uint32 eventID = eventMap.ExecuteEvent())
            {
                executeEvent(eventID);
                return;
            }
        }

    private:

        void executeEvent(uint32 eventID)
        {
            switch (eventID)
            {
                case HEADLESS_HORSEMAN_EVENT_START_MOVE:
                    me->GetMotionMaster()->MovePoint(HEADLESS_HORSEMAN_POS_WP, WaypointLocations[zonearray][1]);
                    break;

                case HEADLESS_HORSEMAN_EVENT_MOVE_TARGET:
                    me->GetMotionMaster()->MovePoint(HEADLESS_HORSEMAN_POS_TARGET, WaypointLocations[zonearray][2]);
                    break;

                case HEADLESS_HORSEMAN_EVENT_SPAWN_FIRE:
                {
                    for (int i = 0; i < HEADLESS_HORSEMAN_FIRE_COUNT; ++i)
                        if (Creature* pCreature = me->SummonCreature(HEADLESS_HORSEMAN_NPC_FIRE_TRIGGER, FireSpawnLocations[zonearray][i]))
                            fireTriggerGUID[i] = pCreature->GetGUID();

                    eventMap.ScheduleEvent(HEADLESS_HORSEMAN_EVENT_LIGHT_FIRE, 1 * IN_MILLISECONDS);
                }break;

                case HEADLESS_HORSEMAN_EVENT_LIGHT_FIRE:
                {
                    if (fireSpawnCount >= HEADLESS_HORSEMAN_FIRE_COUNT)
                    {
                        me->Yell(HEADLESS_HORSEMAN_SAY_RESIZE, LANG_UNIVERSAL, 0);
                        eventMap.ScheduleEvent(HEADLESS_HORSEMAN_EVENT_RESIZE_FIRE, 1000);
                    }
                    else
                    {
                        if (Creature* pCreature = sObjectAccessor->GetCreature(*me, fireTriggerGUID[fireSpawnCount]))
                        {
                            me->SetFacingToObject(pCreature);
                            me->CastSpell(pCreature, HEADLESS_HORSEMAN_SPELL_FIRE_THROW, TRIGGERED_NONE, NULL, NULL, pCreature->GetGUID());
                        }

                        ++fireSpawnCount;
                        eventMap.ScheduleEvent(HEADLESS_HORSEMAN_EVENT_LIGHT_FIRE, 5000);
                    }
                    break;
                }

                case HEADLESS_HORSEMAN_EVENT_RESIZE_FIRE:
                {
                    uint8 random = urand(0, 3);
                    if (Creature* pCreature = sObjectAccessor->GetCreature(*me, fireTriggerGUID[random]))
                        pCreature->CastSpell(pCreature, HEADLESS_HORSEMAN_SPELL_FIRE_SIZE);

                    if (!eventMap.IsEventActive(HEADLESS_HORSEMAN_CHECK_FIRE))
                        eventMap.ScheduleEvent(HEADLESS_HORSEMAN_CHECK_FIRE, 1 * IN_MILLISECONDS);

                    eventMap.ScheduleEvent(HEADLESS_HORSEMAN_EVENT_RESIZE_FIRE, 400);
                }break;

                case HEADLESS_HORSEMAN_EVENT_DESPAWN:
                {
                    endEvent();
                }break;

                case HEADLESS_HORSEMAN_CHECK_FIRE:
                {
                    uint8 counter = NULL;

                    for (uint8 i = 0; i < HEADLESS_HORSEMAN_FIRE_COUNT; ++i)
                        if (Creature* pCreature = sObjectAccessor->GetCreature(*me, fireTriggerGUID[i]))
                            ++counter;

                    if (!counter)
                        endEvent(true);
                    else
                        eventMap.ScheduleEvent(HEADLESS_HORSEMAN_CHECK_FIRE, 1 * IN_MILLISECONDS);
                }break;
            }
        }

        void endEvent(bool success = false)
        {
            eventMap.Reset();

            for (int i = 0; i < 4; i++)
                if (Creature* pCreature = sObjectAccessor->GetCreature(*me, fireTriggerGUID[i]))
                    pCreature->DespawnOrUnsummon();

            if (success)
            {
                me->Yell(HEADLESS_HORSEMAN_SAY_WIN, LANG_UNIVERSAL, 0);
                DoCast(HEADLESS_HORSEMAN_SPELL_QUEST_COMPLETED);
            }
            else
                me->Yell(HEADLESS_HORSEMAN_SAY_FAIL, LANG_UNIVERSAL, 0);

            headlessHorsemanRunning[zonearray] = false;
            me->DespawnOrUnsummon(5 * IN_MILLISECONDS);
        }

        EventMap eventMap;

        bool eventFailed;

        uint8 fireSpawnCount;
        uint8 fireSizeCount;

        uint32 zonearray;
        uint64 fireTriggerGUID[6];
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_Headless_Horseman_Rotten_Hollow_AI(creature);
    }
};

typedef npc_Headless_Horseman_Rotten_Hollow::npc_Headless_Horseman_Rotten_Hollow_AI HeadlessHorsemanAI;


class spell_headless_horseman_start_quest : public SpellScriptLoader
{
    public:
        spell_headless_horseman_start_quest() : SpellScriptLoader("spell_headless_horseman_start_quest") { }

        class spell_headless_horseman_start_quest_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_headless_horseman_start_quest_SpellScript);

            uint32 initVars(Unit* caster)
            {
                uint32 zone = caster->GetMap()->GetZoneId(caster->GetPositionX(), caster->GetPositionY(), caster->GetPositionZ());
                switch (zone)
                {
                    case HEADLESS_HORSEMAN_ZONE_BRILL:
                        return 0;
                    case HEADLESS_HORSEMAN_ZONE_RAZORHILL:
                        return 1;
                    case HEADLESS_HORSEMAN_ZONE_SILVERMOON:
                        return 2;
                    case HEADLESS_HORSEMAN_ZONE_GOLDHAIN:
                        return 3;
                    case HEADLESS_HORSEMAN_ZONE_IRONFORGE:
                        return 4;
                    case HEADLESS_HORSEMAN_ZONE_EXODAR:
                        return 5;
                    default:
                        return 6;
                }
            }
            
            void HandleCast()
            {
                uint32 zonearray = initVars(GetCaster());
                if (zonearray >= 6)
                    return;

                if (headlessHorsemanRunning[zonearray])
                    return;

                headlessHorsemanRunning[zonearray] = true;
                Creature* dummy = GetCaster()->SummonCreature(HEADLESS_HORSEMAN_NPC_HORSEMAN, WaypointLocations[zonearray][0]);
                dummy->AI()->SetData(HEADLESS_HORSEMAN_DATA_ZONE, zonearray);
            }

            void Register()
            {
                BeforeCast += SpellCastFn(spell_headless_horseman_start_quest_SpellScript::HandleCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_headless_horseman_start_quest_SpellScript();
        }
};


class spell_headless_horseman_throw_fire : public SpellScriptLoader
{
    public:

    spell_headless_horseman_throw_fire() : SpellScriptLoader("spell_headless_horseman_throw_fire") { }

    class spell_headless_horseman_throw_fire_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_headless_horseman_throw_fire_SpellScript);

        void HandleCast()
        {
            if (GetCaster()->GetEntry() == HEADLESS_HORSEMAN_NPC_HORSEMAN)
                GetHitCreature()->CastSpell(GetHitCreature(), HEADLESS_HORSEMAN_SPELL_FIRE);
        }

        void Register() override
        {
            OnHit += SpellHitFn(spell_headless_horseman_throw_fire_SpellScript::HandleCast);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_headless_horseman_throw_fire_SpellScript();
    }
};

class spell_headless_horseman_throw_water : public SpellScriptLoader
{
    public:

    spell_headless_horseman_throw_water() : SpellScriptLoader("spell_headless_horseman_throw_water") { }

    class spell_headless_horseman_throw_water_SpellScript : public SpellScript
    {
        public:

            PrepareSpellScript(spell_headless_horseman_throw_water_SpellScript);

            bool Load()
            {
                alreadyAdded = false;
                return true;
            }

            void FilterTargets(SpellEffIndex effIndex)
            {
                if (GetHitCreature())
                    if (GetHitCreature()->GetEntry() == HEADLESS_HORSEMAN_NPC_FIRE_TRIGGER)
                        alreadyAdded = true;

                if (GetHitPlayer())
                {
                    if (alreadyAdded)
                        return;

                    GetHitPlayer()->AddItem(HEADLESS_HORSEMAN_ITEM_WATER, 1);
                    alreadyAdded = true;
                }
            }

            void HandleCast()
            {
                if (GetHitUnit()->GetTypeId() == TYPEID_UNIT)
                {
                    if (Creature* pCreature = GetHitCreature())
                    {
                        if (pCreature->GetEntry() == HEADLESS_HORSEMAN_NPC_FIRE_TRIGGER)
                        {
                            Aura* pAura = pCreature->GetAuraOfRankedSpell(HEADLESS_HORSEMAN_SPELL_FIRE_SIZE);
                            if (pAura == NULL)
                                return;

                            if (pAura->GetStackAmount() <= 10)
                                pCreature->DespawnOrUnsummon();
                            else
                                pAura->ModStackAmount(-10);
                        }
                        else
                            PreventHitDamage();
                    }
                }
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_headless_horseman_throw_water_SpellScript::FilterTargets, EFFECT_1, SPELL_EFFECT_SCRIPT_EFFECT);
                OnHit += SpellHitFn(spell_headless_horseman_throw_water_SpellScript::HandleCast);
            }

        private:

            bool alreadyAdded;
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_headless_horseman_throw_water_SpellScript();
    }
};

void AddSC_Headless_Horseman_Rotten_Hollow()
{
    new npc_Headless_Horseman_Rotten_Hollow();
    new spell_headless_horseman_start_quest();
    new spell_headless_horseman_throw_fire();
    new spell_headless_horseman_throw_water();
}