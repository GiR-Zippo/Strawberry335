/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ScriptedGossip.h"
#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "IcecrownCitadel.h"
#include "Spell.h"
#include <Group.h>

#define GOSSIP_SENDER_ICC_PORT 631

class icecrown_citadel_teleport : public GameObjectScript
{
    public:
        icecrown_citadel_teleport() : GameObjectScript("icecrown_citadel_teleport") { }

        bool OnGossipHello(Player* player, GameObject* go)
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Zu Hammer des Lichts teleportieren.", GOSSIP_SENDER_ICC_PORT, LIGHT_S_HAMMER_TELEPORT);
            if (InstanceScript* instance = go->GetInstanceScript())
            {
                if (player->isGameMaster())
                {
                    const char* LadyDeathwhisper =      (instance->GetBossState(DATA_LORD_MARROWGAR) == DONE)         ? "Zum Oratorium der Verdammten teleportieren. [OPEN]"      : "Zum Oratorium der Verdammten teleportieren. [LOCKED]";
                    const char* RampertOfSkulls =       (instance->GetBossState(DATA_LADY_DEATHWHISPER) == DONE)      ? "Zum Schädelbollwerk teleportieren. [OPEN]"               : "Zum Schädelbollwerk teleportieren. [LOCKED]";
                    const char* DeathbringersRise =     (instance->GetBossState(DATA_GUNSHIP_EVENT) == DONE)          ? "Zum Dom des Todesbringers teleportieren. [OPEN]"         : "Zum Dom des Todesbringers teleportieren. [LOCKED]";
                    const char* UpperSpire =            (instance->GetData(DATA_COLDFLAME_JETS) == DONE)              ? "Zur Oberen Spitze teleportieren. [OPEN]"                 : "Zur Oberen Spitze teleportieren. [LOCKED]";
                    const char* SindragosasLair =       (instance->GetBossState(DATA_VALITHRIA_DREAMWALKER) == DONE)  ? "Zu Sindragosas Hort teleportieren. [OPEN]"               : "Zu Sindragosas Hort teleportieren. [LOCKED]";

                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, LadyDeathwhisper, GOSSIP_SENDER_ICC_PORT, ORATORY_OF_THE_DAMNED_TELEPORT);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, RampertOfSkulls, GOSSIP_SENDER_ICC_PORT, RAMPART_OF_SKULLS_TELEPORT);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, DeathbringersRise, GOSSIP_SENDER_ICC_PORT, DEATHBRINGER_S_RISE_TELEPORT);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, UpperSpire, GOSSIP_SENDER_ICC_PORT, UPPER_SPIRE_TELEPORT);
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, SindragosasLair, GOSSIP_SENDER_ICC_PORT, SINDRAGOSA_S_LAIR_TELEPORT);
                }
                else
                {
                    if (instance->GetBossState(DATA_LORD_MARROWGAR) == DONE)
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Zum Oratorium der Verdammten teleportieren.", GOSSIP_SENDER_ICC_PORT, ORATORY_OF_THE_DAMNED_TELEPORT);
                    if (instance->GetBossState(DATA_LADY_DEATHWHISPER) == DONE)
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Zum Schädelbollwerk teleportieren.", GOSSIP_SENDER_ICC_PORT, RAMPART_OF_SKULLS_TELEPORT);
                    if (instance->GetBossState(DATA_GUNSHIP_EVENT) == DONE)
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Zum Dom des Todesbringers teleportieren.", GOSSIP_SENDER_ICC_PORT, DEATHBRINGER_S_RISE_TELEPORT);
#ifdef SKIP_UPPER_SPIRE_HC
                    if (!player->GetMap()->IsHeroic())
#endif
                    if (instance->GetData(DATA_COLDFLAME_JETS) == DONE)
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Zur Oberen Spitze teleportieren.", GOSSIP_SENDER_ICC_PORT, UPPER_SPIRE_TELEPORT);
                    // TODO: Gauntlet event before Sindragosaa
#ifdef SKIP_UPPER_SPIRE_HC
                    if (!player->GetMap()->IsHeroic())
#endif
                    if (instance->GetBossState(DATA_SINDRAGOSA_GAUNTLET) == DONE)
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Zu Sindragosas Hort teleportieren.", GOSSIP_SENDER_ICC_PORT, SINDRAGOSA_S_LAIR_TELEPORT);
                }
            }

            player->SEND_GOSSIP_MENU(player->GetGossipTextId(go), go->GetGUID());
            return true;
        }

        bool OnGossipSelect(Player* player, GameObject* /*go*/, uint32 sender, uint32 action)
        {
            player->PlayerTalkClass->ClearMenus();
            player->CLOSE_GOSSIP_MENU();

            SpellInfo const* spell = sSpellMgr->GetSpellInfo(action);
            if (!spell)
                return false;

            if (player->isInCombat())
            {
                Spell::SendCastResult(player, spell, 0, SPELL_FAILED_AFFECTING_COMBAT);
                return true;
            }

            if (sender == GOSSIP_SENDER_ICC_PORT)
                player->CastSpell(player, spell, true);

            return true;
        }
};

class at_frozen_throne_teleport : public AreaTriggerScript
{
    public:
        at_frozen_throne_teleport() : AreaTriggerScript("at_frozen_throne_teleport") { }

        bool OnTrigger(Player* player, AreaTriggerEntry const* /*areaTrigger*/)
        {
            if (player->isInCombat())
            {
                if (SpellInfo const* spell = sSpellMgr->GetSpellInfo(FROZEN_THRONE_TELEPORT))
                    Spell::SendCastResult(player, spell, 0, SPELL_FAILED_AFFECTING_COMBAT);
                return true;
            }

#ifdef SKIP_LK_HC
            if (!player->GetMap()->IsHeroic())
#endif
            if (InstanceScript* instance = player->GetInstanceScript())
                if (instance->GetBossState(DATA_PROFESSOR_PUTRICIDE) == DONE &&
                    instance->GetBossState(DATA_BLOOD_QUEEN_LANA_THEL) == DONE &&
                    instance->GetBossState(DATA_SINDRAGOSA) == DONE &&
                    instance->GetBossState(DATA_THE_LICH_KING) != IN_PROGRESS)
                    player->CastSpell(player, FROZEN_THRONE_TELEPORT, true);
            return true;
        }
};

void LoadIcecrownCitadelTeleport()
{
    new icecrown_citadel_teleport();
    new at_frozen_throne_teleport();
}