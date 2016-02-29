#ifndef __THELICHKING_H_
#define __THELICHKING_H_

#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "Spell.h"
#include "Vehicle.h"
#include "Cell.h"
#include "CellImpl.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "CreatureTextMgr.h"
#include "Group.h"
#include "../FrozenThrone.h"


#define HARVEST_SOUL_CHECK(UNIT)   (UNIT->HasAura(68980) || UNIT->HasAura(74325) || UNIT->HasAura(74296) || UNIT->HasAura(74297))
#define HARVEST_SOUL_CHECK_N(UNIT) (!UNIT->HasAura(68980) && !UNIT->HasAura(74325) && !UNIT->HasAura(74296) && !UNIT->HasAura(74297)) // harvest souls nhc/hc

enum Texts
{
    // The Lich King
    SAY_LK_INTRO_1                  = 0,
    SAY_LK_INTRO_2                  = 1,
    SAY_LK_INTRO_3                  = 2,
    SAY_LK_REMORSELESS_WINTER       = 4,
    SAY_LK_QUAKE                    = 5,
    SAY_LK_SUMMON_VALKYR            = 6,
    SAY_LK_HARVEST_SOUL             = 7,
    SAY_LK_FROSTMOURNE_ESCAPE       = 8,    // not said on heroic
    SAY_LK_FROSTMOURNE_KILL         = 9,    // not said on heroic
    SAY_LK_KILL                     = 10,
    SAY_LK_BERSERK                  = 11,
    EMOTE_DEFILE_WARNING            = 12,
    EMOTE_NECROTIC_PLAGUE_WARNING   = 13,
    SAY_LK_OUTRO_1                  = 14,
    SAY_LK_OUTRO_2                  = 15,
    SAY_LK_OUTRO_3                  = 16,
    SAY_LK_OUTRO_4                  = 17,
    SAY_LK_OUTRO_5                  = 18,
    SAY_LK_OUTRO_6                  = 19,
    SAY_LK_OUTRO_7                  = 20,
    SAY_LK_OUTRO_8                  = 21,

    // Highlord Tirion Fordring
    SAY_TIRION_INTRO_1              = 0,
    SAY_TIRION_INTRO_2              = 1,
    SAY_TIRION_OUTRO_1              = 2,
    SAY_TIRION_OUTRO_2              = 3,

    // Terenas Menethil (outro)
    SAY_TERENAS_OUTRO_1             = 0,
    SAY_TERENAS_OUTRO_2             = 1,

    // Terenas Menethil (Frostmourne)
    SAY_TERENAS_INTRO_1             = 0,
    SAY_TERENAS_INTRO_2             = 1,
    SAY_TERENAS_INTRO_3             = 2,
};

enum Spells
{
    // The Lich King
    SPELL_PLAGUE_AVOIDANCE              = 72846,    // raging spirits also get it
    SPELL_EMOTE_SIT_NO_SHEATH           = 73220,
    SPELL_BOSS_HITTIN_YA                = 73878,
    SPELL_EMOTE_SHOUT_NO_SHEATH         = 73213,
    SPELL_ICE_LOCK                      = 71614,

    // Phase 1
    SPELL_SUMMON_SHAMBLING_HORROR       = 70372,
    SPELL_RISEN_WITCH_DOCTOR_SPAWN      = 69639,
    SPELL_SUMMON_DRUDGE_GHOULS          = 70358,
    SPELL_INFEST                        = 70541,
    SPELL_NECROTIC_PLAGUE               = 70337,
    SPELL_NECROTIC_PLAGUE_JUMP          = 70338,
    SPELL_PLAGUE_SIPHON                 = 74074,
    SPELL_SHADOW_TRAP                   = 73539,
    SPELL_SHADOW_TRAP_AURA              = 73525,
    SPELL_SHADOW_TRAP_KNOCKBACK         = 73529,

    // Phase Transition
    SPELL_REMORSELESS_WINTER_1          = 68981,
    SPELL_REMORSELESS_WINTER_2          = 72259,
    SPELL_PAIN_AND_SUFFERING            = 72133,
    SPELL_SUMMON_ICE_SPHERE             = 69104,
    SPELL_ICE_SPHERE                    = 69090,
    SPELL_ICE_BURST_TARGET_SEARCH       = 69109,
    SPELL_ICE_PULSE                     = 69091,
    SPELL_ICE_BURST                     = 69108,
    SPELL_RAGING_SPIRIT                 = 69200,
    SPELL_RAGING_SPIRIT_VISUAL          = 69197,
    SPELL_RAGING_SPIRIT_VISUAL_CLONE    = 69198,
    SPELL_SOUL_SHRIEK                   = 69242,
    SPELL_QUAKE                         = 72262,

