#ifndef __PROFESSOR_PUTRICIDE_H
#define __PROFESSOR_PUTRICIDE_H

#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellAuraEffects.h"
#include "Group.h"
#include "Spell.h"
#include "../PlagueWorks.h"
#include "Vehicle.h"




enum ScriptTexts
{
    // Festergut
    SAY_FESTERGUT_GASEOUS_BLIGHT    = 0,
    SAY_FESTERGUT_DEATH             = 1,

    // Rotface
    SAY_ROTFACE_OOZE_FLOOD          = 2,
    SAY_ROTFACE_DEATH               = 3,

    // Professor Putricide
    SAY_AGGRO                       = 4,
    EMOTE_UNSTABLE_EXPERIMENT       = 5,
    SAY_PHASE_TRANSITION_HEROIC     = 6,
    SAY_TRANSFORM_1                 = 7,
    SAY_TRANSFORM_2                 = 8,    // always used for phase2 change, DO NOT GROUP WITH SAY_TRANSFORM_1
    EMOTE_MALLEABLE_GOO             = 9,
    EMOTE_CHOKING_GAS_BOMB          = 10,
    SAY_KILL                        = 11,
    SAY_BERSERK                     = 12,
    SAY_DEATH                       = 13,
    SAY_AIRLOCK                     = 14,
};

enum Spells
{
    // Festergut
    SPELL_RELEASE_GAS_VISUAL            = 69125,
    SPELL_GASEOUS_BLIGHT_LARGE          = 69157,
    SPELL_GASEOUS_BLIGHT_MEDIUM         = 69162,
    SPELL_GASEOUS_BLIGHT_SMALL          = 69164,
    SPELL_MALLABLE_GOO_H                = 72296,

    // Rotface
    SPELL_MALLEABLE_GOO_H               = 72296,
    SPELL_MALLEABLE_GOO_SUMMON          = 72299,

    // Professor Putricide
    SPELL_SLIME_PUDDLE_TRIGGER          = 70341,
    SPELL_MALLEABLE_GOO                 = 70852,
    SPELL_UNSTABLE_EXPERIMENT           = 70351,
    SPELL_TEAR_GAS                      = 71617,    // phase transition
    SPELL_TEAR_GAS_CREATURE             = 71618,
    SPELL_TEAR_GAS_CANCEL               = 71620,
    SPELL_TEAR_GAS_PERIODIC_TRIGGER     = 73170,
    SPELL_CREATE_CONCOCTION             = 71621,
    SPELL_GUZZLE_POTIONS                = 71893,
    SPELL_OOZE_TANK_PROTECTION          = 71770,    // protects the tank
    SPELL_CHOKING_GAS_BOMB              = 71255,
    SPELL_OOZE_VARIABLE                 = 70352,
    SPELL_GAS_VARIABLE                  = 70353,
    SPELL_UNBOUND_PLAGUE                = 70911,
    SPELL_UNBOUND_PLAGUE_SEARCHER       = 70917,
    SPELL_PLAGUE_SICKNESS               = 70953,
    SPELL_UNBOUND_PLAGUE_PROTECTION     = 70955,
    SPELL_MUTATED_PLAGUE                = 72451,
    SPELL_MUTATED_PLAGUE_CLEAR          = 72618,

    // Slime Puddle
    SPELL_GROW_STACKER                  = 70345,
    SPELL_GROW                          = 70347,
    SPELL_SLIME_PUDDLE_AURA             = 70343,

    // Gas Cloud
    SPELL_GASEOUS_BLOAT_PROC            = 70215,
    SPELL_GASEOUS_BLOAT                 = 70672,
    SPELL_GASEOUS_BLOAT_PROTECTION      = 70812,
    SPELL_EXPUNGED_GAS                  = 70701,

    // Volatile Ooze
    SPELL_OOZE_ERUPTION                 = 70492,
    SPELL_VOLATILE_OOZE_ADHESIVE        = 70447,
    SPELL_OOZE_ERUPTION_SEARCH_PERIODIC = 70457,
    SPELL_VOLATILE_OOZE_PROTECTION      = 70530,

    // Choking Gas Bomb
    SPELL_CHOKING_GAS_BOMB_PERIODIC     = 71259,
    SPELL_CHOKING_GAS_EXPLOSION_TRIGGER = 71280,

    // Mutated Abomination vehicle
    SPELL_ABOMINATION_VEHICLE_POWER_DRAIN = 70385,
    SPELL_MUTATED_TRANSFORMATION          = 70311,
    SPELL_MUTATED_TRANSFORMATION_DAMAGE   = 70405,
    SPELL_MUTATED_TRANSFORMATION_NAME     = 72401,

    // Putricide Trap
    SPELL_GIANT_INSECT_SWARM              = 70475,

    // Misc
    SPELL_AURA_ICE_BLOCK                  = 45438,

};

#define SPELL_GASEOUS_BLOAT_HELPER RAID_MODE<uint32>(70672, 72455, 72832, 72833)
#define SPELL_GAS_VARIABLE_HELPER RAID_MODE<uint32>(70353, 74119, 70353, 74119)
#define SPELL_OOZE_VARIABLE_HELPER RAID_MODE<uint32>(70352, 74118, 70352, 74118)
enum Events
{
    // Festergut
    EVENT_FESTERGUT_DIES        = 1,
    EVENT_FESTERGUT_GOO         = 2,

    // Rotface
    EVENT_ROTFACE_DIES          = 3,
    EVENT_ROTFACE_OOZE_FLOOD    = 4,

    EVENT_ROTFACE_DEAD_CHECK    = 5,

