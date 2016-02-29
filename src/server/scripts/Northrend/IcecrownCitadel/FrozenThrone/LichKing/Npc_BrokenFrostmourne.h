#ifndef __NPC_BROKENFROSTMOURNE_H_
#define __NPC_BROKENFROSTMOURNE_H_

#include "TheLichKing.h"

class NpcBrokenFrostmourneAI : public CreatureAI
        {
        public:
            NpcBrokenFrostmourneAI(Creature* creature) : CreatureAI(creature)
            {
            }

            void Reset();
            void IsSummonedBy(Unit* /*summoner*/);
            void DoAction(int32 const action);
            void EnterEvadeMode();
            void UpdateAI(uint32 const diff);

        private:
            EventMap _events;
        };

class NpcBrokenFrostmourne : public CreatureScript
{
    public:
        NpcBrokenFrostmourne() : CreatureScript("npc_broken_frostmourne") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return GetIcecrownCitadelAI<NpcBrokenFrostmourneAI>(creature);
    }
};

#endif