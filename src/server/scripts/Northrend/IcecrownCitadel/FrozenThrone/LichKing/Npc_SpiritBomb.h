#ifndef __NPC_SPIRITBOMB_H_
#define __NPC_SPIRITBOMB_H_

#include "TheLichKing.h"

class NpcSpiritBombAI : public CreatureAI
        {
        public:
            NpcSpiritBombAI(Creature* creature) : CreatureAI(creature)
            {
            }

            void IsSummonedBy(Unit* /*summoner*/);
            void MovementInform(uint32 type, uint32 point);
            void AttackStart(Unit* /*victim*/);
            void UpdateAI(uint32 const /*diff*/);
        };

class NpcSpiritBomb : public CreatureScript
{
    public:
        NpcSpiritBomb() : CreatureScript("npc_spirit_bomb") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return GetIcecrownCitadelAI<NpcSpiritBombAI>(creature);
    }
};

#endif