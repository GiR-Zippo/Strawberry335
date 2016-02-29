#ifndef _TRIAL_CRUSADER_H
#define _TRIAL_CRUSADER_H

enum
{
    TYPE_ENCOUNTER_STATUS                   = 1,
    TYPE_ENCOUNTER_CURRENT                  = 2,
    TYPE_TRIAL_COUNTER                      = 3,
    TYPE_TEAM                               = 4,

    OPEN                                    = 1,
    CLOSE                                   = 2,

    DESPAWN_TIME                            = 300000,

    NPC_BARRETT                             = 34816,
    NPC_TIRION                              = 34996,
    NPC_FIZZLEBANG                          = 35458,
    NPC_GARROSH                             = 34995,
    NPC_VARIAN                              = 34990,
    NPC_LICH_KING_TOCR                      = 35877,

    NPC_THRALL                              = 34994,
    NPC_PROUDMOORE                          = 34992,
    NPC_WILFRED_PORTAL                      = 17965,
    NPC_TRIGGER                             = 35651,

    NPC_ICEHOWL                             = 34797,
    NPC_GORMOK                              = 34796,
    NPC_DREADSCALE                          = 34799,
    NPC_ACIDMAW                             = 35144,
    
    NPC_JARAXXUS                            = 34780,

    NPC_CHAMPIONS_CONTROLLER                = 34781,

    NPC_LIGHTBANE                           = 34497,
    NPC_DARKBANE                            = 34496,
    NPC_DARK_ESSENCE                        = 34567,
    NPC_LIGHT_ESSENCE                       = 34568,

    NPC_ANUBARAK                            = 34564,

    GO_CRUSADERS_CACHE_10                   = 195631,
    GO_CRUSADERS_CACHE_25                   = 195632,
    GO_CRUSADERS_CACHE_10_H                 = 195633,
    GO_CRUSADERS_CACHE_25_H                 = 195635,

    GO_ARGENT_COLISEUM_FLOOR                = 195527, //20943
    GO_MAIN_GATE_DOOR                       = 195647,
    GO_EAST_PORTCULLIS                      = 195648,
    GO_WEB_DOOR                             = 195485,

    // Tribute
    A_TRIBUTE_TO_SKILL_10_PLAYER            = 12344,
    A_TRIBUTE_TO_SKILL_25_PLAYER            = 12338,
    A_TRIBUTE_TO_MAD_SKILL_10_PLAYER        = 12347,
    A_TRIBUTE_TO_MAD_SKILL_25_PLAYER        = 12341,
    A_TRIBUTE_TO_INSANITY_10_PLAYER         = 12349,
    A_TRIBUTE_TO_INSANITY_25_PLAYER         = 12343,
    A_TRIBUTE_TO_IMMORTALITY_HORDE          = 12358,
    A_TRIBUTE_TO_IMMORTALITY_ALLIANCE       = 12359,
    A_TRIBUTE_TO_DEDICATED_INSANITY         = 12360,
    REALM_FIRST_GRAND_CRUSADER              = 12350,

    // Dummy spells - not existing in dbc but we don't need that
    SPELL_CHAMPIONS_KILLED_IN_MINUTE        = 68620,
    SPELL_DEFEAT_FACTION_CHAMPIONS          = 68184,
    SPELL_TRAITOR_KING_10                   = 68186,
    SPELL_TRAITOR_KING_25                   = 68515,

    // Timed events
    EVENT_START_TWINS_FIGHT                 = 21853
};

const Position TrialCrusaderLoc[] =
{
    {563.672974f, 139.571f, 393.837006f, 1.539f},   // Center of Map
};

#endif
