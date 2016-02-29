/**
* @file
* @author TehKevin for Laenalith-WoW
* @date 27.07.2014
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
* @section DESCRIPTION
*
* The class BattlegroundSA handles the battleground Strand of the Ancients
* This class is inherited by class Battleground
*/

#include "BattlegroundSA.h"
#include "Language.h"
#include "Player.h"
#include "GameObject.h"
#include "ObjectMgr.h"
#include "WorldPacket.h"


BattlegroundSA::BattlegroundSA()
{
    StartMessageIds[BG_STARTING_EVENT_FIRST]  = LANG_BG_SA_START_TWO_MINUTES;
    StartMessageIds[BG_STARTING_EVENT_SECOND] = LANG_BG_SA_START_ONE_MINUTE;
    StartMessageIds[BG_STARTING_EVENT_THIRD]  = LANG_BG_SA_START_HALF_MINUTE;
    StartMessageIds[BG_STARTING_EVENT_FOURTH] = LANG_BG_SA_HAS_BEGUN;
    BgObjects.resize(BG_SA_MAXOBJ);
    BgCreatures.resize(BG_SA_MAXNPC + BG_SA_MAX_GY);
    Reset();
}

BattlegroundSA::~BattlegroundSA()
{
}

void BattlegroundSA::ResetBGSubclass()
{
    // Initialize bool default states
    boolStateMap[STATE_SHIPS_STARTED] = false;
    boolStateMap[STATE_TIMER_ENABLED] = false;
    boolStateMap[STATE_SIGNALED_ROUND_TWO] = false;
    boolStateMap[STATE_SIGNALED_ROUND_TWO_HALF_MIN] = false;
    boolStateMap[STATE_INIT_SECOND_ROUND] = false;
    boolStateMap[STATE_ACHIEVMENT_DOTA] = false;
    boolStateMap[STATE_INVISIBLE_WALLS] = false;

    // Initialize uint32 default states
    uint32StateMap[STATE_TOTAL_TIME] = 0;
    uint32StateMap[STATE_END_ROUND_TIMER] = 0;
    uint32StateMap[STATE_UPDATE_WAIT_TIMER] = 0;
    uint32StateMap[STATE_TIMER_DEMOLISHER_RESPAWN] = BG_SA_UPDATE_TIME_DEMOLISHER;

    memset(&graveyardStatus, 0, sizeof(graveyardStatus));
    attackers = ((urand(0, 1)) ? TEAM_ALLIANCE : TEAM_HORDE);

    for (uint8 i = 0; i <= 5; i++)
        gateStatus[i] = BG_SA_GATE_OK;

    notEvenAScratch[BG_TEAM_ALLIANCE] = true;
    notEvenAScratch[BG_TEAM_HORDE] = true;

    status = BG_SA_WARMUP;
}

void BattlegroundSA::FillInitialWorldStates(WorldPacket& data)
{
    uint32 ally_attacks = uint32(attackers == TEAM_ALLIANCE ? 1 : 0);
    uint32 horde_attacks = uint32(attackers == TEAM_HORDE ? 1 : 0);

    data << uint32(BG_SA_ANCIENT_GATEWS) << uint32(gateStatus[BG_SA_ANCIENT_GATE]);
    data << uint32(BG_SA_YELLOW_GATEWS) << uint32(gateStatus[BG_SA_YELLOW_GATE]);
    data << uint32(BG_SA_GREEN_GATEWS) << uint32(gateStatus[BG_SA_GREEN_GATE]);
    data << uint32(BG_SA_BLUE_GATEWS) << uint32(gateStatus[BG_SA_BLUE_GATE]);
    data << uint32(BG_SA_RED_GATEWS) << uint32(gateStatus[BG_SA_RED_GATE]);
    data << uint32(BG_SA_PURPLE_GATEWS) << uint32(gateStatus[BG_SA_PURPLE_GATE]);

    data << uint32(BG_SA_BONUS_TIMER) << uint32(0);

    data << uint32(BG_SA_HORDE_ATTACKS)<< horde_attacks;
    data << uint32(BG_SA_ALLY_ATTACKS) << ally_attacks;

    // Time will be sent on first update...
    data << uint32(BG_SA_ENABLE_TIMER) << ((GETBOOLSTATE(STATE_TIMER_ENABLED)) ? uint32(1) : uint32(0));
    data << uint32(BG_SA_TIMER_MINS) << uint32(0);
    data << uint32(BG_SA_TIMER_SEC_TENS) << uint32(0);
    data << uint32(BG_SA_TIMER_SEC_DECS) << uint32(0);

    data << uint32(BG_SA_EAST_GY_HORDE) << uint32(graveyardStatus[BG_SA_EAST_CAPTURABLE_GY] == TEAM_HORDE ? 1 : 0);
    data << uint32(BG_SA_WEST_GY_HORDE) << uint32(graveyardStatus[BG_SA_WEST_CAPTURABLE_GY] == TEAM_HORDE ? 1 : 0);
    data << uint32(BG_SA_CENTER_GY_HORDE) << uint32(graveyardStatus[BG_SA_CENTRAL_CAPTURABLE_GY] == TEAM_HORDE ? 1 : 0);

    data << uint32(BG_SA_EAST_GY_ALLIANCE) << uint32(graveyardStatus[BG_SA_EAST_CAPTURABLE_GY] == TEAM_ALLIANCE ? 1 : 0);
    data << uint32(BG_SA_WEST_GY_ALLIANCE) << uint32(graveyardStatus[BG_SA_WEST_CAPTURABLE_GY] == TEAM_ALLIANCE ? 1 : 0);
    data << uint32(BG_SA_CENTER_GY_ALLIANCE) << uint32(graveyardStatus[BG_SA_CENTRAL_CAPTURABLE_GY] == TEAM_ALLIANCE ? 1 : 0);

    data << uint32(BG_SA_HORDE_DEFENCE_TOKEN) << ally_attacks;
    data << uint32(BG_SA_ALLIANCE_DEFENCE_TOKEN) << horde_attacks;

    data << uint32(BG_SA_LEFT_ATT_TOKEN_HRD) << horde_attacks;
    data << uint32(BG_SA_RIGHT_ATT_TOKEN_HRD) << horde_attacks;
    data << uint32(BG_SA_RIGHT_ATT_TOKEN_ALL) <<  ally_attacks;
    data << uint32(BG_SA_LEFT_ATT_TOKEN_ALL) <<  ally_attacks;
}

bool BattlegroundSA::SetupBattleground()
{
    return resetBattleground();
}

bool BattlegroundSA::resetBattleground()
{
    despawnCreatures();
    despawnGameObjects();

    if (spawnCreatures() && spawnGameObjects())
    {
        resetStates();
        return true;
    }

    return false;
}

