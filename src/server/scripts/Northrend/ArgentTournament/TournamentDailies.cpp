/*
 * Copyright (C) 2012 Zirkon
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

#include "ScriptedFollowerAI.h"
#include "ArgentTournament.h"

/*######
## Quest A Blade Fit For A Champion Daily Argent Tournament.
## http://www.wowhead.com/search?q=A+Blade+Fit+For+A+Champion
## 13603, 13666, 13673, 13741, 13746, 13752, 13757, 13762, 13768, 13773, 13778, 13783
######*/

enum eLakeFrog
{
    SPELL_WARTSBGONE_LIP_BALM       = 62574,
    SPELL_FROG_LOVE                 = 62537,
    SPELL_WARTS                     = 62581,
    NPC_MAIDEN_OF_ASHWOOD_LAKE      = 33220,
    SAY_MAIDEM                      = -1850015 // Can it really be? Free after all these years?
};

struct QUEST_A_BLADE_FIT_FOR_A_CHAMPION
{ 
    uint32 quest_id; 
};

QUEST_A_BLADE_FIT_FOR_A_CHAMPION new_quest[] = 
{
    13603, 13666, 13673, 13741, 13746, 13752, 13757, 13762, 13768, 13773, 13778, 13783
};

class npc_lake_frog : public CreatureScript
{
public:
    npc_lake_frog() : CreatureScript("npc_lake_frog") { }

    struct npc_lake_frogAI : public FollowerAI 
    {
        npc_lake_frogAI(Creature *c) : FollowerAI(c) {}
       
        uint32 uiFollowTimer; 
        bool following;	
    
        void Reset () 
        {
            following = false;
            uiFollowTimer = 15000; // 15 seg te siguie la ranita
        }

        void UpdateAI(const uint32 diff)
        {
            if (following)
            {
                if (uiFollowTimer <= diff)
                {
                    SetFollowComplete();
                    me->DisappearAndDie();
                    me->Respawn(true);
                    Reset();
                }
                else uiFollowTimer-=diff;
            }
        }

        void ReceiveEmote(Player* pPlayer, uint32 emote)
        {
            if (following) 
                return;
			
            if (emote==TEXT_EMOTE_KISS)
                for (int i = 0; i < 12; i++)
                {
                    {
                        if (pPlayer->GetQuestStatus(new_quest[i].quest_id) == QUEST_STATUS_INCOMPLETE && !pPlayer->HasAura(SPELL_WARTSBGONE_LIP_BALM))
                            pPlayer->AddAura(SPELL_WARTS,pPlayer);
                        else if (roll_chance_i(10))
                        {
                            //if (Unit* pMaidem = me->SummonCreature(NPC_MAIDEN_OF_ASHWOOD_LAKE,me->GetPositionX(),me->GetPositionY(),me->GetPositionZ(),0,TEMPSUMMON_TIMED_DESPAWN,30000))
                            //{
                            pPlayer->SummonCreature(NPC_MAIDEN_OF_ASHWOOD_LAKE,me->GetPositionX(),me->GetPositionY(),me->GetPositionZ(),0,TEMPSUMMON_TIMED_DESPAWN,30000);
                            //DoScriptText(SAY_MAIDEM, pMaidem);
                            me->DisappearAndDie();		
                            me->Respawn(true); 
                            //}
                        }
                        else
                        {
                            pPlayer->RemoveAura(SPELL_WARTSBGONE_LIP_BALM);	
                            me->AddAura(SPELL_FROG_LOVE,me);
                            StartFollow(pPlayer, 35, NULL); 
                            following=true;
                        }
                    }
                }
        }
    };

    CreatureAI *GetAI(Creature *pCreature) const
    {
        return new npc_lake_frogAI(pCreature);
    }
};


enum MaidenOFAshwoodLake
{
    MAIDEN_DEFAULT_TEXTID       = 14319,
    MAIDEN_REWARD_TEXTID        = 14320,
    SPELL_SUMMON_ASHWOOD_BRAND  = 62554
};

#define GOSSIP_HELLO_MAIDEN "Glad to help. my lady. I'm toid you were once the guardian of a fabled sword. Do you know where I might find it?"

