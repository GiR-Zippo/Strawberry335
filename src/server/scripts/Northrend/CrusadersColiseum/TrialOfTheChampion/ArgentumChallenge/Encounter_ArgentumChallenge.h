/**
 * @file
 * @author TehKevin for Laenalith-WoW
 * @date 26.08.2014
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

#ifndef _ENCOUNTER_ARGENTUM_CHALLENGE_H
#define _ENCOUNTER_ARGENTUM_CHALLENGE_H

#include "../Encounter_TrialOfTheChampion.h"


typedef std::pair<uint32, Position> EntryAndPositionPair;

class Encounter_ArgentumChallenge : public Encounter_TrialOfTheChampion
{
    public:

        Encounter_ArgentumChallenge(InstanceScript* instance)
            : Encounter_TrialOfTheChampion(instance, false) { }

        void DoAction(int32 /*action*/) override;
        void OnFail() override;

    private:

        void onDataChanged(uint32 type) override;
        void onStart() override;

        void executeIntro(uint32 eventID) override;

        /**
         * Used to spawn adds for specified index
         *
         * @param pCreature : Orientation (Target) trigger
         * @param index : Base NULL (MAX: 2)
         */
        void spawnAdds(Creature* pCreature, uint8 index);

        /**
         * Used to refresh the position of the adds
         *
         * @param pCreature : Orientation trigger
         * @param maxIndex : How many add groups to proceed (MAX: 3)
         */
        void refreshAdds(Creature* pCreature, uint8 maxIndex);

        uint64 addsGUID[3][3];
        EntryAndPositionPair addsEntryAndPositionData[3][3];
        uint64 bossGUID;
};


#endif // _ENCOUNTER_ARGENTUM_CHALLENGE_H
