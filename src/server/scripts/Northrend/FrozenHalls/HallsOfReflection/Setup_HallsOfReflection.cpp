/**
 * @file
 * @author TehKevin for Laenalith-WoW
 * @date 22.09.2014
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

#include "FalricMarwynEvent/Falric/Boss_Falric.h"
#include "FalricMarwynEvent/Marwyn/Boss_Marwyn.h"
#include "FalricMarwynEvent/Intro_FalricMarwynEvent.h"
#include "FalricMarwynEvent/Npc_FalricMarwynEvent.h"

#include "Frostworn/Npc_Frostworn.h"

#include "TheLichKingHor/Boss_TheLichKingHor.h"
#include "TheLichKingHor/Npc_TheLichKingHor.h"
#include "TheLichKingHor/Event_TheLichKingHor.h"

#include "Special/Misc_HallsOfReflection.h"
#include "Special/Spell_HallsOfReflection.h"

#include "Instance_HallsOfReflection.h"


void AddSC_halls_of_reflection()
{
    // Instance
    new Instance_HallsOfReflection();

    // FalricMarwynEvent
    new Npc_JainaOrSylvanasHor_FirstPart(false, "npc_jaina_hor_part1");
    new Npc_JainaOrSylvanasHor_FirstPart(true, "npc_sylvanas_hor_part1");
    new Boss_Falric();
    new Boss_Marwyn();
    new Npc_GhostlyPriest();
    new Npc_PhantomMage();
    new Npc_PhantomHallucination();
    new Npc_ShadowyMercenary();
    new Npc_SpectralFootman();
    new Npc_TorturedRifleman();

    // Frostworn
    new Npc_FrostwornGeneral();
    new Npc_SpiritualReflection();

    // The Lich King Hor
    new Npc_JainaOrSylvanasHor_SecondPart();
    new Boss_TheLichKingHor();
    new Npc_Abon();
    new Npc_RagingGhoul();
    new Npc_RisenWitchDoctor();

    // Special
    new At_WavesRestarterHor();
    new Npc_Queldelar();
    new NpcFrostmourneHor();

    // Spells
    new Spell_HorStartQuest();
    new Spell_HorGunshipCannonFire();
    new Spell_HorSummonReflections();
}