class npc_maiden_of_ashwood_lake : public CreatureScript
{
public:
    npc_maiden_of_ashwood_lake() : CreatureScript("npc_maiden_of_ashwood_lake") { }

    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
    {
        if (!pPlayer->HasItemCount(44981,1,true))
        {
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_HELLO_MAIDEN, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
            pPlayer->SEND_GOSSIP_MENU(MAIDEN_DEFAULT_TEXTID, pCreature->GetGUID());
            pCreature->DespawnOrUnsummon(10000);
            return true;
        }

        pPlayer->SEND_GOSSIP_MENU(MAIDEN_DEFAULT_TEXTID, pCreature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* pPlayer, Creature* pCreature /*pCreature*/, uint32 /*uiSender*/, uint32 uiAction)
    {
        pPlayer->PlayerTalkClass->ClearMenus();
        switch (uiAction)
        {
            case GOSSIP_ACTION_INFO_DEF+1:
                pPlayer->CastSpell(pPlayer,SPELL_SUMMON_ASHWOOD_BRAND,true);
                pPlayer->SEND_GOSSIP_MENU(MAIDEN_REWARD_TEXTID, pCreature->GetGUID());
                break;
        }
        return true;
    }
};

#define NPC_TEXTID_MAIDEN_OF_DRAK_MAR_01 -1850000
#define NPC_TEXTID_MAIDEN_OF_DRAK_MAR_02 -1850001
#define NPC_TEXTID_MAIDEN_OF_DRAK_MAR_03 -1850002
#define NPC_TEXTID_MAIDEN_OF_DRAK_MAR_04 -1850003
#define MAIDEN_OF_DRAK_MAR_TIMER_00 2000
#define MAIDEN_OF_DRAK_MAR_TIMER_01 5000
#define MAIDEN_OF_DRAK_MAR_TIMER_02 6000
#define MAIDEN_OF_DRAK_MAR_TIMER_03 7000
#define MAIDEN_OF_DRAK_MAR_TIMER_04 20000
#define MAIDEN_OF_DRAK_MAR_GOB_01 194239
#define MAIDEN_OF_DRAK_MAR_GOB_02 194238
//Summon  :X: 4602.977 Y: -1600.141 Z: 156.7834 O: 0.7504916

class npc_maiden_of_drak_mar : public CreatureScript
{
public:
    npc_maiden_of_drak_mar() : CreatureScript("npc_maiden_of_drak_mar") { }

    struct npc_maiden_of_drak_marAI : public ScriptedAI
    {
        uint32 phase;
        uint32 uiPhaseTimer;
        uint64 firstGobGuid;
        uint64 secondGobGuid;

        npc_maiden_of_drak_marAI(Creature *c) : ScriptedAI(c)
        {
            phase = 0;
            uiPhaseTimer = MAIDEN_OF_DRAK_MAR_TIMER_00;
            if (GameObject* go = me->SummonGameObject(MAIDEN_OF_DRAK_MAR_GOB_01,4602.977f,-1600.141f,156.7834f,0.7504916f,0,0,0,0,0))
                firstGobGuid = go->GetGUID();
        }

        void UpdateAI(const uint32 diff)
        {
            if (uiPhaseTimer <= diff)
            {
                phase++;
                switch (phase)
                {
                    case 1:
                        DoScriptText(NPC_TEXTID_MAIDEN_OF_DRAK_MAR_01, me);
                        uiPhaseTimer = MAIDEN_OF_DRAK_MAR_TIMER_01;
                        break;
                    case 2:
                        DoScriptText(NPC_TEXTID_MAIDEN_OF_DRAK_MAR_02, me);
                        uiPhaseTimer = MAIDEN_OF_DRAK_MAR_TIMER_02;
                        break;
                    case 3:
                        DoScriptText(NPC_TEXTID_MAIDEN_OF_DRAK_MAR_03, me);
                        uiPhaseTimer = MAIDEN_OF_DRAK_MAR_TIMER_03;
                        break;
                    case 4:
                        DoScriptText(NPC_TEXTID_MAIDEN_OF_DRAK_MAR_04, me);
                        if (GameObject* go = me->SummonGameObject(MAIDEN_OF_DRAK_MAR_GOB_02,4603.351f,-1599.288f,156.8822f,2.234018f,0,0,0,0,0))
                           secondGobGuid = go->GetGUID(); 
                        uiPhaseTimer = MAIDEN_OF_DRAK_MAR_TIMER_04;
                        break;
                    case 5:
                        if (GameObject* go = GameObject::GetGameObject(*me,firstGobGuid))
                            go->RemoveFromWorld();
                        if (GameObject* go = GameObject::GetGameObject(*me,secondGobGuid))
                            go->RemoveFromWorld();
                        me->DespawnOrUnsummon();
                        break;
                    default:
                        if (GameObject* go = GameObject::GetGameObject(*me,firstGobGuid))
                            go->RemoveFromWorld();
                        if (GameObject* go = GameObject::GetGameObject(*me,secondGobGuid))
                            go->RemoveFromWorld();
                        me->DespawnOrUnsummon();
                    break;
                }
            }
            else
            {
            uiPhaseTimer -= diff;
        }
    }
};
    CreatureAI *GetAI(Creature *pCreature) const
    {
        return new npc_maiden_of_drak_marAI(pCreature);
    }
};


enum AmongChampions
{
	CHAMPION_TARGET = 1,
	POINT_START = 2,
	POINT_RANDOM = 3,

