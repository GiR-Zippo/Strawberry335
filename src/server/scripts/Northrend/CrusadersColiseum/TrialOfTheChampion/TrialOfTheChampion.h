/**
 * @file
 * @author TehKevin for Laenalith-WoW
 * @date 24.08.2014
 *
 * Contact kevin.unegg@ainet.at
 * Website: http://www.laenalith-wow.com/
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @copyright Copyright 2014 Laenalith-WoW. All rights reserved.
 */

#ifndef _TRIAL_OF_THE_CHAMPION_H
#define _TRIAL_OF_THE_CHAMPION_H

#include "Encounter_TrialOfTheChampion.h"


#define GET_INSTANCESCRIPT_SAFE(x)  x = me->GetInstanceScript() ? me->GetInstanceScript() : NULL;
#define ENCOUNTER_VALUE_MAX         4

enum TrialOfTheChampion_Actions
{
    INSTANCE_ACTION_ENCOUNTER_DONE  = 1
};

enum TrialOfTheChampion_Data
{
    BOSS_GRAND_CHAMPIONS,
    BOSS_ARGENT_CHALLENGE,
    BOSS_BLACK_KNIGHT,

    DATA_TEAM,
    DATA_GRAND_CHAMPION_ENTRY,
    DATA_MEMORY_ENTRY,
    DATA_I_VE_HAD_WORSE,
    DATA_CHAMPION_DEFEATED,
    //DATA_GRAND_CHAMPION_GUID,

    DATA_CURRENT_ENCOUNTER
};

enum TrialOfTheChampion_Data64
{
    DATA64_CHEST = 0
};

enum TrialOfTheChampion_AchievementCriterias
{
    CRITERIA_CHAMPION_JACOB     = 11420,
    CRITERIA_CHAMPION_LANA      = 12298,
    CRITERIA_CHAMPION_COLOSOS   = 12299,
    CRITERIA_CHAMPION_AMBROSE   = 12300,
    CRITERIA_CHAMPION_JAELYNE   = 12301,

    CRITERIA_CHAMPION_MOKRA     = 12302,
    CRITERIA_CHAMPION_VISCERI   = 12303,
    CRITERIA_CHAMPION_RUNOK     = 12304,
    CRITERIA_CHAMPION_ERESSEA   = 12305,
    CRITERIA_CHAMPION_ZULTORE   = 12306,

    CRITERIA_CHAMPION_JACOB_H   = 12310,
    CRITERIA_CHAMPION_LANA_H    = 12311,
    CRITERIA_CHAMPION_COLOSOS_H = 12312,
    CRITERIA_CHAMPION_AMBROSE_H = 12313,
    CRITERIA_CHAMPION_JAELYNE_H = 12314,

    CRITERIA_CHAMPION_MOKRA_H   = 12318,
    CRITERIA_CHAMPION_VISCERI_H = 12319,
    CRITERIA_CHAMPION_RUNOK_H   = 12320,
    CRITERIA_CHAMPION_ERESSEA_H = 12321,
    CRITERIA_CHAMPION_ZULTORE_H = 12322,

    CRITERIA_MEMORY_HOGGER     = 11863,
    CRITERIA_MEMORY_VANCLEEF   = 11904,
    CRITERIA_MEMORY_MUTANUS    = 11905,
    CRITERIA_MEMORY_HEROD      = 11906,
    CRITERIA_MEMORY_LUCIFROM   = 11907,
    CRITERIA_MEMORY_THUNDERAAN = 11908,
    CRITERIA_MEMORY_CHROMAGGUS = 11909,
    CRITERIA_MEMORY_HAKKAR     = 11910,
    CRITERIA_MEMORY_VEKNILASH  = 11911,
    CRITERIA_MEMORY_KALITHRESH = 11912,
    CRITERIA_MEMORY_MALCHEZAAR = 11913,
    CRITERIA_MEMORY_GRUUL      = 11914,
    CRITERIA_MEMORY_VASHJ      = 11915,
    CRITERIA_MEMORY_ARCHIMONDE = 11916,
    CRITERIA_MEMORY_ILLIDAN    = 11917,
    CRITERIA_MEMORY_DELRISSA   = 11918,
    CRITERIA_MEMORY_MURU       = 11919,
    CRITERIA_MEMORY_INGVAR     = 11920,
    CRITERIA_MEMORY_CYANIGOSA  = 11921,
    CRITERIA_MEMORY_ECK        = 11922,
    CRITERIA_MEMORY_ONYXIA     = 11923,
    CRITERIA_MEMORY_HEIGAN     = 11924,
    CRITERIA_MEMORY_IGNIS      = 11925,
    CRITERIA_MEMORY_VEZAX      = 11926,
    CRITERIA_MEMORY_ALGALON    = 11927,