bool BattlegroundSA::spawnCreatures()
{
    // Cannons
    for (uint8 i = 0; i <= BG_SA_GUN_10; ++i)
    {
        if (Creature* pCreature = AddCreature(StrandOfTheAncientsCreatureSpawns[i].entry, i,
            (attackers == TEAM_ALLIANCE ? TEAM_HORDE : TEAM_ALLIANCE),
            StrandOfTheAncientsCreatureSpawns[i].x, StrandOfTheAncientsCreatureSpawns[i].y,
            StrandOfTheAncientsCreatureSpawns[i].z, StrandOfTheAncientsCreatureSpawns[i].o, 600))
            pCreature->setFaction(StrandOfTheAncientsFactions[attackers ? TEAM_ALLIANCE : TEAM_HORDE]);
        else
        {
            sLog->outError("SOTA: Couldn't spawn cannon type: %u", i);
            return false;
        }
    }

    // Triggers
    for (uint8 i = BG_SA_NPC_TRIGGER_1; i <= BG_SA_NPC_TRIGGER_5; ++i)
    {
        if (!AddCreature(StrandOfTheAncientsCreatureSpawns[i].entry, i,
            (attackers == TEAM_ALLIANCE ? TEAM_HORDE : TEAM_ALLIANCE),
            StrandOfTheAncientsCreatureSpawns[i].x, StrandOfTheAncientsCreatureSpawns[i].y,
            StrandOfTheAncientsCreatureSpawns[i].z, StrandOfTheAncientsCreatureSpawns[i].o, 600))
        {
            sLog->outError("SOTA: Couldn't spawn trigger type: %u", i);
            return false;
        }
    }

    // Demolisher
    for (uint8 i = 0; i < (sizeof(StrandOfTheAncientsBeachDemolisherSpawns) / sizeof(StrandOfTheAncientsSpawnData)); ++i)
    {
        if (Creature* pCreature = AddCreature(StrandOfTheAncientsBeachDemolisherSpawns[i].entry, 
            i + BG_SA_BEACH_DEMOLISHER_1, (attackers == TEAM_ALLIANCE ? TEAM_HORDE : TEAM_ALLIANCE),
            StrandOfTheAncientsBeachDemolisherSpawns[i].x, StrandOfTheAncientsBeachDemolisherSpawns[i].y,
            StrandOfTheAncientsBeachDemolisherSpawns[i].z, StrandOfTheAncientsBeachDemolisherSpawns[i].o, 600))
        {
            pCreature->setFaction(StrandOfTheAncientsFactions[attackers]);

            // Apply disable health regen aura
            pCreature->AddAura(BG_SA_SPELL_DISABLE_HEALTH_REGEN, pCreature);

            // Apply spell immunity (Against client error)
            for (uint8 j = 0; j < (sizeof(BattlegroundVehicleSpellImmunity) / sizeof(uint32)); ++j)
                pCreature->ApplySpellImmune(BattlegroundVehicleSpellImmunity[i], IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
        }
        else
        {
            sLog->outError("SOTA: Couldn't spawn demolisher type: %u", i);
            return false;
        }
    }

    demolisherStartState(true);

    // Demolisher engineers - default not visible
    for (uint8 i = 0; i < (sizeof(StrandOfTheAncientsCapturePointSpawns) / sizeof(StrandOfTheAncientsSpawnData)); ++i)
    {
        if (Creature* pCreature = AddCreature(StrandOfTheAncientsCapturePointSpawns[i].entry,
            i + BG_SA_NPC_SPARKLIGHT_EAST, (attackers == TEAM_ALLIANCE ? TEAM_HORDE : TEAM_ALLIANCE),
            StrandOfTheAncientsCapturePointSpawns[i].x, StrandOfTheAncientsCapturePointSpawns[i].y,
            StrandOfTheAncientsCapturePointSpawns[i].z, StrandOfTheAncientsCapturePointSpawns[i].o, 600))
        {
            pCreature->setFaction(StrandOfTheAncientsFactions[attackers]);
            pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            pCreature->SetVisible(false);
        }
        else
        {
            sLog->outError("SOTA: Couldn't spawn demolisher engineer type: %u", i);
            return false;
        }
    }

    // Graveyards
    for (uint8 i = 0; i < BG_SA_MAX_GY; i++)
    {
        WorldSafeLocsEntry const* worldSafeLoc = NULL;
        worldSafeLoc = sWorldSafeLocsStore.LookupEntry(StrandOfTheAncientsGYEntries[i]);

        if (!worldSafeLoc)
        {
            sLog->outError("SOTA: Couldn't find Graveyard entry %u", StrandOfTheAncientsGYEntries[i]);
            return false;
        }

        if (i == BG_SA_BEACH_GY)
        {
            graveyardStatus[i] = attackers;
            if (!AddSpiritGuide(i + BG_SA_MAXNPC, worldSafeLoc->x, worldSafeLoc->y, worldSafeLoc->z, 
                StrandOfTheAncientsGYOrientation[i], ((attackers == TEAM_HORDE) ? HORDE : ALLIANCE)))
                sLog->outError("SOTA: Couldn't spawn Graveyard: %u", i);
        }
        else
        {
            graveyardStatus[i] = ((attackers == TEAM_HORDE) ? TEAM_ALLIANCE : TEAM_HORDE);
            if (!AddSpiritGuide(i + BG_SA_MAXNPC, worldSafeLoc->x, worldSafeLoc->y, worldSafeLoc->z, 
                StrandOfTheAncientsGYOrientation[i], ((attackers == TEAM_HORDE) ? ALLIANCE : HORDE)))
                sLog->outError("SOTA: Couldn't spawn Graveyard: %u", i);
        }
    }

    return true;
}

bool BattlegroundSA::spawnGameObjects()
{
    // Gates + Titan relic
    for (uint8 i = 0; i <= BG_SA_TITAN_RELIC; ++i)
    {
        if (!AddObject(i, StrandOfTheAncientsObjectSpawns[i].entry, StrandOfTheAncientsObjectSpawns[i].x,
            StrandOfTheAncientsObjectSpawns[i].y, StrandOfTheAncientsObjectSpawns[i].z,
            StrandOfTheAncientsObjectSpawns[i].o, 0, 0, 0, 0, RESPAWN_ONE_DAY))
        {
            sLog->outError("SOTA: Couldn't spawn gate or relic type: %u", i);
            return false;
        }
    }

    // Ships
    for (uint8 i = BG_SA_BOAT_ONE; i <= BG_SA_BOAT_TWO; ++i)
    {
        uint32 boatid = 0;

        switch (i)
        {
            case BG_SA_BOAT_ONE:
                boatid = attackers ? BG_SA_BOAT_ONE_H : BG_SA_BOAT_ONE_A;
                break;
            case BG_SA_BOAT_TWO:
                boatid = attackers ? BG_SA_BOAT_TWO_H : BG_SA_BOAT_TWO_A;
                break;
        }

        if (!AddObject(i, boatid,
            StrandOfTheAncientsObjectSpawns[i].x, StrandOfTheAncientsObjectSpawns[i].y,
            StrandOfTheAncientsObjectSpawns[i].z + (attackers ? -3.750f : 0),
            StrandOfTheAncientsObjectSpawns[i].o, 0, 0, 0, 0, RESPAWN_ONE_DAY))
        {
            sLog->outError("SOTA: Couldn't spawn ship type: %u", i);
            return false;
        }
    }

    // Sigils, Flagpoles
    for (uint8 i = BG_SA_SIGIL_1; i <= BG_SA_CENTRAL_FLAGPOLE; ++i)
    {
        if (!AddObject(i, StrandOfTheAncientsObjectSpawns[i].entry, StrandOfTheAncientsObjectSpawns[i].x,
            StrandOfTheAncientsObjectSpawns[i].y, StrandOfTheAncientsObjectSpawns[i].z, 
            StrandOfTheAncientsObjectSpawns[i].o, 0, 0, 0, 0, RESPAWN_ONE_DAY))
        {
            sLog->outError("SOTA: Couldn't spawn sigil or flagpole type: %u", i);
            return false;
        }
    }

    // MAD props for Kiper for discovering those values - 4 hours of his work.
    GetBGObject(BG_SA_BOAT_ONE)->UpdateRotationFields(1.0f, 0.0002f);
    GetBGObject(BG_SA_BOAT_TWO)->UpdateRotationFields(1.0f, 0.00001f);
    SpawnBGObject(BG_SA_BOAT_ONE, RESPAWN_IMMEDIATELY);
    SpawnBGObject(BG_SA_BOAT_TWO, RESPAWN_IMMEDIATELY);

    uint32 atF = StrandOfTheAncientsFactions[attackers];
    uint32 defF = StrandOfTheAncientsFactions[attackers ? TEAM_ALLIANCE : TEAM_HORDE];

    for (uint8 i = 0; i <= BG_SA_TITAN_RELIC; ++i)
    {
        SpawnBGObject(i, RESPAWN_IMMEDIATELY);
        GetBGObject(i)->SetUInt32Value(GAMEOBJECT_FACTION, defF);
    }

    GetBGObject(BG_SA_TITAN_RELIC)->SetUInt32Value(GAMEOBJECT_FACTION, atF);
    GetBGObject(BG_SA_TITAN_RELIC)->Refresh();

    // Flags
    for (uint8 i = BG_SA_EAST_FLAG; i < BG_SA_PORTAL_DEFFENDER_BLUE; i++)
    {
        if (!AddObject(i, (StrandOfTheAncientsObjectSpawns[i].entry - (attackers == TEAM_ALLIANCE ? 1 : 0)),
            StrandOfTheAncientsObjectSpawns[i].x, StrandOfTheAncientsObjectSpawns[i].y,
            StrandOfTheAncientsObjectSpawns[i].z, StrandOfTheAncientsObjectSpawns[i].o,
            0, 0, 0, 0, RESPAWN_ONE_DAY))
        {
            sLog->outError("SOTA: Couldn't spawn flag type: %u", i);
            return false;
        }
        else
            GetBGObject(i)->SetUInt32Value(GAMEOBJECT_FACTION, atF);
    }

    // Defender Portals
    for (uint8 i = BG_SA_PORTAL_DEFFENDER_BLUE; i <= BG_SA_PORTAL_DEFFENDER_RED; ++i)
    {
        if (!AddObject(i, StrandOfTheAncientsObjectSpawns[i].entry, StrandOfTheAncientsObjectSpawns[i].x,
            StrandOfTheAncientsObjectSpawns[i].y, StrandOfTheAncientsObjectSpawns[i].z,
            StrandOfTheAncientsObjectSpawns[i].o, 0, 0, 0, 0, RESPAWN_ONE_DAY))
        {
            sLog->outError("SOTA: Couldn't spawn gate type: %u", i);
            return false;
        }
        else
            GetBGObject(i)->SetUInt32Value(GAMEOBJECT_FACTION, defF);
    }

    // Bombs
    for (uint8 i = BG_SA_BOMB; i < BG_SA_INVISIBLE_WALL_EAST_F; ++i)
    {
        if (!AddObject(i, StrandOfTheAncientsObjectSpawns[i].entry, StrandOfTheAncientsObjectSpawns[i].x,
            StrandOfTheAncientsObjectSpawns[i].y, StrandOfTheAncientsObjectSpawns[i].z,
            StrandOfTheAncientsObjectSpawns[i].o, 0, 0, 0, 0, RESPAWN_ONE_DAY))
        {
            sLog->outError("SOTA: Couldn't spawn bomb type: %u", i);
            return false;
        }
        else
            GetBGObject(i)->SetUInt32Value(GAMEOBJECT_FACTION, atF);
    }

    // Create new boats
    sendBGTransport();
    return true;
}

void BattlegroundSA::despawnCreatures()
{
    for (uint8 i = 0; i < BG_SA_MAXNPC; ++i)
        DelCreature(i);

    for (uint8 i = BG_SA_MAXNPC; i < BG_SA_MAXNPC + BG_SA_MAX_GY; i++)
        DelCreature(i);
}

void BattlegroundSA::despawnGameObjects()
{
    // Remove ships
    if (GETBOOLSTATE(STATE_SHIPS_STARTED))
        sendBGTransport(true);

    for (uint8 i = 0; i < BG_SA_MAXOBJ; ++i)
        DelObject(i);
}

void BattlegroundSA::resetStates()
{
    for (uint8 i = 0; i < 6; i++)
        gateStatus[i] = BG_SA_GATE_OK;

    for (uint8 i = 0; i <= 5; i++)
        gateStatus[i] = BG_SA_GATE_OK;

    uint32StateMap[STATE_TOTAL_TIME] = 0;

    //Player may enter BEFORE we set up BG - lets update his worldstates anyway...
    UpdateWorldState(BG_SA_EAST_GY_HORDE, graveyardStatus[BG_SA_EAST_CAPTURABLE_GY] == TEAM_HORDE ? 1 : 0);
    UpdateWorldState(BG_SA_WEST_GY_HORDE, graveyardStatus[BG_SA_WEST_CAPTURABLE_GY] == TEAM_HORDE ? 1 : 0);
    UpdateWorldState(BG_SA_CENTER_GY_HORDE, graveyardStatus[BG_SA_CENTRAL_CAPTURABLE_GY] == TEAM_HORDE ? 1 : 0);

    UpdateWorldState(BG_SA_EAST_GY_ALLIANCE, graveyardStatus[BG_SA_EAST_CAPTURABLE_GY] == TEAM_ALLIANCE ? 1 : 0);
    UpdateWorldState(BG_SA_WEST_GY_ALLIANCE, graveyardStatus[BG_SA_WEST_CAPTURABLE_GY] == TEAM_ALLIANCE ? 1 : 0);
    UpdateWorldState(BG_SA_CENTER_GY_ALLIANCE, graveyardStatus[BG_SA_CENTRAL_CAPTURABLE_GY] == TEAM_ALLIANCE ? 1 : 0);

    UpdateWorldState(BG_SA_ALLY_ATTACKS, attackers == TEAM_ALLIANCE ? 1 : 0);
    UpdateWorldState(BG_SA_HORDE_ATTACKS, attackers == TEAM_ALLIANCE ? 0 : 1);

    UpdateWorldState(BG_SA_RIGHT_ATT_TOKEN_ALL, attackers == TEAM_ALLIANCE ? 1 : 0);
    UpdateWorldState(BG_SA_LEFT_ATT_TOKEN_ALL, attackers == TEAM_ALLIANCE ? 1 : 0);
    UpdateWorldState(BG_SA_RIGHT_ATT_TOKEN_HRD, attackers == TEAM_ALLIANCE ? 0 : 1);
    UpdateWorldState(BG_SA_LEFT_ATT_TOKEN_HRD, attackers == TEAM_ALLIANCE ? 0 : 1);

    UpdateWorldState(BG_SA_HORDE_DEFENCE_TOKEN, attackers == TEAM_ALLIANCE ? 1 : 0);
    UpdateWorldState(BG_SA_ALLIANCE_DEFENCE_TOKEN, attackers == TEAM_ALLIANCE ? 0 : 1);

    UpdateWorldState(BG_SA_PURPLE_GATEWS, 1);
    UpdateWorldState(BG_SA_RED_GATEWS, 1);
    UpdateWorldState(BG_SA_BLUE_GATEWS, 1);
    UpdateWorldState(BG_SA_GREEN_GATEWS, 1);
    UpdateWorldState(BG_SA_YELLOW_GATEWS, 1);
    UpdateWorldState(BG_SA_ANCIENT_GATEWS, 1);
}

void BattlegroundSA::initSecondRound()
{
    attackers = (attackers == TEAM_ALLIANCE) ? TEAM_HORDE : TEAM_ALLIANCE;
    status = BG_SA_SECOND_WARMUP;
    SendWarningToAll(LANG_BG_SA_ROUND_ONE_END);

    uint32StateMap[STATE_TOTAL_TIME] = 0;
    uint32StateMap[STATE_UPDATE_WAIT_TIMER] = 5000;
    boolStateMap[STATE_SIGNALED_ROUND_TWO] = false;
    boolStateMap[STATE_SIGNALED_ROUND_TWO_HALF_MIN] = false;
    boolStateMap[STATE_INIT_SECOND_ROUND] = true;
    boolStateMap[STATE_INVISIBLE_WALLS] = false;

    toggleTimer();
    resetBattleground();

    // Cast spell "End of Round" on players
    for (BattlegroundPlayerMap::const_iterator itr = GetPlayers().begin(); itr != GetPlayers().end(); ++itr)
    {
        if (Player* pPlayer = sObjectAccessor->FindPlayer(itr->first))
        {
            // Should remove spirit of redemption
            if (pPlayer->HasAuraType(SPELL_AURA_SPIRIT_OF_REDEMPTION))
                pPlayer->RemoveAurasByType(SPELL_AURA_MOD_SHAPESHIFT);

            pPlayer->CastSpell(pPlayer, BG_SA_SPELL_END_OF_ROUND, true);
        }
    }
}

void BattlegroundSA::finishBattleground()
{
    // All players exit vehicle
    for (BattlegroundPlayerMap::const_iterator itr = GetPlayers().begin(); itr != GetPlayers().end(); ++itr)
        if (Player* pPlayer = sObjectAccessor->FindPlayer(itr->first))
            if (pPlayer->GetVehicleBase())
                pPlayer->ExitVehicle();

    if (roundScores[0].time == roundScores[1].time)
        endBattleground(0);
    else if (roundScores[0].time < roundScores[1].time)
        endBattleground(roundScores[0].winner == TEAM_ALLIANCE ? ALLIANCE : HORDE);
    else
        endBattleground(roundScores[1].winner == TEAM_ALLIANCE ? ALLIANCE : HORDE);
}

void BattlegroundSA::PostUpdateImpl(uint32 diff)
{
    if (GETBOOLSTATE(STATE_INIT_SECOND_ROUND))
    {
        if (GETUINT32STATE(STATE_UPDATE_WAIT_TIMER) < diff)
        {
            if (!GETBOOLSTATE(STATE_SIGNALED_ROUND_TWO))
            {
                // Teleport players
                teleportPlayers();

                boolStateMap[STATE_SIGNALED_ROUND_TWO] = true;
                boolStateMap[STATE_INIT_SECOND_ROUND] = false;
                SendMessageToAll(LANG_BG_SA_ROUND_TWO_ONE_MINUTE, CHAT_MSG_BG_SYSTEM_NEUTRAL);
            }
        }
        else
        {
            uint32StateMap[STATE_UPDATE_WAIT_TIMER] -= diff;
            return;
        }
    }

    uint32StateMap[STATE_TOTAL_TIME] += diff;

    if (status == BG_SA_WARMUP )
    {
        uint32StateMap[STATE_END_ROUND_TIMER] = BG_SA_ROUNDLENGTH;

        if (GETUINT32STATE(STATE_TOTAL_TIME) >= BG_SA_WARMUPLENGTH)
        {
            uint32StateMap[STATE_TOTAL_TIME] = 0;
            toggleTimer();
            demolisherStartState(false);
            status = BG_SA_ROUND_ONE;

            if (GETBOOLSTATE(STATE_INVISIBLE_WALLS))
                despawnInvisibleWalls();

            StartTimedAchievement(ACHIEVEMENT_TIMED_TYPE_EVENT, (attackers == TEAM_ALLIANCE) ? 23748 : 21702);
        }

        if (GETUINT32STATE(STATE_TOTAL_TIME) >= BG_SA_BOAT_START && !GETBOOLSTATE(STATE_SHIPS_STARTED))
            // Start ships
            startBGTransport();

        return;
    }
    else if (status == BG_SA_SECOND_WARMUP)
    {
        if (GETUINT32STATE(STATE_TOTAL_TIME) >= 60000)
        {
            if (roundScores[0].time < BG_SA_ROUNDLENGTH)
                uint32StateMap[STATE_END_ROUND_TIMER] = roundScores[0].time;
            else
                uint32StateMap[STATE_END_ROUND_TIMER] = BG_SA_ROUNDLENGTH;

            uint32StateMap[STATE_TOTAL_TIME] = 0;
            SendWarningToAll(LANG_BG_SA_HAS_BEGUN);
            demolisherStartState(false);
            status = BG_SA_ROUND_TWO;
            toggleTimer();

            if (GETBOOLSTATE(STATE_INVISIBLE_WALLS))
                despawnInvisibleWalls();

            StartTimedAchievement(ACHIEVEMENT_TIMED_TYPE_EVENT, (attackers == TEAM_ALLIANCE)?23748:21702);
        }

        if (GETUINT32STATE(STATE_TOTAL_TIME) >= 30000)
        {
            if (!GETBOOLSTATE(STATE_SIGNALED_ROUND_TWO_HALF_MIN))
            {
                boolStateMap[STATE_SIGNALED_ROUND_TWO_HALF_MIN] = true;
                SendMessageToAll(LANG_BG_SA_ROUND_TWO_START_HALF_MINUTE, CHAT_MSG_BG_SYSTEM_NEUTRAL);
            }
        }

        if (!GETBOOLSTATE(STATE_SHIPS_STARTED))
            startBGTransport();

        return;
    }
    else if (GetStatus() == STATUS_IN_PROGRESS)
    {
        if (status == BG_SA_ROUND_ONE)
        {
            if (GETUINT32STATE(STATE_TOTAL_TIME) >= BG_SA_ROUNDLENGTH)
            {
                roundScores[0].winner = (attackers == TEAM_ALLIANCE) ? TEAM_HORDE : TEAM_ALLIANCE;
                roundScores[0].time = BG_SA_ROUNDLENGTH;
                initSecondRound();
                return;
            }
        }
        else if (status == BG_SA_ROUND_TWO)
        {
            if (GETUINT32STATE(STATE_TOTAL_TIME) >= GETUINT32STATE(STATE_END_ROUND_TIMER))
            {
                roundScores[1].winner = (attackers == TEAM_ALLIANCE) ? TEAM_HORDE : TEAM_ALLIANCE;
                roundScores[1].time = BG_SA_ROUNDLENGTH;
                finishBattleground();
                return;
            }
        }

        if (status == BG_SA_ROUND_ONE || status == BG_SA_ROUND_TWO)
        {
            sendTime();

            if (GETUINT32STATE(STATE_TIMER_DEMOLISHER_RESPAWN) < diff)
            {
                updateDemolisherSpawns();
                uint32StateMap[STATE_TIMER_DEMOLISHER_RESPAWN] = BG_SA_UPDATE_TIME_DEMOLISHER;
            }
            else
                uint32StateMap[STATE_TIMER_DEMOLISHER_RESPAWN] -= diff;
        }
    }
}

void BattlegroundSA::StartingEventCloseDoors()
{
    // not used...
}

void BattlegroundSA::StartingEventOpenDoors()
{
    // not used...
}

void BattlegroundSA::spawnInvisibleWalls()
{
    // Spawn invisible walls
    for (uint8 type = BG_SA_INVISIBLE_WALL_EAST_F; type <= BG_SA_INVISIBLE_WALL_EAST_2; ++type)
    {
        if (!AddObject(type, StrandOfTheAncientsObjectSpawns[type].entry, StrandOfTheAncientsObjectSpawns[type].x,
            StrandOfTheAncientsObjectSpawns[type].y, StrandOfTheAncientsObjectSpawns[type].z,
            StrandOfTheAncientsObjectSpawns[type].o, 0, 0, 0, 0, RESPAWN_ONE_DAY))
            sLog->outError("SOTA: Couldn't spawn invisible wall type: %u", type);
        else
        {
            float scale;

            if (type >= BG_SA_INVISIBLE_WALL_EAST_1)
                scale = BG_SA_INVISIBLE_WALL_SCALE;
            else
                scale = BG_SA_INVISIBLE_WALL_F_SCALE;

            // Invisible walls with scale mod
            if (GameObject* pObj = GetBGObject(type))
                pObj->SetObjectScale(scale);

            SpawnBGObject(type, RESPAWN_IMMEDIATELY);
        }
    }

    boolStateMap[STATE_INVISIBLE_WALLS] = true;
}

void BattlegroundSA::despawnInvisibleWalls()
{
    // Despawn invisible walls
    for (uint8 type = BG_SA_INVISIBLE_WALL_EAST_F; type <= BG_SA_INVISIBLE_WALL_EAST_2; ++type)
        DelObject(type);
}

void BattlegroundSA::AddPlayer(Player* pPlayer)
{
    Battleground::AddPlayer(pPlayer);

    //create score and add it to map, default values are set in constructor
    BattlegroundSAScore* playerScore = new BattlegroundSAScore;

    if (pPlayer->GetTeamId() == attackers)
    {
        if (!GETBOOLSTATE(STATE_SHIPS_STARTED))
        {
            // player->CastSpell(player, BG_SA_SPELL_PARACHUTE, true); //Without this player falls before boat loads...

            bool aSpawn = urand(0, 1);
            pPlayer->TeleportTo(StrandOfTheAncientsPlayerSpawns[aSpawn].entry,StrandOfTheAncientsPlayerSpawns[aSpawn].x,
                                StrandOfTheAncientsPlayerSpawns[aSpawn].y, StrandOfTheAncientsPlayerSpawns[aSpawn].z,
                                StrandOfTheAncientsPlayerSpawns[aSpawn].o);
        }
        else
        {
            pPlayer->TeleportTo(StrandOfTheAncientsPlayerSpawns[2].entry, StrandOfTheAncientsPlayerSpawns[2].x,
                                StrandOfTheAncientsPlayerSpawns[2].y, StrandOfTheAncientsPlayerSpawns[2].z,
                                StrandOfTheAncientsPlayerSpawns[2].o);

            // Spawn invisible walls if they arent and bg is in warm up
            if (!GETBOOLSTATE(STATE_INVISIBLE_WALLS) && status == BG_SA_WARMUP)
                spawnInvisibleWalls();
        }
    }
    else
        pPlayer->TeleportTo(StrandOfTheAncientsPlayerSpawns[3].entry, StrandOfTheAncientsPlayerSpawns[3].x,
                            StrandOfTheAncientsPlayerSpawns[3].y, StrandOfTheAncientsPlayerSpawns[3].z,
                            StrandOfTheAncientsPlayerSpawns[3].o);

    sendTransportInit(pPlayer);
    PlayerScores[pPlayer->GetGUID()] = playerScore;

    // Development
    /*if (pPlayer->GetTeamId() == attackers && GETBOOLSTATE(STATE_SHIPS_STARTED))
        teleportPlayerAfterShipStarted(pPlayer);*/
}

void BattlegroundSA::RemovePlayer(Player* pPlayer, uint64 /*guid*/, uint32 /*team*/)
{
    // Sometimes there is a weird bug - pPlayer == NULL
    if (!pPlayer)
    {
        sLog->outError("SOTA: Couldn't track player to remove from battleground");
        return;
    }

    if (pPlayer->GetVehicleBase())
        pPlayer->ExitVehicle();
}

void BattlegroundSA::HandleAreaTrigger(Player* /*pPlayer*/, uint32 /*trigger*/)
{
    // this is wrong way to implement these things. On official it done by gameobject spell cast.
    if (GetStatus() != STATUS_IN_PROGRESS)
        return;
}

void BattlegroundSA::UpdatePlayerScore(Player* pPlayer, uint32 type, uint32 value, bool doAddHonor)
{
    BattlegroundScoreMap::iterator itr = PlayerScores.find(pPlayer->GetGUID());
    if (itr == PlayerScores.end()) // Player not found
        return;

    if (type == SCORE_DESTROYED_DEMOLISHER)
        ((BattlegroundSAScore*)itr->second)->demolishers_destroyed += value;
    else if (type == SCORE_DESTROYED_WALL)
        ((BattlegroundSAScore*)itr->second)->gates_destroyed += value;
    else
        Battleground::UpdatePlayerScore(pPlayer, type, value, doAddHonor);
}

void BattlegroundSA::teleportPlayers()
{
    for (BattlegroundPlayerMap::const_iterator itr = GetPlayers().begin(); itr != GetPlayers().end(); ++itr)
    {
        if (Player* pPlayer = ObjectAccessor::FindPlayer(itr->first))
        {
            pPlayer->RemoveAurasDueToSpell(BG_SA_SPELL_END_OF_ROUND);

            if (pPlayer->GetTeamId() == attackers)
            {
                //player->CastSpell(player, 12438, true);     //Without this player falls before boat loads...

                bool aSpawn = urand(0, 1);
                pPlayer->TeleportTo(StrandOfTheAncientsPlayerSpawns[aSpawn].entry, StrandOfTheAncientsPlayerSpawns[aSpawn].x,
                    StrandOfTheAncientsPlayerSpawns[aSpawn].y, StrandOfTheAncientsPlayerSpawns[aSpawn].z,
                    StrandOfTheAncientsPlayerSpawns[aSpawn].o);
            }
            else
                pPlayer->TeleportTo(StrandOfTheAncientsPlayerSpawns[3].entry, StrandOfTheAncientsPlayerSpawns[3].x,
                    StrandOfTheAncientsPlayerSpawns[3].y, StrandOfTheAncientsPlayerSpawns[3].z,
                    StrandOfTheAncientsPlayerSpawns[3].o);
        }
    }
}

void BattlegroundSA::teleportPlayerAfterShipStarted(Player* pPlayer)
{
    for (BattlegroundPlayerMap::const_iterator itr = GetPlayers().begin(); itr != GetPlayers().end(); ++itr)
    {
        if (Player* playerOnShip = ObjectAccessor::FindPlayer(itr->first))
        {
            if (playerOnShip->GetTeam() == pPlayer->GetTeam())
            {
                // Port player to another player who is on the ship
                pPlayer->TeleportTo(playerOnShip->GetMapId(), playerOnShip->GetPositionX(),
                                    playerOnShip->GetPositionY(), (playerOnShip->GetPositionZ() + 1.0f),
                                    playerOnShip->GetOrientation());

                for (int i = BG_SA_BOAT_ONE; i <= BG_SA_BOAT_TWO; i++)
                {
                    // Get updateData from Objects (updateBlock)
                    UpdateData uData;
                    GetBGObject(i)->BuildValuesUpdateBlockForPlayer(&uData, pPlayer);

                    // Build packet and send it to player
                    WorldPacket wPacket;
                    uData.BuildPacket(&wPacket);
                    pPlayer->SendDirectMessage(&wPacket);
                }
                break;
            }
        }
    }
}

void BattlegroundSA::EventPlayerDamagedGO(Player* /*pPlayer*/, GameObject* pObj, uint32 eventType)
{
    if (!pObj || !pObj->GetGOInfo())
        return;

    int32 gateId = getGateIdFromDamagedOrDestroyEventId(pObj->GetGOInfo()->building.damagedEvent);
    if (gateId != -1)
    {
        if (eventType != pObj->GetGOInfo()->building.destroyedEvent)
            eventGateDamaged(pObj, gateId);
    }
}

void BattlegroundSA::eventGateDamaged(GameObject* gObj, int32 gateId)
{
    gateStatus[gateId] = BG_SA_GATE_DAMAGED;

    // Update worldstates
    uint32 worldState = getWorldStateFromGateId(gateId);
    if (worldState)
        UpdateWorldState(worldState, gateStatus[gateId]);

    SendWarningToAll(LANG_BG_SA_IS_UNDER_ATTACK, gObj->GetNameForLocaleIdx(LOCALE_deDE));
}

void BattlegroundSA::eventGateDestroyed(GameObject* gObj, int32 gateId)
{
    gateStatus[gateId] = BG_SA_GATE_DESTROYED;

    // Update worldstates
    uint32 worldState = getWorldStateFromGateId(gateId);
    if (worldState)
        UpdateWorldState(worldState, gateStatus[gateId]);

    // The last door (chamber)
    if (gObj->GetGOInfo()->building.destroyedEvent == BG_SA_EVENT_ANCIENT_GATE_DESTROYED)
        SendWarningToAll(LANG_BG_SA_CHAMBER_BREACHED);
    else
        SendWarningToAll(LANG_BG_SA_WAS_DESTROYED, gObj->GetNameForLocaleIdx(LOCALE_deDE));
}

bool BattlegroundSA::checkDemolisherCount()
{
    uint8 maxDemolisher = 0;
    if (graveyardStatus[BG_SA_EAST_CAPTURABLE_GY] == attackers)
        maxDemolisher += BG_SA_DEMOLISHER_AMOUNT / 2;
    if (graveyardStatus[BG_SA_WEST_CAPTURABLE_GY] == attackers)
        maxDemolisher += BG_SA_DEMOLISHER_AMOUNT / 2;

    uint8 counter = 0;
    for (uint8 i = 0; i < 4; i++)
    {
        if (IsBgCreature(BG_SA_DEMOLISHER_1 + i))
            ++counter;
    }

    if (counter < maxDemolisher)
        return true;
    return false;
}

void BattlegroundSA::spawnDemolisher(uint64 engineerGUID, TeamId team)
{
    for (uint8 i = 0; i < 2; ++i)
    {
        if (Creature* engineerPtr = GetBGCreature(BG_SA_NPC_SPARKLIGHT_EAST + i))
        {
            if (engineerPtr->GetGUID() == engineerGUID)
                spawnBattleDemolisher(StrandOfTheAncientsDemolisherSpawns[i], team);
        }
    }
}

void BattlegroundSA::spawnBattleDemolisher(const StrandOfTheAncientsSpawnData& spawnData, TeamId team)
{
    for (uint8 i = BG_SA_DEMOLISHER_1; i <= BG_SA_DEMOLISHER_4; ++i)
    {
        // Check for free slot
        if (!IsBgCreature(i))
        {
            if (Creature* pCreature = AddCreature(spawnData.entry, i, team, spawnData.x, spawnData.y, spawnData.z, spawnData.o, 0))
            {
                pCreature->setFaction(StrandOfTheAncientsFactions[team]);

                // Apply disable health regen aura
                pCreature->AddAura(BG_SA_SPELL_DISABLE_HEALTH_REGEN, pCreature);

                // Immunity mask of bg vehicles
                for (uint8 j = 0; j < (sizeof(BattlegroundVehicleSpellImmunity) / sizeof(uint32)); ++j)
                    pCreature->ApplySpellImmune(0, IMMUNITY_EFFECT, BattlegroundVehicleSpellImmunity[i], true);
            }
            return;
        }
    }
}

void BattlegroundSA::HandleKillUnit(Creature* pCreature, Player* pPlayer)
{
    if (pCreature && pCreature->GetEntry() == NPC_DEMOLISHER_SA)
    {
        UpdatePlayerScore(pPlayer, SCORE_DESTROYED_DEMOLISHER, 1);
        notEvenAScratch[attackers] = false;

        // Add Demolisher to the correct respawn/delete list
        uint32 type = GetCreatureType(pCreature->GetGUID());

        if (type >= BG_SA_BEACH_DEMOLISHER_1 && type <= BG_SA_BEACH_DEMOLISHER_4)
            demoliserRespawnList[type] = BG_SA_RESPAWN_TIME_DEMOLISHER + getMSTime();

        if (type >= BG_SA_DEMOLISHER_1 && type <= BG_SA_DEMOLISHER_4)
            demoliserDeleteList[type] = BG_SA_RESPAWN_TIME_DEMOLISHER + getMSTime();
    }
}

void BattlegroundSA::demolisherStartState(bool start)
{
    for (uint8 i = BG_SA_BEACH_DEMOLISHER_1; i <= BG_SA_BEACH_DEMOLISHER_4; i++)
    {
        if (Creature* pCreature = GetBGCreature(i))
        {
            if (start)
                pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
            else
                pCreature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
        }
    }
}

void BattlegroundSA::DestroyGate(Player* pPlayer, GameObject* pObj)
{
    uint32 gateId = getGateIdFromDamagedOrDestroyEventId(pObj->GetGOInfo()->building.destroyedEvent);
    if (!gateStatus[gateId])
        return;

    if (GameObject* gObj = GetBGObject(gateId))
    {
        if (gObj->GetGOValue()->Building.Health == 0)
        {
            bool rewardHonor = true;
            boolStateMap[STATE_ACHIEVMENT_DOTA] = true;

            switch (gateId)
            {
                case BG_SA_GREEN_GATE:
                    if (gateStatus[BG_SA_BLUE_GATE] == BG_SA_GATE_DESTROYED)
                        rewardHonor = false;
                    break;
                case BG_SA_BLUE_GATE:
                    if (gateStatus[BG_SA_GREEN_GATE] == BG_SA_GATE_DESTROYED)
                        rewardHonor = false;
                    break;
                case BG_SA_RED_GATE:
                    if (gateStatus[BG_SA_PURPLE_GATE] == BG_SA_GATE_DESTROYED)
                        rewardHonor = false;
                    break;
                case BG_SA_PURPLE_GATE:
                    if (gateStatus[BG_SA_RED_GATE] == BG_SA_GATE_DESTROYED)
                        rewardHonor = false;
                    break;
            }

            if (gateId < BG_SA_FRONT_GATE_MAX)
                DelObject(gateId + 9);

            UpdatePlayerScore(pPlayer, SCORE_DESTROYED_WALL, 1);

            if (rewardHonor)
                UpdatePlayerScore(pPlayer, SCORE_BONUS_HONOR, GetBonusHonorFromKill(1));

            // Call Event
            eventGateDestroyed(pObj, gateId);
        }
    }
}

WorldSafeLocsEntry const* BattlegroundSA::GetClosestGraveYard(Player* pPlayer)
{
    uint32 safeloc = 0;
    WorldSafeLocsEntry const* ret;
    WorldSafeLocsEntry const* closest;
    float dist, nearest;
    float x, y, z;

    pPlayer->GetPosition(x, y, z);

    if (pPlayer->GetTeamId() == attackers)
        safeloc = StrandOfTheAncientsGYEntries[BG_SA_BEACH_GY];
    else
        safeloc = StrandOfTheAncientsGYEntries[BG_SA_DEFENDER_LAST_GY];

    closest = sWorldSafeLocsStore.LookupEntry(safeloc);
    nearest = sqrt((closest->x - x)*(closest->x - x) + (closest->y - y)*(closest->y - y)+(closest->z - z)*(closest->z - z));

    for (uint8 i = BG_SA_EAST_CAPTURABLE_GY; i < BG_SA_MAX_GY; i++)
    {
        if (graveyardStatus[i] != pPlayer->GetTeamId())
            continue;

        ret = sWorldSafeLocsStore.LookupEntry(StrandOfTheAncientsGYEntries[i]);
        dist = sqrt((ret->x - x)*(ret->x - x) + (ret->y - y)*(ret->y - y)+(ret->z - z)*(ret->z - z));
        if (dist < nearest)
        {
            closest = ret;
            nearest = dist;
        }
    }

    return closest;
}

void BattlegroundSA::sendTime()
{
    uint32 end_of_round = (GETUINT32STATE(STATE_END_ROUND_TIMER) - GETUINT32STATE(STATE_TOTAL_TIME));
    UpdateWorldState(BG_SA_TIMER_MINS, end_of_round/60000);
    UpdateWorldState(BG_SA_TIMER_SEC_TENS, (end_of_round%60000)/10000);
    UpdateWorldState(BG_SA_TIMER_SEC_DECS, ((end_of_round%60000)%10000)/1000);
}

void BattlegroundSA::EventPlayerClickedOnFlag(Player* pPlayer, GameObject* pObj)
{
    switch (pObj->GetEntry())
    {
        case 191307:
        case 191308:
            if (gateStatus[BG_SA_GREEN_GATE] == BG_SA_GATE_DESTROYED || gateStatus[BG_SA_BLUE_GATE] == BG_SA_GATE_DESTROYED)
                captureGraveyard(BG_SA_WEST_CAPTURABLE_GY, pPlayer);
            else
            {
                // Write message to player and log entry
                pPlayer->GetSession()->SendNotification(LANG_BG_SA_GY_CAPTURE_FAIL);
                sLog->outDebug(LOG_FILTER_WARDEN, "SOTA: Possible cheater detected while capturing the graveyard - playerGUID: %u, accountID: %u",
                    GUID_LOPART(pPlayer->GetGUID()), pPlayer->GetSession()->GetAccountId());

                SpawnBGObject(BG_SA_WEST_FLAG, RESPAWN_IMMEDIATELY);
            }
            break;
        case 191305:
        case 191306:
            if (gateStatus[BG_SA_GREEN_GATE] == BG_SA_GATE_DESTROYED || gateStatus[BG_SA_BLUE_GATE] == BG_SA_GATE_DESTROYED)
                captureGraveyard(BG_SA_EAST_CAPTURABLE_GY, pPlayer);
            else
            {
                // Write message to player and log entry
                pPlayer->GetSession()->SendNotification(LANG_BG_SA_GY_CAPTURE_FAIL);
                sLog->outDebug(LOG_FILTER_WARDEN, "SOTA: Possible cheater detected while capturing the graveyard - playerGUID: %u, accountID: %u",
                    GUID_LOPART(pPlayer->GetGUID()), pPlayer->GetSession()->GetAccountId());

                SpawnBGObject(BG_SA_EAST_FLAG, RESPAWN_IMMEDIATELY);
            }
            break;
        case 191310:
        case 191309:
            if ((gateStatus[BG_SA_GREEN_GATE] == BG_SA_GATE_DESTROYED || gateStatus[BG_SA_BLUE_GATE] == BG_SA_GATE_DESTROYED)
                && (gateStatus[BG_SA_RED_GATE] == BG_SA_GATE_DESTROYED || gateStatus[BG_SA_PURPLE_GATE] == BG_SA_GATE_DESTROYED))
                captureGraveyard(BG_SA_CENTRAL_CAPTURABLE_GY, pPlayer);
            else
            {
                // Write message to player and log entry
                pPlayer->GetSession()->SendNotification(LANG_BG_SA_GY_CAPTURE_FAIL);
                sLog->outDebug(LOG_FILTER_WARDEN, "SOTA: Possible cheater detected while capturing the graveyard - playerGUID: %u, accountID: %u",
                    GUID_LOPART(pPlayer->GetGUID()), pPlayer->GetSession()->GetAccountId());

                SpawnBGObject(BG_SA_CENTRAL_FLAG, RESPAWN_IMMEDIATELY);
            }
            break;
        default:
            return;
    };
}

void BattlegroundSA::captureGraveyard(BG_SA_Graveyards gId, Player* pPlayer)
{
    // Calculate graveyard type (spirit guide) and faction
    uint32 graveyardType = gId + BG_SA_MAXNPC;

    // Set new faction @todo
    if (graveyardStatus[gId] == pPlayer->GetTeamId())
        return;

    graveyardStatus[gId] = pPlayer->GetTeamId();

    // Relocate dead players
    relocateDeadPlayersAtGraveyard(gId);

    // Delete current graveyard npc
    DelCreature(graveyardType);

    WorldSafeLocsEntry const* worldSafeLoc = sWorldSafeLocsStore.LookupEntry(StrandOfTheAncientsGYEntries[gId]);
    if (!worldSafeLoc)
    {
        sLog->outError("BattlegroundSA::captureGraveyard: non-existant GY entry: %u", StrandOfTheAncientsGYEntries[gId]);
        return;
    }

    // Add new graveyard
    AddSpiritGuide(graveyardType, worldSafeLoc->x, worldSafeLoc->y, worldSafeLoc->z, StrandOfTheAncientsGYOrientation[gId],
        (graveyardStatus[gId] ? HORDE : ALLIANCE));

    // Show / Hide demolisher engineer
    switchDemolisherEngineer(gId);

    // Calculate flag index (base 15) + 2 (east) / 3 (west) / 4 (center)
    uint32 flagIndex = 15 + gId;

    // Set flag
    DelObject(flagIndex);
    if (!AddObject(flagIndex, (StrandOfTheAncientsObjectSpawns[flagIndex].entry - graveyardStatus[gId]),
        StrandOfTheAncientsObjectSpawns[flagIndex].x, StrandOfTheAncientsObjectSpawns[flagIndex].y,
        StrandOfTheAncientsObjectSpawns[flagIndex].z, StrandOfTheAncientsObjectSpawns[flagIndex].o,
        0, 0, 0, 0, RESPAWN_ONE_DAY))
    {
        sLog->outError("BattlegroundSA::captureGraveyard: Couldn't spawn new flag type %u", flagIndex);
        EndNow();
        return;
    }

    GameObject* pNewFlag = GetBGObject(flagIndex);
    if (!pNewFlag)
        EndNow();

    // Set correct faction for flag and
    pNewFlag->SetUInt32Value(GAMEOBJECT_FACTION, StrandOfTheAncientsFactions[graveyardStatus[gId] ? TEAM_HORDE : TEAM_ALLIANCE]);

    // Worldstates and warning
    switch (gId)
    {
        case BG_SA_EAST_CAPTURABLE_GY:
        {
            // Update world states
            UpdateWorldState(BG_SA_EAST_GY_ALLIANCE, (graveyardStatus[gId] == TEAM_ALLIANCE ? 1 : 0));
            UpdateWorldState(BG_SA_EAST_GY_HORDE, (graveyardStatus[gId] == TEAM_HORDE ? 1 : 0));

            // Send warning
            if (graveyardStatus[gId] == TEAM_ALLIANCE)
                SendWarningToAll(LANG_BG_SA_A_GY_EAST);
            else
                SendWarningToAll(LANG_BG_SA_H_GY_EAST);
        }break;

        case BG_SA_WEST_CAPTURABLE_GY:
        {
            // Update world states
            UpdateWorldState(BG_SA_WEST_GY_ALLIANCE, (graveyardStatus[gId] == TEAM_ALLIANCE ? 1 : 0));
            UpdateWorldState(BG_SA_WEST_GY_HORDE, (graveyardStatus[gId] == TEAM_HORDE ? 1 : 0));

            // Send warning
            if (graveyardStatus[gId] == TEAM_ALLIANCE)
                SendWarningToAll(LANG_BG_SA_A_GY_WEST);
            else
                SendWarningToAll(LANG_BG_SA_H_GY_WEST);
        }break;

        case BG_SA_CENTRAL_CAPTURABLE_GY:
        {
            // Update world states
            UpdateWorldState(BG_SA_CENTER_GY_ALLIANCE, (graveyardStatus[gId] == TEAM_ALLIANCE ? 1 : 0));
            UpdateWorldState(BG_SA_CENTER_GY_HORDE, (graveyardStatus[gId] == TEAM_HORDE ? 1 : 0));

            // Send warning
            if (graveyardStatus[gId] == TEAM_ALLIANCE)
                SendWarningToAll(LANG_BG_SA_A_GY_SOUTH);
            else
                SendWarningToAll(LANG_BG_SA_H_GY_SOUTH);
        }break;

        default:
            ASSERT(false);
            break;
    };
}

void BattlegroundSA::relocateDeadPlayersAtGraveyard(BG_SA_Graveyards gId)
{
    if (GetReviveQueueSize() == 0)
        return;

    if (Creature* spiritKeeper = GetBGCreature(gId + BG_SA_MAXNPC))
    {
        uint64 spiritGuid = spiritKeeper->GetGUID();
        if (m_ReviveQueue[spiritGuid].empty())
            return;

        WorldSafeLocsEntry const* graveyardLoc = NULL;

        for (std::vector<uint64>::const_iterator itr = m_ReviveQueue[spiritGuid].begin(); itr < m_ReviveQueue[spiritGuid].end(); ++itr)
        {
            Player* pPlayer = ObjectAccessor::FindPlayer(*itr);
            if (!pPlayer)
                continue;

            if (!graveyardLoc)
                graveyardLoc = GetClosestGraveYard(pPlayer);

            if (graveyardLoc)
                pPlayer->TeleportTo(GetMapId(), graveyardLoc->x, graveyardLoc->y, graveyardLoc->z, pPlayer->GetOrientation());
        }
    }
}

void BattlegroundSA::switchDemolisherEngineer(BG_SA_Graveyards gId)
{
    Creature* demolisherEngineer = NULL;
    uint32 textId;

    switch (gId)
    {
        case BG_SA_EAST_CAPTURABLE_GY:
            demolisherEngineer = GetBGCreature(BG_SA_NPC_SPARKLIGHT_EAST);
            textId = LANG_BG_SA_GY_EAST_ENGINEER_VISIBLE;
            break;

        case BG_SA_WEST_CAPTURABLE_GY:
            demolisherEngineer = GetBGCreature(BG_SA_NPC_RIGSPARK_WEST);
            textId = LANG_BG_SA_GY_WEST_ENGINEER_VISIBLE;
            break;

        default:
            return;
    }

    if (!demolisherEngineer)
        return;

    // If the offense captured the graveyard - set engineer visible - else set not visible
    if (graveyardStatus[gId] == attackers)
    {
        demolisherEngineer->SetVisible(true);
        demolisherEngineer->YellToZone(textId, LANG_UNIVERSAL, 0);
    }
    else
        demolisherEngineer->SetVisible(false);
}

void BattlegroundSA::EventPlayerUsedGO(Player* pPlayer, GameObject* pObj)
{
    if (pObj->GetEntry() == OBJECT_TITAN_RELIC && gateStatus[BG_SA_ANCIENT_GATE] == BG_SA_GATE_DESTROYED
        && gateStatus[BG_SA_YELLOW_GATE] == BG_SA_GATE_DESTROYED && (gateStatus[BG_SA_PURPLE_GATE] == BG_SA_GATE_DESTROYED 
        || gateStatus[BG_SA_RED_GATE] == BG_SA_GATE_DESTROYED) && (gateStatus[BG_SA_GREEN_GATE] == BG_SA_GATE_DESTROYED 
        || gateStatus[BG_SA_BLUE_GATE] == BG_SA_GATE_DESTROYED))
    {
        if (pPlayer->GetTeamId() == attackers)
        {
            if (pPlayer->GetTeamId() == TEAM_ALLIANCE)
                SendMessageToAll(LANG_BG_SA_ALLIANCE_CAPTURED_RELIC, CHAT_MSG_BG_SYSTEM_NEUTRAL);
            else
                SendMessageToAll(LANG_BG_SA_HORDE_CAPTURED_RELIC, CHAT_MSG_BG_SYSTEM_NEUTRAL);

            if (status == BG_SA_ROUND_ONE)
            {
                roundScores[0].winner = attackers;
                roundScores[0].time = GETUINT32STATE(STATE_TOTAL_TIME);

                //Achievement Storm the Beach (1310)
                for (BattlegroundPlayerMap::const_iterator itr = GetPlayers().begin(); itr != GetPlayers().end(); ++itr)
                {
                    if (Player* player = ObjectAccessor::FindPlayer(itr->first))
                        if (player->GetTeamId() == attackers)
                            player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, 65246);
                }

                initSecondRound();
            }
            else if (status == BG_SA_ROUND_TWO)
            {
                roundScores[1].winner = attackers;
                roundScores[1].time = GETUINT32STATE(STATE_TOTAL_TIME);
                toggleTimer();

                //Achievement Storm the Beach (1310)
                for (BattlegroundPlayerMap::const_iterator itr = GetPlayers().begin(); itr != GetPlayers().end(); ++itr)
                {
                    if (Player* player = ObjectAccessor::FindPlayer(itr->first))
                        if (player->GetTeamId() == attackers && roundScores[1].winner == attackers)
                            player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET, 65246);
                }

                finishBattleground();
            }
        }
    }
}