    // Phase 2
    SPELL_DEFILE                        = 72762,
    SPELL_DEFILE_AURA                   = 72743,
    SPELL_DEFILE_GROW                   = 72756,
    SPELL_SUMMON_VALKYR                 = 69037,
    SPELL_SUMMON_VALKYR_PERIODIC        = 74361,
    SPELL_HARVEST_SOUL_VALKYR           = 68985,    // Val'kyr Shadowguard vehicle aura
    SPELL_SOUL_REAPER                   = 69409,
    SPELL_SOUL_REAPER_BUFF              = 69410,
    SPELL_WINGS_OF_THE_DAMNED           = 74352,
    SPELL_VALKYR_TARGET_SEARCH          = 69030,
    SPELL_CHARGE                        = 42003,    // cast on selected target
    SPELL_VALKYR_CARRY                  = 74445,    // removes unselectable flag
    SPELL_DECREASE_SPEED                = 63004,    // generic spell [DND] NPC Slow (50%)
    SPELL_LIFE_SIPHON                   = 73488,
    SPELL_LIFE_SIPHON_HEAL              = 73489,
    SPELL_EJECT_ALL_PASSENGERS          = 68576,

    // Phase 3
    SPELL_VILE_SPIRITS                  = 70498,
    SPELL_VILE_SPIRIT_MOVE_SEARCH       = 70501,
    SPELL_VILE_SPIRIT_DAMAGE_SEARCH     = 70502,
    SPELL_SPIRIT_BURST                  = 70503,
    SPELL_HARVEST_SOUL                  = 68980,
    SPELL_HARVEST_SOULS                 = 73654,    // Heroic version, weird because it has all 4 difficulties just like above spell
    SPELL_HARVEST_SOUL_VEHICLE          = 68984,
    SPELL_HARVEST_SOUL_VISUAL           = 71372,
    SPELL_HARVEST_SOUL_TELEPORT         = 72546,
    SPELL_HARVEST_SOULS_TELEPORT        = 73655,
    SPELL_HARVEST_SOUL_TELEPORT_BACK    = 72597,
    SPELL_IN_FROSTMOURNE_ROOM           = 74276,
    SPELL_KILL_FROSTMOURNE_PLAYERS      = 75127,
    SPELL_HARVESTED_SOUL                = 72679,
    SPELL_TRIGGER_VILE_SPIRIT_HEROIC    = 73582,
    SPELL_HARVESTED_SOUL_PLAYER         = 74322,

    // Frostmourne
    SPELL_LIGHTS_FAVOR                  = 69382,
    SPELL_RESTORE_SOUL                  = 72595,
    SPELL_RESTORE_SOULS                 = 73650,    // Heroic
    SPELL_DARK_HUNGER                   = 69383,    // Passive proc healing
    SPELL_DESTROY_SOUL                  = 74086,    // Used when Terenas Menethil dies
    SPELL_SOUL_RIP                      = 69397,    // Deals increasing damage
    SPELL_SOUL_RIP_DAMAGE               = 69398,
    SPELL_TERENAS_LOSES_INSIDE          = 72572,
    SPELL_SUMMON_SPIRIT_BOMB_1          = 73581,    // (Heroic)
    SPELL_SUMMON_SPIRIT_BOMB_2          = 74299,    // (Heroic)
    SPELL_EXPLOSION                     = 73576,    // Spirit Bomb (Heroic)
    SPELL_HARVEST_SOUL_DAMAGE_AURA      = 73655,

    // Outro
    SPELL_FURY_OF_FROSTMOURNE           = 72350,
    SPELL_FURY_OF_FROSTMOURNE_NO_REZ    = 72351,
    SPELL_EMOTE_QUESTION_NO_SHEATH      = 73330,
    SPELL_RAISE_DEAD                    = 71769,
    SPELL_LIGHTS_BLESSING               = 71797,
    SPELL_JUMP                          = 71809,
    SPELL_JUMP_TRIGGERED                = 71811,
    SPELL_JUMP_2                        = 72431,
    SPELL_SUMMON_BROKEN_FROSTMOURNE     = 74081,    // visual
    SPELL_SUMMON_BROKEN_FROSTMOURNE_2   = 72406,    // animation
    SPELL_SUMMON_BROKEN_FROSTMOURNE_3   = 73017,    // real summon
    SPELL_BROKEN_FROSTMOURNE            = 72398,
    SPELL_BROKEN_FROSTMOURNE_KNOCK      = 72405,
    SPELL_SOUL_BARRAGE                  = 72305,
    SPELL_SUMMON_TERENAS                = 72420,
    SPELL_MASS_RESURRECTION             = 72429,
    SPELL_MASS_RESURRECTION_REAL        = 72423,
    SPELL_PLAY_MOVIE                    = 73159,

