/**
 * @file
 * @author TehKevin for Laenalith-WoW
 * @date 29.07.2014
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

#ifndef _NPC_WINTERGRASP_H
#define _NPC_WINTERGRASP_H

#include "BattlefieldMgr.h"
#include "Main_BattlefieldWG.h"
#include "Data_BattlefieldWG.h"

#define WG_NPC_QUEUE_FROST_ARMOR_TIMER  180000


enum WGGossipOptionId
{
    WG_GOSSIP_ENGINEER_BUILD_CATAPULT             = 0,
    WG_GOSSIP_ENGINEER_BUILD_DEMOLISHER           = 1,
    WG_GOSSIP_ENGINEER_BUILD_SIEGE_ENGINE         = 2,
    WG_GOSSIP_ENGINEER_CANNOT_BUILD               = 3
};

enum WGqueuenpctext
{
    WG_NPCQUEUE_TEXT_H_NOWAR                      = 14775,
    WG_NPCQUEUE_TEXT_H_QUEUE                      = 14790,
    WG_NPCQUEUE_TEXT_H_WAR                        = 14777,
    WG_NPCQUEUE_TEXT_A_NOWAR                      = 14782,
    WG_NPCQUEUE_TEXT_A_QUEUE                      = 14791,
    WG_NPCQUEUE_TEXT_A_WAR                        = 14781,
    WG_NPCQUEUE_TEXTOPTION_JOIN                   = 20077,
};

enum WGSpells
{
    // Demolisher engineer
    SPELL_BUILD_SIEGE_VEHICLE_FORCE_HORDE         = 61409,
    SPELL_BUILD_SIEGE_VEHICLE_FORCE_ALLIANCE      = 56662,
    SPELL_BUILD_CATAPULT_FORCE                    = 56664,
    SPELL_BUILD_DEMOLISHER_FORCE                  = 56659,
    SPELL_ACTIVATE_CONTROL_ARMS                   = 49899,

    // Spirit guide
    SPELL_CHANNEL_SPIRIT_HEAL                     = 22011,

   // Queue npc
    SPELL_FROST_ARMOR                             = 12544
};

enum QuestIds
{
    QUEST_BONES_AND_ARROWS_HORDE_ATT              = 13193,
    QUEST_JINXING_THE_WALLS_HORDE_ATT             = 13202,
    QUEST_SLAY_THEM_ALL_HORDE_ATT                 = 13180,
    QUEST_FUELING_THE_DEMOLISHERS_HORDE_ATT       = 13200,
    QUEST_HEALING_WITH_ROSES_HORDE_ATT            = 13201,
    QUEST_DEFEND_THE_SIEGE_HORDE_ATT              = 13223,

    QUEST_BONES_AND_ARROWS_HORDE_DEF              = 13199,
    QUEST_WARDING_THE_WALLS_HORDE_DEF             = 13192,
    QUEST_SLAY_THEM_ALL_HORDE_DEF                 = 13178,
    QUEST_FUELING_THE_DEMOLISHERS_HORDE_DEF       = 13191,
    QUEST_HEALING_WITH_ROSES_HORDE_DEF            = 13194,
    QUEST_TOPPLING_THE_TOWERS_HORDE_DEF           = 13539,
    QUEST_STOP_THE_SIEGE_HORDE_DEF                = 13185,

    QUEST_BONES_AND_ARROWS_ALLIANCE_ATT           = 13196,
    QUEST_WARDING_THE_WARRIORS_ALLIANCE_ATT       = 13198,
    QUEST_NO_MERCY_FOR_THE_MERCILESS_ALLIANCE_ATT = 13179,
    QUEST_DEFEND_THE_SIEGE_ALLIANCE_ATT           = 13222,
    QUEST_A_RARE_HERB_ALLIANCE_ATT                = 13195,

    QUEST_BONES_AND_ARROWS_ALLIANCE_DEF           = 13154,
    QUEST_WARDING_THE_WARRIORS_ALLIANCE_DEF       = 13153,
    QUEST_NO_MERCY_FOR_THE_MERCILESS_ALLIANCE_DEF = 13177,
    QUEST_SHOUTHERN_SABOTAGE_ALLIANCE_DEF         = 13538,
    QUEST_STOP_THE_SIEGE_ALLIANCE_DEF             = 13186,
    QUEST_A_RARE_HERB_ALLIANCE_DEF                = 13156,
};


/** Used to spawn wintergrasp vehicle at capturable points while battle */
class npc_wg_demolisher_engineer : public CreatureScript
{
    public:

        npc_wg_demolisher_engineer() : CreatureScript("npc_wg_demolisher_engineer") { }

        bool OnGossipHello(Player* pPlayer, Creature* pCreature);
        bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 /*sender*/, uint32 action);

    private:

        bool CanBuild(Creature* pCreature);
};

/** Used to relocate player to a different graveyard */
class npc_wg_spirit_guide : public CreatureScript
{
    public:

        npc_wg_spirit_guide() : CreatureScript("npc_wg_spirit_guide") { }

        bool OnGossipHello(Player* pPlayer, Creature* pCreature);
        bool OnGossipSelect(Player* pPlayer, Creature* /*pCreature*/, uint32 /*sender*/, uint32 action);

        struct npc_wg_spirit_guideAI : public ScriptedAI
        {
            npc_wg_spirit_guideAI(Creature* pCreature) : ScriptedAI(pCreature) { }

            void UpdateAI(const uint32 /*diff*/);
        };

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_wg_spirit_guideAI(pCreature);
        }
};

/** Used to get into wintergrasp queue (Dalaran) */
class npc_wg_queue : public CreatureScript
{
    public:

        npc_wg_queue() : CreatureScript("npc_wg_queue") { }

        bool OnGossipHello(Player* pPlayer, Creature* pCreature);
        bool OnGossipSelect(Player* pPlayer, Creature* /*pCreature*/, uint32 /*sender*/, uint32 /*action*/);

        struct npc_wg_queueAI : public ScriptedAI
        {
            npc_wg_queueAI(Creature* pCreature) : ScriptedAI(pCreature) { }

            uint32 FrostArmor_Timer;

            void Reset();

            void EnterCombat(Unit* /*who*/) { }

            void UpdateAI(const uint32 diff);
        };

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new npc_wg_queueAI(pCreature);
        }
};


#endif // _NPC_WINTERGRASP_H