    CRITERIA_I_VE_HAD_WORSE    = 11789,

    SPELL_ACHIEVEMENT_CHAMPIONS = 68572,
    SPELL_ACHIEVEMENT_PALETRESS = 68574,
    SPELL_ACHIEVEMENT_MEMORIES  = 68206,
    SPELL_ACHIEVEMENT_EADRIC    = 68575,
    SPELL_ACHIEVEMENT_BLACK_KNIGHT = 68663,
};

enum TrialOfTheChampion_Npcs
{
    // Horde Champions
    NPC_MOKRA                   = 35572,
    NPC_ERESSEA                 = 35569,
    NPC_RUNOK                   = 35571,
    NPC_ZULTORE                 = 35570,
    NPC_VISCERI                 = 35617,

    // Alliance Champions
    NPC_JACOB                   = 34705,
    NPC_AMBROSE                 = 34702,
    NPC_COLOSOS                 = 34701,
    NPC_JAELYNE                 = 34657,
    NPC_LANA                    = 34703,

    // Alliance Pre-Champs
    NPC_STORMWIND_CHAMPION      = 35328,
    NPC_IRONFORGE_CHAMPION      = 35329,
    NPC_GNOMEREGAN_CHAMPION     = 35331,
    NPC_DARNASSUS_CHAMPION      = 35332,
    NPC_EXODAR_CHAMPION         = 35330,

    // Horde Pre-Champs
    NPC_ORGRIMMAR_CHAMPION      = 35314,
    NPC_THUNDER_BLUFF_CHAMPION  = 35325,
    NPC_UNDERCITY_CHAMPION      = 35327,
    NPC_SENJIN_CHAMPION         = 35323,
    NPC_SILVERMOON_CHAMPION     = 35326,

    // Argent Champions
    NPC_EADRIC                  = 35119,
    NPC_PALETRESS               = 34928,

    NPC_MEMORY_HOGGER           = 34942,
    NPC_MEMORY_VANCLEEF         = 35028,
    NPC_MEMORY_MUTANUS          = 35029,
    NPC_MEMORY_HEROD            = 35030,
    NPC_MEMORY_LUCIFROM         = 35031,
    NPC_MEMORY_THUNDERAAN       = 35032,
    NPC_MEMORY_CHROMAGGUS       = 35033,
    NPC_MEMORY_HAKKAR           = 35034,
    NPC_MEMORY_VEKNILASH        = 35036,
    NPC_MEMORY_KALITHRESH       = 35037,
    NPC_MEMORY_MALCHEZAAR       = 35038,
    NPC_MEMORY_GRUUL            = 35039,
    NPC_MEMORY_VASHJ            = 35040,
    NPC_MEMORY_ARCHIMONDE       = 35041,
    NPC_MEMORY_ILLIDAN          = 35042,
    NPC_MEMORY_DELRISSA         = 35043,
    NPC_MEMORY_MURU             = 35044,
    NPC_MEMORY_INGVAR           = 35045,
    NPC_MEMORY_CYANIGOSA        = 35046,
    NPC_MEMORY_ECK              = 35047,
    NPC_MEMORY_ONYXIA           = 35048,
    NPC_MEMORY_HEIGAN           = 35049,
    NPC_MEMORY_IGNIS            = 35050,
    NPC_MEMORY_VEZAX            = 35051,
    NPC_MEMORY_ALGALON          = 35052,

    // Argen Pre-Champs
    NPC_ARGENT_LIGHWIELDER      = 35309,
    NPC_ARGENT_MONK             = 35305,
    NPC_ARGENT_PRIEST           = 35307,

    NPC_BLACK_KNIGHT            = 35451,

    NPC_RISEN_JAEREN            = 35545,
    NPC_RISEN_ARELAS            = 35564,
    NPC_RISEN_CHAMPION          = 35590,

    // Announcers
    NPC_JAEREN                  = 35004,
    NPC_ARELAS                  = 35005,

    NPC_TIRION                  = 34996,
    NPC_THRALL                  = 34994,
    NPC_GARROSH                 = 34995,
    NPC_VARIAN                  = 34990,
    NPC_JAINA                   = 34992,

    // Misc
    NPC_ORIENTATION_TRIGGER     = 28073
};

enum TrialOfTheChampion_GameObjects
{
    GO_MAIN_GATE                = 195647,
    GO_PORTCULLIS               = 195650,

