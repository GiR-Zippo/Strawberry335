#include "ScriptPCH.h"
#include "boss_helper.h"
enum eEnums
{
    SPELL_NEEDLE_SPINE              = 39992,
    SPELL_TIDAL_BURST               = 39878,
    SPELL_IMPALING_SPINE            = 39837,
    SPELL_CREATE_NAJENTUS_SPINE     = 39956,
    SPELL_HURL_SPINE                = 39948,

    GOBJECT_SPINE                   = 401004,

    EVENT_SPINE                     = 1,
    EVENT_KICK_BACK                 = 2,
    EVENT_SHIELD                    = 3,

    EVENT_CHRIS_1                   = 4,
    EVENT_CHRIS_2                   = 5,

    EVENT_ZAZI_1                    = 6,
    EVENT_ZAZI_2                    = 7,

    EVENT_IDOSTYLE_1                = 8,
    EVENT_IDOSTYLE_2                = 9,
};

uint64 HelperGUID;

class boss_bday_laena : public CreatureScript
{
public:
    boss_bday_laena() : CreatureScript("boss_bday_laena") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_bday_laenaAI (pCreature);
    }

    struct boss_bday_laenaAI : public ScriptedAI
    {
        boss_bday_laenaAI(Creature *c) : ScriptedAI(c)
        {
            HelperGUID = me->GetGUID();
            spawned = false;
            triggered = false;
        }
        EventMap events;

        std::list<uint64> spineList;
        uint8 Phase;
        bool spawned;
        bool triggered;

        void Reset()
        {
            events.Reset();

            spineList.clear();
            Phase = 0;
            spawned = false;
            triggered = false;
        }

        void KilledUnit(Unit* victim)
        {
            if (victim->GetTypeId() != TYPEID_PLAYER)
                return;
            me->SetHealth(me->GetHealth() + (me->GetMaxHealth() * 5 / 100));
            me->MonsterYell("Danke für euer Leben!!", LANG_UNIVERSAL, 0);
        }
            
        void JustDied(Unit * /*victim*/)
        {
            me->MonsterYell("Ihr....habt..mich....bezwungen...", LANG_UNIVERSAL, 0);
            me->MonsterYell("Ich lasse meine Gefangenen wieder frei...", LANG_UNIVERSAL, 0);
           Creature* temp1 = me->SummonCreature(43292, me->GetPositionX()+10, me->GetPositionY()+10, me->GetPositionZ());
           Creature* temp2 = me->SummonCreature(43293, me->GetPositionX()+8, me->GetPositionY()+10, me->GetPositionZ());
           Creature* temp3 = me->SummonCreature(43294, me->GetPositionX()+5, me->GetPositionY()+10, me->GetPositionZ());
          if (!temp1 || !temp2 || !temp3)
              return;

           temp1->AI()->DoAction(1);
           temp3->AI()->DoAction(1);
           temp2->AI()->DoAction(1);          
        }

        void SpellHit(Unit * caster, const SpellEntry *spell)
        {
            if (spell->Id == SPELL_HURL_SPINE)
            {
                if (me->HasAura(39872))
                    {
                     me->MonsterYell("Der ist mir zu dick.. das kriegst du zurück!", LANG_UNIVERSAL, caster->GetGUID());
                     me->RemoveAura(39872);
                     me->SetHealth(me->GetHealth() - (me->GetMaxHealth() * 2 / 100));
                     DoCast(me, SPELL_TIDAL_BURST, true);
                    }

                
            }
        }

        void EnterCombat(Unit * /*who*/)
        {
            Phase = 0;
            events.ScheduleEvent(EVENT_SPINE, 20000);
            events.ScheduleEvent(EVENT_KICK_BACK, 60000);
            events.ScheduleEvent(EVENT_SHIELD, 30000);
            ResetTimer();
        }

        bool RemoveImpalingSpine(Unit* pPlayer)
        {
        for (std::list<uint64>::const_iterator itr = spineList.begin(), next; itr != spineList.end(); itr = next)
                {
                next = itr;
                ++next;
                    if (Player *pTarget = Unit::GetPlayer(*me, *itr))
                        {
                        if (pPlayer && pPlayer == pTarget && pPlayer->HasAura(SPELL_IMPALING_SPINE))
                            {
                              pPlayer->RemoveAurasDueToSpell(SPELL_IMPALING_SPINE);
                              spineList.remove(*itr);
                              return true;
                            }
                        }
                 }
            return true;
        }

        void ResetTimer(uint32 inc = 0)
        {
            events.RescheduleEvent(EVENT_SPINE, 20000);
            events.RescheduleEvent(EVENT_KICK_BACK, 60000);
            events.RescheduleEvent(EVENT_SHIELD, 30000);
        }

        void DamageTaken(Unit *done_by, uint32 &damage)
        {
                if (HealthBelowPct(10))
                    {
                        int32 bp0 = damage / 2;
                        me->CastCustomSpell(done_by, 41352, &bp0, NULL, NULL, true);
                    }
                return;
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);
            
            if (me->GetHealthPct() <= 50.0f && !spawned)
                {
                            me->MonsterYell("Futter fassen Freunde", LANG_UNIVERSAL, 0);
                            std::list<Unit*> spawnList;
                            for (int i = 0; i < 20; i++)
                                {
                                 Unit* temp = me->SummonCreature(27649, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 0.0f, TEMPSUMMON_CORPSE_DESPAWN);
                                 spawnList.push_back(temp);
                                } 
                            spawnList.clear();
                            spawned = true;
                            return;
                }

            if (Phase == 0 && HealthBelowPct(80) || Phase == 1 && HealthBelowPct(60) || Phase == 2 && HealthBelowPct(40) || Phase == 3 && HealthBelowPct(20))
                {
                    me->MonsterYell("Ich habe die Schauze voll!", LANG_UNIVERSAL, 0);
                    DoCast(me, 64487, false);
                    Phase++;
                }

            if (HealthBelowPct(10) && !triggered)
                {
                me->MonsterYell("Oh oh, mein Höschen ist voll!!!", LANG_UNIVERSAL, 0);
                DoCast(41350);
                triggered = true;
                }

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_SPINE:
                    {
                        me->MonsterYell("Warum liegt hier eigentlich Stroh?", LANG_UNIVERSAL, 0);
                        int random = rand() % 10 + 5;
                        std::list<Unit*> pTargets;
                        SelectTargetList(pTargets, random, SELECT_TARGET_RANDOM, 0.0f, true);
                        HelperGUID = me->GetGUID();
                        for (std::list<Unit*>::const_iterator i = pTargets.begin(); i != pTargets.end(); ++i)
                        {
                            DoCast(*i, SPELL_IMPALING_SPINE, true);
                            spineList.push_back((*i)->GetGUID());
                            (*i)->SummonGameObject(GOBJECT_SPINE, (*i)->GetPositionX(), (*i)->GetPositionY(), (*i)->GetPositionZ(), me->GetOrientation(), 0, 0, 0, 0, 30);
                        }
                        events.ScheduleEvent(EVENT_SPINE, 20000);
                        break;
                    }
                    case EVENT_KICK_BACK:
                        {
                            DoCast(me, 55821, true);
                            events.RescheduleEvent(EVENT_KICK_BACK, 45000);
                            break;
                        }
                    case EVENT_SHIELD:
                        {
                            DoCast(me, 39872, true);
                            events.RescheduleEvent(EVENT_SHIELD, 30000);
                            break;
                        }
                }
            }

            DoMeleeAttackIfReady();
        }
    };

};

