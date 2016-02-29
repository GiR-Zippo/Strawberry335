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


/*######
## The Grand Melee Quests 
## 13665-13745-13750-13756-13761-13767-13772-13777-13782-13787.- http://www.wowhead.com/search?q=The+Grand+Melee SELECT entry,title FROM quest_template WHERE title LIKE 'The Grand Melee%'
######*/

enum GrandMelee
{
    VALIANT_TARGET              = 1,
    POINT_RANDOM                = 2,

    EVENT_MELEE                 = 1,
    EVENT_DEFEND                = 2,
    EVENT_COMBO                 = 3,
    EVENT_COMBO_FINISH          = 4,

    TIME_MELEE                  = 2000,
    TIME_DEFEND                 = 8000,
    TIME_COMBO                  = 7000,
    TIME_COMBO_FINISH           = 500,

    SPELL_SHIELD_BREAKER        = 62626,
    SPELL_CHARGE                = 68321,
    SPELL_THRUST                = 62544,
    SPELL_DEFEND                = 62552,

    QUEST_GRAND_MELEE_1                     = 13665,
    QUEST_GRAND_MELEE_2                     = 13745,
    QUEST_GRAND_MELEE_3                     = 13750,
    QUEST_GRAND_MELEE_4                     = 13756,
    QUEST_GRAND_MELEE_5                     = 13761,
    QUEST_GRAND_MELEE_6                     = 13767,
    QUEST_GRAND_MELEE_7                     = 13772,
    QUEST_GRAND_MELEE_8                     = 13777,
    QUEST_GRAND_MELEE_9                     = 13782,
    QUEST_GRAND_MELEE_10                    = 13787,

    MOUNTED_MELEE_VICTORY_GRAND             = 62724,
};

#define GOSSIP_VALIANT_1_GRAND      "I am ready to fight."
#define ARGENT_VALIANT_SAY_START_1  "On your guard!"
#define ARGENT_VALIANT_SAY_START_2  "Prepare yourself!"
#define ARGENT_VALIANT_SAY_LOOSE    "I have been defeated. Good Fight !"
#define ARGENT_VALIANT_SAY_WIN      "You lost! Try again!"


class ArgentValiant : public CreatureScript
{
public:

    class ArgentValiantAI : public ScriptedAI
    {
    public :
        ArgentValiantAI(Creature* creature) : ScriptedAI(creature) {}
        
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
            if (id != VALIANT_TARGET)
                return;

            enemy = me->GetUnit(*me, guid);

            if (!enemy)
                return;

            me->setFaction(14);
            me->SetReactState(REACT_AGGRESSIVE);
            me->TauntApply(enemy);
            events.ScheduleEvent(EVENT_COMBO, TIME_COMBO);
            events.ScheduleEvent(EVENT_MELEE, TIME_MELEE);
            events.ScheduleEvent(EVENT_DEFEND, TIME_DEFEND);
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type == EFFECT_MOTION_TYPE && enemy)
                me->GetMotionMaster()->MoveChase(enemy);

            if (type != POINT_MOTION_TYPE && id != POINT_RANDOM)
                return;

