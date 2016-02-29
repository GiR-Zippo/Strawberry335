#ifndef __NPC_BALLOFFLAME_H_
#define __NPC_BALLOFFLAME_H_

#include "BloodPrinceCouncil.h"

class NpcBallOfFlameAI : public ScriptedAI
        {
        public:
            NpcBallOfFlameAI(Creature* creature) : ScriptedAI(creature), _instance(creature->GetInstanceScript())
            {
                _despawnTimer = 0;
            }

            void Reset();
            void MovementInform(uint32 type, uint32 id);
            void SetGUID(uint64 guid, int32 /*type*/);
            void DoAction(int32 const action);
            void DamageDealt(Unit* /*target*/, uint32& damage, DamageEffectType damageType);
            void UpdateAI(uint32 const diff);

        private:
            uint64 _chaseGUID;
            InstanceScript* _instance;
            uint32 _despawnTimer;
        };

class NpcBallOfFlame : public CreatureScript
{
    public:
        NpcBallOfFlame() : CreatureScript("npc_ball_of_flame") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return GetIcecrownCitadelAI<NpcBallOfFlameAI>(creature);
    }
};

#endif