    GO_CHAMPIONS_LOOT           = 195709,
    GO_CHAMPIONS_LOOT_H         = 195710,

    GO_EADRIC_LOOT              = 195374,
    GO_EADRIC_LOOT_H            = 195375,

    GO_PALETRESS_LOOT           = 195323,
    GO_PALETRESS_LOOT_H         = 195324
};

enum TrialOfTheChampion_Vehicles
{
    //Grand Champions Alliance Vehicles
    VEHICLE_MARSHAL_JACOB_ALERIUS_MOUNT             = 35637,
    VEHICLE_AMBROSE_BOLTSPARK_MOUNT                 = 35633,
    VEHICLE_COLOSOS_MOUNT                           = 35768,
    VEHICLE_EVENSONG_MOUNT                          = 34658,
    VEHICLE_LANA_STOUTHAMMER_MOUNT                  = 35636,
    //Faction Champions (ALLIANCE)
    VEHICLE_DARNASSIA_NIGHTSABER                    = 33319,
    VEHICLE_EXODAR_ELEKK                            = 33318,
    VEHICLE_STORMWIND_STEED                         = 33217,
    VEHICLE_GNOMEREGAN_MECHANOSTRIDER               = 33317,
    VEHICLE_IRONFORGE_RAM                           = 33316,
    //Grand Champions Horde Vehicles
    VEHICLE_MOKRA_SKILLCRUSHER_MOUNT                = 35638,
    VEHICLE_ERESSEA_DAWNSINGER_MOUNT                = 35635,
    VEHICLE_RUNOK_WILDMANE_MOUNT                    = 35640,
    VEHICLE_ZUL_TORE_MOUNT                          = 35641,
    VEHICLE_DEATHSTALKER_VESCERI_MOUNT              = 35634,
    //Faction Champions (HORDE)
    VEHICLE_FORSAKE_WARHORSE                        = 33324,
    VEHICLE_THUNDER_BLUFF_KODO                      = 33322,
    VEHICLE_ORGRIMMAR_WOLF                          = 33320,
    VEHICLE_SILVERMOON_HAWKSTRIDER                  = 33323,
    VEHICLE_DARKSPEAR_RAPTOR                        = 33321,

    VEHICLE_ARGENT_WARHORSE                         = 35644,
    VEHICLE_ARGENT_BATTLEWORG                       = 36558,

    VEHICLE_BLACK_KNIGHT                            = 35491
};

enum TrialOfTheChampion_IntroSpells
{
    SPELL_MOUNT_LANCE_STAND   = 64723,
    SPELL_DEATH_RESPITE_INTRO = 66798,
    SPELL_DEATH_PUSH_INTRO    = 66797,
};

enum TrialOfTheChampion_Says
{
    // Grand Champions
    SAY_TIRION_INTRO_CHAMPS_1  = 0,

    // Horde
    SAY_THRALL_INTRO_CHAMPS_H  = 0,
    SAY_GARROSH_INTRO_CHAMPS_H = 0,
    SAY_VARIAN_INTRO_CHAMPS_H  = 0,
    SAY_JAINA_INTRO_CHAMPS_H   = 0,

    // Alliance
    SAY_VARIAN_INTRO_CHAMPS_A  = 1,
    SAY_JAINA_INTRO_CHAMPS_A   = 1,
    SAY_GARROSH_INTRO_CHAMPS_A = 1,
    SAY_THRALL_INTRO_CHAMPS_A  = 1,

    SAY_TIRION_INTRO_CHAMPS_2  = 1,

    SAY_ANNOUNCER_NPC_MAGE     = 1,
    SAY_ANNOUNCER_NPC_SHAMAN   = 2,
    SAY_ANNOUNCER_NPC_HUNTER   = 3,
    SAY_ANNOUNCER_NPC_WARRIOR  = 4,
    SAY_ANNOUNCER_NPC_ROUGE    = 5,
    SAY_ANNOUNCER_INTRO_PLAYER = 0,
    SAY_ANNOUNCER_PLAYER       = -2000000,
    EMOTE_FACTION_CHEER        = -1999980,

    // Argent Challenge
    SAY_TIRION_INTRO_ARGENT_1  = 2,
    SAY_ANNOUNCER_EADRIC       = 11,
    SAY_ANNOUNCER_PALETRESS    = 12,
    SAY_EADRIC_INTRO           = 10,
    SAY_PALETRESS_INTRO_1      = 10,
    SAY_PALETRESS_INTRO_2      = 11,
    SAY_TIRION_INTRO_ARGENT_2  = 3,