uint32 BattlegroundSA::GetData(uint32 type)
{
    switch (type)
    {
        case BG_SA_DATA_CAN_DEMOLISHER_BUILD:
            return checkDemolisherCount();
        case BG_SA_DATA_ATTACKER_TEAM:
            return attackers;
        case BG_SA_DATA_ACHIEVMENT_DOTA:
            return checkAchievmentCriteria(BG_SA_ACHIEVMENT_DEFENSE_OF_THE_ANCIENTS, NULL);
        case BG_SA_DATA_ACHIEVMENT_NEAS_A:
            return checkAchievmentCriteria(BG_SA_ACHIEVMENT_NOT_EVEN_A_SCRETCH, TEAM_ALLIANCE);
        case BG_SA_DATA_ACHIEVMENT_NEAS_H:
            return checkAchievmentCriteria(BG_SA_ACHIEVMENT_NOT_EVEN_A_SCRETCH, TEAM_HORDE);
        case BG_SA_DATA_DEMOLISHER_TIMER:
        {
            // Return seconds to activate new demolisher
            if (!demoliserDeleteList.empty())
                return (getMSTimeDiff(getMSTime(), demoliserDeleteList.begin()->second) / 1000);
            else
                return 0;
        }

        default:
            return 0;
    }
}

void BattlegroundSA::DoAction(uint32 type, uint64 value)
{
    switch (type)
    {
        case BG_SA_ACTION_BUILD_DEMOLISHER_ALLIANCE:
            spawnDemolisher(value, TEAM_ALLIANCE);
            break;

        case BG_SA_ACTION_BUILD_DEMOLISHER_HORDE:
            spawnDemolisher(value, TEAM_HORDE);
            break;

        default:
            break;
    }
}

