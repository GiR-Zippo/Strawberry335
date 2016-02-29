#include "Custom_Boss_VarianWrynn.h"

/*######
## Custom_Boss_VarianWrynn
######*/

void Custom_Boss_VarianWrynnAI::Reset()
{
	WorldBossAI::Reset();
	_maxPlayerCount = 0;
	for (auto itr = debuffedChars_.begin(); itr != debuffedChars_.end(); ++itr)
	{
		if (Player * player = sObjectMgr->GetPlayerByLowGUID(*itr))
		{
			player->RemoveAurasDueToSpell(SPELL_PUNCTRUE);
		}
	}
	debuffedChars_.clear();
	me->RemoveLootMode(LOOT_MODE_HARD_MODE_1);										// Remove additional Loot
	me->RemoveLootMode(LOOT_MODE_HARD_MODE_2);										// Remove additional Loot
	me->RemoveLootMode(LOOT_MODE_HARD_MODE_3);										// Remove additional Loot
}

void Custom_Boss_VarianWrynnAI::EnterEvadeMode()
{
	WorldBossAI::EnterEvadeMode();
}

void Custom_Boss_VarianWrynnAI::EnterCombat(Unit* who)
{
	WorldBossAI::EnterCombat(who);

	events.ScheduleEvent(EVENT_WHILING_SLASH, TIMER_WHILING_SLASH / 2);
	events.ScheduleEvent(EVENT_CHARGE_PLAYER, TIMER_CHARGE_COOLDOWN / 2);
	events.ScheduleEvent(EVENT_PUNCTURE, TIMER_PUNCTURE / 2);
}

void Custom_Boss_VarianWrynnAI::JustDied(Unit* killer)
{
	WorldBossAI::JustDied(killer);
	for (auto itr = debuffedChars_.begin(); itr != debuffedChars_.end(); ++itr)
	{
		if (Player * player = sObjectMgr->GetPlayerByLowGUID(*itr))
		{
			player->RemoveAurasDueToSpell(SPELL_PUNCTRUE);
		}
	}
	if (_maxPlayerCount > 10)
		me->AddLootMode(LOOT_MODE_HARD_MODE_1);										// Add additional Loot
	if (_maxPlayerCount > 20)
		me->AddLootMode(LOOT_MODE_HARD_MODE_2);										// Add additional Loot
	if (_maxPlayerCount > 30)
		me->AddLootMode(LOOT_MODE_HARD_MODE_3);										// Add additional Loot
}

void Custom_Boss_VarianWrynnAI::KilledUnit(Unit* victim)
{
	WorldBossAI::KilledUnit(victim);
}

void Custom_Boss_VarianWrynnAI::UpdateAI(uint32 const diff)
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
		case EVENT_WHILING_SLASH:
			events.RescheduleEvent(EVENT_CHARGE_PLAYER, TIMER_CHARGE_WAIT);
			DoCast(me, SPELL_WHILING_SLASH);
			events.ScheduleEvent(EVENT_WHILING_SLASH, TIMER_WHILING_SLASH);
			break;
		case EVENT_CHARGE_PLAYER:
			if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, -5.0f, true))
			{
				DoCast(target, SPELL_CHARGE);
				events.ScheduleEvent(EVENT_CHARGE_PLAYER, TIMER_CHARGE_COOLDOWN);
			}
			else
				events.ScheduleEvent(EVENT_CHARGE_PLAYER, TIMER_CHARGE_RECALL);
		case EVENT_UPDATE:
			{
			uint32 _playerCount = 0;
			auto& threatlist = me->getThreatManager().getThreatList();
			for (auto itr = threatlist.begin(); itr != threatlist.end(); ++itr)
			{
				 if (Unit* unit = Unit::GetUnit(*me, (*itr)->getUnitGuid()))
					if (unit->GetTypeId() == TYPEID_PLAYER)
						_playerCount++;
			}

			if (_playerCount > _maxPlayerCount)
				_maxPlayerCount = _playerCount;

			if (_maxPlayerCount > 10)
				me->SetAuraStack(SPELL_TENACITY, me, ceil((_maxPlayerCount - 10) / 2));
			else
				me->RemoveAura(SPELL_TENACITY);
			events.ScheduleEvent(EVENT_UPDATE, TIMER_UPDATE);
			break;
			}
		case EVENT_PUNCTURE:
			Unit* victim = me->getVictim();
			
			auto search = debuffedChars_.find(victim->GetGUIDLow());
			if (search == debuffedChars_.end()) {
				debuffedChars_.insert(victim->GetGUIDLow());
			}
			DoCast(victim, SPELL_PUNCTRUE);
			events.ScheduleEvent(EVENT_PUNCTURE, TIMER_PUNCTURE);
			break;
		}
	}

	DoMeleeAttackIfReady();
}