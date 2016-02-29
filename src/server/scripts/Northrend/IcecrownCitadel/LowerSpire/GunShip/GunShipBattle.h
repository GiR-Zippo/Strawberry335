#ifndef __GUNSHIPBATTLE_H_
#define __GUNSHIPBATTLE_H_

#include "../LowerSpire.h"
#include "MapManager.h"
#include "Transport.h"
#include "Vehicle.h"
#include "Group.h"

enum eAchievements
{
    IM_ON_A_BOAT_10 = 4536,
    IM_ON_A_BOAT_25 = 4612
};

enum Actions
{
    ACTION_INTRO_START = 1,
    ACTION_BATTLE_EVENT = 2,
    ACTION_MAGE_DIE = 3,
    ACTION_ROCK_DIE = 4,
    ACTION_AXES_RIFL_DIE = 5,
    ACTION_DONE = 6,
    ACTION_FAIL = 7,
};

enum Spells
{
    SPELL_BURNING_PITCH_A = 71335,
    SPELL_BURNING_PITCH_H = 71339,
    SPELL_BURNING_PITCH_SIEGE_DMG_A = 70383,
    SPELL_BURNING_PITCH_SIEGE_DMG_H = 70374,
    SPELL_BURNING_PITCH_AOE_DAMAGE = 69660,
    
    SPELL_WOUNDING_STRIKE_10_NM = 69651,
    SPELL_WOUNDING_STRIKE_10_HM = 72570,
    SPELL_WOUNDING_STRIKE_25_NM = 72569,
    SPELL_WOUNDING_STRIKE_25_HM = 72571,
    
    SPELL_TELEPORT_VISUAL = 64446,
    SPELL_BLADESTORM = 69652,
    SPELL_BLADESTORM_TRIGGER = 69653,
    
    // Cannon
    SPELL_HEAT_DRAIN = 69470,
    SPELL_OVERHEAT = 69487, // Triggers spell #69488 every 0.25s. It should consume 10 Energy but does not.
    SPELL_CANNON_BLAST = 69399,
    SPELL_INCINERATING_BLAST = 69401,
    
    // Auras
    SPELL_ON_ORGRIMS_HAMMERS_DECK = 70121,
    SPELL_ON_SKYBREAKERS_DECK = 70120,
    
    // Achievement spell required target
    SPELL_ACHIEVEMENT = 72959,
    
    // Rampart of Skulls NPCs Spells
    // Kor'kron Primalist
    SPELL_WRATH = 69968,
    SPELL_HEALING_TOUCH = 69899,
    SPELL_REGROWTH = 69882,
    SPELL_REJUVENATION = 69898,
    
    SPELL_SUMMON_BATTLE_STANDART_A = 69810,
    SPELL_SUMMON_BATTLE_STANDART_H = 69811,
    
    // Kor'kron Defender
    SPELL_DEVASTATE = 69902,
    SPELL_THUNDERCLAP = 69965,
    SPELL_SUNDER_ARMOR = 57807,
    SPELL_SPELL_REFLECT = 69901,
    
    // Skybreaker Vindicator
    SPELL_AVENGERS_SHIELD = 69927,
    SPELL_CONCENTRATION = 69930,
    SPELL_HOLY_WRATH = 69934,
    
    // Muradin Bronzebeard / High Overlord Saurfang
    SPELL_CLEAVE = 15284,
    SPELL_RENDING_THROW = 70309,
    SPELL_TASTE_OF_BLOOD = 69634,
    
    // Kor'kron Battle-mage & Skybreaker Sorcerer
    SPELL_BELOW_ZERO = 69705,
    SPELL_SHADOW_CHANNELING = 45104,
    
    // Experience spells
    SPELL_EXPERIENCED = 71188,
    SPELL_VETERAN = 71193,
    SPELL_ELITE = 71195,
    SPELL_DESPERATE_RESOLVE_10_NM = 69647,
    SPELL_DESPERATE_RESOLVE_10_HM = 72537,
    SPELL_DESPERATE_RESOLVE_25_NM = 72536,
    SPELL_DESPERATE_RESOLVE_25_HM = 72538,
    
    // Kor'kron Axethrower & Skybreaker Rifleman
    SPELL_HURL_AXE = 70161,
    SPELL_SHOOT = 70162,
    
    // Kor'kron Rocketeer & Skybreaker Mortar Soldier
    SPELL_ROCKET_ARTILLERY_MARKER = 71371,
    SPELL_ROCKET_ARTILLERY_TRIGGERED = 69679,
    SPELL_ROCKET_ARTILLERY_HORDE = 69678,
    SPELL_ROCKET_ARTILLERY_ALLIANCE = 70609,
    SPELL_EXPLOSION = 69680,
    
    // Ship Explsion
    SPELL_SHIP_EXPLOSION = 72137,
    
