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

#include "ArgentTournament.h"

enum DavidData
{
    QUEST_ASPIRANT_CHALLENGE_H          = 13680,
    QUEST_ASPIRANT_CHALLENGE_A          = 13679,
    NPC_ARGENT_ASPIRANT                 = 33448,
    GOSSIP_TEXTID_SQUIRE                = 14407,

    ASPIRANT_TARGET                     = 1,
};

#define GOSSIP_SQUIRE_ITEM_1 "I am ready to fight!"
#define GOSSIP_SQUIRE_ITEM_2 "How do the Argent Crusader raiders fight?"

class SquireDavid : public CreatureScript
{
public:
    SquireDavid() : CreatureScript("npc_squire_david") { }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if (player->GetQuestStatus(QUEST_ASPIRANT_CHALLENGE_H) != QUEST_STATUS_INCOMPLETE &&
            player->GetQuestStatus(QUEST_ASPIRANT_CHALLENGE_A) != QUEST_STATUS_INCOMPLETE)
            return false;

        if (player->isInCombat())
            return false;

        if (!player->HasAura(RIDE_VEHICLE) || !player->HasAura(PLAYER_ON_TOURNAMENT_MOUNT) || 
            !player->HasAura(LANCE_EQUIPPED))
            return false;

        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SQUIRE_ITEM_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SQUIRE_ITEM_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
        player->SEND_GOSSIP_MENU(GOSSIP_TEXTID_SQUIRE, creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();

        if (action == GOSSIP_ACTION_INFO_DEF+1)
        {
            player->CLOSE_GOSSIP_MENU();
            if (Creature* summoned = creature->SummonCreature(NPC_ARGENT_ASPIRANT, 8575.451f, 952.472f, 547.554f, 0.38f))
                if (UnitAI* ai = summoned->GetAI())
                   ai->SetGUID(player->GetTransGUID(), ASPIRANT_TARGET);
        }
        return true;
    }
};

enum AspirantData
{
    POINT_START                 = 1,
    POINT_RANDOM                = 2,

    EVENT_MELEE                 = 1,
    EVENT_DEFEND                = 2,
    EVENT_COMBO                 = 3,
    EVENT_COMBO_FINISH          = 4,
    EVENT_DESPAWN_CHECK         = 5,

    TIME_MELEE                  = 2000,
    TIME_DEFEND                 = 8000,
    TIME_COMBO                  = 7000,
    TIME_COMBO_FINISH           = 500,
    TIME_DESPAWN_CHECK          = 500,

    SPELL_CHARGE                = 63010,
    SPELL_SHIELD_BREAKER        = 65147,
    SPELL_DEFEND                = 62719,
    SPELL_THRUST                = 62544,
    
    ASPIRANT_CREDIT             = 38595,

    ARGENT_ASPIRANT_SAY_START   = -1850007, //You think you have the courage?  We shall see.
    ARGENT_ASPIRANT_SAY_LOOSE   = -1850008, //Impressive demonstration. I think you're able to join the ranks of the valiants.
    ARGENT_ASPIRANT_SAY_WIN     = -1850009, //I've won. You'll probably have more luck next time.

};

class ArgentAspirant : public CreatureScript
{
public:
    class ArgentAspirantAI : public ScriptedAI
    {
    public :
        ArgentAspirantAI(Creature* creature) : ScriptedAI(creature) {}
        
        void SetGUID(uint64 guid, int32 id = 0)
        {
            me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

            if (id != ASPIRANT_TARGET)
                return;

            _enemy = guid;
            me->GetMotionMaster()->MovePoint(POINT_START, 8599.258f, 963.951f, 547.553f);
            me->setFaction(35);
            events.ScheduleEvent(EVENT_DESPAWN_CHECK, TIME_DESPAWN_CHECK);
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type == EFFECT_MOTION_TYPE)
                if (auto enemy = ObjectAccessor::GetUnit(*me, _enemy))
                me->GetMotionMaster()->MoveChase(enemy);