    // Shambling Horror
    SPELL_SHOCKWAVE                     = 72149,
    SPELL_ENRAGE                        = 72143,
    SPELL_FRENZY                        = 28747,
};

#define NECROTIC_PLAGUE_LK   RAID_MODE<uint32>(70337, 73912, 73913, 73914)
#define NECROTIC_PLAGUE_PLR  RAID_MODE<uint32>(70338, 73785, 73786, 73787)
#define REMORSELESS_WINTER_1 RAID_MODE<uint32>(68981, 74270, 74271, 74272)
#define REMORSELESS_WINTER_2 RAID_MODE<uint32>(72259, 74273, 74274, 74275)
#define SUMMON_VALKYR        RAID_MODE<uint32>(69037, 74361, 69037, 74361)
#define HARVEST_SOUL         RAID_MODE<uint32>(68980, 74325, 74296, 74297)

enum Events
{
    // The Lich King
    // intro events
    EVENT_INTRO_MOVE_1              = 1,
    EVENT_INTRO_MOVE_2              = 2,
    EVENT_INTRO_MOVE_3              = 3,
    EVENT_INTRO_TALK_1              = 4,
    EVENT_EMOTE_CAST_SHOUT          = 5,
    EVENT_INTRO_EMOTE_1             = 6,
    EVENT_INTRO_CHARGE              = 7,
    EVENT_INTRO_CAST_FREEZE         = 8,
    EVENT_FINISH_INTRO              = 9,

    // combat events
    EVENT_SUMMON_SHAMBLING_HORROR   = 10,
    EVENT_SUMMON_DRUDGE_GHOUL       = 11,
    EVENT_INFEST                    = 12,
    EVENT_NECROTIC_PLAGUE           = 13,
    EVENT_SHADOW_TRAP               = 14,   // heroic only
    EVENT_SOUL_REAPER               = 15,
    EVENT_DEFILE                    = 16,
    EVENT_HARVEST_SOUL              = 17,   // normal mode only
    EVENT_PAIN_AND_SUFFERING        = 18,
    EVENT_SUMMON_ICE_SPHERE         = 19,
    EVENT_SUMMON_RAGING_SPIRIT      = 20,
    EVENT_QUAKE                     = 21,
    EVENT_SUMMON_VALKYR             = 22,
    EVENT_GRAB_PLAYER               = 23,
    EVENT_MOVE_TO_DROP_POS          = 24,
    EVENT_LIFE_SIPHON               = 25,   // heroic only
    EVENT_START_ATTACK              = 26,
    EVENT_QUAKE_2                   = 27,
    EVENT_VILE_SPIRITS              = 28,
    EVENT_HARVEST_SOULS             = 29,   // heroic only
    EVENT_BERSERK                   = 30,
    EVENT_SOUL_RIP                  = 31,
    EVENT_DESTROY_SOUL              = 32,
    EVENT_FROSTMOURNE_TALK_1        = 33,
    EVENT_FROSTMOURNE_TALK_2        = 34,
    EVENT_FROSTMOURNE_TALK_3        = 35,
    EVENT_TELEPORT_BACK             = 36,
    EVENT_FROSTMOURNE_HEROIC        = 37,
    EVENT_OUTRO_TALK_1              = 38,
    EVENT_OUTRO_TALK_2              = 39,
    EVENT_OUTRO_EMOTE_TALK          = 40,
    EVENT_OUTRO_TALK_3              = 41,
    EVENT_OUTRO_MOVE_CENTER         = 42,
    EVENT_OUTRO_TALK_4              = 43,
    EVENT_OUTRO_RAISE_DEAD          = 44,
    EVENT_OUTRO_TALK_5              = 45,
    EVENT_OUTRO_BLESS               = 46,
    EVENT_OUTRO_REMOVE_ICE          = 47,
    EVENT_OUTRO_MOVE_1              = 48,
    EVENT_OUTRO_JUMP                = 49,
    EVENT_OUTRO_TALK_6              = 50,
    EVENT_OUTRO_KNOCK_BACK          = 51,
    EVENT_OUTRO_SOUL_BARRAGE        = 52,
    EVENT_OUTRO_SUMMON_TERENAS      = 53,
    EVENT_OUTRO_TERENAS_TALK_1      = 54,
    EVENT_OUTRO_TERENAS_TALK_2      = 55,
    EVENT_OUTRO_TALK_7              = 56,
    EVENT_OUTRO_TALK_8              = 57,

