#include "LowerSpire.h"
#include "ScriptedCreature.h"

enum NpcNerubarEvents
{
    EVENT_CAST_CRYPT_SCARABS    = 1,
    EVENT_CAST_WEB              = 2,
    EVENT_CAST_DARK_MENDING     = 3,
};

enum NpcNerubarPoints
{
    POINT_NPC_DOWN              = 1,
    POINT_NPC_GROUND            = 2,
    POINT_NPC_END               = 3,
};

enum NpcNerubarSpells
{
    SPELL_WEBROPE               = 56223,
    SPELL_CRYPT_SCARABS         = 71326,
    SPELL_WEB                   = 70980,
    SPELL_DARK_MENDING          = 71020,
    SPELL_SOULFEAST             = 71203
};

class NpcLowerSpireNerubarAI : public ScriptedAI
{
public:
    NpcLowerSpireNerubarAI(Creature* creature) : ScriptedAI(creature)
    {
        _StartCounter = 1;
    }

    void Reset();
    void DoAction(const int32 id);
    void JustDied(Unit* killer);
    void MovementInform(uint32 type, uint32 id);
    void UpdateAI(const uint32 diff);
private:
    EventMap events;
    uint64 stalker;
    uint32 _StartCounter;
};

class NpcLowerSpireNerubar : public CreatureScript
{
public:
    NpcLowerSpireNerubar() : CreatureScript("npc_lower_spire_nerubar") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return GetIcecrownCitadelAI<NpcLowerSpireNerubarAI>(creature);
    }
};