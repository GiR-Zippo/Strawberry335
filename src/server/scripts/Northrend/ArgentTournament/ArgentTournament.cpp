/*
 * Copyright (C) 2012 Zirkon
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

void AddTournamentBase();
void AddTournamentPhase1();
void AddTournamentPhase2();
void AddTournamentDailies();

void AddSC_Argen_Tournament()
{
    AddTournamentBase();
    AddTournamentPhase1();
    AddTournamentPhase2();
    AddTournamentDailies();
}

//Old scripts
/*
    new npc_chillmaw;
    new npc_keritose;
    new npc_valiant;
    new npc_vile;
    new npc_lady_nightswood;
    new npc_leaper;
    new npc_ValiantGrandMelee;
    new npc_squire_danny;
    new npc_argent_champion;
    new npc_maloric;
    new npc_Scarlet_Onslaught;
    new npc_apprentice_osterkilgr;
    new npc_dying_soldier;
    new npc_dying_berserker;
*/