	EVENT_MELEE = 1,
	EVENT_DEFEND = 2,
	EVENT_COMBO = 3,
	EVENT_COMBO_FINISH = 4,

	TIME_MELEE = 2000,
	TIME_DEFEND = 8000,
	TIME_COMBO = 7000,
	TIME_COMBO_FINISH = 500,

	SPELL_SHIELD_BREAKER = 62626,
	SPELL_CHARGE = 68321,
	SPELL_THRUST = 62544,
	SPELL_DEFEND = 62552,
	QUEST_AMONG_CHAMPIONS_A = 13790,
	QUEST_AMONG_CHAMPIONS_A_DK = 13793,
	QUEST_AMONG_CHAMPIONS_H = 13811,
	QUEST_AMONG_CHAMPIONS_H_DK = 13814,

	MOUNTED_MELEE_VICTORY_GRAND = 63596,

	NPC_GO_TO_BUNNY = 34319,
};

#define GOSSIP_VALIANT_1_GRAND      "I am ready to fight."
#define GENERIC_CHAMPION_SAY_START_1  "On your guard!"
#define GENERIC_CHAMPION_SAY_START_2  "Prepare yourself!"
#define GENERIC_CHAMPION_SAY_LOOSE    "I have been defeated. Good Fight !"
#define GENERIC_CHAMPION_SAY_WIN      "You lost! Try again!"

class GenericChampion : public CreatureScript
{
public:

	class GenericChampionAI : public ScriptedAI
	{
	public:
		GenericChampionAI(Creature* creature) : ScriptedAI(creature) {}

		void Reset()
		{
			me->setFaction(35);
			events.Reset();
			DoCast(SPELL_DEFEND);
			DoCast(SPELL_DEFEND);
			me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
		}

		void SetGUID(uint64 guid, int32 id = 0)
		{
			me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

			if (id != CHAMPION_TARGET)
				return;

			enemy = me->GetUnit(*me, guid);
			nextCreature = me->FindNearestCreature(34319, 25.0f, true);

			
			me->GetMotionMaster()->MovePoint(POINT_START, nextCreature->GetPosition());
			me->setFaction(35);
		}

		void MovementInform(uint32 type, uint32 id)
		{
			if (type == EFFECT_MOTION_TYPE && enemy)
				me->GetMotionMaster()->MoveChase(enemy);

			if (type != POINT_MOTION_TYPE)
				return;

			switch (id)
			{
			case POINT_START:
				if (!enemy)
				{
					EnterEvadeMode();
					return;
				}
				me->setFaction(14);
				me->SetReactState(REACT_AGGRESSIVE);
				me->TauntApply(enemy);
				events.ScheduleEvent(EVENT_COMBO, TIME_COMBO);
				events.ScheduleEvent(EVENT_MELEE, TIME_MELEE);
				events.ScheduleEvent(EVENT_DEFEND, TIME_DEFEND);
				break;
			case POINT_RANDOM:
				DoCastVictim(SPELL_SHIELD_BREAKER);
				events.ScheduleEvent(EVENT_COMBO_FINISH, TIME_COMBO_FINISH);
				break;
			}
		}

