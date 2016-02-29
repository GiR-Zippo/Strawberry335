/**
 * @file
 * @author TehKevin for Laenalith-WoW
 * @date 10.10.2014
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

#ifndef _ENCOUNTER_FALRIC_MARWYN_EVENT_H
#define _ENCOUNTER_FALRIC_MARWYN_EVENT_H

#include "../HallsOfReflection.h"
#include "../Encounter_HallsOfReflection.h"


namespace ENCOUNTER_FALRIC_MARWYN_EVENT
{
    #define GET_CREATURE_INTRO(x)           x = pInstanceMap->GetCreature(pInstance->GetData64(NPC_JAINA_OR_SYLVANAS))
    #define BOSS_INDEX_FALRIC               NULL
    #define BOSS_INDEX_MARWYN               1
    #define TO_RADIAN(x)                    2 * M_PI / 360 * x

    enum Says
    {
        // Part 1
        SAY_JAINA_INTRO_1                   = -1668001,
        SAY_JAINA_INTRO_2                   = -1668002,
        SAY_JAINA_INTRO_3                   = -1668003,
        SAY_JAINA_INTRO_4                   = -1668004,
        SAY_UTHER_INTRO_A2_1                = -1668005,
        SAY_JAINA_INTRO_5                   = -1668006,
        SAY_UTHER_INTRO_A2_2                = -1668007,
        SAY_JAINA_INTRO_6                   = -1668008,
        SAY_UTHER_INTRO_A2_3                = -1668009,
        SAY_JAINA_INTRO_7                   = -1668010,
        SAY_UTHER_INTRO_A2_4                = -1668011,
        SAY_JAINA_INTRO_8                   = -1668012,
        SAY_UTHER_INTRO_A2_5                = -1668013,
        SAY_JAINA_INTRO_9                   = -1668014,
        SAY_UTHER_INTRO_A2_6                = -1668015,
        SAY_UTHER_INTRO_A2_7                = -1668016,
        SAY_JAINA_INTRO_10                  = -1668017,
        SAY_UTHER_INTRO_A2_8                = -1668018,
        SAY_JAINA_INTRO_11                  = -1668019,
        SAY_UTHER_INTRO_A2_9                = -1668020,

        SAY_SYLVANAS_INTRO_1                = -1668021,
        SAY_SYLVANAS_INTRO_2                = -1668022,
        SAY_SYLVANAS_INTRO_3                = -1668023,
        SAY_UTHER_INTRO_H2_1                = -1668024,
        SAY_SYLVANAS_INTRO_4                = -1668025,
        SAY_UTHER_INTRO_H2_2                = -1668026,
        SAY_SYLVANAS_INTRO_5                = -1668027,
        SAY_UTHER_INTRO_H2_3                = -1668028,
        SAY_SYLVANAS_INTRO_6                = -1668029,
        SAY_UTHER_INTRO_H2_4                = -1668030,
        SAY_SYLVANAS_INTRO_7                = -1668031,
        SAY_UTHER_INTRO_H2_5                = -1668032,
        SAY_UTHER_INTRO_H2_6                = -1668033,
        SAY_SYLVANAS_INTRO_8                = -1668034,
        SAY_UTHER_INTRO_H2_7                = -1668035,

        SAY_LK_INTRO_1                      = -1668036,
        SAY_LK_INTRO_2                      = -1668037,
        SAY_LK_INTRO_3                      = -1668038,
        SAY_FALRIC_INTRO_1                  = -1668039,
        SAY_MARWYN_INTRO_1                  = -1668040,
        SAY_FALRIC_INTRO_2                  = -1668041,

        SAY_JAINA_INTRO_END                 = -1668042,
        SAY_SYLVANAS_INTRO_END              = -1668043,
        SAY_LK_JAINA_INTRO_END              = -1594473,
        SAY_LK_SYLVANAS_INTRO_END           = -1594474,
    };

    enum Spells
    {
        // Intro
        SPELL_CAST_VISUAL                   = 65633, // Jaina/Sylavana lo lanzan para invocar a uther
        SPELL_UTHER_DESPAWN                 = 70693,
        SPELL_TAKE_FROSTMOURNE              = 72729,
        SPELL_FROSTMOURNE_DESPAWN           = 72726,
        SPELL_FROSTMOURNE_VISUAL            = 73220,
        SPELL_FROSTMOURNE_SOUNDS            = 70667,

        // Event
        SPELL_SPIRIT_ACTIVATE               = 72130,
        SPELL_WELL_OF_SOULS                 = 72630
    };

    enum EncounterData
    {
        ENCOUNTER_DATA_FENRIC               = ENCOUNTER_PREFIX + 1
    };

    enum EncounterEvents
    {
        ENCOUNTER_EVENT_WAIT_BOSS           = 100
    };

    enum EncounterPhases
    {
        // Intro
        ENCOUNTER_PHASE_INTRO_ALLIANCE      = 1,
        ENCOUNTER_PHASE_INTRO_HORDE         = 2,
        ENCOUNTER_PHASE_INTRO_LICH_KING     = 3,

        ENCOUNTER_PHASEMASK_INTRO_ALLIANCE  = (1 << (ENCOUNTER_PHASE_INTRO_ALLIANCE - 1)),
        ENCOUNTER_PHASEMASK_INTRO_HORDE     = (1 << (ENCOUNTER_PHASE_INTRO_HORDE- 1)),
        ENCOUNTER_PHASEMASK_INTRO_LICH_KING = (1 << (ENCOUNTER_PHASE_INTRO_LICH_KING - 1)),

        // PreEncounter
        ENCOUNTER_PHASE_FALRIC              = 1,
        ENCOUNTER_PHASE_MARWYN              = 2,
    };

    enum EncounterTimers
    {
        ENCOUNTER_TIMER_WAVE                = 45 * IN_MILLISECONDS,
        ENCOUNTER_TIMER_BOSS                = 60 * IN_MILLISECONDS
    };

    enum EncounterWaves
    {
        ENCOUNTER_WAVE_COUNT                = 4,
        ENCOUNTER_WAVE_NPC_COUNT            = 5
    };

    const uint32 EncounterWaveEntries[ENCOUNTER_WAVE_NPC_COUNT]
    {
        NPC_WAVE_MERCENARY,
        NPC_WAVE_FOOTMAN,
        NPC_WAVE_RIFLEMAN,
        NPC_WAVE_PRIEST,
        NPC_WAVE_MAGE
    };

    const float EncounterWaveRadius[2] = { 30.0f, 35.0f };
    const float EncounterWaveDegree[ENCOUNTER_WAVE_COUNT][ENCOUNTER_WAVE_NPC_COUNT] =
    {
        { 343.0f, 353.0f,   3.0f,  13.0f,  23.0f },
        {  70.0f,  80.0f,  90.0f, 100.0f, 110.0f },
        { 163.0f, 173.0f, 183.0f, 193.0f, 203.0f },
        { 250.0f, 260.0f, 270.0f, 280.0f, 290.0f }
    };

    const Position EncounterWaveSpawnPosition = { 5309.417969f, 2006.525513f, 707.695862f, 0.0f };

    const Position SpawnPos                 = { 5262.540527f, 1949.693726f, 707.695007f, 0.808736f }; // Jaina/Sylvanas Beginning Position
    const Position MoveDoorPos              = { 5268.254395f, 1955.522705f, 707.699585f, 1.277278f };
    const Position MoveThronePos            = { 5306.952148f, 1998.499023f, 709.341431f, 1.277278f }; // Jaina/Sylvanas walks to throne
    const Position UtherSpawnPos            = { 5308.310059f, 2003.857178f, 709.341431f, 4.650315f };

    const Position LichKingSpawnPos         = { 5362.917480f, 2062.307129f, 707.695374f, 3.945812f };
    const Position LichKingMoveThronePos    = { 5314.210450f, 2011.523920f, 709.341614f, 3.926883f }; // Lich King walks to throne
    const Position LichKingMoveAwayPos      = { 5400.069824f, 2102.7131689f, 707.69525f, 0.843803f }; // Lich King walks away

    const Position FalricEventPos           = { 5283.309f, 2031.173f, 709.319f, 5.53268f }; // Falric after spawn position
    const Position MarwynEventPos           = { 5335.585f, 1981.439f, 709.319f, 2.37538f }; // Marwyn after spawn position

    class SpawnWaveEvent : public BasicEvent
    {
        public:

            SpawnWaveEvent(Creature* owner) : pCreature(owner) { }

            bool Execute(uint64 /*time*/, uint32 /*diff*/)
            {
                pCreature->CastSpell(pCreature, SPELL_WELL_OF_SOULS);
                return true;
            }

        private:

            Creature* pCreature;
    };
}

