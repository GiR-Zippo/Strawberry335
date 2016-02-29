#ifndef __ROTFACE_H_
#define __ROTFACE_H_

#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellAuras.h"
#include "SpellScript.h"
#include "GridNotifiers.h"
#include "../PlagueWorks.h"

enum Texts
{
    SAY_AGGRO                   = 1,
    EMOTE_SLIME_SPRAY           = 2,
    SAY_SLIME_SPRAY             = 3,
    EMOTE_UNSTABLE_EXPLOSION    = 4,
    SAY_UNSTABLE_EXPLOSION      = 5,
    SAY_KILL                    = 6,
    SAY_BERSERK                 = 7,
    SAY_DEATH                   = 8,
    EMOTE_MUTATED_INFECTION     = 9,
};

enum Creatures
{
    NPC_LITTLE_OOZE             = 36897,
    NPC_BIG_OOZE                = 36899,
};

enum Spells
{
    // Rotface
    SPELL_SLIME_SPRAY                       = 69508,    // every 20 seconds
    SPELL_VILE_GAS_TRIGGER_SUMMON           = 72287,

    SPELL_MUTATED_INFECTION_14_SECONDS      = 70090,    // 14 seconds
    SPELL_MUTATED_INFECTION_12_SECONDS      = 70003,    // 12 seconds
    SPELL_MUTATED_INFECTION_10_SECONDS      = 70004,    // 10 seconds
    SPELL_MUTATED_INFECTION_8_SECONDS       = 70005,    // 8 seconds
    SPELL_MUTATED_INFECTION_6_SECONDS       = 70006,    // 6 seconds

    SPELL_GREEN_BLIGHT_RESIDUE_DEBUFF       = 72145,

    // Oozes
    SPELL_LITTLE_OOZE_COMBINE               = 69537,    // combine 2 Small Oozes
    SPELL_LARGE_OOZE_COMBINE                = 69552,    // combine 2 Large Oozes
    SPELL_LARGE_OOZE_BUFF_COMBINE           = 69611,    // combine Large and Small Ooze
    SPELL_OOZE_MERGE                        = 69889,    // 2 Small Oozes summon a Large Ooze
    SPELL_WEAK_RADIATING_OOZE               = 69750,    // passive damage aura - small
    SPELL_RADIATING_OOZE                    = 69760,    // passive damage aura - large
    SPELL_UNSTABLE_OOZE                     = 69558,    // damage boost and counter for explosion
    SPELL_GREEN_ABOMINATION_HITTIN__YA_PROC = 70001,    // prevents getting hit by infection
    SPELL_UNSTABLE_OOZE_EXPLOSION           = 69839,
    SPELL_STICKY_OOZE                       = 69774,
    SPELL_UNSTABLE_OOZE_EXPLOSION_TRIGGER   = 69832,

    // Professor Putricide
    SPELL_VILE_GAS_H                        = 72272,
    SPELL_VILE_GAS_TRIGGER                  = 72285, 
};

uint32 const MutatedInfectionAura[5] =
{
    SPELL_MUTATED_INFECTION_14_SECONDS,
    SPELL_MUTATED_INFECTION_12_SECONDS,
    SPELL_MUTATED_INFECTION_10_SECONDS,
    SPELL_MUTATED_INFECTION_8_SECONDS,
    SPELL_MUTATED_INFECTION_6_SECONDS
};

uint32 const MutatedInfectionTimer[5] =
{
    100 + 4 * 14 * IN_MILLISECONDS,
    100 + 4 * 12 * IN_MILLISECONDS,
    100 + 4 * 10 * IN_MILLISECONDS,
    100 + 4 * 8 * IN_MILLISECONDS,
    100 + 4 * 6 * IN_MILLISECONDS
};

#define MUTATED_INFECTION RAID_MODE<int32>(69674, 71224, 73022, 73023)

enum Events
{
    // Rotface
    EVENT_SLIME_SPRAY       = 1,
    EVENT_SLIME_SPRAY_RESET = 2,
    EVENT_HASTEN_INFECTIONS = 3,
    EVENT_MUTATED_INFECTION = 4,
    EVENT_VILE_GAS          = 5, 

    EVENT_STICKY_OOZE       = 6,
};

enum InfectionEnums
{
    TIMER_INFECTION_MAX         = 14 * IN_MILLISECONDS,
    TIMER_INFECTION_MIN         = 6 * IN_MILLISECONDS,
    TIMER_INFECTION_REDUCTION   = 2 * IN_MILLISECONDS,
    COUNT_INFECTION_HASTE       = 4
};

class MutatedInfectionCheck {
public:
  MutatedInfectionCheck(Unit *tank, uint32 auraId)
      : _tank(tank), _aura(auraId) {}

  bool operator()(WorldObject *obj) {
    if (obj == _tank || obj->GetTypeId() != TYPEID_PLAYER)
      return true;
    if (Unit *unit = obj->ToUnit())
      if (unit->HasAura(_aura))
        return true;
    return false;
  }

private:
  Unit *_tank;
  uint32 _aura;
};

#endif