            DoCastVictim(SPELL_SHIELD_BREAKER);
            events.ScheduleEvent(EVENT_COMBO_FINISH, TIME_COMBO_FINISH);
        }

        void JustDied(Unit* killer)
        {
            me->MonsterSay(ARGENT_VALIANT_SAY_LOOSE, LANG_UNIVERSAL, 0);
            me->SetHealth(1);

            if (killer)
                killer->CastSpell(killer, MOUNTED_MELEE_VICTORY_GRAND, true);

            EnterEvadeMode();
        }

        void KilledUnit(Unit* /*victim*/)
        {
            me->MonsterSay(ARGENT_VALIANT_SAY_WIN, LANG_UNIVERSAL, 0);
            EnterEvadeMode();
        }

        void EnterCombat(Unit* /*who*/)
        {
            if (!enemy)
                return;

            me->MonsterSay(urand(0,1) ? ARGENT_VALIANT_SAY_START_1 : ARGENT_VALIANT_SAY_START_2, LANG_UNIVERSAL, 0);
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
            EventMap events;
    };
    
    ArgentValiant() : CreatureScript("npc_ValiantGrandMelee") {}

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if (((player->GetQuestStatus(QUEST_GRAND_MELEE_1) == QUEST_STATUS_INCOMPLETE) ||
            (player->GetQuestStatus(QUEST_GRAND_MELEE_2) == QUEST_STATUS_INCOMPLETE)   ||
            (player->GetQuestStatus(QUEST_GRAND_MELEE_3) == QUEST_STATUS_INCOMPLETE)   ||
            (player->GetQuestStatus(QUEST_GRAND_MELEE_4) == QUEST_STATUS_INCOMPLETE)   ||
            (player->GetQuestStatus(QUEST_GRAND_MELEE_5) == QUEST_STATUS_INCOMPLETE)   ||
            (player->GetQuestStatus(QUEST_GRAND_MELEE_6) == QUEST_STATUS_INCOMPLETE)   ||
            (player->GetQuestStatus(QUEST_GRAND_MELEE_7) == QUEST_STATUS_INCOMPLETE)   ||
            (player->GetQuestStatus(QUEST_GRAND_MELEE_8) == QUEST_STATUS_INCOMPLETE)   ||
            (player->GetQuestStatus(QUEST_GRAND_MELEE_9) == QUEST_STATUS_INCOMPLETE)   ||
            (player->GetQuestStatus(QUEST_GRAND_MELEE_10) == QUEST_STATUS_INCOMPLETE)) &&
            player->HasAura(RIDE_VEHICLE) &&
            player->HasAura(PLAYER_ON_TOURNAMENT_MOUNT) &&
            player->HasAura(LANCE_EQUIPPED) &&
            !player->isInCombat())
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_VALIANT_1_GRAND, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
            player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
            return true;
        }

        return false;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*uiSender*/, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();
        if (action == GOSSIP_ACTION_INFO_DEF+1)
        {
            player->CLOSE_GOSSIP_MENU();
            if (creature)
                if (UnitAI* ai = creature->GetAI())
                   ai->SetGUID(player->GetTransGUID(), VALIANT_TARGET);
        }

        return true;
    }

    CreatureAI *GetAI(Creature *creature) const
    {
        return new ArgentValiantAI(creature);
    }
};


enum Boneguards
{
    SPELL_LIEUTNANT_THRUST_AURA     = 62710,
    SPELL_LIEUTNANT_DEFEND          = 64100,
    SPELL_LIEUTNANT_CHARGE          = 63010,
    SPELL_NECROCUTION               = 63233,
};

class BoneguardLieutnant : public CreatureScript
{
public:
    class BoneguardLieutnantAI : public ScriptedAI
    {
    public :
        BoneguardLieutnantAI(Creature* creature) : ScriptedAI(creature)
        {
            me->SetReactState(REACT_AGGRESSIVE);
        }
        
        void Reset()
        {
            if (!me->HasAura(SPELL_LIEUTNANT_THRUST_AURA))
                DoCast(SPELL_LIEUTNANT_THRUST_AURA);
            
            if (!me->HasAura(SPELL_LIEUTNANT_DEFEND))
                DoCast(SPELL_LIEUTNANT_DEFEND);

            enemy = 0;
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (me->getVictim() && enemy != me->getVictim()->GetGUID())
            {
                enemy = me->getVictim()->GetGUID();
                DoCastVictim(SPELL_LIEUTNANT_CHARGE);
            }

            DoMeleeAttackIfReady();
        }

    private:
        uint64 enemy;
    };

    BoneguardLieutnant() : CreatureScript("npc_boneguard_lieutnant") {}

    CreatureAI *GetAI(Creature *creature) const
    {
        return new BoneguardLieutnantAI(creature);
    }
};

class BoneguardScout : public CreatureScript
{
public:
    class BoneguardScoutAI : public ScriptedAI
    {
    public :
        BoneguardScoutAI(Creature* creature) : ScriptedAI(creature)
        {
            me->SetReactState(REACT_AGGRESSIVE);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            DoSpellAttackIfReady(SPELL_NECROCUTION);
        }

    };

    BoneguardScout() : CreatureScript("npc_boneguard_scout") {}

    CreatureAI *GetAI(Creature *creature) const
    {
        return new BoneguardScoutAI(creature);
    }
};

enum TrampleScourgeData
{
    SPELL_TRAMPLE_SCOURGE       = 63001,
    SCOURGE_BONEGUARD           = 33438,
};

class TrampleScourgeDummy : public SpellScriptLoader
{
public:
    class TrampleScourgeDummyScript : public AuraScript
    {
        PrepareAuraScript(TrampleScourgeDummyScript);
    
        bool Validate()
        {
            return sSpellMgr->GetSpellInfo(SPELL_TRAMPLE_SCOURGE);
        }
    
        void Trample(AuraEffect const* aurEff)
        {
            if (Unit* caster = GetCaster())
                caster->CastSpell(caster, SPELL_TRAMPLE_SCOURGE, true);
        }
    
        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(TrampleScourgeDummyScript::Trample, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    TrampleScourgeDummy() : SpellScriptLoader("spell_trample_scourge_dummy") { }
    
    AuraScript* GetAuraScript() const
    {
        return new TrampleScourgeDummyScript();
    }
};

class TrampleScourge : public SpellScriptLoader
{
public:
    TrampleScourge() : SpellScriptLoader("spell_trample_scourge") { }
    
