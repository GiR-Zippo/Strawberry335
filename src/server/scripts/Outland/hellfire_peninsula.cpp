/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Hellfire_Peninsula
SD%Complete: 100
SDComment: Quest support: 9375, 9410, 9418, 10129, 10146, 10162, 10163, 10340, 10346, 10347, 10382 (Special flight paths)
SDLaenalith: Quest support: 10838(#2749)
SDCategory: Hellfire Peninsula
EndScriptData */

/* ContentData
npc_aeranas
npc_ancestral_wolf
go_haaleshi_altar
npc_naladu
npc_tracy_proudwell
npc_trollbane
npc_wounded_blood_elf
EndContentData */

#include "ScriptPCH.h"
#include "ScriptedEscortAI.h"

/*######
## npc_aeranas
######*/

enum eAeranas
{
    SAY_SUMMON              = -1000138,
    SAY_FREE                = -1000139,

    FACTION_HOSTILE         = 16,
    FACTION_FRIENDLY        = 35,

    SPELL_ENVELOPING_WINDS  = 15535,
    SPELL_SHOCK             = 12553,

    C_AERANAS               = 17085
};

class npc_aeranas : public CreatureScript
{
public:
    npc_aeranas() : CreatureScript("npc_aeranas") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_aeranasAI (creature);
    }

    struct npc_aeranasAI : public ScriptedAI
    {
        npc_aeranasAI(Creature* creature) : ScriptedAI(creature) {}

        uint32 Faction_Timer;
        uint32 EnvelopingWinds_Timer;
        uint32 Shock_Timer;

        void Reset()
        {
            Faction_Timer = 8000;
            EnvelopingWinds_Timer = 9000;
            Shock_Timer = 5000;

            me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
            me->setFaction(FACTION_FRIENDLY);

            DoScriptText(SAY_SUMMON, me);
        }

        void UpdateAI(const uint32 diff)
        {
            if (Faction_Timer)
            {
                if (Faction_Timer <= diff)
                {
                    me->setFaction(FACTION_HOSTILE);
                    Faction_Timer = 0;
                } else Faction_Timer -= diff;
            }

            if (!UpdateVictim())
                return;

            if (HealthBelowPct(30))
            {
                me->setFaction(FACTION_FRIENDLY);
                me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                me->RemoveAllAuras();
                me->DeleteThreatList();
                me->CombatStop(true);
                DoScriptText(SAY_FREE, me);
                return;
            }

            if (Shock_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_SHOCK);
                Shock_Timer = 10000;
            } else Shock_Timer -= diff;

            if (EnvelopingWinds_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_ENVELOPING_WINDS);
                EnvelopingWinds_Timer = 25000;
            } else EnvelopingWinds_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    };
};

/*######
## npc_ancestral_wolf
######*/

enum eAncestralWolf
{
    EMOTE_WOLF_LIFT_HEAD            = -1000496,
    EMOTE_WOLF_HOWL                 = -1000497,
    SAY_WOLF_WELCOME                = -1000498,

    SPELL_ANCESTRAL_WOLF_BUFF       = 29938,

    NPC_RYGA                        = 17123
};

class npc_ancestral_wolf : public CreatureScript
{
public:
    npc_ancestral_wolf() : CreatureScript("npc_ancestral_wolf") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_ancestral_wolfAI(creature);
    }

    struct npc_ancestral_wolfAI : public npc_escortAI
    {
        npc_ancestral_wolfAI(Creature* creature) : npc_escortAI(creature)
        {
            if (creature->GetOwner() && creature->GetOwner()->GetTypeId() == TYPEID_PLAYER)
                Start(false, false, creature->GetOwner()->GetGUID());
            else
                sLog->outError("TRINITY: npc_ancestral_wolf can not obtain owner or owner is not a player.");

            creature->SetSpeed(MOVE_WALK, 1.5f);
        }

        Unit* pRyga;

        void Reset()
        {
            pRyga = NULL;
        }

        void MoveInLineOfSight(Unit* who)
        {
            if (!pRyga && who->GetTypeId() == TYPEID_UNIT && who->GetEntry() == NPC_RYGA && me->IsWithinDistInMap(who, 15.0f))
                pRyga = who;

            npc_escortAI::MoveInLineOfSight(who);
        }

        void WaypointReached(uint32 waypointId)
        {
            switch (waypointId)
            {
                case 0:
                    me->AddAura(SPELL_ANCESTRAL_WOLF_BUFF, me);
                    DoScriptText(EMOTE_WOLF_LIFT_HEAD, me);
                    break;
                case 2:
                    DoScriptText(EMOTE_WOLF_HOWL, me);
                    break;
                case 50:
                    if (pRyga && pRyga->isAlive() && !pRyga->isInCombat())
                        DoScriptText(SAY_WOLF_WELCOME, pRyga);
                    break;
            }
        }
    };
};

