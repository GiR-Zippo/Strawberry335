#ifndef __BLOODPRINCECOUNCIL_H_
#define __BLOODPRINCECOUNCIL_H_

#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellAuraEffects.h"
#include "../CrimsonHall.h"

enum Texts
{
    // Blood Queen Lana'Thel
    SAY_INTRO_1                 = 0,
    SAY_INTRO_2                 = 1,

    // Prince Keleseth
    SAY_KELESETH_INVOCATION     = 0,
    EMOTE_KELESETH_INVOCATION   = 1,
    SAY_KELESETH_SPECIAL        = 2,
    SAY_KELESETH_KILL           = 3,
    EMOTE_KELESETH_BERSERK      = 4,
    SAY_KELESETH_DEATH          = 5,

    // Prince Taldaram
    SAY_TALDARAM_INVOCATION     = 0,
    EMOTE_TALDARAM_INVOCATION   = 1,
    SAY_TALDARAM_SPECIAL        = 2,
    EMOTE_TALDARAM_FLAME        = 3,
    SAY_TALDARAM_KILL           = 4,
    EMOTE_TALDARAM_BERSERK      = 5,
    EMOTE_TALDARAM_DEATH        = 6,

    // Prince Valanar
    SAY_VALANAR_INVOCATION      = 0,
    EMOTE_VALANAR_INVOCATION    = 1,
    SAY_VALANAR_SPECIAL         = 2,
    EMOTE_VALANAR_SHOCK_VORTEX  = 3,
    SAY_VALANAR_KILL            = 4,
    SAY_VALANAR_BERSERK         = 5,
    SAY_VALANAR_DEATH           = 6,
};

enum Spells
{
    SPELL_FEIGN_DEATH                   = 71598,
    SPELL_OOC_INVOCATION_VISUAL         = 70934,
    SPELL_INVOCATION_VISUAL_ACTIVE      = 71596,

    // Heroic mode
    SPELL_SHADOW_PRISON                 = 72998,
    SPELL_SHADOW_PRISON_DAMAGE          = 72999,
    SPELL_SHADOW_PRISON_DUMMY           = 73001,

    // Prince Keleseth
    SPELL_INVOCATION_OF_BLOOD_KELESETH  = 70981,
    SPELL_SHADOW_RESONANCE              = 71943,
    SPELL_SHADOW_LANCE                  = 71405,
    SPELL_EMPOWERED_SHADOW_LANCE        = 71815,

    // Dark Nucleus
    SPELL_SHADOW_RESONANCE_AURA         = 72980,
    SPELL_SHADOW_RESONANCE_RESIST       = 71822,

    // Prince Taldaram
    SPELL_INVOCATION_OF_BLOOD_TALDARAM  = 70982,
    SPELL_GLITTERING_SPARKS             = 71806,
    SPELL_CONJURE_FLAME                 = 71718,
    SPELL_CONJURE_EMPOWERED_FLAME       = 72040,

    // Ball of Flame
    SPELL_FLAME_SPHERE_SPAWN_EFFECT     = 55891, // cast from creature_template_addon (needed cast before entering world)
    SPELL_BALL_OF_FLAMES_VISUAL         = 71706,
    SPELL_BALL_OF_FLAMES                = 71714,
    SPELL_FLAMES                        = 71393,
    SPELL_FLAME_SPHERE_DEATH_EFFECT     = 55947,

    // Ball of Inferno Flame
    SPELL_BALL_OF_FLAMES_PROC           = 71756,
    SPELL_BALL_OF_FLAMES_PERIODIC       = 71709,

    // Prince Valanar
    SPELL_INVOCATION_OF_BLOOD_VALANAR   = 70952,
    SPELL_KINETIC_BOMB_TARGET           = 72053,
    SPELL_KINETIC_BOMB                  = 72080,
    SPELL_SHOCK_VORTEX                  = 72037,
    SPELL_EMPOWERED_SHOCK_VORTEX        = 72039,

    // Kinetic Bomb
    SPELL_UNSTABLE                      = 72059,
    SPELL_KINETIC_BOMB_VISUAL           = 72054,
    SPELL_KINETIC_BOMB_EXPLOSION        = 72052,
    SPELL_KINETIC_BOMB_KNOCKBACK        = 72087,

    // Shock Vortex
    SPELL_SHOCK_VORTEX_PERIODIC         = 71945,
    SPELL_SHOCK_VORTEX_DUMMY            = 72633,
};

enum Events
{
    EVENT_INTRO_1               = 1,
    EVENT_INTRO_2               = 2,

    EVENT_INVOCATION_OF_BLOOD   = 3,
    EVENT_BERSERK               = 4,

    // Keleseth
    EVENT_SHADOW_RESONANCE      = 5,
    EVENT_SHADOW_LANCE          = 6,

    // Taldaram
    EVENT_GLITTERING_SPARKS     = 7,
    EVENT_CONJURE_FLAME         = 8,

    // Valanar
    EVENT_KINETIC_BOMB          = 9,
    EVENT_SHOCK_VORTEX          = 10,
    EVENT_BOMB_DESPAWN          = 11,
    // Valanar Shockvortex Activation
    EVENT_SHOCK_VORTEX_ACTIVATE = 12,
};

enum Actions
{
    ACTION_STAND_UP             = 1,
    ACTION_CAST_INVOCATION      = 2,
    ACTION_REMOVE_INVOCATION    = 3,
    ACTION_KINETIC_BOMB_UP      = 4,
    ACTION_KINETIC_BOMB_DOWN    = 5,
    ACTION_FLAME_BALL_CHASE     = 6,
};

enum Points
{
    POINT_INTRO_DESPAWN           = 380040,
    POINT_KINETIC_BOMB_IMPACT     = 384540,
    POINT_KINETIC_BOMB_MAX_HEIGHT = 384541,
};

enum Displays
{
    DISPLAY_KINETIC_BOMB        = 31095,
};

class StandUpEvent : public BasicEvent
{
    public:
        StandUpEvent(Creature& owner) : BasicEvent(), _owner(owner) { }

        bool Execute(uint64 /*eventTime*/, uint32 /*diff*/)
        {
            _owner.HandleEmoteCommand(EMOTE_ONESHOT_ROAR);
            return true;
        }

    private:
        Creature& _owner;
};

Position const introFinalPos = {4660.490f, 2769.200f, 430.0000f, 0.000000f};
Position const triggerPos    = {4680.231f, 2769.134f, 379.9256f, 3.121708f};
Position const triggerEndPos = {4680.180f, 2769.150f, 365.5000f, 3.121708f};

#endif