            if (type == POINT_MOTION_TYPE)
                if (auto enemy = ObjectAccessor::GetUnit(*me, _enemy))
                {
                    switch (id)
                    {
                    case POINT_START:
                        if (!enemy)
                        {
                            me->DespawnOrUnsummon();
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
        }

        void JustDied(Unit* killer)
        {
            DoScriptText(ARGENT_ASPIRANT_SAY_LOOSE, me);
            me->setFaction(35);
            me->SetHealth(1);
            me->DespawnOrUnsummon(5000);

            if (!killer)
                return;

            if (Player* player = killer->ToPlayer())
                player->KilledMonsterCredit(ASPIRANT_CREDIT, 0);
            else if (Unit* owner = killer->GetCharmerOrOwner())
                if (Player* player = owner->ToPlayer())
                    player->KilledMonsterCredit(ASPIRANT_CREDIT, 0);
        }

        void KilledUnit(Unit* /*victim*/)
        {
            DoScriptText(ARGENT_ASPIRANT_SAY_WIN, me);
            me->setFaction(35);
            me->DespawnOrUnsummon(5000);
        }

        void EnterCombat(Unit* /*who*/)
        {
            if (auto enemy = ObjectAccessor::GetUnit(*me, _enemy))
            {
                DoScriptText(ARGENT_ASPIRANT_SAY_START, me);
                me->GetMotionMaster()->MoveChase(enemy);
            }
            else
                me->DespawnOrUnsummon();
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

                        if (auto enemy = ObjectAccessor::GetUnit(*me, _enemy))
                        {
                            enemy->GetPosition(&_pos);
                            me->GetRandomPoint(_pos, 20.0f, pos);
                            me->GetMotionMaster()->MovePoint(POINT_RANDOM, pos);
                        }
                        break;
                    }
                    case EVENT_COMBO_FINISH:
                        DoCastVictim(SPELL_CHARGE);
                        events.ScheduleEvent(EVENT_COMBO, TIME_COMBO);
                        break;
                    case EVENT_MELEE:
                        DoCastVictim(SPELL_THRUST);
                        me->GetMotionMaster()->MoveChase(me->getVictim());
                        events.ScheduleEvent(EVENT_MELEE, TIME_MELEE);
                        break;
                    case EVENT_DEFEND:
                        DoCast(SPELL_DEFEND);
                        events.ScheduleEvent(EVENT_DEFEND, TIME_DEFEND);
                        break;
                    case EVENT_DESPAWN_CHECK:
                        if (!ObjectAccessor::GetUnit(*me, _enemy))
                        {
                            me->DespawnOrUnsummon();
                            return;
                        }
                        events.ScheduleEvent(EVENT_DESPAWN_CHECK, TIME_DESPAWN_CHECK);
                        break;
                }
        }

        private:
            uint64 _enemy;
            EventMap events;
    };

    ArgentAspirant() : CreatureScript("npc_argent_aspirant") {}

    CreatureAI *GetAI(Creature *creature) const
    {
        return new ArgentAspirantAI(creature);
    }
};

enum ArgentTrainingData
{
	NPC_MELEE                   = 33229,
	NPC_CHARGE                  = 33272,
	NPC_RANGE                   = 33243,

    TYPE_MELEE                  = 1,
    TYPE_RANGE                  = 2,
    TYPE_CHARGE                 = 3,

    EVENT_SHIELD                = 1,
    TIME_SHIELD                 = 8000,

    EVENT_RESET                 = 2,
    TIME_RESET                  = 5000,

	CREDIT_RANGE                = 33339,
	CREDIT_CHARGE               = 33340,
	CREDIT_MELEE                = 33341,
    
    SPELL_ARGENT_MELEE          = 62544,
    SPELL_ARGENT_CHARGE         = 62960,
	SPELL_ARGENT_BREAK_SHIELD   = 62575,

	SPELL_DEFEND_RANGE_AURA     = 62719,
	SPELL_DEFEND_CHARGE_AURA    = 64100,
};

class ArgentTrainingDummy : public CreatureScript
{
public:
	class ArgentTrainingDummyAI : public Scripted_NoMovementAI
	{
    public:
	    ArgentTrainingDummyAI(Creature *creature) : Scripted_NoMovementAI(creature)
        {
            me->RemoveAllAuras();
            events.ScheduleEvent(EVENT_SHIELD, 1);
            events.ScheduleEvent(EVENT_RESET, TIME_RESET);
            
            //rotate disable
			me->SetControlled(true, UNIT_STATE_STUNNED);

            // immunities
			me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
			me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_STUN, true);
        }

	    void DamageTaken(Unit* /*attacker*/, uint32& damage)
	    {
            events.RescheduleEvent(EVENT_RESET, TIME_RESET);
			damage = 0;
	    }