    // Remove Rocket Packs
    SPELL_REMOVE_ROCKET_PACK = 70713,
    
    // Achievements
    SPELL_ACHIEVEMENT_CHECK = 72959,
};

enum Events
{
    EVENT_NULL,
    EVENT_INTRO_ALLIANCE_1, // Muradin Bronzebeard yells: Fire up the engines! We got a meetin' with destiny, lads!
    EVENT_INTRO_ALLIANCE_2, // Muradin Bronzebeard yells: Hold on to yer hats!
    EVENT_INTRO_ALLIANCE_3, // Muradin Bronzebeard yells: What in the world is that? Grab me spyglass, crewman!
    EVENT_INTRO_ALLIANCE_4, // Muradin Bronzebeard yells: By me own beard! HORDE SAILIN' IN FAST 'N HOT!
    EVENT_INTRO_ALLIANCE_5, // Muradin Bronzebeard yells: EVASIVE ACTION! MAN THE GUNS!
    EVENT_INTRO_ALLIANCE_6, // Muradin Bronzebeard yells: Cowardly dogs! Ye blindsided us!
    EVENT_INTRO_ALLIANCE_7, // High Overlord Saurfang yells: This is not your battle, dwarf. Back down or we will be forced to destroy your ship.
    EVENT_INTRO_ALLIANCE_8, // Muradin Bronzebeard yells: Not me battle? I dunnae who ye think ye are, mister, but I got a score to settle with Arthas and yer not gettin' in me way! FIRE ALL GUNS! FIRE! FIRE!
    
    EVENT_INTRO_HORDE_1, // High Overlord Saurfang yells: Rise up, sons and daughters of the Horde! Today we battle a hated enemy of the Horde! LOK'TAR OGAR! Kor'kron, take us out!
    EVENT_INTRO_HORDE_1_1, // Kor'kron, take us out!
    EVENT_INTRO_HORDE_2, // High Overlord Saurfang yells: What is that?! Something approaching in the distance!
    EVENT_INTRO_HORDE_3, // High Overlord Saurfang yells: ALLIANCE GUNSHIP! ALL HANDS ON DECK!
    EVENT_INTRO_HORDE_4, // Muradin Bronzebeard yells: Move yer jalopy or we'll blow it out of the sky, orc! The Horde's got no business here!
    EVENT_INTRO_HORDE_5, // High Overlord Saurfang yells: You will know our business soon! KOR'KRON, ANNIHILATE THEM!
    
    EVENT_OUTRO_ALLIANCE_1,
    EVENT_OUTRO_ALLIANCE_2,
    EVENT_OUTRO_ALLIANCE_3,
    
    EVENT_OUTRO_HORDE_1,
    EVENT_OUTRO_HORDE_2,
    EVENT_OUTRO_HORDE_3,
    
    //Fly
    EVENT_START_FLY,
    
    //Misc battle
    EVENT_WALK_MOBS,
    EVENT_SUMMON_PORTAL,
    EVENT_FREEZE_CANNON,
    EVENT_SHADOW_CHANNELING,
    EVENT_UNSUMMON_PORTAL,
    EVENT_BOARDING_GUNSHIP,
    EVENT_BOARDING_TALK,
    EVENT_BURNING_PITCH,
    EVENT_WOUNDING_STRIKE,
    EVENT_BLADE_STORM,
    EVENT_DONE,
    EVENT_FAIL,
    EVENT_RESTART_EVENT,
    EVENT_BOARDING_REAVERS_MARINE,
    EVENT_WIPE_CHECK,
    
    // Rampart of Skulls NPCs Events
    EVENT_WRATH,
    EVENT_HEAL,
    
    EVENT_SUNDER_ARMOR,
    EVENT_SPELL_REFLECT,
    EVENT_THUNDERCLAP,
    EVENT_DEVASTATE,
    
    // Skybreaker Vindicator
    EVENT_AVENGERS_SHILED,
    EVENT_CONCENTRATION,
    EVENT_HOLY_WRATH,
    
    // First Squad Assisted
    EVENT_FIRST_SQUAD_ASSISTED_1,
    EVENT_FIRST_SQUAD_ASSISTED_2,
    
    // Second Squad Assisted
    EVENT_SECOND_SQUAD_ASSISTED_1,
    EVENT_SECOND_SQUAD_ASSISTED_2,
    EVENT_SECOND_SQUAD_ASSISTED_3,
    
    // Shared experience events
    EVENT_EXPERIENCED,
    EVENT_VETERAN,
    EVENT_ELITE,
    EVENT_ATACK_START,
    
    // Kor'kron Axethrower & Skybreaker Rifleman
    EVENT_HURL_AXE,
    EVENT_SHOOT,
    
    // Kor'kron Rocketeer & Skybreaker Mortar Soldier
    EVENT_ROCKET_ART,
    