class go_helper_spine : public GameObjectScript
{
public:
    go_helper_spine() : GameObjectScript("go_helper_spine") { }

    bool OnGossipHello(Player* pPlayer, GameObject* pGo)
    {
        Creature* Helper = Unit::GetCreature(*pGo, HelperGUID);
        if (!Helper)
            return true;
        if (!Helper->AI())
            return true;

        if (CAST_AI(boss_bday_laena::boss_bday_laenaAI, Helper->AI())->RemoveImpalingSpine(pGo->GetOwner()))
            {
                    pPlayer->CastSpell(pPlayer, SPELL_CREATE_NAJENTUS_SPINE, true);
                    pGo->Delete();
            }
        return true;
    }

};


class npc_event_chris : public CreatureScript
{
public:
    npc_event_chris() : CreatureScript("npc_event_chris") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_event_chrisAI (pCreature);
    }

    struct npc_event_chrisAI : public ScriptedAI
    {
        npc_event_chrisAI(Creature *c) : ScriptedAI(c)
        {
        }
        EventMap events;
        void Reset()
        {
            events.Reset();
        }
        void DoAction(int32 const action)
            {
                switch (action)
                    {
                    case 1:
                         {
                                me->MonsterYell("Ihr habt uns befreit", LANG_UNIVERSAL, 0);
                                events.ScheduleEvent(EVENT_CHRIS_1, 1000);
                                break;
                         }
                    }
                
            }

       void UpdateAI(const uint32 diff)
        {
            events.Update(diff);

               while (uint32 eventId = events.ExecuteEvent())
                    {
                        switch (eventId)
                        {
                        case EVENT_CHRIS_1:
                            {
                                me->MonsterYell("Endlich kann ich mich wieder meiner wichtigsten Arbeit widmen.. Frauen!", LANG_UNIVERSAL, 0);
                                events.ScheduleEvent(EVENT_CHRIS_2, 14000);
                                break;
                            }
                        case EVENT_CHRIS_2:
                            {
                            me->DespawnOrUnsummon();
                            break;
                            }
                        }

                    }

             }
    };
};

