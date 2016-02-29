#ifndef __NPC_SINDRAGOSATRASH_H_
#define __NPC_SINDRAGOSATRASH_H_

#include "Sindragosa.h"
#include "ScriptedCreature.h"
#include "Creature.h"

class NpcSindragosaTrashAI : public ScriptedAI
{
public:
    NpcSindragosaTrashAI(Creature* creature) : ScriptedAI(creature)
    {
        _instance = creature->GetInstanceScript();
    }

    void InitializeAI();
    void Reset();
    void JustRespawned();
    void SetData(uint32 type, uint32 data);
    uint32 GetData(uint32 type);
    void UpdateAI(uint32 const diff);

private:
    EventMap _events;
    InstanceScript* _instance;
    uint32 _frostwyrmId;
    bool _isTaunted; // Frostwing Whelp only
};

class NpcSindragosaTrash : public CreatureScript
{
    public:
        NpcSindragosaTrash() : CreatureScript("npc_sindragosa_trash") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return GetIcecrownCitadelAI<NpcSindragosaTrashAI>(creature);
    }
};

#endif