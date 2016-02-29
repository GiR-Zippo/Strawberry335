#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "Creature.h"


/*######
## Custom_Boss_Thrall
######*/


enum quests
{
	QUEST_6566						= 6566,					// Handling for Quest of normal Thrall
};

enum spells
{
	// scaling
	SPELL_TENACITY					= 58549,				// Boss Scaling

	// thrall
	SPELL_THUNDERSHOCK				= 60029,				// cast the the casters position to let the tank move him around
	SPELL_CHAIN_LIGHTNING			= 64213,				// cast it to rnd players who are not in meele range

	// totem
	SPELL_LIGHTNING_TENDRILS		= 61887,				// aura of that totem
};

enum timer
{
	// scaling
	TIMER_UPDATE					= 1 * IN_MILLISECONDS,	// Scaling Update Timer

	// thrall
	TIMER_THUNDERSHOCK				= 10 * IN_MILLISECONDS,
	TIMER_CHAIN_LIGHTNING			= 8 * IN_MILLISECONDS,
	TIMER_SUMMON_TOTEM				= 30 * IN_MILLISECONDS,

	// totem
	TIMER_DELAY_LIGHTNING_TENDRILS	= 10 * IN_MILLISECONDS,
};

enum event
{
	// scaling
	EVENT_UPDATE					= 1,

	// thrall
	EVENT_THUNDERSHOCK				= 2,
	EVENT_CHAIN_LIGHTNING			= 3,
	EVENT_SUMMON_TOTEM				= 4,

	// totem
	EVENT_LIGHTNING_TENDRILS		= 5,
};

enum npcs
{
	NPC_TOTEM = 70003,
};

Position const totemPos[4] =
{
	{ 1933.5f, -4159.1f, 40.93f, 2.40f },
	{ 1932.1f, -4135.7f, 40.82f, 4.00f },
	{ 1908.5f, -4137.4f, 40.95f, 5.57f },
	{ 1911.1f, -4160.8f, 40.92f, 0.80f },
};

#define GOSSIP_HTW "Please share your wisdom with me, Warchief."
#define GOSSIP_STW1 "What discoveries?"
#define GOSSIP_STW2 "Usurper?"
#define GOSSIP_STW3 "With all due respect, Warchief - why not allow them to be destroyed? Does this not strengthen our position?"
#define GOSSIP_STW4 "I... I did not think of it that way, Warchief."
#define GOSSIP_STW5 "I live only to serve, Warchief! My life is empty and meaningless without your guidance."
#define GOSSIP_STW6 "Of course, Warchief!"

class Custom_Boss_ThrallAI : public WorldBossAI
{
public:
	Custom_Boss_ThrallAI(Creature* creature) : WorldBossAI(creature)
	{
		_maxPlayerCount = 0;
		guids_.clear();
		for (auto i = 0; i < 4; i++)
			Totems[i] = 0;
	}

	void Reset();
	void EnterEvadeMode();
	void EnterCombat(Unit* /*who*/);
	void JustDied(Unit* /*killer*/);
	void KilledUnit(Unit* victim);
	void UpdateAI(uint32 const diff);

private:

	void DespawnSummons();
	void UpdateBuff();
	void TotemHandler();
	void ChainLightning();

	uint32 _maxPlayerCount;
	uint64 Totems[4];
	std::vector<uint64> guids_;
};

class Custom_Boss_Thrall : public CreatureScript
{
public:
	Custom_Boss_Thrall() : CreatureScript("Custom_Boss_Thrall") { }

	bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action);
	bool OnGossipHello(Player* player, Creature* creature);

	CreatureAI* GetAI(Creature* creature) const
	{
		return new Custom_Boss_ThrallAI(creature);
	}
};

void AddSC_Custom_Boss_Thrall()
{
	new Custom_Boss_Thrall();
}