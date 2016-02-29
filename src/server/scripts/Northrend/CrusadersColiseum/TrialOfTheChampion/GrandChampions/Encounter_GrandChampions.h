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

#ifndef _ENCOUNTER_GRAND_CHAMPIONS_H
#define _ENCOUNTER_GRAND_CHAMPIONS_H

#include "../Encounter_TrialOfTheChampion.h"


class Encounter_GrandChampions : public Encounter_TrialOfTheChampion
{
    public:

        Encounter_GrandChampions(InstanceScript* instance)
            : Encounter_TrialOfTheChampion(instance, true) { }

        void OnFail() override;

    private:

        void onDataChanged(uint32 type) override;
        void onStart() override;

        void executeIntro(uint32 eventID) override;
        void executePreEncounter(uint32 eventID) override;

        uint32 getAddEntry(uint32 bossEntry);
        const Position& getBossPosition(uint8 positionId) const;

        void setGrandChampionsForEncounter();

        /**
         * Used to spawn boss + adds for specified index
         *
         * @param pAnnouncer : Global announcer
         * @param index : Base NULL (MAX: 2)
         */
        void spawnGroup(Creature* pAnnouncer, uint8 index);

        /**
         * Used to refresh the position of boss + adds
         *
         * @param pCreature : Orientation trigger
         * @param maxIndex : How many groups to proceed (MAX: 3)
         */
        void refreshGroup(Creature* pCreature, uint8 maxIndex);

        void startAdds(uint8 bossId);

        uint32 bossEntry[3];

        uint64 bossGUID[3];
        uint64 addsGUID[3][3];
};


#endif // _ENCOUNTER_GRAND_CHAMPIONS_H
