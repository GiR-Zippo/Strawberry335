#ifndef __NPC_RIMEFANG_H_
#define __NPC_RIMEFANG_H_

#include "Sindragosa.h"
#include "ScriptedCreature.h"
#include "Creature.h"

class NpcRimefangAI : public ScriptedAI
{
public:
    NpcRimefangAI(Creature* creature) : ScriptedAI(creature), _instance(creature->GetInstanceScript()), _summoned(false)
    {
    }

    void InitializeAI();
    void Reset();
    void JustRespawned();
    void JustDied(Unit* /*killer*/);
    void DoAction(int32 const action);
    void MovementInform(uint32 type, uint32 point);
    void EnterCombat(Unit* /*victim*/);
    void UpdateAI(uint32 const diff);

private:
    EventMap _events;
    InstanceScript* _instance;
    uint8 _icyBlastCounter;
    bool _summoned;
};

class NpcRimefang : public CreatureScript
{
    public:
        NpcRimefang() : CreatureScript("npc_rimefang_icc") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return GetIcecrownCitadelAI<NpcRimefangAI>(creature);
    }
};

#endif