    // Shambling Horror
    EVENT_SHOCKWAVE                 = 58,
    EVENT_ENRAGE                    = 59,

    // Raging Spirit
    EVENT_SOUL_SHRIEK               = 60,

    // Strangulate Vehicle (Harvest Soul)
    EVENT_TELEPORT                  = 61,
    EVENT_MOVE_TO_LICH_KING         = 62,
    EVENT_DESPAWN_SELF              = 63,

    // Valkyr
    EVENT_VALKYR_NEXT_STEP          = 64,
    EVENT_CHECK_MOVEMENT            = 65,

    // Various
    EVENT_CHECK_IN_ROOM             = 66
};

enum EventGroups
{
    EVENT_GROUP_VILE_SPIRITS    = 1,
};

enum Phases
{
    PHASE_INTRO                 = 1,
    PHASE_ONE                   = 2,
    PHASE_TWO                   = 3,
    PHASE_THREE                 = 4,
    PHASE_TRANSITION            = 5,
    PHASE_FROSTMOURNE           = 6,    // only set on heroic mode when all players are sent into frostmourne
    PHASE_OUTRO                 = 7,

    PHASE_MASK_INTRO            = 1 << (PHASE_INTRO - 1),
    PHASE_MASK_ONE              = 1 << (PHASE_ONE - 1),
    PHASE_MASK_TWO              = 1 << (PHASE_TWO - 1),
    PHASE_MASK_THREE            = 1 << (PHASE_THREE - 1),
    PHASE_MASK_TRANSITION       = 1 << (PHASE_TRANSITION - 1),
    PHASE_MASK_FROSTMOURNE      = 1 << (PHASE_FROSTMOURNE - 1),
    PHASE_MASK_OUTRO            = 1 << (PHASE_OUTRO - 1),
    PHASE_MASK_NO_CAST_CHECK    = PHASE_MASK_TRANSITION | PHASE_MASK_FROSTMOURNE | PHASE_MASK_OUTRO,
    PHASE_MASK_NO_VICTIM        = PHASE_MASK_INTRO | PHASE_MASK_OUTRO | PHASE_MASK_FROSTMOURNE,
};

#define PHASE_TWO_THREE  (events.GetPhaseMask() & PHASE_MASK_TWO ? PHASE_TWO : PHASE_THREE)

Position const CenterPosition     = {503.6282f, -2124.655f, 840.8569f, 0.0f};
Position const TirionIntro        = {489.2970f, -2124.840f, 840.8569f, 0.0f};
Position const TirionCharge       = {482.9019f, -2124.479f, 840.8570f, 0.0f};
Position const LichKingIntro[3]   =
{
    {432.0851f, -2123.673f, 864.6582f, 0.0f},
    {457.8351f, -2123.423f, 841.1582f, 0.0f},
    {465.0730f, -2123.470f, 840.8569f, 0.0f},
};
Position const OutroPosition1     = {493.6286f, -2124.569f, 840.8569f, 0.0f};
Position const OutroFlying        = {508.9897f, -2124.561f, 845.3565f, 0.0f};

enum MovePoints
{
    POINT_CENTER_1          = 1,
    POINT_CENTER_2          = 2,
    POINT_TIRION_INTRO      = 3,
    POINT_LK_INTRO_1        = 4,
    POINT_LK_INTRO_2        = 5,
    POINT_LK_INTRO_3        = 6,
    POINT_TIRION_CHARGE     = 7,
    POINT_DROP_PLAYER       = 8,
    POINT_LK_OUTRO_1        = 9,
    POINT_TIRION_OUTRO_1    = 10,
    POINT_OUTRO_JUMP        = 11,
    POINT_LK_OUTRO_2        = 12,
    POINT_GROUND            = 13,
    POINT_VALKYR            = 20,
    POINT_CHARGE            = 1003, // globally used number for charge spell effects
};