        void SpellHit(Unit* caster, SpellInfo const* spell)
		{
            Player* player = NULL;

            if (Unit* owner = caster->GetCharmerOrOwner())
                if (Player* _player = owner->ToPlayer())
                    player = _player;

            if (spell->Id == SPELL_ARGENT_MELEE)
                player = caster->ToPlayer();

            if (!player)
                return;

            switch (me->GetEntry())
            {
            case NPC_MELEE:
                if (player && spell->Id == SPELL_ARGENT_MELEE)
			        player->KilledMonsterCredit(CREDIT_MELEE, 0);
                break;
            case NPC_RANGE:
                if (me->HasAura(SPELL_DEFEND_RANGE_AURA))
                    return;

                if (player && spell->Id == SPELL_ARGENT_BREAK_SHIELD)
			        player->KilledMonsterCredit(CREDIT_RANGE, 0);
                break;
            case NPC_CHARGE:
                if (me->HasAura(SPELL_DEFEND_CHARGE_AURA))
                    return;

                if (player && spell->Id == SPELL_ARGENT_CHARGE)
			        player->KilledMonsterCredit(CREDIT_CHARGE, 0);
                break;
            }
		}

	    void UpdateAI(uint32 const diff)
	    {
            //rotate disable
		    if (!me->HasUnitState(UNIT_STATE_STUNNED))
		        me->SetControlled(true, UNIT_STATE_STUNNED);

            events.Update(diff);
            
            while (uint32 Event = events.ExecuteEvent())
                if (EVENT_SHIELD == Event)
                {
                    if (me->GetEntry() == NPC_RANGE)
					{
                        DoCast(SPELL_DEFEND_RANGE_AURA);
                        
                        events.ScheduleEvent(EVENT_SHIELD, TIME_SHIELD);
                    }
                    else if (me->GetEntry() == NPC_CHARGE)
                    {
                        if (!me->HasAura(SPELL_DEFEND_CHARGE_AURA))
                            DoCast(me, SPELL_DEFEND_CHARGE_AURA, true);

                        events.ScheduleEvent(EVENT_SHIELD, TIME_SHIELD);
                    }
                }
                else if (EVENT_RESET == Event)
                {
                    events.ScheduleEvent(EVENT_RESET, TIME_RESET);
                    if (me->getVictim())
                        EnterEvadeMode();
                    return;
                }
        }

        private:
            EventMap events;
	};
    
	ArgentTrainingDummy(char const* name): CreatureScript(name){}
		
	CreatureAI* GetAI(Creature* creature) const
	{
	    return new ArgentTrainingDummyAI(creature);
	}
};


/*######
## Npc Jeran Lockwood (33973)
#######*/
#define JERAN_DEFAULT_TEXTID 14453
#define JERAN_QUEST_TEXTID 14431
#define JERAN_RP_TEXTID 14434
#define GOSSIP_HELLO_JERAN_1 "Show me how to train with a Melee Target?"
#define GOSSIP_HELLO_JERAN_2 "Tell me more about Defend and Thrust!."
#define SPELL_CREDIT_JERAN 64113

class npc_jeran_lockwood : public CreatureScript
{
public:
    npc_jeran_lockwood() : CreatureScript("npc_jeran_lockwood") { }

    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
    {
        if ((pPlayer->GetQuestStatus(13828) == QUEST_STATUS_INCOMPLETE) || (pPlayer->GetQuestStatus(13829) == QUEST_STATUS_INCOMPLETE))
        {
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_HELLO_JERAN_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_HELLO_JERAN_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
            pPlayer->SEND_GOSSIP_MENU(JERAN_QUEST_TEXTID, pCreature->GetGUID());
        }
        else
        {
            pPlayer->SEND_GOSSIP_MENU(JERAN_DEFAULT_TEXTID, pCreature->GetGUID());
        }
        return true;
    }

    bool OnGossipSelect(Player* pPlayer, Creature* pCreature /*pCreature*/, uint32 /*uiSender*/, uint32 uiAction)
    {
        pPlayer->PlayerTalkClass->ClearMenus();
        switch (uiAction)
        {
        case GOSSIP_ACTION_INFO_DEF+1:
            pPlayer->CastSpell(pPlayer,SPELL_CREDIT_JERAN,true);
            pPlayer->CLOSE_GOSSIP_MENU();
            break;
        case GOSSIP_ACTION_INFO_DEF+2:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_HELLO_JERAN_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
            pPlayer->SEND_GOSSIP_MENU(JERAN_RP_TEXTID, pCreature->GetGUID());
            break;
        }
        return true;
    }
};