/*######
## go_haaleshi_altar
######*/

class go_haaleshi_altar : public GameObjectScript
{
public:
    go_haaleshi_altar() : GameObjectScript("go_haaleshi_altar") { }

    bool OnGossipHello(Player* /*player*/, GameObject* go)
    {
        go->SummonCreature(C_AERANAS, -1321.79f, 4043.80f, 116.24f, 1.25f, TEMPSUMMON_TIMED_DESPAWN, 180000);
        return false;
    }
};

/*######
## npc_naladu
######*/

#define GOSSIP_NALADU_ITEM1 "Why don't you escape?"

enum eNaladu
{
    GOSSIP_TEXTID_NALADU1   = 9788
};

class npc_naladu : public CreatureScript
{
public:
    npc_naladu() : CreatureScript("npc_naladu") { }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();
        if (action == GOSSIP_ACTION_INFO_DEF+1)
            player->SEND_GOSSIP_MENU(GOSSIP_TEXTID_NALADU1, creature->GetGUID());

        return true;
    }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if (creature->isQuestGiver())
            player->PrepareQuestMenu(creature->GetGUID());

        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_NALADU_ITEM1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
        player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
        return true;
    }
};

/*######
## npc_tracy_proudwell
######*/

#define GOSSIP_TEXT_REDEEM_MARKS        "I have marks to redeem!"
#define GOSSIP_TRACY_PROUDWELL_ITEM1    "I heard that your dog Fei Fei took Klatu's prayer beads..."
#define GOSSIP_TRACY_PROUDWELL_ITEM2    "<back>"

enum eTracy
{
    GOSSIP_TEXTID_TRACY_PROUDWELL1       = 10689,
    QUEST_DIGGING_FOR_PRAYER_BEADS       = 10916
};

class npc_tracy_proudwell : public CreatureScript
{
public:
    npc_tracy_proudwell() : CreatureScript("npc_tracy_proudwell") { }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();
        switch (action)
        {
            case GOSSIP_ACTION_INFO_DEF+1:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_TRACY_PROUDWELL_ITEM2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
                player->SEND_GOSSIP_MENU(GOSSIP_TEXTID_TRACY_PROUDWELL1, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+2:
                player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
                break;
            case GOSSIP_ACTION_TRADE:
                player->GetSession()->SendListInventory(creature->GetGUID());
                break;
        }

        return true;
    }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if (creature->isQuestGiver())
            player->PrepareQuestMenu(creature->GetGUID());

        if (creature->isVendor())
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, GOSSIP_TEXT_REDEEM_MARKS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);

        if (player->GetQuestStatus(QUEST_DIGGING_FOR_PRAYER_BEADS) == QUEST_STATUS_INCOMPLETE)
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_TRACY_PROUDWELL_ITEM1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

        player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
        return true;
    }
};

/*######
## npc_trollbane
######*/

#define GOSSIP_TROLLBANE_ITEM1      "Tell me of the Sons of Lothar."
#define GOSSIP_TROLLBANE_ITEM2      "<more>"
#define GOSSIP_TROLLBANE_ITEM3      "Tell me of your homeland."

enum eTrollbane
{
    GOSSIP_TEXTID_TROLLBANE1        = 9932,
    GOSSIP_TEXTID_TROLLBANE2        = 9933,
    GOSSIP_TEXTID_TROLLBANE3        = 8772
};

class npc_trollbane : public CreatureScript
{
public:
    npc_trollbane() : CreatureScript("npc_trollbane") { }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();
        switch (action)
        {
            case GOSSIP_ACTION_INFO_DEF+1:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_TROLLBANE_ITEM2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
                player->SEND_GOSSIP_MENU(GOSSIP_TEXTID_TROLLBANE1, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+2:
                player->SEND_GOSSIP_MENU(GOSSIP_TEXTID_TROLLBANE2, creature->GetGUID());
                break;
            case GOSSIP_ACTION_INFO_DEF+3:
                player->SEND_GOSSIP_MENU(GOSSIP_TEXTID_TROLLBANE3, creature->GetGUID());
                break;
        }

        return true;
    }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if (creature->isQuestGiver())
            player->PrepareQuestMenu(creature->GetGUID());

        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_TROLLBANE_ITEM1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_TROLLBANE_ITEM3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
        player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
        return true;
    }
};

/*######
## npc_wounded_blood_elf
######*/

