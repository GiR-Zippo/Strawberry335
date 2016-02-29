#ifndef __NPC_ICETOMB_H_
#define __NPC_ICETOMB_H_

#include "Sindragosa.h"
#include "ScriptedCreature.h"
#include "Creature.h"

class NpcIceTombAI : public Scripted_NoMovementAI
{
public:
    NpcIceTombAI(Creature* creature) : Scripted_NoMovementAI(creature)
    {
        _trappedPlayerGUID = 0;
    }

    void Reset();
    void SetGUID(uint64 guid, int32 type/* = 0 */);
    void DoAction(int32 const action);
    void JustDied(Unit* /*killer*/);
    void UpdateAI(uint32 const diff);

private:
    uint64 _trappedPlayerGUID;
    uint32 _existenceCheckTimer;
};

class NpcIceTomb : public CreatureScript
{
    public:
        NpcIceTomb() : CreatureScript("npc_ice_tomb") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return GetIcecrownCitadelAI<NpcIceTombAI>(creature);
    }
};

#endif