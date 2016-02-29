#include "ScriptedCreature.h"
#include "Creature.h"

/*######
## Custom_Boss_VarianWrynn
######*/

enum spells
{
	// scaling
	SPELL_TENACITY				= 58549,		// Boss Scaling

	// varian_wrynn
	SPELL_WHILING_SLASH			= 59824,		// bladestorm that causes bleeding debuffs -> player need to move away from the boss
	SPELL_CHARGE				= 60067,		// charge a player who is out of meele range -> player need to group in meelerange of the boss
	SPELL_PUNCTRUE				= 59826,		// tanks will get this stack of bleeding debuff
};

enum timer
{
	// scaling
	TIMER_UPDATE				= 1 * IN_MILLISECONDS,

	// varian_wrynn
	TIMER_WHILING_SLASH			= 60 * IN_MILLISECONDS,
	TIMER_PUNCTURE				= 10 * IN_MILLISECONDS,

	TIMER_CHARGE_WAIT			= 10 * IN_MILLISECONDS,
	TIMER_CHARGE_COOLDOWN		= 2 * IN_MILLISECONDS,
	TIMER_CHARGE_RECALL			= 1 * IN_MILLISECONDS,
};

enum events
{
	EVENT_UPDATE				= 1,
	EVENT_WHILING_SLASH			= 2,
	EVENT_CHARGE_PLAYER			= 3,
	EVENT_PUNCTURE				= 4,
};

class Custom_Boss_VarianWrynnAI : public WorldBossAI
{
public:
	Custom_Boss_VarianWrynnAI(Creature* creature) : WorldBossAI(creature)
	{
		_maxPlayerCount = 0;
		debuffedChars_.clear();
	}

	void Reset();
	void EnterEvadeMode();
	void EnterCombat(Unit* /*who*/);
	void JustDied(Unit* /*killer*/);
	void KilledUnit(Unit* victim);
	void UpdateAI(uint32 const diff);

private:
	uint32 _maxPlayerCount;
	std::set<uint64> debuffedChars_;
};

class Custom_Boss_VarianWrynn : public CreatureScript
{
public:
	Custom_Boss_VarianWrynn() : CreatureScript("Custom_Boss_VarianWrynn") { }

	CreatureAI* GetAI(Creature* creature) const
	{
		return new Custom_Boss_VarianWrynnAI(creature);
	}
};

void AddSC_Custom_Boss_VarianWrynn()
{
	new Custom_Boss_VarianWrynn();
}