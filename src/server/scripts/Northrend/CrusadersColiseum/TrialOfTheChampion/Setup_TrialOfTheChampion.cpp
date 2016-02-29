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

#include "Instance_TrialOfTheChampion.h"
#include "Npc_TrialOfTheChampion.h"

#include "GrandChampions/Boss_GrandChampions.h"
#include "GrandChampions/Vehicle_GrandChampions.h"
#include "GrandChampions/Spell_GrandChampions.h"

#include "ArgentumChallenge/Boss_ArgentumChallenge.h"
#include "ArgentumChallenge/Npc_ArgentumChallenge.h"
#include "ArgentumChallenge/Spell_ArgentumChallenge.h"

#include "BlackKnight/Boss_BlackKnight.h"
#include "BlackKnight/Npc_BlackKnight.h"

void AddSC_trial_of_the_champion()
{
    // Instance
    new Instance_TrialOfTheChampion();
    new Npc_TrialOfTheChampion_Announcer();


    // GrandChampions
    new Boss_GrandChampionWarrior();
    new Boss_GrandChampionMage();
    new Boss_GrandChampionShaman();
    new Boss_GrandChampionHunter();
    new Boss_GrandChampionRouge();

    new Npc_MountedChampion();

    new Spell_GrandChampions_RideMount();
    new Spell_GrandChampions_Defend();
    new Spell_GrandChampions_HexMending();


    // ArgentumChallenge
    new Boss_Eadric();
    new Boss_Paletress();

    new Npc_Memory();
    new Npc_ArgentMonk();
    new Npc_ArgentLightwielder();
    new Npc_ArgentPriest();

    new Spell_Eadric_Radiance();
    new Spell_Eadric_HammerOfRighteous();
    new Spell_Paletress_Shield();


    // BlackKnight
    new Boss_BlackKnight();

    new Npc_RisenGhoul();
    new Npc_BlackKnight_SkeletalGryphon();
}