void BattlegroundSA::toggleTimer()
{
    boolStateMap[STATE_TIMER_ENABLED] = !GETBOOLSTATE(STATE_TIMER_ENABLED);
    UpdateWorldState(BG_SA_ENABLE_TIMER, (GETBOOLSTATE(STATE_TIMER_ENABLED)) ? 1 : 0);
}

void BattlegroundSA::endBattleground(uint32 winnerTeam)
{
    // Honor reward for winning
    if (winnerTeam == ALLIANCE)
        RewardHonorToTeam(GetBonusHonorFromKill(1), ALLIANCE);
    else if (winnerTeam == HORDE)
        RewardHonorToTeam(GetBonusHonorFromKill(1), HORDE);

    // Complete map_end rewards (even if no team wins)
    RewardHonorToTeam(GetBonusHonorFromKill(2), ALLIANCE);
    RewardHonorToTeam(GetBonusHonorFromKill(2), HORDE);

    Battleground::EndBattleground(winnerTeam);
}

void BattlegroundSA::updateDemolisherSpawns()
{
    SotADemolisherMap::iterator itr;

    // Beach Demolisher
    if (!demoliserRespawnList.empty())
    {
        for (itr = demoliserRespawnList.begin(); itr != demoliserRespawnList.end();)
        {
            if (itr->second > getMSTime())
                ++itr;
            else
            {
                if (Creature* demolisher = GetBGCreature(itr->first))
                {
                    demolisher->Relocate(demolisher->GetHomePosition());
                    demolisher->Respawn();
                    demoliserRespawnList.erase(itr++);
                }
            }
        }
    }

    // Workshop Demolisher
    if (!demoliserDeleteList.empty())
    {
        for (itr = demoliserDeleteList.begin(); itr != demoliserDeleteList.end();)
        {
            if (itr->second > getMSTime())
                ++itr;
            else
            {
                DelCreature(itr->first);
                demoliserDeleteList.erase(itr++);
            }
        }
    }
}

