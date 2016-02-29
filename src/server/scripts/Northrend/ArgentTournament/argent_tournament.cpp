///*
// * Copyright (C) 2008-2011 TrinityCore <http://www.trinitycore.org/>
// * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
// *
// * This program is free software; you can redistribute it and/or modify it
// * under the terms of the GNU General Public License as published by the
// * Free Software Foundation; either version 2 of the License, or (at your
// * option) any later version.
// *
// * This program is distributed in the hope that it will be useful, but WITHOUT
// * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
// * more details.
// *
// * You should have received a copy of the GNU General Public License along
// * with this program. If not, see <http://www.gnu.org/licenses/>.
// */
//
//#include "ArgentTournament.h"
//#include "ScriptPCH.h"
//#include "SpellAuraEffects.h"
//#include "ScriptMgr.h"
//#include "Group.h"
//#include "ScriptedFollowerAI.h"
//#include "Vehicle.h"
//#include "CreatureAIImpl.h"
//
//enum eValiantText
//{
//    NPC_FACTION_VAILIANT_TEXT_SAY_START_1   = -1850004, //Stand ready!
//    NPC_FACTION_VAILIANT_TEXT_SAY_START_2   = -1850005, //Let the battle begin!
//    NPC_FACTION_VAILIANT_TEXT_SAY_START_3   = -1850006, //Prepare your self!
//
//    NPC_FACTION_VAILIANT_TEXT_SAY_WIN_1     = -1850010, //I stand defeated. Nice battle !
//    NPC_FACTION_VAILIANT_TEXT_SAY_WIN_2     = -1850011, //It seems that I\'ve underestimated your skills. Well done.'
//    NPC_FACTION_VAILIANT_TEXT_SAY_LOOSE     = -1850012  //You\'ll probably have more luck next time.
// };
//
///*######
//## Quest Soporte Threat From Above
//## npc_Chillmaw
//######*/
//
//// UPDATE `creature_template` SET `ScriptName`='npc_chillmaw' WHERE `entry`=33687;
//// *Known bug, The NPC doesn't despawns until server restarts
//
//enum Chillmaw
//{
//    SPELL_FROST_BREATH  = 65248,
//    SPELL_WING_BUFFET   = 65260,
//    BOMBARDIER_1        = 0,
//    BOMBARDIER_2        = 1,
//    BOMBARDIER_3        = 2
//};
//
//enum TimersGral
//{
//    TIMER_SPELL_MIN         = 1000,
//    TIMER_SPELL_MAX         = 2000,
//    TIMER_MoviTimer_MIN     = 1000,
//    TIMER_MoviTimer_MAX     = 2000
//};
//
//enum SpellGral
//{
//    SHIELD_BREAKER                          = 62626,
//    CHARGE                                  = 68321,
//    THRUST                                  = 62544,
//    DEFEND                                  = 62552,
//    SHIELD_LEVEL1                           = 62719,
//    SHIELD_LEVEL2                           = 64100,
//    SHIELD_LEVEL3                           = 63132,
//    YELLOW_ROSE_FIREWORK                    = 11544
//};
//
//enum seats
//{
//    SEAT_1 = 0,
//    SEAT_2 = 1,
//    SEAT_3 = 2,
//};
//
//class npc_chillmaw : public CreatureScript
//{
//public:
//    npc_chillmaw() : CreatureScript("npc_chillmaw") { }
//
//    struct npc_chillmawAI : public ScriptedAI
//    {
//        npc_chillmawAI(Creature *creature) : ScriptedAI(creature), vehicle(creature->GetVehicleKit())
//        {
//            assert(vehicle);
//        }
//        
//        EventMap events;
//        Vehicle *vehicle;
//
//        uint32 Spell_FrostBreath_Timer;
//        uint32 Spell_WingBuffet_Timer;
//
//        bool Pasajero_1;
//        bool Pasajero_2;
//        bool Pasajero_3;
//
//        void Reset()
//        {
//            me->SetReactState(REACT_AGGRESSIVE);            
//            Spell_FrostBreath_Timer = 6000;
//            Spell_WingBuffet_Timer = 1500;
//            Pasajero_1 = false;
//            Pasajero_2 = false;
//            Pasajero_3 = false;
//        }
//
//        void UpdateAI(const uint32 diff)
//        {
//            if (!UpdateVictim())
//                return;
//            
//            if (me->HasUnitState(UNIT_STATE_CASTING))
//                    return;
//  
//            if (Spell_FrostBreath_Timer <= diff)
//            {   
//                if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0))
//                    DoCast(pTarget, SPELL_FROST_BREATH);
//                Spell_FrostBreath_Timer = urand(15000, 16500);
//            } else Spell_FrostBreath_Timer -=diff;
//
//            if (Spell_WingBuffet_Timer <= diff)
//            {
//                DoCast(me, SPELL_WING_BUFFET);
//                Spell_WingBuffet_Timer = urand(4000, 6500);
//            } else Spell_WingBuffet_Timer -= diff;
//
//            if (!Pasajero_1 && (me->GetHealth() < me->GetMaxHealth() * 0.70))
//            {
//                if (Creature *Bombardier1 = CAST_CRE(vehicle->GetPassenger(BOMBARDIER_1)))
//                {
//                    /*Bombardier1->ExitVehicle();
//                    Bombardier1->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
//                    Bombardier1->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
//                    Bombardier1->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_1);
//                    Bombardier1->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);*/
//                }
//                Pasajero_1 = true;
//            }
//
//            if (!Pasajero_2 && (me->GetHealth() < me->GetMaxHealth() * 0.50))
//            {
//                if (Creature *Bombardier2 = CAST_CRE(vehicle->GetPassenger(BOMBARDIER_2)))
//                {
//                    Bombardier2->ExitVehicle();
//                    Bombardier2->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
//                    Bombardier2->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
//                    Bombardier2->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_1);
//                    Bombardier2->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
//                }
//                Pasajero_2 = true;
//            }
//
//            if (!Pasajero_3 && (me->GetHealth() < me->GetMaxHealth() * 0.25))
//            {
//                if (Creature *Bombardier3 = CAST_CRE(vehicle->GetPassenger(BOMBARDIER_3)))
//                {
//                    Bombardier3->ExitVehicle();
//                    Bombardier3->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);                    
//                    Bombardier3->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
//                    Bombardier3->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_1);
//                    Bombardier3->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
//                }
//                Pasajero_3 = true;
//            }
//            DoMeleeAttackIfReady();
//        }
//    };
//
//    void EnterCombat(Unit* /*who*/) { }
//        
//    CreatureAI* GetAI(Creature* pCreature) const
//    {
//        return new npc_chillmawAI (pCreature);
//    }
//};
//
//
//
//
//
//
///*######
//## npc_Keritose Quest Support 13172
//-- Quest Support Seeds of Chaos (13172)
//UPDATE creature_template SET type_flags=8, spell1=59234, spell2=53112, VehicleId=156, unk16=25, unk17=20, movementId=199, RegenHealth=1 WHERE entry=31157;
//UPDATE creature_template SET `ScriptName` = 'npc_keritose', npcflag='3' WHERE entry=30946;
//UPDATE creature_template SET KillCredit2=31555 WHERE entry IN (31554, 30949 , 30951)
//######*/
//#define GOSSIP_KERITOSE_I  "I am prepared to join the assault, keritose do you have a Skeletal Gryphon ready for me!"
//
//enum eKeritose
//{
//	QUEST_SEEDS_OF_CHAOS	            	  = 13172,
//	SPELL_TAXI_KERITOSE	       		          = 58698, 
//};
//class npc_keritose : public CreatureScript
//{
//public:
//    npc_keritose() : CreatureScript("npc_keritose") { }
//
//    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
//    {
//        if (pCreature->isQuestGiver())
//            pPlayer->PrepareQuestMenu(pCreature->GetGUID());
//
//        if (pPlayer->GetQuestStatus(QUEST_SEEDS_OF_CHAOS) == QUEST_STATUS_INCOMPLETE)
//            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_KERITOSE_I, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
//
//        pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
//        return true;
//    }
//
//    bool OnGossipSelect(Player* pPlayer, Creature* pCreature /*pCreature*/, uint32 /*uiSender*/, uint32 uiAction)
//    {
//        pPlayer->PlayerTalkClass->ClearMenus();
//       
//        switch (uiAction)
//        {
//            case GOSSIP_ACTION_INFO_DEF+1:
//                pPlayer->CastSpell(pPlayer, SPELL_TAXI_KERITOSE, true);
//                pPlayer->CLOSE_GOSSIP_MENU();
//                break;
//        }
//        return true;
//    }
//};
//
///*######
//## npc_valiant  
//######*/
//
//#define GOSSIP_VALIANT_1    "I am ready to fight."
////#define GOSSIP_VALIANT_GOSS "Are you sure you ready to fight a champion?"
//#define SAY_START_1         "On your guard!"
//#define SAY_START_2         "Prepare yourself!"
//#define SAY_END             "I have been defeated. Good Fight !"
//
//enum ValiantQuest
//{
//    QUEST_AMONG_THECHAMPIONS_A      = 13790,
//    QUEST_AMONG_THECHAMPIONS_H      = 13811,
//    QUEST_AMONG_THECHAMPIONS_A2     = 13793,
//    QUEST_AMONG_THECHAMPIONS_H2     = 13814,
//    MOUNTED_MELEE_VICTORY           = 63596
//
//};
//
//class npc_valiant : public CreatureScript
//{
//public:
//    npc_valiant() : CreatureScript("npc_valiant") { }
//
//    struct npc_valiantAI : public ScriptedAI
//    {
//        Unit *pTarget;
//        uint32 SpellTimer,MoviTimer;
//        int8 x,y;    
//       
//        npc_valiantAI(Creature* pCreature) : ScriptedAI(pCreature){}
//       
//        void Escudo()
//        {
//            if (Aura *cAura = me->GetAura(DEFEND))
//            {
//                switch (cAura->GetStackAmount())
//                {
//                    case 1:
//                        me->RemoveAura(SHIELD_LEVEL2);
//                        me->RemoveAura(SHIELD_LEVEL3);
//                        me->CastSpell(me, SHIELD_LEVEL1, true);
//                        break;
//                    case 2:
//                        me->RemoveAura(SHIELD_LEVEL1);
//                        me->RemoveAura(SHIELD_LEVEL3);
//                        me->CastSpell(me, SHIELD_LEVEL2, true);
//                        break;
//                    case 3:
//                        me->RemoveAura(SHIELD_LEVEL2);
//                        me->RemoveAura(SHIELD_LEVEL1);
//                        me->CastSpell(me, SHIELD_LEVEL3, true);
//                        break;
//                }
//            }
//        }
//
//        void Reset()
//        {
//            me->setFaction(35);
//            me->SetReactState(REACT_PASSIVE);
//            me->SetVisible(true);
//        }
//       
//        void EnterCombat(Unit* who)
//        {
//            SpellTimer= urand(TIMER_SPELL_MIN,TIMER_SPELL_MAX );
//            MoviTimer = urand(TIMER_MoviTimer_MIN,TIMER_MoviTimer_MAX);
//        }
//       
//        void JustDied(Unit* Killer)
//        {
//            me->MonsterSay(SAY_END, LANG_UNIVERSAL, 0);
//            me->setFaction(35);
//            me->SetHealth(1);
//            pTarget->CastSpell(pTarget, MOUNTED_MELEE_VICTORY, true);
//            me->SetVisible(false);
//        }
//       
//        void KilledUnit(Unit *victim)
//        {
//            Reset();
//            EnterEvadeMode();
//        }
//       
//        void SpellHit(Unit *caster, const SpellEntry *spell)
//        {
//            if ((spell->Id == SHIELD_BREAKER)||(spell->Id == CHARGE))
//            {
//                if (Aura *cAura = me->GetAura(DEFEND))
//                {
//                    switch (cAura->GetStackAmount())
//                    {
//                        case 0:
//                        case 1:
//                            me->RemoveAura(DEFEND);
//                            me->RemoveAura(SHIELD_LEVEL2);
//                            me->RemoveAura(SHIELD_LEVEL3);
//                            me->RemoveAura(SHIELD_LEVEL1);
//                            break;
//                        case 2:
//                            cAura->SetStackAmount(1);
//                            break;
//                        case 3:
//                            cAura->SetStackAmount(2);
//                            break;
//                    }
//                }
//                Escudo();
//            }
//        }
//
//        void SpellHitTarget(Unit *pTarget, const SpellEntry *spell)
//        {
//            if ((spell->Id == SHIELD_BREAKER)||(spell->Id == CHARGE))
//            {
//                if (Aura *pAura = pTarget->GetAura(DEFEND))
//                {
//                    switch (pAura->GetStackAmount())
//                    {
//                        case 0:
//                            pTarget->RemoveAura(DEFEND);
//                            break;
//                        case 1:
//                            pAura->SetStackAmount(0);
//                            break;
//                        case 2:
//                            pAura->SetStackAmount(1);
//                            break;
//                        case 3:
//                            pAura->SetStackAmount(2);
//                            break;
//                    }
//                }
//            }
//        }
//
//        void UpdateAI(const uint32 uiDiff)
//        {
//            if (!UpdateVictim())
//                return;
//
//            pTarget = SelectTarget(SELECT_TARGET_TOPAGGRO, 0, 200, true);
//               
//            if (SpellTimer<=uiDiff)
//            {
//                if (pTarget && me->IsInRange(pTarget,5,40,false))
//                {
//                    switch (urand(0,7))
//                    {
//                        case 0: 
//                        case 1: 
//                        case 2: 
//                            DoCastAOE(SHIELD_BREAKER, true);
//                            break;
//                        case 3: 
//                        case 4:
//                        case 5:
//                        case 6:
//                            DoCastAOE(CHARGE, true);
//                            break;
//                        case 7:
//                            me->CastSpell(me, DEFEND, true);
//                            Escudo();
//                            break;
//                    }
//                }
//                else
//                {
//                    switch (urand(0,4))
//                    {
//                        case 0: 
//                            me->CastSpell(me, DEFEND, true);
//                            Escudo();
//                            break;
//                        case 1:
//                        case 2:
//                        case 3:
//                        case 4:
//                            DoCastAOE(THRUST, true);
//                            break;
//                    }
//                }
//                SpellTimer= urand(TIMER_SPELL_MIN,TIMER_SPELL_MAX );
//            } else 
//                SpellTimer -= uiDiff;
//            if (MoviTimer<=uiDiff)
//            {
//                x =urand(0,7);
//                y =urand(0,7);
//                switch (urand(0,3))
//                {
//                    case 0:
//                        me->GetMotionMaster()->MovePoint(0, (pTarget->GetPositionX()+x), (pTarget->GetPositionY()+y), pTarget->GetPositionZ());
//                        break;
//                    case 1: 
//                        me->GetMotionMaster()->MovePoint(0, (pTarget->GetPositionX()-x), (pTarget->GetPositionY()+y), pTarget->GetPositionZ());
//                        break;
//                    case 2: 
//                        me->GetMotionMaster()->MovePoint(0, (pTarget->GetPositionX()+x), (pTarget->GetPositionY()-y), pTarget->GetPositionZ());
//                        break;
//                    case 3: 
//                        me->GetMotionMaster()->MovePoint(0, (pTarget->GetPositionX()-x), (pTarget->GetPositionY()-y), pTarget->GetPositionZ());
//                        break;
//                }
//                MoviTimer = urand(TIMER_MoviTimer_MIN,TIMER_MoviTimer_MAX);
//                } else 
//                    MoviTimer -= uiDiff;
//
//                DoMeleeAttackIfReady();
//        }
//    };
//
//    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
//    {
//        if (pCreature->isQuestGiver())
//            pPlayer->PrepareQuestMenu(pCreature->GetGUID());
//
//        if (((pPlayer->GetQuestStatus(QUEST_AMONG_THECHAMPIONS_H) == QUEST_STATUS_INCOMPLETE)  ||
//            (pPlayer->GetQuestStatus(QUEST_AMONG_THECHAMPIONS_H2) == QUEST_STATUS_INCOMPLETE) ||
//            (pPlayer->GetQuestStatus(QUEST_AMONG_THECHAMPIONS_A) == QUEST_STATUS_INCOMPLETE)  ||
//            (pPlayer->GetQuestStatus(QUEST_AMONG_THECHAMPIONS_A2) == QUEST_STATUS_INCOMPLETE)) &&
//            (pPlayer->HasAura(RIDE_VEHICLE)) &&
//            (pPlayer->HasAura(PLAYER_ON_TOURNAMENT_MOUNT)) &&
//            (pPlayer->HasAura(LANCE_EQUIPPED)) &&
//            (!pPlayer->isInCombat()))
//
//        {
//            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_VALIANT_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
//            pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
//            return true;
//        }
//        return false;
//    }
//
//    bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 /*uiSender*/, uint32 uiAction)
//    {
//        pPlayer->PlayerTalkClass->ClearMenus();
//        if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
//        {
//            pPlayer->CLOSE_GOSSIP_MENU();
//            pCreature->setFaction(14);
//            pCreature->SetReactState(REACT_AGGRESSIVE);
//            pCreature->MonsterSay(urand(0,1) ? SAY_START_1 : SAY_START_2, LANG_UNIVERSAL, 0);
//            pCreature->AI()->AttackStart(pPlayer->GetVehicleCreatureBase());
//            pCreature->AddThreat(pPlayer, 0.0f);
//            pCreature->SetInCombatWith(pPlayer);
//            pPlayer->SetInCombatWith(pCreature);
//        }
//        return true;
//    }        
//            
//    CreatureAI* GetAI(Creature* pCreature) const
//    {
//        return new npc_valiantAI (pCreature);
//    }
//};
//
///*######
//## Free Your Mind Quest 12893
//######*/
//
//enum YourMind
//{
//    QUEST_FREE_YOUR_MIND                = 12893,
//    SPELL_SOVEREIGN_ROD                 = 29070,
//    NPC_VILE_CREDIT_BUNNY               = 29845,
//    NPC_LADY_NIGHTSWOOD_CREDIT_BUNNY    = 29846,
//    NPC_LEAPER_BUNNY                    = 29847
//};
//
//class npc_vile : public CreatureScript
//{
//public:
//    npc_vile() : CreatureScript("npc_vile") { }
//
//    CreatureAI* GetAI(Creature* pCreature) const
//    {
//        return new npc_vileAI (pCreature);
//    }
//
//    struct npc_vileAI : public ScriptedAI
//    {
//        npc_vileAI(Creature *c) : ScriptedAI(c) {}
//
//        bool spellHit;
//
//        void Reset()
//        {
//            spellHit = false;
//        }
//
//        void SpellHit(Unit *Hitter, const SpellEntry *Spellkind)
//        {
//            if ((Spellkind->Id == SPELL_SOVEREIGN_ROD) && !spellHit &&
//                (Hitter->GetTypeId() == TYPEID_PLAYER) && (CAST_PLR(Hitter)->IsActiveQuest(QUEST_FREE_YOUR_MIND)))
//            {
//                CAST_PLR(Hitter)->KilledMonsterCredit(NPC_VILE_CREDIT_BUNNY, 0);
//                spellHit = true;
//            }
//        }
//    };
//};
//
//class npc_lady_nightswood : public CreatureScript
//{
//public:
//    npc_lady_nightswood() : CreatureScript("npc_lady_nightswood") { }
//
//    CreatureAI* GetAI(Creature* pCreature) const
//    {
//        return new npc_lady_nightswoodAI (pCreature);
//    }
//
//    struct npc_lady_nightswoodAI : public ScriptedAI
//    {
//        npc_lady_nightswoodAI(Creature *c) : ScriptedAI(c) {}
//
//        bool spellHit;
//
//        void Reset()
//        {
//            spellHit = false;
//        }
//
//        void SpellHit(Unit *Hitter, const SpellEntry *Spellkind)
//        {
//            if ((Spellkind->Id == SPELL_SOVEREIGN_ROD) && !spellHit &&
//                (Hitter->GetTypeId() == TYPEID_PLAYER) && (CAST_PLR(Hitter)->IsActiveQuest(QUEST_FREE_YOUR_MIND)))
//            {
//                CAST_PLR(Hitter)->KilledMonsterCredit(NPC_LADY_NIGHTSWOOD_CREDIT_BUNNY, 0);
//                spellHit = true;
//            }
//        }
//    };
//
//};
//
//class npc_leaper : public CreatureScript
//{
//public:
//    npc_leaper() : CreatureScript("npc_leaper") { }
//
//    CreatureAI* GetAI(Creature* pCreature) const
//    {
//        return new npc_leaperAI (pCreature);
//    }
//
//    struct npc_leaperAI : public ScriptedAI
//    {
//        npc_leaperAI(Creature *c) : ScriptedAI(c) {}
//
//        bool spellHit;
//
//        void Reset()
//        {
//            spellHit = false;
//        }
//
//        void SpellHit(Unit *Hitter, const SpellEntry *Spellkind)
//        {
//            if ((Spellkind->Id == SPELL_SOVEREIGN_ROD) && !spellHit &&
//                (Hitter->GetTypeId() == TYPEID_PLAYER) && (CAST_PLR(Hitter)->IsActiveQuest(QUEST_FREE_YOUR_MIND)))
//            {
//                CAST_PLR(Hitter)->KilledMonsterCredit(NPC_LEAPER_BUNNY, 0);
//                spellHit = true;
//            }
//        }
//    };
//
//};

