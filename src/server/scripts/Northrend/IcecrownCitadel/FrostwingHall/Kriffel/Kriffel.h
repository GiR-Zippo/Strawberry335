#include "../FrostwingHall.h"

#define GOSSIP_ITEM_LEADER    "Wir suchen eine neue Herausforderung"

enum Spells
{
    SPELL_LIGHT_ESSENCE             =   65712,
    SPELL_DARK_ESSENCE              =   65684,
    SPELL_VISUAL_EXPLODE            =   54942,
    SPELL_EXPLODE                   =   70509,
    SPELL_EXPLODE_DAMAGE            =   75876,
    SPELL_EXPLODE_BIG               =   63548,
    SPELL_KRACHEN                   =   37431,
    SPELL_BIG_PRE                   =   41986,
    SPELL_CAMERA_SHAKE              =   75033,
    SPELL_VISUAL_ENERGY             =   70499,
    SPELL_SHADOW_GLITTER            =   52582,
    SPELL_SHADOW_EXPLO              =   62922,
    SPELL_SHADOW_EXPLO2             =   65108,
    SPELL_SHADOW_EXPLO3             =   59128,
    SPELL_DARK_RECKONING            =   69483,
    SPELL_UNYIELDING_PAIN           =   57381,

    SPELL_FROSTEINSCHLAG            =   70123,
    SPELL_VISUAL_FROST              =   62462,
    SPELL_ARRIVAL                   =   64997,

    SPELL_AURA_OF_DESIRE            =   41350,
    SPELL_AURA_OF_DESIRE_DAMAGE     =   41352,

    SPELL_LEGION_FLAME              =   68125,
    SPELL_PACKT_OF_THE_DARKFALLEN   =   71340,
    SPELL_FROST_AURA                =   70084,

    SPELL_INCITE_TERROR             =   73070,
    SPELL_WINTER_CHANNEL            =   69780,
    SPELL_FIRE_CHANNEL              =   45576,
    SPELL_GET_BIGGER                =   25462,

    SPELL_SPIRAL                    =   72313,
    SPELL_VERDERBNIS_EXPLOSION      =   59477, /* alt 71268 */
    SPELL_VERDERBNIS_EXPLOSION_VIS  =   57555,

    SPELL_PFAEHLEN                  =   54378, /* alt 67478 */

    SPELL_GLITZERN                  =   51444,
    SPELL_FROSTEXPLOSION            =   71053,
    SPELL_PERMAFROST                =   68786,
    SPELL_WACHSEN                   =   74996,
    SPELL_BLUE_PILLAR               =   63773,
    SPELL_PRE_CHANNEL               =   64996,
    SPELL_VISUAL_RAGE               =   59208,
    SPELL_P2_ATSP                   =   19779,
};

#define CLEAVE_KRIFFEL     RAID_MODE<uint32>(40816, 43690, 40816, 43690)
#define P2_DOT             RAID_MODE<uint32>(72368, 72369, 72368, 72369)

enum Event
{
    // Kriffel
    EVENT_KRACHEN           =   1,
    EVENT_KRACHEN_2         =   2,
    EVENT_KRACHEN_3         =   3,
    EVENT_BIG_KRACHEN       =   4,
    EVENT_RESET             =   5,
    EVENT_MARK_ENERGY       =   6,
    EVENT_P2_DOT            =   7,
    EVENT_CHANGE_P2         =   8,
    EVENT_CHANGE_P1         =   11,
    EVENT_CHECK_TANK        =   12,
    EVENT_CHANGE_PHASE      =   13,
    // Add
    EVENT_EXPLODE           =   20,
    EVENT_EXPLODE_BIG       =   21,
    EVENT_IMPLODE_ENERGY    =   22,
    EVENT_EXPLODE_ENERGY    =   23,
    // Frost Event
    EVENT_ACTIVATE_VERDERBNIS   =   24,

    EVENT_INTRO_0           =   36,
    EVENT_INTRO_1           =   37,

    EVENT_PACKT             =   38,
    EVENT_FIRE              =   39,
    EVENT_PACKT_2           =   40,