class npc_event_zazi : public CreatureScript
{
public:
    npc_event_zazi() : CreatureScript("npc_event_zazi") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_event_zaziAI (pCreature);
    }

    struct npc_event_zaziAI : public ScriptedAI
    {
        npc_event_zaziAI(Creature *c) : ScriptedAI(c)
        {
        }
        EventMap events;
        void Reset()
        {
            events.Reset();
        }

        void DoAction(int32 const action)
            {
                switch (action)
                    {
                    case 1:
                         {
                                me->MonsterYell("Ich habe seit 2 Wochen nichtmehr geschlafen! Aber eigentlich ist das normal ...", LANG_UNIVERSAL, 0);
                                events.ScheduleEvent(EVENT_ZAZI_1, 3000);
                                break;
                         }
                    }
                
            }

       void UpdateAI(const uint32 diff)
        {
            events.Update(diff);

               while (uint32 eventId = events.ExecuteEvent())
                    {
                        switch (eventId)
                        {
                        case EVENT_ZAZI_1:
                            {
                                me->MonsterYell("Der Little Helper hätte mich beinahe dazu gebracht Windows zu benutzen... danke das ihr mich davor bewahrt habt!", LANG_UNIVERSAL, 0);
                                events.ScheduleEvent(EVENT_ZAZI_2, 10000);
                                break;
                            }
                        case EVENT_ZAZI_2:
                            {
                            me->DespawnOrUnsummon();
                             break;
                            }
                        }

                    }

             }
    };
};

class npc_event_idostyle : public CreatureScript
{
public:
    npc_event_idostyle() : CreatureScript("npc_event_idostyle") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_event_idostyleAI (pCreature);
    }

    struct npc_event_idostyleAI : public ScriptedAI
    {
        npc_event_idostyleAI(Creature *c) : ScriptedAI(c)
        {
        }
        EventMap events;
        void Reset()
        {
            events.Reset();
        }
        void DoAction(int32 const action)
            {
                switch (action)
                    {
                    case 1:
                         {
                                me->MonsterYell("Danke! Meine Onlinezeit ist durch die Gefangenschaft ganz schön zurückgegangen.... Tickets ich komme!", LANG_UNIVERSAL, 0);
                                events.ScheduleEvent(EVENT_IDOSTYLE_1, 5000);
                                 break;
                         }
                    }
                
            }

       void UpdateAI(const uint32 diff)
        {
            events.Update(diff);

               while (uint32 eventId = events.ExecuteEvent())
                    {
                        switch (eventId)
                        {
                        case EVENT_IDOSTYLE_1:
                            {
                                me->MonsterYell("ICC ist bald fertig.. ich denke ich bin soweit!", LANG_UNIVERSAL, 0);
                                events.ScheduleEvent(EVENT_IDOSTYLE_2, 12000);
                                break;
                            }
                        case EVENT_IDOSTYLE_2:
                            {
                            me->DespawnOrUnsummon();
                            break;
                            }
                        }

                    }

             }
    };
};

void AddSC_boss_bday_laena()
{
    new boss_bday_laena();
    new go_helper_spine();
    new npc_event_chris();
    new npc_event_idostyle();
    new npc_event_zazi();
}