    // Black Knight
    SAY_TIRION_INTRO_BK_1     = 4,
    SAY_ANNOUNCER_BK          = 13,
    SAY_BK_INTRO_1            = 10,
    SAY_TIRION_INTRO_BK_2     = 5,
    SAY_BK_INTRO_2            = 11,
    SAY_BK_INTRO_3            = 12,
    SAY_VARIAN_INTRO_BK       = 10,
    SAY_GARROSH_INTRO_BK      = 10,
};

enum TrialOfTheChampion_Waypoints
{
    ENCOUNTER_WP_DESPAWN_POSITION   = ENCOUNTER_PREFIX + 1,
    ANNOUNCER_HOME_POSITION         = 1,
    ANNOUNCER_CHEST_POSITION        = 2
};

enum EncounterActions
{
    // ArgentumChallenge
    ENCOUNTER_ACTION_AC_START   = ENCOUNTER_PREFIX + 1,

    // BlackKnight
    ENCOUNTER_ACTION_BK_GRYPHON_FINISHED,
};

enum EncounterData
{
    // GrandChampions
    ENCOUNTER_DATA_GC_ADDS_DEFEATED = ENCOUNTER_PREFIX + 1,
    ENCOUNTER_DATA_GC_BOSS_DEFEATED,

    // ArgentumChallenge
    ENCOUNTER_DATA_AC_ADDS_DEFEATED,
};

enum EncounterValue
{
    ENCOUNTER_VALUE_GRAND_CHAMPIONS = ENCOUNTER_PREFIX - ENCOUNTER_VALUE_MAX,
    ENCOUNTER_VALUE_GRAND_CHAMPIONS_FAST,
    ENCOUNTER_VALUE_ARGENTUM_CHALLENGE,
    ENCOUNTER_VALUE_BLACK_KNIGHT,
};

enum AnnouncerActions
{
    ANNOUNCER_ACTION_MOVE_HOME          = 1,
    ANNOUNCER_ACTION_MOVE_CHEST         = 2
};

enum SharedFactions
{
	FACTION_HOSTILE_FOR_ALL = 16,
	FACTION_FRIENDLY_FOR_ALL = 35,
};

const Position SpawnPosition                = { 746.777f,   660.191f,   411.681f,   4.77922f    };
const Position GrandChampionSpawnPosition   = { 746.777f,   652.191f,   411.681f,   4.6842f     };
const Position StadiumPosition              = { 746.59f,    618.49f,    411.09f,    1.42f       };
const Position OutStadiumPosition           = { 747.03f,    687.483f,   412.373f,   1.53475f    };
const Position AllianceTriggerPosition      = { 762.12921f, 659.37933f, 412.393f,   1.59758f    };

const Position AnnouncerPosition[2] =
{
    { 731.585f, 658.719f, 412.393f, 4.61586f }, // Door
    { 742.742f, 630.207f, 411.172f, 4.61586f }  // Chest
};

const Position FactionChampionPos[2][3] =
{
    {
        // Horde
        { 724.854f, 640.344f, 411.829f, 5.60704f    },
        { 714.172f, 618.206f, 411.604f, 0.0f        },
        { 724.300f, 595.990f, 411.568f, 0.819248f   },
    },
    {
        // Alliance
        { 767.643f, 639.450f, 411.757f, 3.3663f     },
        { 779.169f, 618.351f, 411.607f, 3.06471f    },
        { 769.012f, 596.386f, 411.569f, 2.92883f    },
    },
};

const Position ArgentSoldierPosition[3] =
{
    { 746.875f, 650.358f, 411.569f, 4.77922f },
    { 717.771f, 647.165f, 411.923f, 5.54734f },
    { 775.734f, 644.413f, 411.919f, 3.79826f }
};

static void AggroAllPlayers(Creature* creature)
{
    Map::PlayerList const &PlList = creature->GetMap()->GetPlayers();

    if (PlList.isEmpty())
        return;

    for (Map::PlayerList::const_iterator i = PlList.begin(); i != PlList.end(); ++i)
    {
        if (Player* player = i->getSource())
        {
            if (player->isGameMaster())
                continue;

            if (player->isAlive())
            {
                if (Unit* mount = player->GetVehicleBase())
                {
                    creature->SetInCombatWith(mount);
                    mount->SetInCombatWith(creature);
                    creature->AddThreat(mount, 0.0f);
                }
                else
                {
                    creature->SetInCombatWith(player);
                    player->SetInCombatWith(creature);
                    creature->AddThreat(player, 0.0f);
                }
            }
        }
    }
};