//
///*######
//## Squire Danny 
//## Quest The Valiant's Challenge 13699-13713-13723-13724-13725-13726-13727-13728-13729-13731.
//SELECT * FROM quest_template WHERE title LIKE 'The Valiant%s Challenge'
//######*/
//
//enum eSquireDanny
//{
//    GOSSIP_TEXTID_SQUIRE                = 14407,
//    NPC_ARGENT_CHAMPION_CREDIT                          = 33708,
//    NPC_ARGENT_CHAMPION                                 = 33707,
//    SAY_START_VALIANT                                   = -1850013,//"You believe you are ready to be a champion? Defend yourself!"  
//    SAY_END_VALIANT                                     = -1850014//"Most impressive. You are worthy to gain the rank of champion"
//};
//
//struct QUEST_THE_VALIANT_CHALLENGE
//{ 
//  uint32 quest_id; 
//};
//
//QUEST_THE_VALIANT_CHALLENGE m_quest[] = {13699, 13713, 13723, 13724, 13725, 13726, 13727, 13728, 13729, 13731};
//
//#define GOSSIP_SQUIRE_DANNY_1   "I am ready to fight!"
//
//class npc_squire_danny : public CreatureScript
//{
//public:
//    npc_squire_danny() : CreatureScript("npc_squire_danny") { }
//
//    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
//    {    
//        for (int i = 0; i < 10; i++)
//        {
//            if (((pPlayer->GetQuestStatus(m_quest[i].quest_id) == QUEST_STATUS_INCOMPLETE))&&
//                (pPlayer->HasAura(RIDE_VEHICLE)) &&
//                (pPlayer->HasAura(PLAYER_ON_TOURNAMENT_MOUNT)) &&
//                (pPlayer->HasAura(LANCE_EQUIPPED)) &&
//                (!pPlayer->isInCombat()))
//            {
//                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SQUIRE_DANNY_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
//
//            }
//        }
//
//        pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_SQUIRE, pCreature->GetGUID());
//        return true;
//    }
//
//    bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 /*uiSender*/, uint32 uiAction)
//    {
//        pPlayer->PlayerTalkClass->ClearMenus();
//        if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
//        {
//            pPlayer->CLOSE_GOSSIP_MENU();
//            pCreature->SummonCreature(NPC_ARGENT_CHAMPION,8542.566406f,1086.951904f,556.769836f,1.044363f);
//        }
//        return true;
//    }
//};
//
///*######
//## npc argent champion.
//######*/
//
//class npc_argent_champion : public CreatureScript
//{
//public:
//    npc_argent_champion() : CreatureScript("npc_argent_champion") { }
//
//    struct npc_argent_championAI : public ScriptedAI
//    {
//        npc_argent_championAI(Creature* pCreature) : ScriptedAI(pCreature)
//        {
//            pCreature->GetMotionMaster()->MovePoint(0,8556.964844f,1114.514282f,556.787842f);            
//            pCreature->setFaction(35); 
//        }
//
//        Unit *pTarget;
//        uint32 SpellTimer,MoviTimer;
//        int8 x,y;  
//
//        void Escudo()
//        {
//            if (Aura *cAura = me->GetAura(DEFEND))
//            {
//                switch (cAura->GetStackAmount())
//                {
//                    case 1:
//                        me->RemoveAura(SHIELD_LEVEL2);
//                        me->RemoveAura(SHIELD_LEVEL3);
//                        me->CastSpell(me, SHIELD_LEVEL1, true);
//                        break;
//                    case 2:
//                        me->RemoveAura(SHIELD_LEVEL1);
//                        me->RemoveAura(SHIELD_LEVEL3);
//                        me->CastSpell(me, SHIELD_LEVEL2, true);
//                        break;
//                    case 3:
//                        me->RemoveAura(SHIELD_LEVEL2);
//                        me->RemoveAura(SHIELD_LEVEL1);
//                        me->CastSpell(me, SHIELD_LEVEL3, true);
//                        break;
//                }
//            }
//        }
//
//        void Reset(){ }
//
//        void MovementInform(uint32 uiType, uint32 /*uiId*/)
//        {
//            if (uiType != POINT_MOTION_TYPE)
//                return;
//            me->setFaction(14);
//        }
//
//        void DamageTaken(Unit* pDoneBy, uint32& uiDamage)
//        {
//            if (pDoneBy)
//            {
//                if (uiDamage > me->GetHealth() && (pDoneBy->GetTypeId() == TYPEID_PLAYER || pDoneBy->GetOwner()))
//                { 
//                    me->CastSpell(me,YELLOW_ROSE_FIREWORK , true); // Fuegos Articiales al derrotarlo
//                    DoScriptText(SAY_END_VALIANT , me);
//                    uiDamage = 0;
//
//                    if (pDoneBy->GetOwner())
//                        (pDoneBy->GetOwner())->ToPlayer()->KilledMonsterCredit(NPC_ARGENT_CHAMPION_CREDIT,0);
//                    if (pDoneBy->GetTypeId() == TYPEID_PLAYER)
//                        pDoneBy->ToPlayer()->KilledMonsterCredit(NPC_ARGENT_CHAMPION_CREDIT,0);
//                    
//                    me->setFaction(35);
//                    me->DespawnOrUnsummon(5000);
//                    me->SetHomePosition(me->GetPositionX(),me->GetPositionY(),me->GetPositionZ(),me->GetOrientation());
//                    EnterEvadeMode();
//                }
//            }
//        }
//
//        void KilledUnit(Unit* /*victim*/)
//        {
//            me->setFaction(35);
//            me->DespawnOrUnsummon(5000);           
//            me->CombatStop(true);
//        }      
//       
//        void EnterCombat(Unit* /*who*/)
//        {
//            DoScriptText(SAY_START_VALIANT, me);
//            SpellTimer= urand(TIMER_SPELL_MIN,TIMER_SPELL_MAX);
//            MoviTimer = urand(TIMER_MoviTimer_MIN,TIMER_MoviTimer_MAX);
//        }
//
//        void SpellHit(Unit *caster, const SpellEntry *spell)
//        {
//            if ((spell->Id == SHIELD_BREAKER)||(spell->Id == CHARGE))
//            {
//                if (Aura *cAura = me->GetAura(DEFEND))
//                {
//                    switch (cAura->GetStackAmount())
//                    {
//                        case 0:
//                        case 1:
//                            me->RemoveAura(DEFEND);
//                            me->RemoveAura(SHIELD_LEVEL2);
//                            me->RemoveAura(SHIELD_LEVEL3);
//                            me->RemoveAura(SHIELD_LEVEL1);
//                            break;
//                        case 2:
//                            cAura->SetStackAmount(1);
//                            break;
//                        case 3:
//                            cAura->SetStackAmount(2);
//                            break;
//                    }
//                }
//                Escudo();
//            }
//        }
//
//        void SpellHitTarget(Unit *pTarget, const SpellEntry *spell)
//        {
//            if ((spell->Id == SHIELD_BREAKER)||(spell->Id == CHARGE))
//            {
//                if (Aura *pAura = pTarget->GetAura(DEFEND))
//                {
//                    switch (pAura->GetStackAmount())
//                    {
//                        case 0:
//                            pTarget->RemoveAura(DEFEND);
//                            break;
//                        case 1:
//                            pAura->SetStackAmount(0);
//                            break;
//                        case 2:
//                            pAura->SetStackAmount(1);
//                            break;
//                        case 3:
//                            pAura->SetStackAmount(2);
//                            break;
//                    }
//                }
//            }
//        }
//       
//        void UpdateAI(const uint32 uiDiff)
//        {
//            if (!UpdateVictim())
//                return;
//
//            pTarget = SelectTarget(SELECT_TARGET_TOPAGGRO, 0, 200, true);
//               
//            if (SpellTimer<=uiDiff)
//            {
//                if (pTarget && me->IsInRange(pTarget,5,40,false))
//                {
//                    switch (urand(0,7))
//                    {
//                        case 0:
//                        case 1:
//                        case 2:
//                            DoCastAOE(SHIELD_BREAKER, true);
//                            break;
//                        case 3: 
//                        case 4:
//                        case 5:
//                        case 6:
//                            DoCastAOE(CHARGE, true);
//                            break;
//                        case 7:
//                            me->CastSpell(me, DEFEND, true);
//                            Escudo();
//                            break;
//                    }
//                }
//                else
//                {
//                    switch (urand(0,4))
//                    {
//                        case 0: 
//                            me->CastSpell(me, DEFEND, true);
//                            Escudo();
//                            break;
//                        case 1:
//                        case 2:
//                        case 3:
//                        case 4:
//                            DoCastAOE(THRUST, true);
//                            break;
//                    }
//                }
//                SpellTimer= urand(TIMER_SPELL_MIN,TIMER_SPELL_MAX );
//            } else 
//                SpellTimer -= uiDiff;
//
//            if (MoviTimer<=uiDiff)
//            {
//                x =urand(0,7);
//                y =urand(0,7);
//                switch (urand(0,3))
//                {
//                    case 0:
//                        me->GetMotionMaster()->MovePoint(0, (pTarget->GetPositionX()+x), (pTarget->GetPositionY()+y), pTarget->GetPositionZ());
//                        break;
//                    case 1: 
//                        me->GetMotionMaster()->MovePoint(0, (pTarget->GetPositionX()-x), (pTarget->GetPositionY()+y), pTarget->GetPositionZ());
//                        break;
//                    case 2: 
//                        me->GetMotionMaster()->MovePoint(0, (pTarget->GetPositionX()+x), (pTarget->GetPositionY()-y), pTarget->GetPositionZ());
//                        break;
//                    case 3: 
//                        me->GetMotionMaster()->MovePoint(0, (pTarget->GetPositionX()-x), (pTarget->GetPositionY()-y), pTarget->GetPositionZ());
//                        break;
//                }
//                MoviTimer = urand(TIMER_MoviTimer_MIN,TIMER_MoviTimer_MAX);
//            } else
//                MoviTimer -= uiDiff;
//
//                DoMeleeAttackIfReady();
//        }
//    };
//
//    CreatureAI *GetAI(Creature *creature) const
//    {
//        return new npc_argent_championAI(creature);
//    }
//};
//
///*######
//## Argent Tournament: Quest There's Something About the Squire "13654"
//######*/
//
//enum Maloric
//{
//    SPELL_INCAPACITATE_MALORIC              = 63124,
//    SPELL_SEARCH_MALORIC_CREDIT             = 63126,
//    QUEST_THERES_SOMETHING_ABOUT_THE_SQUIRE = 13654
//};
//
//class npc_maloric : public CreatureScript
//{
//public:
//    npc_maloric() : CreatureScript("npc_maloric") {}
//
//    struct npc_maloricAI : public ScriptedAI
//    {
//        npc_maloricAI(Creature *c) : ScriptedAI(c) {}
//        
//        void Reset() {}
//
//        void SpellHit(Unit* pCaster, const SpellEntry* spell)
//        {
//            if (pCaster->GetTypeId() == TYPEID_PLAYER)
//            {
//                if (spell->Id == SPELL_INCAPACITATE_MALORIC && CAST_PLR(pCaster)->GetQuestStatus(QUEST_THERES_SOMETHING_ABOUT_THE_SQUIRE) == QUEST_STATUS_INCOMPLETE)
//                {
//                    DoCast(pCaster, SPELL_SEARCH_MALORIC_CREDIT, true);
//                    me->DespawnOrUnsummon();
//                }        
//            }           
//        }
//    };
//
//    CreatureAI *GetAI(Creature *creature) const
//    {
//        return new npc_maloricAI(creature);
//    }
//};
//
///*######
//##  npc_Scarlet_Onslaught 
//######*/
//
//enum Scarlet_Onslaught
//{
//    SPELL_DARKMENDERS_TINCTURE         = 52741, 
//    QUEST_FROM_THEIR_CORPORSES_RISE    = 12813,
//    NPC_CREDIT_BUNNY                   = 29398 // Quest template
//};
//
//class npc_Scarlet_Onslaught : public CreatureScript
//{
//public:
//    npc_Scarlet_Onslaught() : CreatureScript("npc_Scarlet_Onslaught") {}
//
//    CreatureAI *GetAI(Creature *creature) const
//    {
//        return new npc_Scarlet_OnslaughtAI(creature);
//    }	
//
//    struct npc_Scarlet_OnslaughtAI : public ScriptedAI
//    {
//        npc_Scarlet_OnslaughtAI(Creature *c) : ScriptedAI(c) {}
//
//        void SpellHit(Unit* pCaster, const SpellEntry* spell)
//        {
//            if (pCaster->GetTypeId() == TYPEID_PLAYER)
//            {
//                if (spell->Id == SPELL_DARKMENDERS_TINCTURE && CAST_PLR(pCaster)->GetQuestStatus(QUEST_FROM_THEIR_CORPORSES_RISE) == QUEST_STATUS_INCOMPLETE)
//                    me->DespawnOrUnsummon();
//            }           
//        }
//	};
//};
//
///*######
//## Quest 13042
//## "Deep in the Bowels of The Underhalls"
//######*/
//enum eOsterkilgr
//{
//    SAY_QUEST1                                   = -1603500,
//    SAY_QUEST2                                   = -1603501,    
//    SPELL_FIREBALL                               = 14034,
//    SPELL_FIRE_WAVE                              = 60290,
//    QUEST_DEEP_IN_THE_BOWELS_OF_THE_UNDERHALLS   = 13042,
//    CREATURE_CREDIT_BUNNY                        = 30412,
//};
//
//class npc_apprentice_osterkilgr : public CreatureScript
//{
//public:
//    npc_apprentice_osterkilgr() : CreatureScript("npc_apprentice_osterkilgr") { }
//
//    CreatureAI *GetAI(Creature *pCreature) const
//    {
//        return new npc_apprentice_osterkilgrAI(pCreature);
//    }
//
//    struct npc_apprentice_osterkilgrAI : public ScriptedAI
//    {
//        npc_apprentice_osterkilgrAI(Creature *pCreature) : ScriptedAI(pCreature) { }
//
//        uint64 uiPlayerGUID;
//        uint32 uiBlastWaveTimer;
//        uint32 uiFireballTimer;
//        uint32 uiYellTimer;
//        bool bYelled;
//
//        void Reset()
//        {
//            uiPlayerGUID = NULL;
//            uiBlastWaveTimer = urand(8000,10000);
//            uiFireballTimer = urand(2000,3000);
//            bYelled = false;
//        }
//
//        void EnterCombat(Unit * pWho)
//        {
//            if (pWho->GetTypeId() == TYPEID_PLAYER)
//                uiPlayerGUID = pWho->GetGUID();                
//        }
//
//        void UpdateAI(const uint32 uiDiff)
//        {
//            if (!UpdateVictim())
//                return;
//            
//            if (Player *pPlayer = Unit::GetPlayer(*me, uiPlayerGUID))
//            {                
//                if (HealthBelowPct(40))
//                {
//                    if (!bYelled)
//                    {
//                        DoScriptText(SAY_QUEST1, me);
//                        DoScriptText(SAY_QUEST2, me);
//                        bYelled = true;
//                    }
//
//                    if (Group *pGroup = pPlayer->GetGroup())
//                    {
//                        for (GroupReference *itr = pGroup->GetFirstMember(); itr != NULL; itr = itr->next())
//                        {
//                            Player *PlayerGroup = itr->getSource();
//                            if (PlayerGroup->isAlive() && PlayerGroup->IsAtGroupRewardDistance(me))
//                                PlayerGroup->KilledMonsterCredit(CREATURE_CREDIT_BUNNY, 0);
//                        }
//                    }
//                    else
//                        pPlayer->KilledMonsterCredit(CREATURE_CREDIT_BUNNY, 0);
//                }
//            }
//
//            if (uiBlastWaveTimer <= uiDiff)
//            {
//                DoCast(me->getVictim(), SPELL_FIRE_WAVE);
//                uiBlastWaveTimer = urand(8000,10000);
//            }
//            else
//                uiBlastWaveTimer -= uiDiff;
//
//            if (uiFireballTimer <= uiDiff)
//            {
//                DoCast(me->getVictim(), SPELL_FIREBALL);
//                uiFireballTimer = urand(3000,4000);
//            }
//            else
//                uiFireballTimer -= uiDiff;
//
//            DoMeleeAttackIfReady();
//        }
//    };
//};
//
///*######
//## Quest 13231 - 13228 (A/H) - 13232
//## "The Broken Front"
//## "Finish Me!"
//######*/
//enum eSoldier
//{
//    SAY_KILL_QUEST1                              = -1603511,
//    SAY_KILL_QUEST2                              = -1603512,
//    SAY_KILL_QUEST3                              = -1603513,
//    SAY_KILL_QUEST4                              = -1603514,
//    SAY_KILL_QUEST5                              = -1603515,
//    SAY_QUEST_SOLDIER1                           = -1603516,
//    SAY_QUEST_SOLDIER2                           = -1603517,
//    SAY_QUEST_SOLDIER3                           = -1603518,
//    SAY_QUEST_SOLDIER4                           = -1603519,
//    SAY_QUEST_SOLDIER5                           = -1603520,
//    SAY_QUEST_SOLDIER6                           = -1603521,
//    SAY_QUEST_SOLDIER7                           = -1603522,
//    SAY_QUEST_BERSERKER1                         = -1603523,
//    SAY_QUEST_BERSERKER2                         = -1603524,
//    SAY_QUEST_BERSERKER3                         = -1603525,
//    SAY_QUEST_BERSERKER4                         = -1603526,
//    SAY_QUEST_BERSERKER5                         = -1603527,
//    SAY_QUEST_BERSERKER6                         = -1603528,
//    SAY_QUEST_BERSERKER7                         = -1603529,
//    SAY_QUEST_BERSERKER8                         = -1603530,
//    CREATURE_DYING_SOLDIER_KC                    = 31312,
//    CREATURE_DYING_BERSERKER_KC                  = 31272,
//    QUEST_FINISH_ME                              = 13232,
//    QUEST_THE_BROKEN_FRONT_A                     = 13231,
//    QUEST_THE_BROKEN_FRONT_H                     = 13228,
//};
//
//#define GOSSIP_ITEM_DYING_SOLDIER   "Travel well, hero of the Alliance!"
//#define GOSSIP_ITEM_DYING_SOLDIER1  "Stay with me, friend. I must know what happened here."
//#define GOSSIP_ITEM_DYING_BERSERKER "Stay with me, friend. I must know what happened here."
//
//class npc_dying_soldier : public CreatureScript
//{
//public:
//    npc_dying_soldier() : CreatureScript("npc_dying_soldier") { }
//
//    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
//    {
//        if (pPlayer->GetQuestStatus(QUEST_FINISH_ME) == QUEST_STATUS_INCOMPLETE)
//            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_DYING_SOLDIER, GOSSIP_SENDER_MAIN, GOSSIP_SENDER_INFO);
//
//        if (pPlayer->GetQuestStatus(QUEST_THE_BROKEN_FRONT_A) == QUEST_STATUS_INCOMPLETE)
//            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_DYING_SOLDIER1, GOSSIP_SENDER_MAIN, GOSSIP_SENDER_INFO+1);
//            
//        pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
//
//        return true;
//    }
//
//    bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
//    {
//        pPlayer->PlayerTalkClass->ClearMenus();
//        
//        if (uiAction == GOSSIP_SENDER_INFO)
//        {
//            pPlayer->CLOSE_GOSSIP_MENU();
//            if (pPlayer->GetQuestStatus(QUEST_FINISH_ME) == QUEST_STATUS_INCOMPLETE)
//                pPlayer->KilledMonsterCredit(CREATURE_DYING_SOLDIER_KC, 0);
//            pCreature->Kill(pCreature);
//            DoScriptText(RAND(SAY_KILL_QUEST1, SAY_KILL_QUEST2, SAY_KILL_QUEST3, SAY_KILL_QUEST4, SAY_KILL_QUEST5), pCreature);
//        }
//
//        if (uiAction == GOSSIP_SENDER_INFO+1)
//        {
//            pPlayer->CLOSE_GOSSIP_MENU();
//            DoScriptText(SAY_QUEST_SOLDIER1, pCreature);
//            CAST_AI(npc_dying_soldier::npc_dying_soldierAI, pCreature->AI())->uiPlayerGUID = pPlayer->GetGUID();
//            CAST_AI(npc_dying_soldier::npc_dying_soldierAI, pCreature->AI())->uiTalkTimer = 3000;
//            CAST_AI(npc_dying_soldier::npc_dying_soldierAI, pCreature->AI())->bTalkTime = true;
//        }
//
//        return true;
//    }
//    
//    CreatureAI *GetAI(Creature *pCreature) const
//    {
//        return new npc_dying_soldierAI(pCreature);
//    }
//
//    struct npc_dying_soldierAI : public ScriptedAI
//    {
//        npc_dying_soldierAI(Creature *pCreature) : ScriptedAI(pCreature) { }
//
//        uint64 uiPlayerGUID;
//        uint32 uiTalkTimer;
//        uint32 uiStep;
//        bool bTalkTime;
//
//        void Reset()
//        {
//            uiPlayerGUID = 0;
//            bTalkTime = false;
//            uiStep = 0;
//        }
//
//        void UpdateAI(const uint32 uiDiff)
//        {
//            if (bTalkTime && uiTalkTimer <= uiDiff)
//            {
//                if (Player *pPlayer = Unit::GetPlayer(*me, uiPlayerGUID))
//                {
//                    switch (uiStep)
//                    {
//                        case 0:
//                            DoScriptText(SAY_QUEST_SOLDIER2, me);
//                            uiTalkTimer = 3000;
//                            ++uiStep;
//                            break;
//                        case 1:
//                            DoScriptText(SAY_QUEST_SOLDIER3, pPlayer);
//                            uiTalkTimer = 3000;
//                            ++uiStep;
//                            break;
//                        case 2:
//                            DoScriptText(SAY_QUEST_SOLDIER4, me);
//                            uiTalkTimer = 3000;
//                            ++uiStep;
//                            break;
//                        case 3:
//                            DoScriptText(SAY_QUEST_SOLDIER5, me);
//                            uiTalkTimer = 3000;
//                            ++uiStep;
//                            break;
//                        case 4:
//                            DoScriptText(SAY_QUEST_SOLDIER6, me);
//                            uiTalkTimer = 3000;
//                            ++uiStep;
//                            break;
//                        case 5:
//                            DoScriptText(SAY_QUEST_SOLDIER7, me);
//                            if (pPlayer->GetQuestStatus(QUEST_THE_BROKEN_FRONT_A) == QUEST_STATUS_INCOMPLETE)
//                                pPlayer->KilledMonsterCredit(CREATURE_DYING_SOLDIER_KC, 0);
//                            bTalkTime = false;
//                            uiStep = 0;
//                            break;
//                    }
//                }
//            }
//            else
//                uiTalkTimer -= uiDiff;
//        }
//    };
//};
//
//class npc_dying_berserker : public CreatureScript
//{
//public:
//    npc_dying_berserker() : CreatureScript("npc_dying_berserker") { }
//
//    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
//    {
//        if (pPlayer->GetQuestStatus(QUEST_THE_BROKEN_FRONT_H) == QUEST_STATUS_INCOMPLETE)
//            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_DYING_BERSERKER, GOSSIP_SENDER_MAIN, GOSSIP_SENDER_INFO);
//            
//        pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
//
//        return true;
//    }
//
//    bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
//    {
//        pPlayer->PlayerTalkClass->ClearMenus();
//        
//        if (uiAction == GOSSIP_SENDER_INFO)
//        {
//            pPlayer->CLOSE_GOSSIP_MENU();
//            DoScriptText(SAY_QUEST_BERSERKER1, pCreature);
//            CAST_AI(npc_dying_berserker::npc_dying_berserkerAI, pCreature->AI())->uiPlayerGUID = pPlayer->GetGUID();
//            CAST_AI(npc_dying_berserker::npc_dying_berserkerAI, pCreature->AI())->uiTalkTimer = 3000;
//            CAST_AI(npc_dying_berserker::npc_dying_berserkerAI, pCreature->AI())->bTalkTime = true;
//        }
//
//        return true;
//    }
//
//    CreatureAI *GetAI(Creature *pCreature) const
//    {
//        return new npc_dying_berserkerAI(pCreature);
//    }
//
//    struct npc_dying_berserkerAI : public ScriptedAI
//    {
//        npc_dying_berserkerAI(Creature *pCreature) : ScriptedAI(pCreature) { }
//
//        uint64 uiPlayerGUID;
//        uint32 uiTalkTimer;
//        uint32 uiStep;
//        bool bTalkTime;
//
//        void Reset()
//        {
//            uiPlayerGUID = 0;
//            bTalkTime = false;
//            uiStep = 0;
//        }
//
//        void UpdateAI(const uint32 uiDiff)
//        {
//            if (bTalkTime && uiTalkTimer <= uiDiff)
//            {
//                if (Player *pPlayer = Unit::GetPlayer(*me, uiPlayerGUID))
//                {
//                    switch (uiStep)
//                    {
//                        case 0:
//                            DoScriptText(SAY_QUEST_BERSERKER2, me);
//                            uiTalkTimer = 3000;
//                            ++uiStep;
//                            break;
//                        case 1:
//                            DoScriptText(SAY_QUEST_BERSERKER3, pPlayer);
//                            uiTalkTimer = 3000;
//                            ++uiStep;
//                            break;
//                        case 2:
//                            DoScriptText(SAY_QUEST_BERSERKER4, me);
//                            uiTalkTimer = 3000;
//                            ++uiStep;
//                            break;
//                        case 3:
//                            DoScriptText(SAY_QUEST_BERSERKER5, me);
//                            uiTalkTimer = 3000;
//                            ++uiStep;
//                            break;
//                        case 4:
//                            DoScriptText(SAY_QUEST_BERSERKER6, me);
//                            uiTalkTimer = 3000;
//                            ++uiStep;
//                            break;
//                        case 5:
//                            DoScriptText(SAY_QUEST_BERSERKER7, me);
//                            uiTalkTimer = 3000;
//                            ++uiStep;
//                            break;
//                        case 6:
//                            DoScriptText(SAY_QUEST_BERSERKER8, me);
//                            if (pPlayer->GetQuestStatus(QUEST_THE_BROKEN_FRONT_H) == QUEST_STATUS_INCOMPLETE)
//                                pPlayer->KilledMonsterCredit(CREATURE_DYING_BERSERKER_KC, 0);
//                            bTalkTime = false;
//                            uiStep = 0;
//                            break;
//                    }
//                }
//            }
//            else
//                uiTalkTimer -= uiDiff;
//        }
//    };
//};
//
//
//enum BlackKnightOrders
//{
//  QUEST_THE_BLACK_KNIGHT_ORDERS = 13663,
//  NPC_CREDIT_BLACK_GRYPHON      = 33519,
//  SPELL_EJECT_PASSENGER         = 50630,
//};
//
//const Position BlackKnightGryphonWaypoints[44] =
//{
//    {8521.271f,  569.596f,  552.8375f},
//    {8517.864f,  579.1095f, 553.2125f},
//    {8513.146f,  594.6724f, 551.2125f},
//	{8505.263f, 606.5569f, 550.4177f},
//	{8503.017f, 628.4188f, 547.4177f},
//    {8480.271f, 652.7083f, 547.4177f},
//    {8459.121f, 686.1427f, 547.4177f},
//    {8436.802f, 713.8687f, 547.3428f},
//    {8405.380f, 740.0045f, 547.4177f},
//    {8386.139f, 770.6009f, 547.5881f},
//    {8374.297f, 802.2525f, 547.9304f},
//    {8374.271f, 847.0363f, 548.0427f},
//    {8385.988f, 868.9881f, 548.0491f},
//    {8413.027f, 867.8573f, 547.2991f},
//    {8452.552f, 869.0339f, 547.2991f},
//    {8473.058f, 875.2012f, 547.2955f},
//    {8472.278f, 912.3134f, 547.4169f},
//    {8479.666f, 954.1650f, 547.3298f},
//    {8477.349f, 1001.368f, 547.3372f},
//    {8484.538f, 1025.797f, 547.4622f},
//    {8525.363f, 1029.284f, 547.4177f},
//    {8532.808f, 1052.904f, 548.1677f},
//    {8537.356f, 1077.927f, 554.5791f},
//    {8540.528f, 1083.379f, 569.6827f},
//    {8563.641f, 1140.965f, 569.6827f},
//    {8594.897f, 1205.458f, 569.6827f},
//    {8617.104f, 1257.399f, 566.1833f},
//    {8648.496f, 1329.349f, 558.0187f},
//    {8667.723f, 1388.411f, 546.188f},
//    {8699.145f, 1474.898f, 528.2197f},
//    {8726.869f, 1546.006f, 501.7741f},
//    {8739.058f, 1592.157f, 478.5511f},
//    {8750.799f, 1636.771f, 455.0797f},
//    {8760.006f, 1669.482f, 423.2208f},
//    {8783.31f, 1701.852f, 375.8872f},
//    {8817.336f, 1735.731f, 343.3323f},
//    {8882.32f, 1789.754f, 301.5807f},
//    {8958.597f, 1841.807f, 259.9141f},
//	{9045.891f, 1908.076f, 233.4143f},
//    {9107.177f, 1964.594f, 215.9704f},
//    {9134.763f, 2036.925f, 175.1925f},
//    {9128.608f, 2089.091f, 141.3593f},
//    {9093.364f, 2128.384f, 99.38685f},
//    {9050.709f, 2123.656f, 60.24802f}
//};
//class npc_black_knight_gryphon : public CreatureScript
//{
//public:
//    npc_black_knight_gryphon() : CreatureScript("npc_black_knight_gryphon") { }
//
//    struct npc_black_knight_gryphonAI : public ScriptedAI
//    {
//        npc_black_knight_gryphonAI(Creature* creature) : ScriptedAI(creature) { }
//
//        uint8 count;
//        bool wp_reached;
//        bool movementStarted;
//
//    void Reset()
//        {
//            count = 0;
//            wp_reached = false;
//            movementStarted = false;
//        }
//
//        void PassengerBoarded(Unit* who, int8 /*seatId*/, bool apply) 
//        {
//            if (who && apply)
//            {
//                    wp_reached = true;
//                    //! HACK: Creature's can't have MOVEMENTFLAG_FLYING
//                    // Fly Away
//                    me->AddUnitMovementFlag(MOVEMENTFLAG_CAN_FLY | MOVEMENTFLAG_FLYING);
//                    me->SetSpeed(MOVE_FLIGHT, 5.0f);
//            }
//        }
//
//        void MovementInform(uint32 type, uint32 id)
//        {
//            if (type != POINT_MOTION_TYPE || id != count)
//                return;
//
//            if (id < 43)
//            {
//                ++count;
//                wp_reached = true;
//            }
//            else 
//            {
//                Unit* player = me->GetVehicleKit()->GetPassenger(0);
//                if (player && player->GetTypeId() == TYPEID_PLAYER && player->ToPlayer()->GetQuestStatus(QUEST_THE_BLACK_KNIGHT_ORDERS) == QUEST_STATUS_INCOMPLETE)
//                {
//                    player->ToPlayer()->KilledMonsterCredit(NPC_CREDIT_BLACK_GRYPHON, 0);
//                    me->CastSpell(player,SPELL_EJECT_PASSENGER,true);
//                    me->DespawnOrUnsummon(5000);
//                }
//            }
//        }
//
//        void UpdateAI(const uint32 /*diff*/)
//        {
//            if (!me->isCharmed() && !movementStarted)
//            {
//                movementStarted = true;
//            }
//
//            if (wp_reached)
//            {
//                wp_reached = false;
//                me->GetMotionMaster()->MovePoint(count, BlackKnightGryphonWaypoints[count]);
//            }
//        }
//
//    };
//
//    CreatureAI* GetAI(Creature* creature) const
//    {
//        return new npc_black_knight_gryphonAI(creature);
//    }
//};
