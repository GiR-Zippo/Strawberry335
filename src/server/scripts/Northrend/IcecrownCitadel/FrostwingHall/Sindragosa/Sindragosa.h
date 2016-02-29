#ifndef __SINDRAGOSA_H_
#define __SINDRAGOSA_H_

#include "../FrostwingHall.h"

enum Texts
{
    SAY_AGGRO                           = 0, // You are fools to have come to this place! The icy winds of Northrend will consume your souls!
    SAY_UNCHAINED_MAGIC                 = 1, // Suffer, mortals, as your pathetic magic betrays you!
    EMOTE_WARN_BLISTERING_COLD          = 2, // %s prepares to unleash a wave of blistering cold!
    SAY_BLISTERING_COLD                 = 3, // Can you feel the cold hand of death upon your heart?
    SAY_RESPITE_FOR_A_TORMENTED_SOUL    = 4, // Aaah! It burns! What sorcery is this?!
    SAY_AIR_PHASE                       = 5, // Your incursion ends here! None shall survive!
    SAY_PHASE_2                         = 6, // Now feel my master's limitless power and despair!
    EMOTE_WARN_FROZEN_ORB               = 7, // %s fires a frozen orb towards $N!
    SAY_KILL                            = 8, // Perish!
                                             // A flaw of mortality...
    SAY_BERSERK                         = 9, // Enough! I tire of these games!
    SAY_DEATH                           = 10, // Free...at last...
};

enum Spells
{
    // Sindragosa
    SPELL_SINDRAGOSA_S_FURY     = 70608,
    SPELL_TANK_MARKER           = 71039,
    SPELL_FROST_AURA            = 70084,
    SPELL_PERMAEATING_CHILL     = 70109,
    SPELL_CLEAVE                = 19983,
    SPELL_TAIL_SMASH            = 71077,
    SPELL_FROST_BREATH_P1       = 69649,
    SPELL_FROST_BREATH_P2       = 73061,
    SPELL_UNCHAINED_MAGIC       = 69762,
    SPELL_BACKLASH              = 69770,
    SPELL_ICY_GRIP              = 70117,
    SPELL_ICY_GRIP_JUMP         = 70122,
    SPELL_BLISTERING_COLD       = 70123,
    SPELL_FROST_BEACON          = 70126,
    SPELL_ICE_TOMB_TARGET       = 69712,
    SPELL_ICE_TOMB_DUMMY        = 69675,
    SPELL_ICE_TOMB_UNTARGETABLE = 69700,
    SPELL_ICE_TOMB_DAMAGE       = 70157,
    SPELL_ASPHYXIATION          = 71665,
    SPELL_FROST_BOMB_TRIGGER    = 69846,
    SPELL_FROST_BOMB_VISUAL     = 70022,
    SPELL_BIRTH_NO_VISUAL       = 40031,
    SPELL_FROST_BOMB            = 69845,
    SPELL_MYSTIC_BUFFET         = 70128,

    // Spinestalker
    SPELL_BELLOWING_ROAR        = 36922,
    SPELL_CLEAVE_SPINESTALKER   = 40505,
    SPELL_TAIL_SWEEP            = 71370,

    // Rimefang
    SPELL_FROST_BREATH          = 71386,
    SPELL_FROST_AURA_RIMEFANG   = 71387,
    SPELL_ICY_BLAST             = 71376,
    SPELL_ICY_BLAST_AREA        = 71380,

    // Frostwarden Handler
    SPELL_FOCUS_FIRE            = 71350,
    SPELL_ORDER_WHELP           = 71357,
    SPELL_CONCUSSIVE_SHOCK      = 71337,
};

enum Events
{
    // Sindragosa
    EVENT_BERSERK                   = 1,
    EVENT_CLEAVE                    = 2,
    EVENT_TAIL_SMASH                = 3,
    EVENT_FROST_BREATH              = 4,
    EVENT_UNCHAINED_MAGIC           = 5,
    EVENT_ICY_GRIP                  = 6,
    EVENT_BLISTERING_COLD           = 7,
    EVENT_BLISTERING_COLD_YELL      = 8,
    EVENT_AIR_PHASE                 = 9,
    EVENT_ICE_TOMB                  = 10,
    EVENT_FROST_BOMB                = 11,
    EVENT_LAND                      = 12,
    EVENT_AIR_MOVEMENT              = 21,
    EVENT_THIRD_PHASE_CHECK         = 22,
    EVENT_AIR_MOVEMENT_FAR          = 23,
    EVENT_LAND_GROUND               = 24,
    EVENT_ASPHYXIATION              = 25,

    // Spinestalker
    EVENT_BELLOWING_ROAR            = 13,
    EVENT_CLEAVE_SPINESTALKER       = 14,
    EVENT_TAIL_SWEEP                = 15,

    // Rimefang
    EVENT_FROST_BREATH_RIMEFANG     = 16,
    EVENT_ICY_BLAST                 = 17,
    EVENT_ICY_BLAST_CAST            = 18,

    // Trash
    EVENT_FROSTWARDEN_ORDER_WHELP   = 19,
    EVENT_CONCUSSIVE_SHOCK          = 20,

    // event groups
    EVENT_GROUP_LAND_PHASE          = 1,
};