    class TrampleScourgeScript : public SpellScript
    {
        PrepareSpellScript(TrampleScourgeScript);
    
        bool Validate(SpellInfo const* /*spellEntry*/)
        {
            return true;
        }

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            for (std::list<WorldObject*>::iterator itr = targets.begin(); itr != targets.end();)
                if ((*itr)->GetTypeId() == TYPEID_PLAYER)
                    targets.erase(itr++);
                else if ((*itr)->GetTypeId() != TYPEID_UNIT)
                    targets.erase(itr++);
                else if ((*itr)->GetEntry() != SCOURGE_BONEGUARD)
                    targets.erase(itr++);
                else 
                    itr++;
        }
    
        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(TrampleScourgeScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new TrampleScourgeScript();
    }
};

class Necrocution : public SpellScriptLoader
{
public:
    Necrocution() : SpellScriptLoader("spell_necrocution") { }

    class NecrocutionScript : public SpellScript
    {
        PrepareSpellScript(NecrocutionScript);

        bool Validate(SpellInfo const* /*spellEntry*/)
        {
            return true;
        }
        
        void RemoveDefend(SpellEffIndex /*effIndex*/)
        {
            Unit* target = GetHitUnit();

            if (!target)
                return;

            Unit::AuraApplicationMap const& auras = target->GetAppliedAuras();
            for (Unit::AuraApplicationMap::const_iterator itr = auras.begin(); itr != auras.end(); ++itr)
            {
                Aura* aura = itr->second->GetBase();
                SpellInfo const* auraInfo = aura->GetSpellInfo();
                if (aura && auraInfo->SpellIconID == 2007 && aura->HasEffectType(SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN))
                {
                    aura->ModStackAmount(-1, AURA_REMOVE_BY_ENEMY_SPELL);
                    // Remove dummys from rider (Necessary for updating visual shields)
                    if (Unit* rider = target->GetCharmer())
                        if (Aura* defend = rider->GetAura(aura->GetId()))
                            defend->ModStackAmount(-1, AURA_REMOVE_BY_ENEMY_SPELL);
                    break;
                }
            }
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(NecrocutionScript::RemoveDefend, EFFECT_1, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new NecrocutionScript();
    }
};

/*######
## The Valiant's Challenge Quests
## 13699-13713-13723-13724-13725-13726-13727-13728-13729-13731.- http://www.wowhead.com/search?q=The+Valiant%27s+Challenge SELECT Id,Title FROM quest_template WHERE Title = "The Valiant's Challenge";
######*/

enum DannyData
{
	QUEST_VALIANT_CHALLENGE_HUMAN		= 13699,
	QUEST_VALIANT_CHALLENGE_DWARF		= 13713,
	QUEST_VALIANT_CHALLENGE_GNOME		= 13723,
	QUEST_VALIANT_CHALLENGE_DRAENEI		= 13724,
	QUEST_VALIANT_CHALLENGE_NIGHTELF	= 13725,
	QUEST_VALIANT_CHALLENGE_ORC			= 13726,
	QUEST_VALIANT_CHALLENGE_UNDEAD		= 13727,
	QUEST_VALIANT_CHALLENGE_TAUREN		= 13728,
	QUEST_VALIANT_CHALLENGE_TROLL		= 13729,
	QUEST_VALIANT_CHALLENGE_BLOODELF	= 13731,

	NPC_ARGENT_CHAMPION = 33707,
	GOSSIP_TEXTID_SQUIRE = 14407,

	CHAMPION_TARGET = 1,
};

#define GOSSIP_SQUIRE_ITEM_1 "I am ready to fight!"

class SquireDanny : public CreatureScript
{
public:
	SquireDanny() : CreatureScript("npc_squire_danny") { }

