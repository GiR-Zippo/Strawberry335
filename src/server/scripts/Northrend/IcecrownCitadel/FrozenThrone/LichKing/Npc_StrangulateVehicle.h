#ifndef __NPC_STRANGULATEVEHICLE_H_
#define __NPC_STRANGULATEVEHICLE_H_

#include "TheLichKing.h"

class NpcStrangulateVehicleAI : public ScriptedAI
        {
        public:
            NpcStrangulateVehicleAI(Creature* creature) : ScriptedAI(creature),
                _instance(creature->GetInstanceScript())
            {
            }

            void IsSummonedBy(Unit* summoner);
            void DoAction(int32 const action);
            void UpdateAI(uint32 const diff);

        private:
            EventMap _events;
            InstanceScript* _instance;
        };

class NpcStrangulateVehicle : public CreatureScript
{
    public:
        NpcStrangulateVehicle() : CreatureScript("npc_strangulate_vehicle") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return GetIcecrownCitadelAI<NpcStrangulateVehicleAI>(creature);
    }
};

#endif