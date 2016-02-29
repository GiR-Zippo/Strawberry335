#ifndef __NPC_DARKNUCLEUS_H_
#define __NPC_DARKNUCLEUS_H_

#include "BloodPrinceCouncil.h"

class NpcDarkNucleusAI : public ScriptedAI
        {
        public:
            NpcDarkNucleusAI(Creature* creature) : ScriptedAI(creature)
            {
                _lockedTarget = false;
                _targetAuraCheck = 0;
            }

            void Reset();
            void EnterCombat(Unit* who);
            void MoveInLineOfSight(Unit* who);
            void DamageTaken(Unit* attacker, uint32& /*damage*/);
            void UpdateAI(uint32 const diff);

        private:
            uint32 _targetAuraCheck;
            bool _lockedTarget;
        };

class NpcDarkNucleus : public CreatureScript
{
    public:
        NpcDarkNucleus() : CreatureScript("npc_dark_nucleus") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return GetIcecrownCitadelAI<NpcDarkNucleusAI>(creature);
    }
};

#endif