/*#####
## Npc Rugan Steelbelly (33972)
#######*/
#define RUGAN_DEFAULT_TEXTID 14453
#define RUGAN_QUEST_TEXTID 14436
#define RUGAN_RP_TEXTID 14437
#define GOSSIP_HELLO_RUGAN_1 "Show me how to train with a Charge Target?"
#define GOSSIP_HELLO_RUGAN_2 "Tell me more about the Charge!"
#define SPELL_CREDIT_RUGAN 64114

class npc_rugan_steelbelly : public CreatureScript
{
public:
    npc_rugan_steelbelly() : CreatureScript("npc_rugan_steelbelly") { }

    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
    {
        if ((pPlayer->GetQuestStatus(13837) == QUEST_STATUS_INCOMPLETE) || (pPlayer->GetQuestStatus(13839) == QUEST_STATUS_INCOMPLETE))
        {
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_HELLO_RUGAN_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_HELLO_RUGAN_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
            pPlayer->SEND_GOSSIP_MENU(RUGAN_QUEST_TEXTID, pCreature->GetGUID());
        }
        else
        {
            pPlayer->SEND_GOSSIP_MENU(RUGAN_DEFAULT_TEXTID, pCreature->GetGUID());
        }
        return true;
    }

    bool OnGossipSelect(Player* pPlayer, Creature* pCreature /*pCreature*/, uint32 /*uiSender*/, uint32 uiAction)
    {
        pPlayer->PlayerTalkClass->ClearMenus();
        switch (uiAction)
        {
        case GOSSIP_ACTION_INFO_DEF+1:
            pPlayer->CastSpell(pPlayer,SPELL_CREDIT_RUGAN,true);
            pPlayer->CLOSE_GOSSIP_MENU();
            break;
        case GOSSIP_ACTION_INFO_DEF+2:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_HELLO_RUGAN_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
            pPlayer->SEND_GOSSIP_MENU(RUGAN_RP_TEXTID, pCreature->GetGUID());
            break;
        }
        return true;
    }
};

/*######
## Npc Valis Windchaser
#######*/
#define VALIS_DEFAULT_TEXTID 14453
#define VALIS_QUEST_TEXTID 14438
#define VALIS_RP_TEXTID 14439
#define GOSSIP_HELLO_VALIS_1 "Show me how to train with a Ranged Target."
#define GOSSIP_HELLO_VALIS_2 "Tell me more about using the Shield-Breaker."
#define SPELL_CREDIT_VALIS 64115

class npc_valis_windchaser : public CreatureScript
{
public:
    npc_valis_windchaser() : CreatureScript("npc_valis_windchaser") { }

    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
    {
        if ((pPlayer->GetQuestStatus(13835) == QUEST_STATUS_INCOMPLETE) || 
            (pPlayer->GetQuestStatus(13838) == QUEST_STATUS_INCOMPLETE))
        {
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_HELLO_VALIS_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_HELLO_VALIS_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
            pPlayer->SEND_GOSSIP_MENU(VALIS_QUEST_TEXTID, pCreature->GetGUID());
        }
        else
        pPlayer->SEND_GOSSIP_MENU(VALIS_DEFAULT_TEXTID, pCreature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* pPlayer, Creature* pCreature /*pCreature*/, uint32 /*uiSender*/, uint32 uiAction)
    {
        pPlayer->PlayerTalkClass->ClearMenus();
        
        switch (uiAction)
        {
            case GOSSIP_ACTION_INFO_DEF+1:
                pPlayer->CastSpell(pPlayer,SPELL_CREDIT_VALIS,true);
                pPlayer->CLOSE_GOSSIP_MENU();
                break;
            case GOSSIP_ACTION_INFO_DEF+2:
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_HELLO_VALIS_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
                pPlayer->SEND_GOSSIP_MENU(VALIS_RP_TEXTID, pCreature->GetGUID());
                break;
        }
        return true;
    }
};



void AddTournamentPhase1()
{
    new SquireDavid();
    new ArgentAspirant();
    new ArgentTrainingDummy("npc_argent_melee_target");
    new ArgentTrainingDummy("npc_argent_range_target");
    new ArgentTrainingDummy("npc_argent_charge_target");

    new npc_valis_windchaser;
    new npc_rugan_steelbelly;
    new npc_jeran_lockwood;
}