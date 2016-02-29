#ifndef __PLAGUEWORKS_H_
#define __PLAGUEWORKS_H_

#include "../IcecrownCitadel.h"

enum PlagueWorksTexts
{
        SAY_PRECIOUS_DIES           = 0,
        EMOTE_PRECIOUS_ZOMBIES      = 0,
        SAY_STINKY_DEAD             = 0,
};

enum PlagueWorksEvents
{
    EVENT_DECIMATE          = 4,
    EVENT_MORTAL_WOUND      = 5,

    // Precious

    EVENT_SUMMON_ZOMBIES    = 6,
};

enum PlagueWorksSpells
{
    SPELL_MORTAL_WOUND                      = 71127,
    SPELL_DECIMATE                          = 71123,

    // Precious
    SPELL_AWAKEN_PLAGUED_ZOMBIES            = 71159,

    //Stinky
    SPELL_PLAGUE_STENCH         = 71805,
};

enum trapActions
{
    ACTION_START_EVENT      = 1,
};

#endif