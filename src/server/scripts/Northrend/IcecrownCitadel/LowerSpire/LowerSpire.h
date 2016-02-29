#ifndef __LOWER_SPIRE_H
#define __LOWER_SPIRE_H


#include "../IcecrownCitadel.h"

enum LowerSpireActions
{
    ACTION_START_MOVE                = 1
};

enum LowerSpireSays
{
    // Froswyrm
    SAY_FROSTWYRM_LAND_H_0           = 0,
    SAY_FROSTWYRM_LAND_A_1           = 1,

    // Rotting Frost Giant
    EMOTE_DEATH_PLAGUE_WARNING       = 0,
};

enum LowerSpireEvents
{
    // Froswyrm
    EVENT_FROST_BREATH,
    EVENT_BLIZZARD,
    EVENT_CLEAVE,

    // Rotting Frost Giant
    EVENT_DEATH_PLAGUE,
    EVENT_STOMP,
    EVENT_ARCTIC_BREATH,
};

enum LowerSpireSpells
{
    // Frostwyrm
    SPELL_FROST_BREATH                = 70116,
    SPELL_BLIZZARD                    = 70362,
    SPELL_FROST_CLEAVE                = 70361,

    // Rotting Frost Giant
    SPELL_DEATH_PLAGUE                = 72879,
    SPELL_DEATH_PLAGUE_AURA           = 72865,
    SPELL_RECENTLY_INFECTED           = 72884,
    SPELL_DEATH_PLAGUE_KILL           = 72867,
    SPELL_STOMP                       = 64652,
    SPELL_ARCTIC_BREATH               = 72848,
};

#endif