enum eWoundedBloodElf
{
    SAY_ELF_START               = -1000117,
    SAY_ELF_SUMMON1             = -1000118,
    SAY_ELF_RESTING             = -1000119,
    SAY_ELF_SUMMON2             = -1000120,
    SAY_ELF_COMPLETE            = -1000121,
    SAY_ELF_AGGRO               = -1000122,

    QUEST_ROAD_TO_FALCON_WATCH  = 9375
};

class npc_wounded_blood_elf : public CreatureScript
{
public:
    npc_wounded_blood_elf() : CreatureScript("npc_wounded_blood_elf") { }

    bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest)
    {
        if (quest->GetQuestId() == QUEST_ROAD_TO_FALCON_WATCH)
        {
            if (npc_escortAI* pEscortAI = CAST_AI(npc_wounded_blood_elf::npc_wounded_blood_elfAI, creature->AI()))
                pEscortAI->Start(true, false, player->GetGUID());

            // Change faction so mobs attack
            creature->setFaction(775);
        }

        return true;
    }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_wounded_blood_elfAI(creature);
    }

    struct npc_wounded_blood_elfAI : public npc_escortAI
    {
        npc_wounded_blood_elfAI(Creature* creature) : npc_escortAI(creature) {}

        void WaypointReached(uint32 waypointId)
        {
            Player* player = GetPlayerForEscort();
            if (!player)
                return;

            switch (waypointId)
            {
                case 0:
                    DoScriptText(SAY_ELF_START, me, player);
                    break;
                case 9:
                    DoScriptText(SAY_ELF_SUMMON1, me, player);
                    // Spawn two Haal'eshi Talonguard
                    DoSpawnCreature(16967, -15, -15, 0, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000);
                    DoSpawnCreature(16967, -17, -17, 0, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000);
                    break;
                case 13:
                    DoScriptText(SAY_ELF_RESTING, me, player);
                    break;
                case 14:
                    DoScriptText(SAY_ELF_SUMMON2, me, player);
                    // Spawn two Haal'eshi Windwalker
                    DoSpawnCreature(16966, -15, -15, 0, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000);
                    DoSpawnCreature(16966, -17, -17, 0, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000);
                    break;
                case 27:
                    DoScriptText(SAY_ELF_COMPLETE, me, player);
                    // Award quest credit
                    player->GroupEventHappens(QUEST_ROAD_TO_FALCON_WATCH, me);
                    break;
            }
        }

        void Reset() { }

        void EnterCombat(Unit* /*who*/)
        {
            if (HasEscortState(STATE_ESCORT_ESCORTING))
                DoScriptText(SAY_ELF_AGGRO, me);
        }

        void JustSummoned(Creature* summoned)
        {
            summoned->AI()->AttackStart(me);
        }
    };
};

/*######
## npc_fel_guard_hound
######*/

enum eFelGuard
{
    SPELL_SUMMON_POO                              = 37688,

    NPC_DERANGED_HELBOAR                          = 16863
};

class npc_fel_guard_hound : public CreatureScript
{
public:
    npc_fel_guard_hound() : CreatureScript("npc_fel_guard_hound") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_fel_guard_houndAI(creature);
    }

    struct npc_fel_guard_houndAI : public ScriptedAI
    {
        npc_fel_guard_houndAI(Creature* creature) : ScriptedAI(creature) {}

        uint32 uiCheckTimer;
        uint64 uiHelboarGUID;

        void Reset()
        {
            uiCheckTimer = 5000; //check for creature every 5 sec
            uiHelboarGUID = 0;
        }

        void MovementInform(uint32 uiType, uint32 uiId)
        {
            if (uiType != POINT_MOTION_TYPE || uiId != 1)
                return;

            if (Creature* pHelboar = me->GetCreature(*me, uiHelboarGUID))
            {
                pHelboar->RemoveCorpse();
                DoCast(SPELL_SUMMON_POO);

                if (Player* owner = me->GetCharmerOrOwnerPlayerOrPlayerItself())
                    me->GetMotionMaster()->MoveFollow(owner, 0.0f, 0.0f);
            }
        }

        void UpdateAI(const uint32 uiDiff)
        {
            if (uiCheckTimer <= uiDiff)
            {
                if (Creature* pHelboar = me->FindNearestCreature(NPC_DERANGED_HELBOAR, 10.0f, false))
                {
                    if (pHelboar->GetGUID() != uiHelboarGUID && me->GetMotionMaster()->GetCurrentMovementGeneratorType() != POINT_MOTION_TYPE && !me->FindCurrentSpellBySpellId(SPELL_SUMMON_POO))
                    {
                        uiHelboarGUID = pHelboar->GetGUID();
                        me->GetMotionMaster()->MovePoint(1, pHelboar->GetPositionX(), pHelboar->GetPositionY(), pHelboar->GetPositionZ());
                    }
                }
                uiCheckTimer = 5000;
            }else uiCheckTimer -= uiDiff;

            if (!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }
    };
};

