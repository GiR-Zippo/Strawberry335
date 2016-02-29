#ifndef __UPPER_SPIRE_H
#define __UPPER_SPIRE_H

#include "../IcecrownCitadel.h"

class NpcValkyrUpperSpireAI : public ScriptedAI
{
public:
    NpcValkyrUpperSpireAI(Creature* creature) : ScriptedAI(creature) {}

    void Reset();
    void UpdateAI(uint32 const diff);

private:
    EventMap _events;
};


class NpcValkyrUpperSpire : public CreatureScript
{
public:
    NpcValkyrUpperSpire() : CreatureScript("npc_valkyr_upper_spire_icc") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return GetIcecrownCitadelAI<NpcValkyrUpperSpireAI>(creature);
    }
};

class NpcSeveredEssenceUpperSpireAI : public ScriptedAI
{
public:
    NpcSeveredEssenceUpperSpireAI(Creature* creature) : ScriptedAI(creature) {}

    void Reset();
    void UpdateAI(uint32 const diff);
    void DoAction(const int32 action);

private:
    EventMap _events;
};


class NpcSeveredEssenceUpperSpire : public CreatureScript
{
public:
    NpcSeveredEssenceUpperSpire() : CreatureScript("npc_severed_essence_icc") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return GetIcecrownCitadelAI<NpcSeveredEssenceUpperSpireAI>(creature);
    }
};

enum UpperSpireEvents
{
    EVENT_ACTIVATE_TRAP         = 1,
    EVENT_SEVERED_ESSENCE       = 2,
    EVENT_SET_CLASS             = 3,
    EVENT_WARRIOR, 
    EVENT_MAGE,
    EVENT_WARLOCK_SHADOWBOLT,
    EVENT_WARLOCK_RAIN,
    EVENT_WARLOCK_CORRUPTION,
    EVENT_HUNTER,
    EVENT_DEATHKNIGHT,
    EVENT_DEATHKNIGHT_NECRO,
    EVENT_PALADIN,
    EVENT_SHAMAN,
    EVENT_SHAMAN_HEAL,
    EVENT_DRUID,
    EVENT_DRUID_RIP,
    EVENT_ROGUE,
    EVENT_PRIEST,
    EVENT_PRIEST_HEAL,
};

enum UpperSpireNPCs
{
    NPC_SEVERED_ESSENCE         = 38410,
};

enum UpperSpireActions
{
    ACTION_CLONE_OWNER          = 1,
};

enum UpperSpireSpells
{
    // Frost Freeze Trap
    SPELL_COLDFLAME_JETS        = 70460,
    // Valkyr
    SPELL_SEVERED_ESSENCE       = 71906,
    SPELL_CLONE_IMAGE           = 57507,
    SPELL_CLONE_WEAPON          = 69891,
    SPELL_CLONE_OFFHAND         = 69896,
    // Class spells for essence
    // Warlock
    SPELL_RAIN_OF_CHAOS         = 71965,
    SPELL_SHADOW_BOLT           = 71936,
    SPELL_CORRUPTION            = 71937,
    // Paladin
    SPELL_RADIANCE_AURA         = 71953,
    // Shaman
    SPELL_LIGHTNING_BOLT        = 71934,
    SPELL_HEALING_RAIN          = 71956,
    // Priest
    SPELL_RENEW                 = 71932,
    SPELL_FLASH_HEAL            = 71930,
    // Rogue
    SPELL_FOCUSED_ATTACKS       = 71955,
    // Mage
    SPELL_FIREBALL              = 71928,
    // DK
    SPELL_NECRO_STRIKE          = 71951,
    SPELL_PLAGUE_STRIKE         = 71924,
    // Warrior
    SPELL_HEROIC_LEAP           = 71961,
    // Hunter
    SPELL_SHOT                  = 71927,
    // Druid
    SPELL_RIP                   = 71926,
    SPELL_CAT_FORM              = 57655,
};

#endif