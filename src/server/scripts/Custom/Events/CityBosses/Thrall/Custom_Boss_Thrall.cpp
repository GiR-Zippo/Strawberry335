#include "Custom_Boss_Thrall.h"

/*######
## Custom_Boss_Thrall
######*/

void Custom_Boss_ThrallAI::Reset()
{
	WorldBossAI::Reset();
	_maxPlayerCount = 0;
	DespawnSummons();
	for (auto i = 0; i < 4; i++)
		Totems[i] = 0;
	guids_.clear();
	me->RemoveLootMode(LOOT_MODE_HARD_MODE_1);										// Remove additional Loot
	me->RemoveLootMode(LOOT_MODE_HARD_MODE_2);										// Remove additional Loot
	me->RemoveLootMode(LOOT_MODE_HARD_MODE_3);										// Remove additional Loot
}

void Custom_Boss_ThrallAI::EnterEvadeMode()
{
	WorldBossAI::EnterEvadeMode();
}

void Custom_Boss_ThrallAI::EnterCombat(Unit* who)
{
	WorldBossAI::EnterCombat(who);

	events.ScheduleEvent(EVENT_THUNDERSHOCK, TIMER_THUNDERSHOCK/2);
	events.ScheduleEvent(EVENT_CHAIN_LIGHTNING, TIMER_CHAIN_LIGHTNING/2);
	events.ScheduleEvent(EVENT_SUMMON_TOTEM, TIMER_SUMMON_TOTEM/2);
}

void Custom_Boss_ThrallAI::JustDied(Unit* killer)
{
	WorldBossAI::JustDied(killer);
	DespawnSummons();
	for (auto i = 0; i < 4; i++)
		Totems[i] = 0;
	guids_.clear();
	if (_maxPlayerCount > 10)
		me->AddLootMode(LOOT_MODE_HARD_MODE_1);										// Add additional Loot
	if (_maxPlayerCount > 20)
		me->AddLootMode(LOOT_MODE_HARD_MODE_2);										// Add additional Loot
	if (_maxPlayerCount > 30)
		me->AddLootMode(LOOT_MODE_HARD_MODE_3);										// Add additional Loot
}

void Custom_Boss_ThrallAI::KilledUnit(Unit* victim)
{
	WorldBossAI::KilledUnit(victim);
}

void Custom_Boss_ThrallAI::UpdateAI(uint32 const diff)
{
	if (!UpdateVictim())
		return;

	events.Update(diff);

	if (me->HasUnitState(UNIT_STATE_CASTING))
		return;

	if (uint32 eventId = events.ExecuteEvent())
	{
		switch (eventId)
		{
			case EVENT_UPDATE:
				UpdateBuff();
				break;
			case EVENT_THUNDERSHOCK:
				DoCast(me, SPELL_THUNDERSHOCK);
				events.ScheduleEvent(EVENT_THUNDERSHOCK, TIMER_THUNDERSHOCK);
				break;
			case EVENT_CHAIN_LIGHTNING:
				ChainLightning();
				events.ScheduleEvent(EVENT_CHAIN_LIGHTNING, TIMER_CHAIN_LIGHTNING);
				break;
			case EVENT_SUMMON_TOTEM:
				TotemHandler();
				events.ScheduleEvent(EVENT_SUMMON_TOTEM, TIMER_SUMMON_TOTEM);
				break;
			default: 
				break;
		}
	}

	DoMeleeAttackIfReady();
}

void Custom_Boss_ThrallAI::DespawnSummons()
{
	for (auto guid : guids_)
	{
		if (auto creature = ObjectAccessor::GetCreature(*me, guid))
			creature->DespawnOrUnsummon();
	}
	guids_.clear();
}

