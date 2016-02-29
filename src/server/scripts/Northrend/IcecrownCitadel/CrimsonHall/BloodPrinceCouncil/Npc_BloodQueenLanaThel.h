#ifndef __NPC_BLOODQUEENLANATHEL_H_
#define __NPC_BLOODQUEENLANATHEL_H_

#include "BloodPrinceCouncil.h"

class NpcBloodQueenLanaThelAI : public ScriptedAI
{
public:
    NpcBloodQueenLanaThelAI(Creature* creature) : ScriptedAI(creature)
    {
        _introDone = false;
        _instance = creature->GetInstanceScript();
    }

    void Reset();
    void MoveInLineOfSight(Unit* who);
    void MovementInform(uint32 type, uint32 id);
    void UpdateAI(uint32 const diff);

private:
    EventMap _events;
    InstanceScript* _instance;
    bool _introDone;
};

class NpcBloodQueenLanaThel : public CreatureScript
{
    public:
        NpcBloodQueenLanaThel() : CreatureScript("npc_blood_queen_lana_thel") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return GetIcecrownCitadelAI<NpcBloodQueenLanaThelAI>(creature);
    }
};

#endif