    EVENT_FROST_EXPLODE     =   41,
    EVENT_BERSERK           =   42,

    EVENT_EXPLODE_ENERGY_PRE =  43,
    EVENT_EXPLODE_VERDERBNIS =  44,
    EVENT_PFAEHLEN          =   45,
    EVENT_STOP_GLITZERN     =   46,
    EVENT_FIRE_2            =   47,
    EVENT_FIRE_3            =   48,
    EVENT_GLITZERN          =   49,
    EVENT_ACTIVATE_FROSTORB =   50,
    EVENT_FROSTORB_FROST    =   51,
    EVENT_FROSTORB_DETO     =   52,
    EVENT_SUMMON_FROSTORB   =   53,
    EVENT_RECKONING         =   54,
};

enum Texts
{
    TEXT_AGGRO          = -1564039,
    TEXT_INTRO          = -1564037,
    TEXT_PHASE          = -1564045, 
    TEXT_DEATH          = -1564046, 
    TEXT_SLAY           = -1564043,
    TEXT_PHASE_1        = -1564038,
    TEXT_SPECIAL        = -1564044,
    TEXT_EMOTE_CLEAVE   = -1006003,
    TEXT_EMOTE_PHASE_1  = -1006004,
    TEXT_EMOTE_PHASE_2  = -1006005,
    TEXT_EMOTE_ENERGY   = -1006006,
    TEXT_EMOTE_KRACHEN  = -1006007,
    TEXT_EMOTE_START    = -1006008,
    TEXT_EMOTE_BERSERK  = -1006009,
    TEXT_ZERMALMEN      = -1006010,
};

enum Actions
{
    ACTION_EXPLODE_START    = 1,
    ACTION_EXPLODE_BIG      = 2,
    ACTION_APPEAR           = 3,
    ACTION_INTRO            = 4,
    ACTION_WARN_EXPLODE     = 5,
};

enum Phases
{
    PHASE_1                 =   1,
    PHASE_2                 =   2,
};

enum Groups
{
    EVENT_GROUP_P1          =   1,
    EVENT_GROUP_CHECK_TANK  =   2,
};

enum Creatures
{
    NPC_VISUAL_STORM    =   1006010,
    NPC_PRE_ENERGY      =   1006011,
    NPC_BOSS            =   1006003,
    NPC_VERDERBNIS      =   1006015,
    NPC_FROSTORB        =   1006020,
    NPC_FIRE            =   34784,
};

enum Gameobjects
{
    GOBJECT_RING    =   300304,
    GO_LOOT_10N_A   =   201872,
    GO_LOOT_10H_A   =   201874,
    GO_LOOT_25N_A   =   201873,
    GO_LOOT_25H_A   =   201875,

    GO_LOOT_10N_H   =   202177,
    GO_LOOT_10H_H   =   202179,
    GO_LOOT_25N_H   =   202178,
    GO_LOOT_25H_H   =   202180,
};

enum Points
{
    POINT_MID           =   1,
    POINT_PHASE_2       =   2,
    POINT_ROTATE        =   3,
    POINT_ROTATE_2      =   4,
};

const float dir[7] =
{
    2.78f, 
    3.72f, 
    4.67f, 
    5.51f,
    0.08f,
    1.00f,
    1.98f,
};

const uint32 creatureKrachen[7] =
{
    1006006,
    1006007,
    1006008,
    1006009,
    1006012,
    1006013,
    1006014,
};

const Position midPoint = {4181.3984f, 2484.4849f, 211.0332f, 3.14f};
const Position loot = {4212.91f, 2484.72f, 211.03f, 3.0f};
const Position energyPoints[6] =
{
    {4181.27f, 2506.54f, 211.03f, 4.72f},
    {4198.50f, 2499.95f, 211.03f, 3.90f},
    {4204.21f, 2482.54f, 211.03f, 3.90f},
    {4189.30f, 2463.69f, 211.03f, 3.90f},
    {4167.40f, 2465.39f, 211.03f, 3.90f},
    {4157.62f, 2487.22f, 211.03f, 3.90f},
};