    // Muradin Bronzebeard & High Overlord Saurfang
    EVENT_RENDING_THROW,
    EVENT_TASTE_OF_BLOOD,
    
    // Misc (used in various NPCs)
    EVENT_SPAWN_MAGE,
    EVENT_RESPAWN_AXES_RIFLEMEN,
    EVENT_RESPAWN_ROCKETEER,
};

enum Texts
{
    // Kor'kron Primalist
    SAY_FIRST_SQUAD_RESCUED_HORDE_0 = 0,
    
    // Kor'kron Invoker
    SAY_FIRST_SQUAD_RESCUED_HORDE_1 = 0,
    
    // Kor'kron Defender
    SAY_SECOND_SQUAD_RESCUED_HORDE_0 = 0,
    SAY_SECOND_SQUAD_RESCUED_HORDE_1 = 1,
    SAY_FROSTWYRM_SUMMON_0 = 2,
    SAY_FROSTWYRM_SUMMON_1 = 3,
    
    // Skybreaker Vindicator
    SAY_FIRST_SQUAD_RESCUED_ALLIANCE_0 = 0,
    
    // Skybreaker Sorcerer
    SAY_FIRST_SQUAD_RESCUED_ALLIANCE_1 = 0,
    
    // Skybreaker Protector
    SAY_SECOND_SQUAD_RESCUED_ALLIANCE_0 = 0,
    SAY_SECOND_SQUAD_RESCUED_ALLIANCE_1 = 1,
    SAY_SECOND_SQUAD_RESCUED_ALLIANCE_2 = 2,
    
    // Kor'kron Invoker & Skybreaker Sorcerer
    SAY_SUMMON_BATTLE_STANDARD = 1,
    
    // Muradin Bronzebeard
    SAY_INTRO_ALLIANCE_0 = 0,
    SAY_INTRO_ALLIANCE_1 = 1,
    SAY_INTRO_ALLIANCE_2 = 2,
    SAY_INTRO_ALLIANCE_3 = 3,
    SAY_INTRO_ALLIANCE_4 = 4,
    SAY_INTRO_ALLIANCE_5 = 5,
    SAY_INTRO_ALLIANCE_7 = 6,
    SAY_INTRO_HORDE_3 = 7,
    SAY_BOARDING_SKYBREAKER_1 = 8,
    SAY_BOARDING_ORGRIMS_HAMMER_0 = 9,
    SAY_NEW_RIFLEMEN_SPAWNED = 10,
    SAY_NEW_MORTAR_TEAM_SPAWNED = 11,
    SAY_NEW_MAGE_SPAWNED = 12,
    SAY_ALLIANCE_VICTORY = 13,
    SAY_ALLIANCE_DEFEAT = 14, // How will we handle that case ? Ie. the player loses
    
    // High Overlord Saurfang
    SAY_INTRO_HORDE_0 = 0,
    SAY_INTRO_HORDE_0_1 = 1,
    SAY_INTRO_HORDE_1 = 2,
    SAY_INTRO_HORDE_2 = 3,
    SAY_INTRO_HORDE_4 = 4,
    SAY_BOARDING_SKYBREAKER_0 = 5,
    SAY_BOARDING_ORGRIMS_HAMMER_1 = 6,
    SAY_NEW_AXETHROWER_SPAWNED = 7,
    SAY_NEW_ROCKETEERS_SPAWNED = 8,
    SAY_NEW_BATTLE_MAGE_SPAWNED = 9,
    SAY_HORDE_VICTORY = 10,
    SAY_HORDE_DEFEAT = 11,
    
    SAY_HIGH_OVERLORD_SAURFANG_NOT_VISUAL = 0,
    SAY_BOARDING_SKYBREAKER_SAURFANG = 1,
    SAY_MURADIN_BRONZEBEARD_NOT_VISUAL = 0,
    SAY_BOARDING_SKYBREAKER_MURADIN = 1,
};

struct mortarMarksLoc
{
    uint32 durationBeforeRefreshing;
    Position location;
};

typedef std::list<Player*> TPlayerLists;

TPlayerLists GetPlayersInTheMaps(Map *pMap);
Player* SelectRandomPlayerFromLists(TPlayerLists &players);
Player* SelectRandomPlayerInTheMaps(Map* pMap);
void StartFlyShip(Transport* t);
void StopFlyShip(Transport* t);
void UpdateTransportMotionInMap(Transport* t);
void RelocateTransport(Transport* t);
Transport* CheckUnfriendlyShip(Creature* me, InstanceScript* instance, uint32 data);
void TeleportPlayers(Map* map, uint64 TeamInInstance);
void DoShipExplosion(Transport* t);
bool DoWipeCheck(Transport* t);
void DoCheckFallingPlayer(Creature* me);
void RestartEvent(Transport* t1, Transport* t2, Map* instance, uint64 TeamInInstance);
void StopFight(Transport* t1, Transport* t2);

#endif