void BattlegroundSA::startBGTransport()
{
    if (GETBOOLSTATE(STATE_SHIPS_STARTED))
        return;

    DoorOpen(BG_SA_BOAT_ONE);
    DoorOpen(BG_SA_BOAT_TWO);

    for (int i = BG_SA_BOAT_ONE; i <= BG_SA_BOAT_TWO; i++)
    {
        for (BattlegroundPlayerMap::const_iterator itr = GetPlayers().begin(); itr != GetPlayers().end(); ++itr)
        {
            if (Player* pPlayer = ObjectAccessor::FindPlayer(itr->first))
            {
                // Get updateData from Objects (updateBlock)
                UpdateData uData;
                GetBGObject(i)->BuildValuesUpdateBlockForPlayer(&uData, pPlayer);

                // Build packet and send it to player
                WorldPacket wPacket;
                uData.BuildPacket(&wPacket);
                pPlayer->SendDirectMessage(&wPacket);
            }
        }
    }

    boolStateMap[STATE_SHIPS_STARTED] = true;
}

void BattlegroundSA::sendBGTransport(bool remove)
{
    for (BattlegroundPlayerMap::const_iterator itr = GetPlayers().begin(); itr != GetPlayers().end(); ++itr)
    {
        if (Player* pPlayer = ObjectAccessor::FindPlayer(itr->first))
        {
            if (!remove)
                sendTransportInit(pPlayer);
            else
                sendTransportsRemove(pPlayer);
        }
    }

    if (remove)
        boolStateMap[STATE_SHIPS_STARTED] = false;
}