class Encounter_FalricMarwynEvent : public Encounter_HallsOfReflection
{
    public:

        Encounter_FalricMarwynEvent(InstanceScript* instance)
            : Encounter_HallsOfReflection(instance, true, false) { }

        typedef std::set<uint64> SummonSet;

        void OnFail(bool handleReal = true) override;

    private:

        void onDataChanged(uint32 type) override;
        void onStart() override;

        void executeIntro(uint32 eventID) override;

        void executeIntroCommon(uint32 eventID);
        void executeIntroAlliance(uint32 eventID);
        void executeIntroHorde(uint32 eventID);
        void executeIntroLichKing(uint32 eventID);

        void executePreEncounter(uint32 eventID) override;

        void executePreEncounterFalric(uint32 eventID);
        void executePreEncounterMarwyn(uint32 eventID);

        void eventSpawnBoss(Creature* pCreature);

        /* Used to spawn 4 waves with 5 npcs
         *
         * @param bossID : Falric = 0 - Marwyn = 1
         */
        void spawnWaves(uint8 bossID);

        /* Used to desspawn npcs
         *
         * @param bossID : Falric = 0 - Marwyn = 1
         * @param force : despawn without conditions - default: false
         */
        void despawnWave(uint8 bossID, bool force = false);

        /* Used to activate 5 npcs with random wave position
         *
         * @param bossID : Falric = 0 - Marwyn = 1
         */
        void activateWave(uint8 bossID);
        void activateBoss();

        /* Used to reset bosses
         *
         * @param bossID : Falric = 0 - Marwyn = 1
         */
        void resetBoss(uint8 bossID);

        uint64 utherGUID;
        uint64 lichKingGUID;

        SummonSet summonSet[2][ENCOUNTER_FALRIC_MARWYN_EVENT::ENCOUNTER_WAVE_NPC_COUNT];
};


#endif // _ENCOUNTER_FALRIC_MARWYN_EVENT_H