    // Professor Putricide
    EVENT_BERSERK               = 6,    // all phases
    EVENT_SLIME_PUDDLE          = 7,    // all phases
    EVENT_UNSTABLE_EXPERIMENT   = 8,    // P1 && P2
    EVENT_TEAR_GAS              = 9,    // phase transition not heroic
    EVENT_RESUME_ATTACK         = 10,
    EVENT_MALLEABLE_GOO         = 11,
    EVENT_CHOKING_GAS_BOMB      = 12,
    EVENT_UNBOUND_PLAGUE        = 13,
    EVENT_MUTATED_PLAGUE        = 14,
    EVENT_PHASE_TRANSITION      = 15,

    // Putricide Trap
    EVENT_GIANT_INSECT_SWARM    = 16,
    EVENT_SPAWN_ADD_WAVE        = 17,
    EVENT_STOP_EVENT            = 18,

    // Putricide Ooze
    EVENT_OOZE_SELECT_TARGET    = 19,
    EVENT_OOZE_ICE_BLOCK        = 20
};

enum Phases
{
    PHASE_NONE          = 1,
    PHASE_FESTERGUT     = 2,
    PHASE_ROTFACE       = 3,
    PHASE_COMBAT_1      = 4,
    PHASE_COMBAT_2      = 5,
    PHASE_COMBAT_3      = 6,

    PHASE_MASK_COMBAT   = (1 << (PHASE_COMBAT_1 - 1)) | (1 << (PHASE_COMBAT_2 - 1)) | (1 << (PHASE_COMBAT_3 - 1)),
    PHASE_MASK_NOT_SELF = (1 << (PHASE_FESTERGUT - 1)) | (1 << (PHASE_ROTFACE - 1)),
};

enum Points
{
    POINT_RESET     = 1,
    POINT_FESTERGUT = 366260,
    POINT_ROTFACE   = 366270,
    POINT_TABLE     = 366780,
};

Position const festergutWatchPos = {4324.820f, 3166.03f, 389.3831f, 3.316126f}; //emote 432 (release gas)
Position const rotfaceWatchPos   = {4390.371f, 3164.50f, 389.3890f, 5.497787f}; //emote 432 (release ooze)
Position const tablePos          = {4356.190f, 3262.90f, 389.4820f, 1.483530f};

// used in Rotface encounter
uint32 const oozeFloodSpells[4] = {69782, 69796, 69798, 69801};



enum PutricideData
{
    DATA_EXPERIMENT_STAGE   = 1,
    DATA_PHASE              = 2,
    DATA_ABOMINATION        = 3
};

enum PutricideStages
{
    DATA_OOZE               = 0,
    DATA_GAS                = 1
};

#define EXPERIMENT_STATE_OOZE   false
#define EXPERIMENT_STATE_GAS    true

class AbominationDespawner
{
public:
    explicit AbominationDespawner(Unit* owner) : _owner(owner) { }

    bool operator()(uint64 guid)
    {
        if (Unit* summon = ObjectAccessor::GetUnit(*_owner, guid))
        {
            if (summon->GetEntry() == NPC_MUTATED_ABOMINATION_10 || summon->GetEntry() == NPC_MUTATED_ABOMINATION_25)
            {
                if (Vehicle* veh = summon->GetVehicleKit())
                    veh->RemoveAllPassengers(); // also despawns the vehicle

                return true;
            }

            return false;
        }

        return true;
    }

private:
    Unit* _owner;
};

class BeamProtectionCheck
{
public:
    explicit BeamProtectionCheck(uint32 excludeAura) : _excludeAura(excludeAura) { }

    bool operator()(WorldObject* unit)
    {
        return unit->GetTypeId() != TYPEID_PLAYER || unit->ToUnit()->HasAura(_excludeAura);
    }

private:
    uint32 _excludeAura;
};

class ExactDistanceCheck
{
public:
    ExactDistanceCheck(Unit* source, float dist) : _source(source), _dist(dist) {}

    bool operator()(WorldObject* unit) const
    {
        return _source->GetExactDist2d(unit) > _dist;
    }

private:
    Unit* _source;
    float _dist;
};

class VariableCheck
{
public:
    VariableCheck(Creature* prof, PutricideStages stage) : _prof(prof), _stage(stage) {}

    bool operator()(WorldObject* target) const
    {
        if (target->GetTypeId() != TYPEID_PLAYER)
            return true;
        switch (_stage)
        {
            case DATA_OOZE:
                return target->ToPlayer()->HasAura(SPELL_GAS_VARIABLE);
            case DATA_GAS:
                return target->ToPlayer()->HasAura(SPELL_OOZE_VARIABLE);
        }
        return false;
    }

private:
    Creature* _prof;
    uint8 _stage;
};

class TankCheck
{
public:
    TankCheck(Creature* prof) : _prof(prof) {}
    
    bool operator()(WorldObject* target) const
    {
        if (_prof->getVictim() && _prof->getVictim()->GetGUID() == target->GetGUID())
            return true;
        return false;
    }
private:
    Creature* _prof;
};

class AbominationCheck
{
    public:

    explicit AbominationCheck() {}

    bool operator()(WorldObject* target) const
    {
        if (target->GetTypeId() == TYPEID_PLAYER && target->ToPlayer()->GetVehicle())
            return true;
        return false;
    }
};

class IsNotAPlayerCheck
{
public:
    explicit IsNotAPlayerCheck() {}

    bool operator()(WorldObject* target) const
    {
        return target->GetTypeId() != TYPEID_PLAYER;
    }
};

#endif