void BattlegroundSA::sendTransportInit(Player* pPlayer)
{
    if (BgObjects[BG_SA_BOAT_ONE] || BgObjects[BG_SA_BOAT_TWO])
    {
        // Get updateData from Objects (create)
        UpdateData uData;
        if (BgObjects[BG_SA_BOAT_ONE])
            GetBGObject(BG_SA_BOAT_ONE)->BuildCreateUpdateBlockForPlayer(&uData, pPlayer);
        if (BgObjects[BG_SA_BOAT_TWO])
            GetBGObject(BG_SA_BOAT_TWO)->BuildCreateUpdateBlockForPlayer(&uData, pPlayer);

        // Build packet and send it to player
        WorldPacket wPacket;
        uData.BuildPacket(&wPacket);
        pPlayer->SendDirectMessage(&wPacket);
    }
}

void BattlegroundSA::sendTransportsRemove(Player* pPlayer)
{
    if (BgObjects[BG_SA_BOAT_ONE] || BgObjects[BG_SA_BOAT_TWO])
    {
        // Get updateData from Objects
        UpdateData uData;
        if (BgObjects[BG_SA_BOAT_ONE])
            GetBGObject(BG_SA_BOAT_ONE)->BuildOutOfRangeUpdateBlock(&uData);
        if (BgObjects[BG_SA_BOAT_TWO])
            GetBGObject(BG_SA_BOAT_TWO)->BuildOutOfRangeUpdateBlock(&uData);

        // Build packet and send it to player
        WorldPacket wPacket;
        uData.BuildPacket(&wPacket);
        pPlayer->SendDirectMessage(&wPacket);
    }
}