void Custom_Boss_ThrallAI::UpdateBuff()
{
	uint32 PlayerCount = 0;
	auto& threatlist = me->getThreatManager().getThreatList();
	for (auto itr = threatlist.begin(); itr != threatlist.end(); ++itr)
	{
		if (Unit* unit = Unit::GetUnit(*me, (*itr)->getUnitGuid()))
		if (unit->GetTypeId() == TYPEID_PLAYER)
			PlayerCount++;
	}

	if (PlayerCount > _maxPlayerCount)
		_maxPlayerCount = PlayerCount;

	if (_maxPlayerCount > 10)
	{
		uint32 BuffStack = ceil((_maxPlayerCount - 10) / 2);
		me->SetAuraStack(SPELL_TENACITY, me, BuffStack);

		for (auto guid : guids_)
		{
			if (auto creature = ObjectAccessor::GetCreature(*me, guid))
				if (creature->isAlive())
					creature->SetAuraStack(SPELL_TENACITY, creature, BuffStack);
		}
	}
	else
	{
		me->RemoveAura(SPELL_TENACITY);
		for (auto guid : guids_)
		{
			if (auto creature = ObjectAccessor::GetCreature(*me, guid))
				if (creature->isAlive())
					creature->RemoveAura(SPELL_TENACITY);
		}
	}
}

void Custom_Boss_ThrallAI::TotemHandler()
{
	std::vector<int> tempTotems;

	for (int i = 0; i < 4; i++)
	{
		auto creature = ObjectAccessor::GetCreature(*me, Totems[i]);

		if (!creature || !creature->isAlive())
			tempTotems.push_back(i);
	}

	if (tempTotems.size() > 0)
	{
		int index = Trinity::Containers::SelectRandomContainerElement(tempTotems);

		if (Creature* creature = me->SummonCreature(NPC_TOTEM, totemPos[index], TEMPSUMMON_MANUAL_DESPAWN))
		{
			guids_.push_back(creature->GetGUID());
			Totems[index] = creature->GetGUID();
			creature->DelayedCastSpell(TIMER_DELAY_LIGHTNING_TENDRILS, creature, SPELL_LIGHTNING_TENDRILS);
		}
	}
}

void Custom_Boss_ThrallAI::ChainLightning()
{
	std::list<Unit*> ranged;
	Unit* target = nullptr;
	SelectTargetList(ranged, _maxPlayerCount, SELECT_TARGET_RANDOM, -5.0f, true);
	if (ranged.size() > ceil(_maxPlayerCount / 3))
		target = Trinity::Containers::SelectRandomContainerElement(ranged);
	else
		target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.0f, true);
	DoCast(target, SPELL_CHAIN_LIGHTNING);
}

bool Custom_Boss_Thrall::OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
{
	player->PlayerTalkClass->ClearMenus();
	switch (action)
	{
	case GOSSIP_ACTION_INFO_DEF + 1:
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_STW1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
		player->SEND_GOSSIP_MENU(5733, creature->GetGUID());
		break;
	case GOSSIP_ACTION_INFO_DEF + 2:
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_STW2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
		player->SEND_GOSSIP_MENU(5734, creature->GetGUID());
		break;
	case GOSSIP_ACTION_INFO_DEF + 3:
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_STW3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
		player->SEND_GOSSIP_MENU(5735, creature->GetGUID());
		break;
	case GOSSIP_ACTION_INFO_DEF + 4:
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_STW4, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
		player->SEND_GOSSIP_MENU(5736, creature->GetGUID());
		break;
	case GOSSIP_ACTION_INFO_DEF + 5:
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_STW5, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
		player->SEND_GOSSIP_MENU(5737, creature->GetGUID());
		break;
	case GOSSIP_ACTION_INFO_DEF + 6:
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_STW6, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);
		player->SEND_GOSSIP_MENU(5738, creature->GetGUID());
		break;
	case GOSSIP_ACTION_INFO_DEF + 7:
		player->CLOSE_GOSSIP_MENU();
		player->AreaExploredOrEventHappens(QUEST_6566);
		break;
	}
	return true;
}

bool Custom_Boss_Thrall::OnGossipHello(Player* player, Creature* creature)
{
	if (creature->isQuestGiver())
		player->PrepareQuestMenu(creature->GetGUID());

	if (player->GetQuestStatus(QUEST_6566) == QUEST_STATUS_INCOMPLETE)
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_HTW, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

	player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
	return true;
}