		void JustDied(Unit* killer)
		{
			me->MonsterSay(GENERIC_CHAMPION_SAY_LOOSE, LANG_UNIVERSAL, 0);
			me->SetHealth(1);

			if (killer)
				killer->CastSpell(killer, MOUNTED_MELEE_VICTORY_GRAND, true);

			EnterEvadeMode();
		}

		void KilledUnit(Unit* /*victim*/)
		{
			me->MonsterSay(GENERIC_CHAMPION_SAY_WIN, LANG_UNIVERSAL, 0);
			EnterEvadeMode();
		}

		void EnterCombat(Unit* /*who*/)
		{
			if (!enemy)
				return;

			me->MonsterSay(urand(0, 1) ? GENERIC_CHAMPION_SAY_START_1 : GENERIC_CHAMPION_SAY_START_2, LANG_UNIVERSAL, 0);
			me->GetMotionMaster()->MoveChase(enemy);
		}

		void UpdateAI(const uint32 diff)
		{
			if (!UpdateVictim())
				return;

			events.Update(diff);

			while (uint32 Event = events.ExecuteEvent())
				switch (Event)
			{
				case EVENT_COMBO:
				{
				    events.RescheduleEvent(EVENT_MELEE, TIME_MELEE * 3);
				    
				    Position _pos;
				    Position pos;
				    
				    if (!enemy)
				    	return;
				    
				    enemy->GetPosition(&_pos);
				    me->GetRandomPoint(_pos, 20.0f, pos);
				    me->GetMotionMaster()->MovementExpired();
				    me->GetMotionMaster()->MovePoint(POINT_RANDOM, pos);
				    break;
				}
				case EVENT_COMBO_FINISH:
					DoCastVictim(SPELL_CHARGE);
					events.ScheduleEvent(EVENT_COMBO, TIME_COMBO);
					break;
				case EVENT_MELEE:
					DoCastVictim(SPELL_THRUST);
					me->GetMotionMaster()->MovementExpired();
					me->GetMotionMaster()->MoveChase(me->getVictim());
					events.ScheduleEvent(EVENT_MELEE, TIME_MELEE);
					break;
				case EVENT_DEFEND:
					DoCast(SPELL_DEFEND);
					events.ScheduleEvent(EVENT_DEFEND, TIME_DEFEND);
					break;
			};
		}

	private:
		Unit* enemy;
		Unit* nextCreature;
		EventMap events;
	};

	GenericChampion() : CreatureScript("GenericChampion") {}

	bool OnGossipHello(Player* player, Creature* creature)
	{
		if (((player->GetQuestStatus(QUEST_AMONG_CHAMPIONS_A) == QUEST_STATUS_INCOMPLETE) ||
			(player->GetQuestStatus(QUEST_AMONG_CHAMPIONS_A_DK) == QUEST_STATUS_INCOMPLETE) ||
			(player->GetQuestStatus(QUEST_AMONG_CHAMPIONS_H) == QUEST_STATUS_INCOMPLETE) ||
			(player->GetQuestStatus(QUEST_AMONG_CHAMPIONS_H_DK) == QUEST_STATUS_INCOMPLETE)) &&
			player->HasAura(RIDE_VEHICLE) &&
			player->HasAura(PLAYER_ON_TOURNAMENT_MOUNT) &&
			player->HasAura(LANCE_EQUIPPED) &&
			!player->isInCombat())
		{
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_VALIANT_1_GRAND, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
			player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
			return true;
		}

		return false;
	}

	bool OnGossipSelect(Player* player, Creature* creature, uint32 /*uiSender*/, uint32 action)
	{
		player->PlayerTalkClass->ClearMenus();
		if (action == GOSSIP_ACTION_INFO_DEF + 1)
		{
			player->CLOSE_GOSSIP_MENU();
			if (creature)
			if (UnitAI* ai = creature->GetAI())
				ai->SetGUID(player->GetTransGUID(), CHAMPION_TARGET);
		}

		return true;
	}

	CreatureAI *GetAI(Creature *creature) const
	{
		return new GenericChampionAI(creature);
	}
};

void AddTournamentDailies()
{
    new npc_lake_frog;
    new npc_maiden_of_ashwood_lake;
    new npc_maiden_of_drak_mar;
	new GenericChampion();
}