enum EncounterActions
{
    ACTION_START_ENCOUNTER          = 0,
    ACTION_CONTINUE_INTRO           = 1,
    ACTION_START_ATTACK             = 2,
    ACTION_OUTRO                    = 3,
    ACTION_PLAY_MUSIC               = 4,
    ACTION_BREAK_FROSTMOURNE        = 5,
    ACTION_SUMMON_TERENAS           = 6,
    ACTION_FINISH_OUTRO             = 7,
    ACTION_TELEPORT_BACK            = 8,

    ACTION_RAGINGSPIRIT_FROSTMOURNE = 9,
};

enum MiscData
{
    LIGHT_SNOWSTORM             = 2490,
    LIGHT_SOULSTORM             = 2508,

    MUSIC_FROZEN_THRONE         = 17457,
    MUSIC_SPECIAL               = 17458,    // Summon Shambling Horror, Remorseless Winter, Quake, Summon Val'kyr Periodic, Harvest Soul, Vile Spirits
    MUSIC_FURY_OF_FROSTMOURNE   = 17459,
    MUSIC_FINAL                 = 17460,    // Raise Dead, Light's Blessing

    SOUND_PAIN                  = 17360,    // separate sound, not attached to any text

    EQUIP_ASHBRINGER_GLOWING    = 50442,
    EQUIP_BROKEN_FROSTMOURNE    = 50840,

    MOVIE_FALL_OF_THE_LICH_KING = 16,
};

enum Data
{
    DATA_PLAGUE_STACK           = 70337,
    DATA_VILE                   = 45814622,

    DATA_IS_FROSTMOURNE_PHASE   = 0,
    DATA_ENCOUNTER_DIFFICULTY   = 1
};

class NecroticPlagueTargetCheck : public std::unary_function<Unit*, bool>
{
    public:
        NecroticPlagueTargetCheck(Unit const* obj, uint32 notAura1 = 0, uint32 notAura2 = 0)
            : _sourceObj(obj), _notAura1(notAura1), _notAura2(notAura2)
        {
        }

        bool operator()(Unit* unit) const
        {
            if (!unit || unit == _sourceObj || !unit->isTargetableForAttack()
                || unit->HasAura(SPELL_PLAGUE_AVOIDANCE)
                || (unit->GetOwner() && unit->GetOwner()->GetTypeId() == TYPEID_PLAYER))
                return false;
            if ((_notAura1 && unit->HasAura(_notAura1)) || (_notAura2 && unit->HasAura(_notAura2)))
                return false;
            return true;
        }

    private:
        Unit const* _sourceObj;
        uint32 _notAura1;
        uint32 _notAura2;
};

class HeightDifferenceCheck
{
    public:
        HeightDifferenceCheck(GameObject* go, float diff, bool reverse)
            : _baseObject(go), _difference(diff), _reverse(reverse)
        {
        }

        bool operator()(WorldObject* unit) const
        {
            return (unit->GetPositionZ() - _baseObject->GetPositionZ() > _difference) != _reverse;
        }

    private:
        GameObject* _baseObject;
        float _difference;
        bool _reverse;
};

class FrozenThroneResetWorker
{
    public:
        FrozenThroneResetWorker() { }

        bool operator()(GameObject* go)
        {
            switch (go->GetEntry())
            {
                case GO_ARTHAS_PLATFORM:
                    go->SetDestructibleState(GO_DESTRUCTIBLE_REBUILDING);
                    break;
                case GO_DOODAD_ICECROWN_THRONEFROSTYWIND01:
                    go->SetGoState(GO_STATE_ACTIVE);
                    break;
                case GO_DOODAD_ICECROWN_THRONEFROSTYEDGE01:
                    go->SetGoState(GO_STATE_READY);
                    break;
                case GO_DOODAD_ICESHARD_STANDING02:
                case GO_DOODAD_ICESHARD_STANDING01:
                case GO_DOODAD_ICESHARD_STANDING03:
                case GO_DOODAD_ICESHARD_STANDING04:
                    go->ResetDoorOrButton();
                    break;
                default:
                    break;
            }

            return false;
        }
};

class StartMovementEvent : public BasicEvent
{
    public:
        StartMovementEvent(Creature* owner)
            : _owner(owner)
        {
        }