enum FrostwingData
{
    DATA_MYSTIC_BUFFET_STACK    = 0,
    DATA_FROSTWYRM_OWNER        = 1,
    DATA_WHELP_MARKER           = 2,
    DATA_LINKED_GAMEOBJECT      = 3,
    DATA_TRAPPED_PLAYER         = 4,
    DATA_FROST_BOMB_HIT         = 5,
};

enum MovementPoints
{
    POINT_FROSTWYRM_FLY_IN  = 1,
    POINT_FROSTWYRM_LAND    = 2,
    POINT_AIR_PHASE         = 3,
    POINT_TAKEOFF           = 4,
    POINT_LAND              = 5,
    POINT_AIR_PHASE_FAR     = 6,
    POINT_LAND_GROUND       = 7,
};

enum Shadowmourne
{
    QUEST_FROST_INFUSION        = 24757,
    ITEM_SHADOW_S_EDGE          = 49888,

    SPELL_FROST_INFUSION_CREDIT = 72289,
    SPELL_FROST_INFUSION        = 72292,
    SPELL_FROST_IMBUED_BLADE    = 72290,
};

Position const RimefangFlyPos      = {4413.309f, 2456.421f, 233.3795f, 2.890186f};
Position const RimefangLandPos     = {4413.309f, 2456.421f, 203.3848f, 2.890186f};
Position const SpinestalkerFlyPos  = {4418.895f, 2514.233f, 230.4864f, 3.396045f};
Position const SpinestalkerLandPos = {4418.895f, 2514.233f, 203.3848f, 3.396045f};

Position const SindragosaFlyPos    = {4475.190f, 2484.570f, 234.8510f, 3.141593f};
Position const SindragosaAirPos    = {4475.990f, 2484.430f, 247.9340f, 3.141593f};
Position const SindragosaAirPosFar = {4525.600f, 2485.150f, 245.0820f, 3.141593f};
Position const SindragosaFlyInPos  = {4419.190f, 2484.360f, 232.5150f, 3.141593f};

class FrostwyrmLandEvent : public BasicEvent
{
    public:
        FrostwyrmLandEvent(Creature& owner, Position const& dest) : _owner(owner), _dest(dest) { }

        bool Execute(uint64 /*eventTime*/, uint32 /*updateTime*/)
        {
            _owner.GetMotionMaster()->MoveLand(POINT_FROSTWYRM_LAND, _dest);
            return true;
        }

    private:
        Creature& _owner;
        Position const& _dest;
};

class FrostBombExplosion : public BasicEvent
{
    public:
        FrostBombExplosion(Creature* owner, uint64 sindragosaGUID) : _owner(owner), _sindragosaGUID(sindragosaGUID) { }

        bool Execute(uint64 /*eventTime*/, uint32 /*updateTime*/)
        {
            _owner->CastSpell((Unit*)NULL, SPELL_FROST_BOMB, false, NULL, NULL, _sindragosaGUID);
            _owner->RemoveAurasDueToSpell(SPELL_FROST_BOMB_VISUAL);
            if (Creature* sindragosa = ObjectAccessor::GetCreature(*_owner, _sindragosaGUID))
                sindragosa->AI()->SetData(DATA_FROST_BOMB_HIT, 1);
            return true;
        }

    private:
        Creature* _owner;
        uint64 _sindragosaGUID;
};

class UnchainedMagicTargetSelector
{
    public:
        UnchainedMagicTargetSelector() { }

        bool operator()(WorldObject* object) const
        {
            if (Unit* unit = object->ToUnit())
                return unit->getPowerType() != POWER_MANA;
            return true;
        }
};

class MysticBuffetTargetFilter
{
    public:
        explicit MysticBuffetTargetFilter(Unit* caster) : _caster(caster) { }

        bool operator()(WorldObject* unit) const
        {
            return !unit->IsWithinLOSInMap(_caster);
        }

    private:
        Unit* _caster;
};

class OrderWhelpTargetSelector
{
    public:
        explicit OrderWhelpTargetSelector(Creature* owner) : _owner(owner) { }

        bool operator()(Creature* creature)
        {
            if (!creature->AI()->GetData(DATA_WHELP_MARKER) && creature->AI()->GetData(DATA_FROSTWYRM_OWNER) == _owner->AI()->GetData(DATA_FROSTWYRM_OWNER))
                return false;
            return true;
        }

    private:
        Creature* _owner;
};

class FrostBombTargetSelector
{
    public:
        FrostBombTargetSelector(Unit* caster, std::list<Creature*> const& collisionList) : _caster(caster), _collisionList(collisionList) { }

        bool operator()(WorldObject* tar)
        {
            if (Unit* target = tar->ToUnit())
            {
                if (target->HasAura(SPELL_ICE_TOMB_DAMAGE))
                    return true;

                for (std::list<Creature*>::const_iterator itr = _collisionList.begin(); itr != _collisionList.end(); ++itr)
                    if ((*itr)->isAlive() && (*itr)->IsInBetween(_caster, target, 5.0f))
                        return true;
            }
            return false;
        }

    private:
        Unit* _caster;
        std::list<Creature*> const& _collisionList;
};

class FrostBeaconSelector
{
    public:
        FrostBeaconSelector(Unit* source) : _source(source) { }

        bool operator()(Unit* target) const
        {
            return target->GetTypeId() == TYPEID_PLAYER &&
                target != _source->getVictim() &&
                !target->HasAura(SPELL_ICE_TOMB_UNTARGETABLE);
        }

    private:
        Unit* _source;
};

#endif