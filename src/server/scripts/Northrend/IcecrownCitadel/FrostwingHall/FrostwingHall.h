#include "../IcecrownCitadel.h"

#define SINDRAGOSA_GAUNTLET_SPAWN_MAX 8
#define SINDRAGOSA_GAUNTLET_FROSTWARDEN_MAX 6
const Position SindragosaGauntletSpawn[SINDRAGOSA_GAUNTLET_SPAWN_MAX] =
{
    { 4137.93f, 2505.52f, 310.033f, 0 },
    { 4160.64f, 2528.13f, 310.033f, 0 },
    { 4200.92f, 2527.18f, 310.033f, 0 },
    { 4222.23f, 2503.56f, 310.033f, 0 },
    { 4222.01f, 2464.93f, 310.033f, 0 },
    { 4201.55f, 2441.03f, 310.033f, 0 },
    { 4161.86f, 2441.94f, 310.033f, 0 },
    { 4138.78f, 2463.95f, 310.033f, 0 },
};

const Position SindragosaGauntletFrostwarden[SINDRAGOSA_GAUNTLET_FROSTWARDEN_MAX] =
{
    { 4181.06f, 2542.26f, 211.03f, 4.75409f }, //Caster
    { 4175.40f, 2542.26f, 211.03f, 4.75409f }, //Warrior
    { 4186.72f, 2542.26f, 211.03f, 4.75409f }, //Warrior
    { 4181.06f, 2426.30f, 211.03f, 1.58178f }, //Caster
    { 4175.40f, 2426.30f, 211.03f, 1.58178f }, //Warrior
    { 4186.72f, 2426.30f, 211.03f, 1.58178f }, //Warrior
};

/*
 * 8 Spinnen
 *
 * 10Sec
 * kleine Spinnen
 * 3*8
 * abstand 3s
 *
 * 15sec später
 * kleine Spinnen
 * 3*8
 * abstand 3s
 *
 * 5sec später
 * NPCs
 * 2*3
 * 1mage
 *
 * Nach 5 sec
 * kleine Spinnen
 * 3*8
 * abstand 1s
 *
 * Pause bis NPCs tod
 *
 * nach 5sec
 *
 * echt viele viecher
 * 4charger 4normale
 *
 * nach 0.5s
 * kleine spinnen
 * 1*4
 *
 * nach 8s
 * binnen 30s
 * 15 spinnen
 * random spawn
 *
 * alle 5s eine kleine spinne bis große tod
 * 
 * alle 2s eine kleine spinne bis 10s nach große tod
 *
 * DONE
 * 
 */

enum vrykul_events
{
    // frostbinder
    EVENT_ARCTIC_CHILL = 1,
    EVENT_FROZEN_ORB = 2,
};

enum vrykul_spells
{
    // frostbinder
    SPELL_ARCTIC_CHILL = 71270,
    SPELL_FROZEN_ORB = 71270,
};