        bool Execute(uint64 /*time*/, uint32 /*diff*/)
        {
            _owner->SetReactState(REACT_AGGRESSIVE);
            if (Unit* target = _owner->AI()->SelectTarget(SELECT_TARGET_TOPAGGRO, 1))
                _owner->AI()->AttackStart(target);
            return true;
        }

    private:
        Creature* _owner;
};

class StartIceSphereEvent : public BasicEvent
{
    public:
        StartIceSphereEvent(Creature* owner, Unit* target)
            : _owner(owner), _target(target)
        {
        }

        bool Execute(uint64 /*time*/, uint32 /*diff*/)
        {
            if (!_target->isAlive())
            {
                _target = NULL;
                _target = _owner->SelectNearestPlayer(100.0f);
            }
            if (_target)
            {
                _owner->CastSpell(_owner, SPELL_ICE_SPHERE, false);
                _owner->CastSpell(_owner, SPELL_ICE_BURST_TARGET_SEARCH, false);
                _owner->CastSpell(_target, SPELL_ICE_PULSE, false);
                if (Aura* pAura = _owner->AddAura(SPELL_DECREASE_SPEED, _owner))
                    pAura->SetDuration(-1);

                _owner->ClearUnitState(UNIT_STATE_CASTING);
                _owner->GetMotionMaster()->MoveFollow(_target, 0, 0);
            }
            else
                _owner->DespawnOrUnsummon();

            return true;
        }

    private:
        Creature* _owner;
        Unit* _target;
};

class SetAggressiveEvent : public BasicEvent
{
    public:
        SetAggressiveEvent(Creature* owner)
            : _owner(owner)
        {
        }

        bool Execute(uint64 /*time*/, uint32 /*diff*/)
        {
            _owner->SetReactState(REACT_AGGRESSIVE);
            return true;
        }

    private:
        Creature* _owner;
};

class VileSpiritActivateEvent : public BasicEvent
{
    public:
        explicit VileSpiritActivateEvent(Creature* owner)
            : _owner(owner)
        {
        }

        bool Execute(uint64 /*time*/, uint32 /*diff*/)
        {
            _owner->SetReactState(REACT_AGGRESSIVE);
            _owner->CastSpell(_owner, SPELL_VILE_SPIRIT_MOVE_SEARCH, true);
            _owner->CastSpell((Unit*)NULL, SPELL_VILE_SPIRIT_DAMAGE_SEARCH, true);
            return true;
        }

    private:
        Creature* _owner;
};

class TriggerWickedSpirit : public BasicEvent
{
    public:
        explicit TriggerWickedSpirit(Creature* owner)
            : _owner(owner)
        {
            _is25ManRaid = _owner->GetMap()->Is25ManRaid();

            if (_is25ManRaid)
            {
                _counter = 29; // 29 ghost will come down / 1500ms
                _timer = 1500;
            }
            else
            {
                _counter = 14; // 14 ghost will come down / 3000ms
                _timer = 3000;
            }
        }

        bool Execute(uint64 /*time*/, uint32 /*diff*/)
        {
            _owner->CastSpell(_owner, SPELL_TRIGGER_VILE_SPIRIT_HEROIC);

            if (--_counter)
            {
                _owner->m_Events.AddEvent(this, _owner->m_Events.CalculateTime(_timer));
                return false;
            }

            return true;
        }

    private:
        Creature* _owner;
        uint32 _counter;
        uint32 _timer;
        bool _is25ManRaid;
};

class WickedSpiritResetEvent : public BasicEvent
{
    public:
    explicit WickedSpiritResetEvent(Creature* owner)
        : _owner(owner)
    {
    }

    bool Execute(uint64 /*time*/, uint32 /*diff*/)
    {
        std::list<Creature*> spirits;
        _owner->GetCreatureListWithEntryInGrid(spirits, NPC_WICKED_SPIRIT, 200.0f);
        for (std::list<Creature*>::iterator itr = spirits.begin(); itr != spirits.end(); ++itr)
        {
            (*itr)->m_Events.KillAllEvents(true);
            (*itr)->Kill((*itr));
            (*itr)->SetPosition((*itr)->GetHomePosition());
            (*itr)->Respawn();
        }

        std::list<Creature*> bombs;
        _owner->GetCreatureListWithEntryInGrid(bombs, NPC_SPIRIT_BOMB, 200.0f);
        for (std::list<Creature*>::iterator itr = bombs.begin(); itr != bombs.end(); ++itr)
            (*itr)->DespawnOrUnsummon(500);

        return true;
    }

    private:
    Creature* _owner;
};


#endif