static bool AreAllPlayersMounted(Map* pMap, bool dismountAndTeleport = false)
{
    ASSERT(pMap != NULL);

    Map::PlayerList const &PlList = pMap->GetPlayers();

    if (PlList.isEmpty())
        return false;

    for (Map::PlayerList::const_iterator i = PlList.begin(); i != PlList.end(); ++i)
    {
        if (Player* player = i->getSource())
        {
            if (player->isGameMaster())
                continue;

            if (player->isAlive())
            {
                if (Creature* pCreature = player->GetVehicleCreatureBase())
                {
                    if (dismountAndTeleport)
                    {
                        player->ExitVehicle();
                        pCreature->DespawnOrUnsummon();
                    }
                }
                else if (!dismountAndTeleport)
                    return false;
            }
        }
    }

    return true;
};

static void AnnounceChampion(Unit* pUnit, Creature* pAnnouncer)
{
    ASSERT(pUnit != NULL && pAnnouncer != NULL);

    if (pUnit->GetTypeId() == TYPEID_PLAYER)
    {
        if (Player* pPlayer = pUnit->ToPlayer())
        {
            int32 announce = SAY_ANNOUNCER_PLAYER - irand(0, 5);
            DoScriptText(announce, pAnnouncer, pPlayer);

            // Public emote
            DoScriptText(EMOTE_FACTION_CHEER - pPlayer->getRace(), pPlayer);
        }
    }
    else
    {
        switch (pUnit->GetEntry())
        {
            case NPC_MOKRA: // Warrior
                pAnnouncer->AI()->Talk(SAY_ANNOUNCER_NPC_WARRIOR);
                DoScriptText(EMOTE_FACTION_CHEER - RACE_ORC, pUnit);
                break;
            case NPC_JACOB:
                pAnnouncer->AI()->Talk(SAY_ANNOUNCER_NPC_WARRIOR);
                DoScriptText(EMOTE_FACTION_CHEER - RACE_HUMAN, pUnit);
                break;
            case NPC_ERESSEA: // Mage
                pAnnouncer->AI()->Talk(SAY_ANNOUNCER_NPC_MAGE);
                DoScriptText(EMOTE_FACTION_CHEER - RACE_BLOODELF, pUnit);
                break;
            case NPC_AMBROSE:
                pAnnouncer->AI()->Talk(SAY_ANNOUNCER_NPC_MAGE);
                DoScriptText(EMOTE_FACTION_CHEER - RACE_GNOME, pUnit);
                break;
            case NPC_RUNOK: // Saman
                pAnnouncer->AI()->Talk(SAY_ANNOUNCER_NPC_SHAMAN);
                DoScriptText(EMOTE_FACTION_CHEER - RACE_TAUREN, pUnit);
                break;
            case NPC_COLOSOS:
                pAnnouncer->AI()->Talk(SAY_ANNOUNCER_NPC_SHAMAN);
                DoScriptText(EMOTE_FACTION_CHEER - RACE_DRAENEI, pUnit);
                break;
            case NPC_JAELYNE: // Hunter
                pAnnouncer->AI()->Talk(SAY_ANNOUNCER_NPC_HUNTER);
                DoScriptText(EMOTE_FACTION_CHEER - RACE_NIGHTELF, pUnit);
                break;
            case NPC_ZULTORE:
                pAnnouncer->AI()->Talk(SAY_ANNOUNCER_NPC_HUNTER);
                DoScriptText(EMOTE_FACTION_CHEER - RACE_TROLL, pUnit);
                break;
            case NPC_VISCERI: // Rouge
                pAnnouncer->AI()->Talk(SAY_ANNOUNCER_NPC_ROUGE);
                DoScriptText(EMOTE_FACTION_CHEER - RACE_UNDEAD_PLAYER, pUnit);
                break;
            case NPC_LANA:
                pAnnouncer->AI()->Talk(SAY_ANNOUNCER_NPC_ROUGE);
                DoScriptText(EMOTE_FACTION_CHEER - RACE_DWARF, pUnit);
                break;
            case NPC_EADRIC:
                pAnnouncer->AI()->Talk(SAY_ANNOUNCER_EADRIC);
                DoScriptText(EMOTE_FACTION_CHEER, pUnit);
                break;
            case NPC_PALETRESS:
                pAnnouncer->AI()->Talk(SAY_ANNOUNCER_PALETRESS);
                DoScriptText(EMOTE_FACTION_CHEER, pUnit);
                break;
        }
    }
}


#endif // _TRIAL_OF_THE_CHAMPION_H