	bool OnGossipHello(Player* player, Creature* creature)
	{
		if ((player->GetQuestStatus(QUEST_VALIANT_CHALLENGE_HUMAN) == QUEST_STATUS_INCOMPLETE ||
			player->GetQuestStatus(QUEST_VALIANT_CHALLENGE_DWARF) == QUEST_STATUS_INCOMPLETE ||
			player->GetQuestStatus(QUEST_VALIANT_CHALLENGE_GNOME) == QUEST_STATUS_INCOMPLETE ||
			player->GetQuestStatus(QUEST_VALIANT_CHALLENGE_DRAENEI) == QUEST_STATUS_INCOMPLETE ||
			player->GetQuestStatus(QUEST_VALIANT_CHALLENGE_NIGHTELF) == QUEST_STATUS_INCOMPLETE ||
			player->GetQuestStatus(QUEST_VALIANT_CHALLENGE_ORC) == QUEST_STATUS_INCOMPLETE ||
			player->GetQuestStatus(QUEST_VALIANT_CHALLENGE_UNDEAD) == QUEST_STATUS_INCOMPLETE ||
			player->GetQuestStatus(QUEST_VALIANT_CHALLENGE_TAUREN) == QUEST_STATUS_INCOMPLETE ||
			player->GetQuestStatus(QUEST_VALIANT_CHALLENGE_TROLL) == QUEST_STATUS_INCOMPLETE ||
			player->GetQuestStatus(QUEST_VALIANT_CHALLENGE_BLOODELF) == QUEST_STATUS_INCOMPLETE) &&
			player->HasAura(RIDE_VEHICLE) && 
			player->HasAura(PLAYER_ON_TOURNAMENT_MOUNT) &&
			player->HasAura(LANCE_EQUIPPED) &&
			!player->isInCombat())
		{
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SQUIRE_ITEM_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
			player->SEND_GOSSIP_MENU(GOSSIP_TEXTID_SQUIRE, creature->GetGUID());
			return true;
		}
		return false;
	}

	bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
	{
		player->PlayerTalkClass->ClearMenus();

		if (action == GOSSIP_ACTION_INFO_DEF + 1)
		{
			player->CLOSE_GOSSIP_MENU();
			if (Creature* summoned = creature->SummonCreature(NPC_ARGENT_CHAMPION, 8541.74f, 1084.32f, 556.4f, 1.14f))
			if (UnitAI* ai = summoned->GetAI())
				ai->SetGUID(player->GetTransGUID(), CHAMPION_TARGET);
		}
		return true;
	}
};

enum ChampionData
{
	POINT_START = 1,

	CHAMPION_CREDIT = 38595,

	ARGENT_CHAMPION_SAY_START = -1850013, //You believe you are ready to be a champion? Defend yourself!
	ARGENT_CHAMPION_SAY_LOOSE = -1850016, //You have shown that you are ready for the rank of champion.
	ARGENT_CHAMPION_SAY_WIN = -1850014, //Most impressive. You are worthy to gain the rank of champion.
};

class ArgentChampion : public CreatureScript
{
public:
	class ArgentChampionAI : public ScriptedAI
	{
	public:
		ArgentChampionAI(Creature* creature) : ScriptedAI(creature) {}

		void SetGUID(uint64 guid, int32 id = 0)
		{
			me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

			if (id != CHAMPION_TARGET)
				return;

			enemy = me->GetUnit(*me, guid);
			me->GetMotionMaster()->MovePoint(POINT_START, 8549.11f, 1133.89f, 556.79f);
			me->SetWalk(false);
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
					me->DespawnOrUnsummon();
					return;
				}
				me->SetWalk(true);
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
			DoScriptText(ARGENT_CHAMPION_SAY_LOOSE, me);
			me->setFaction(35);
			me->SetHealth(1);
			me->DespawnOrUnsummon(5000);

			if (!killer)
				return;

			if (Player* player = killer->ToPlayer())
				player->KilledMonsterCredit(CHAMPION_CREDIT, 0);
			else if (Unit* owner = killer->GetCharmerOrOwner())
			if (Player* player = owner->ToPlayer())
				player->KilledMonsterCredit(CHAMPION_CREDIT, 0);
		}

		void KilledUnit(Unit* /*victim*/)
		{
			DoScriptText(ARGENT_CHAMPION_SAY_WIN, me);
			me->setFaction(35);
			me->DespawnOrUnsummon(5000);
		}

		void EnterCombat(Unit* /*who*/)
		{
			if (enemy)
			{
				DoScriptText(ARGENT_CHAMPION_SAY_START, me);
				me->GetMotionMaster()->MoveChase(enemy);
			}
			else
				me->DespawnOrUnsummon();
		}

		void UpdateAI(const uint32 diff)
		{
			if (!enemy)
			{
				me->DespawnOrUnsummon();
				return;
			}

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
					me->GetMotionMaster()->MovePoint(POINT_RANDOM, pos);
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
			};
		}

	private:
		Unit* enemy;
		EventMap events;
	};

	ArgentChampion() : CreatureScript("npc_argent_champion") {}

	CreatureAI *GetAI(Creature *creature) const
	{
		return new ArgentChampionAI(creature);
	}
};

void AddTournamentPhase2()
{
    new ArgentValiant();
    new BoneguardLieutnant();
    new BoneguardScout();
    new TrampleScourge();
    new TrampleScourgeDummy();
    new Necrocution();
	new SquireDanny();
	new ArgentChampion();
}