// Bug #2749 fix
// QuestId:          10838
// CreatureEntry:    22258
// CreatureSummon1:  23100
// CreatureSummon2:  23100
enum TheDemoniacScryer
{
    QUEST_THE_DEMONIAC_SCRYER    = 10838,
    ITEM_DEMONIAC_SCRYER_READING = 31607,
    NPC_FACTION_ENEMY            = 17,
    NPC_SUMMON_ORC               = 19414,
    NPC_SUMMON_IMP               = 17477,

    EVENT_WAVE                   = 1,
    EVENT_FINAL                  = 2,
    EVENT_MAX_TIME               = 3,

    FIRST_WAVE_CREATURES         = 3,
    WAVE_COUNT                   = 4,
    TIMER_MAX_EVENT_TIME         = 5*MINUTE*IN_MILLISECONDS,
};

class npc_q10838_trigger : public CreatureScript
{
public:
    npc_q10838_trigger() : CreatureScript("npc_q10838_trigger"){}

    struct npc_q10838_triggerAI : public ScriptedAI
    {
        npc_q10838_triggerAI(Creature* creature) : ScriptedAI(creature) {}

        void Initialize()
        {
            pPlayer = NULL;
            if (pPlayer = me->FindNearestPlayer(50.0f))
            {
                AddSpawnPosition.Relocate(me);
                AddSpawnPosition.m_positionX -= 9.0f;
                AddSpawnPosition.m_positionY += 25.0f;
                AddSpawnPosition.m_positionZ += 4.5f;
                AddSpawnPosition.m_orientation = 4.73f;
                _currentWave = 0;

                me->SetReactState(REACT_PASSIVE);

                events.ScheduleEvent(EVENT_WAVE, 5*IN_MILLISECONDS);
            }
            else
                me->DespawnOrUnsummon();
        };

        void JustSummoned(Creature* summoned)
        {
            if (pPlayer)
            {
                summoned->setFaction(NPC_FACTION_ENEMY);
                summoned->AddThreat(pPlayer, 0.0f);
                summoned->AI()->AttackStart(pPlayer);
            }
        };

        void UpdateAI(const uint32 diff)
        {
            if (events.Empty())
            {
                Initialize();
                return;
            }

            events.Update(diff);
            ExecuteEvents();
        };

        void ExecuteEvents()
        {
            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_WAVE:
                    {
                        if (_currentWave != WAVE_COUNT)
                        {
                            SpawnWave();
                            ++_currentWave;
                            events.ScheduleEvent(EVENT_WAVE, 20*IN_MILLISECONDS);
                        }
                        else
                            events.ScheduleEvent(EVENT_FINAL, 20*IN_MILLISECONDS);
                    }break;

                    case EVENT_FINAL:
                    {
                        std::list<Player*> PlList = me->GetNearestPlayersList(40.0f);
                        if (!PlList.empty())
                        {
                            std::list<Player*>::iterator itr;
                            for (itr = PlList.begin(); itr != PlList.end(); ++itr)
                            {
                                if ((*itr)->IsActiveQuest(QUEST_THE_DEMONIAC_SCRYER))
                                    (*itr)->AddItem(ITEM_DEMONIAC_SCRYER_READING, 1);
                            }
                        }
                        me->DespawnOrUnsummon();
                    }break;

                    case EVENT_MAX_TIME:
                    {
                        me->DespawnOrUnsummon();
                    }break;
                }
            }
        };

        void SpawnWave()
        {
            Position SpawnPosition = AddSpawnPosition;

            for (uint8 i = 0; i < (FIRST_WAVE_CREATURES+_currentWave); ++i)
            {
                DoSummon(i%2?NPC_SUMMON_ORC:NPC_SUMMON_IMP, SpawnPosition, 30000, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT);
                SpawnPosition.m_positionX += 3.0f;
            }
        };

    private:
        EventMap events;
        uint8 _currentWave;
        Position AddSpawnPosition;
        Player* pPlayer;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_q10838_triggerAI(creature);
    }
};

void AddSC_hellfire_peninsula()
{
    new npc_aeranas();
    new npc_ancestral_wolf();
    new go_haaleshi_altar();
    new npc_naladu();
    new npc_tracy_proudwell();
    new npc_trollbane();
    new npc_wounded_blood_elf();
    new npc_fel_guard_hound();
    new npc_q10838_trigger();
}
