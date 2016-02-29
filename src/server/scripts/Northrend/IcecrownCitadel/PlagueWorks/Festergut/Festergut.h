#ifndef __FESTERGUT_H
#define __FESTERGUT_H

#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellAuras.h"
#include "../PlagueWorks.h"

enum ScriptTexts
{
    SAY_AGGRO                   = 1,
    EMOTE_GAS_SPORE             = 2,
    EMOTE_WARN_GAS_SPORE        = 3,
    SAY_PUNGENT_BLIGHT          = 4,
    EMOTE_WARN_PUNGENT_BLIGHT   = 5,
    EMOTE_PUNGENT_BLIGHT        = 6,
    SAY_KILL                    = 7,
    SAY_BERSERK                 = 8,
    SAY_DEATH                   = 9,
};

enum Spells
{
    // Festergut
    SPELL_SPORES                = 69279,
    SPELL_INHALE_BLIGHT         = 69165,
    SPELL_GASTRIC_EXPLOSION     = 72227,
    SPELL_BLIGHTED_SPORE        = 69290,
    SPELL_INOCULATED            = 69291,
};
// Used for HasAura checks
#define GAS_SPORE_HELPER RAID_MODE<uint32>(69278, 71221, 69278, 71221)
#define VILE_GAS_HELPER RAID_MODE<uint32>(69240, 71218, 73019, 73020)
#define PUNGENT_BLIGHT_HELPER RAID_MODE<uint32>(69195, 71219, 73031, 73032)
#define INOCULATED_HELPER     RAID_MODE<uint32>(69291, 72101, 72102, 72103)
#define GASTRIC_BLOAT_HELPER     RAID_MODE<uint32>(72219, 72551, 72552, 72553) // 72214 is the proper way (with proc) but atm procs can't have cooldown for creatures

uint32 const gaseousBlight[3]        = {69157, 69162, 69164};
uint32 const gaseousBlightVisual[3]  = {69126, 69152, 69154};

enum Events
{
    EVENT_BERSERK       = 1,
    EVENT_INHALE_BLIGHT = 2,
    EVENT_VILE_GAS      = 3,
    EVENT_GAS_SPORE     = 4,
    EVENT_GASTRIC_BLOAT = 5,
};

#define DATA_INOCULATED_STACK 69291

#endif