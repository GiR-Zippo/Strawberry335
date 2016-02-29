/**
 * @file
 * @author TehKevin for Laenalith-WoW
 * @date 11.10.2014
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

#ifndef _ENCOUNTER_THELICHKINGHOR_H
#define _ENCOUNTER_THELICHKINGHOR_H

#include "../HallsOfReflection.h"
#include "../Encounter_HallsOfReflection.h"
#include "Transport.h"


namespace ENCOUNTER_THELICHKINGHOR
{
    #define GET_CREATURE_OUTRO(x)       x = pInstanceMap->GetCreature(pInstance->GetData64(NPC_JAINA_OR_SYLVANAS_O))
    #define GET_CREATURE_LK(x)          x = pInstanceMap->GetCreature(pInstance->GetData64(BOSS_LICH_KING))

    enum Says
    {
        SAY_LICH_KING_AGGRO_A           = -1594477,
        SAY_LICH_KING_AGGRO_H           = -1594478,
        SAY_JAINA_AGGRO                 = -1594479,
        SAY_SYLVANA_AGGRO               = -1594480,

        SAY_JAINA_WALL_01               = -1594487,
        SAY_SYLVANA_WALL_01             = -1594488,
        SAY_JAINA_WALL_02               = -1594489,
        SAY_SYLVANA_WALL_02             = -1594490,
        SAY_LICH_KING_WALL_02           = -1594491,
        SAY_LICH_KING_WALL_03           = -1594492,
        SAY_LICH_KING_WALL_04           = -1594493,
        SAY_JAINA_WALL_03               = -1594494,
        SAY_JAINA_WALL_04               = -1594495,
        SAY_SYLVANA_WALL_03             = -1594496,
        SAY_SYLVANA_WALL_04             = -1594497,
        SAY_JAINA_ESCAPE_01             = -1594498,
        SAY_JAINA_ESCAPE_02             = -1594499,
        SAY_SYLVANA_ESCAPE_01           = -1594500,
        SAY_SYLVANA_ESCAPE_02           = -1594501,
        SAY_JAINA_TRAP                  = -1594502,
        SAY_SYLVANA_TRAP                = -1594503,
        SAY_MATHEAS_JAINA               = -1594505,
        SAY_JAINA_FIRE                  = -1594526,
        SAY_SYLVANA_FIRE                = -1594522,
        SAY_SYLVANA_FINAL_1             = -1594523,
        SAY_SYLVANA_FINAL_2             = -1594521,
        SAY_JAINA_FINAL_1               = -1594527,
        SAY_JAINA_FINAL_2               = -1594524,
        SAY_JAINA_FINAL_3               = -1594525,
        SAY_LICH_KING_END_01            = -1594506,
        SAY_LICH_KING_END_02            = -1594507,
        SAY_LICH_KING_END_03            = -1594508,
        SAY_LICH_KING_END_DUN           = -1594504
    };

    enum Spells
    {
        SPELL_FURY_OF_FROSTMOURNE       = 70063,
        SPELL_ICE_PRISON                = 69708,
        SPELL_DARK_ARROW                = 70194,
        SPELL_ICE_BARRIER               = 69787,
        SPELL_SYLVANA_JUMP              = 68339,
        SPELL_SYLVANA_STEP              = 69087,
        SPELL_FIRE_CANNON               = 67461,
        SPELL_HARVEST_SOUL              = 70070,

        // Quest Credits
        SPELL_CREDIT_FINDING_SYLVANAS   = 71536,
        SPELL_CREDIT_FINDING_JAINA      = 71538,
        SPELL_CREDIT_ESCAPING_ARTHAS    = 71352,
    };

    enum EncounterActions
    {
        ENCOUNTER_ACTION_START_COMBAT   = ENCOUNTER_PREFIX + 1,
        ENCOUNTER_ACTION_START_EVENT    = ENCOUNTER_PREFIX + 2,
        ENCOUNTER_ACTION_PREPARE_WALL   = ENCOUNTER_PREFIX + 3,
        ENCOUNTER_ACTION_REACHED_WALL   = ENCOUNTER_PREFIX + 4,
        ENCOUNTER_ACTION_NEXT_WALL      = ENCOUNTER_PREFIX + 5,
        ENCOUNTER_ACTION_LAST_WALL      = ENCOUNTER_PREFIX + 6,
        ENCOUNTER_ACTION_LICH_KING_WIN  = ENCOUNTER_PREFIX + 7,
        ENCOUNTER_ACTION_DESPAWN_ADDS   = ENCOUNTER_PREFIX + 8,
        ENCOUNTER_ACTION_START_OUTRO    = ENCOUNTER_PREFIX + 9,
    };

    enum EncounterData
    {
        ENCOUNTER_DATA_ADDS_DEFEATED    = ENCOUNTER_PREFIX + 1,
        ENCOUNTER_DATA_WAVE_COUNT       = ENCOUNTER_PREFIX + 2
    };

    enum EncounterDataValues
    {
        ENCOUNTER_DATA_VALUE_CALC       = 2,
        ENCOUNTER_DATA_VALUE_WAVES      = 4,
        ENCOUNTER_DATA_VALUE_ADDS       = 7
    };

    enum EncounterMovementPoints
    {
        ENCOUNTER_M_P_AFTER_INTRO       = ENCOUNTER_PREFIX + 1
    };

    const uint32 JainaOrSylvanaWallText[2][ENCOUNTER_THELICHKINGHOR::ENCOUNTER_DATA_VALUE_WAVES] =
    {
        {
            SAY_JAINA_WALL_01,
            SAY_JAINA_WALL_02,
            SAY_JAINA_WALL_03,
            SAY_JAINA_WALL_04
        },
        {
            SAY_SYLVANA_WALL_01,
            SAY_SYLVANA_WALL_02,
            SAY_SYLVANA_WALL_03,
            SAY_SYLVANA_WALL_04
        }
    };

    const uint32 LichKingWallText[ENCOUNTER_DATA_VALUE_WAVES]
    {
        NULL,
        SAY_LICH_KING_WALL_02,
        SAY_LICH_KING_WALL_03,
        SAY_LICH_KING_WALL_04
    };

    const Position OutroSpawns[2] =
    {
        { 5551.38916f, 2261.13012f, 733.01123f, 0.79314f },  // Jaina / Lich King
        { 5554.29834f, 2263.92993f, 733.01123f, 3.97357f }  // Lich King / Sylvana
    };

    const Position JainaOrSylvanasRunPosition   = { 5577.187f, 2236.003f, 733.012f, 3.23f };

    const Position IceWallTargetPosition[] =
    {
        { 5547.833f, 2083.701f, 731.4332f, 0.729744f }, // 1st Icewall
        { 5496.554f, 1974.244f, 736.7929f, 0.943581f }, // 2nd Icewall
        { 5431.675f, 1874.450f, 752.6858f, 0.982852f }, // 3rd Icewall
        { 5318.289f, 1749.184f, 771.9423f, 0.872664f }  // 4th Icewall
    };
}

class Encounter_TheLichKingHor : public Encounter_HallsOfReflection
{
    public:

        Encounter_TheLichKingHor(InstanceScript* instance)
            : Encounter_HallsOfReflection(instance, false, true) { }

        void OnFail(bool handleReal = true) override;
        void DoAction(int32 action) override;

    private:

        void onDataChanged(uint32 type) override;
        void onStart() override;

        void executeIntro(uint32 eventID) override;
        void executeOutro(uint32 eventID) override;

        uint64 iceWallTargetGUID;
};


class WallDeleteDelayEvent : public BasicEvent
{
    public:

        WallDeleteDelayEvent(Creature* owner, uint64 goGUID) : pCreature(owner), gameobjectGUID(goGUID) { }

        void Delete()
        {
            if (GameObject* pObject = sObjectAccessor->GetGameObject(*pCreature, gameobjectGUID))
                pObject->Delete();

            pCreature->DisappearAndDie();
        }

        bool Execute(uint64 /*execTime*/, uint32 /*diff*/) override
        {
            Delete();
            return true;
        }

        void Abort(uint64 /*execTime*/) override
        {
            Delete();
        }

    private:

        Creature* pCreature;
        uint64 gameobjectGUID;
};


#endif // _ENCOUNTER_THELICHKINGHOR_H
