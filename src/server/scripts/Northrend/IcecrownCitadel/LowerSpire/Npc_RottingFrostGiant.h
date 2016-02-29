#ifndef __ROTTINGFROSTGIANT_H_
#define __ROTTINGFROSTGIANT_H_

#include "LowerSpire.h"
#include "ScriptedCreature.h"
#include "Creature.h"

class NpcRottingFrostGiantAI : public ScriptedAI
{
public:
    NpcRottingFrostGiantAI(Creature* creature) : ScriptedAI(creature)
    {
    }

    void Reset();
    void JustDied(Unit* /*killer*/);
    void UpdateAI(uint32 const diff);

private:
    EventMap _events;
};

class NpcRottingFrostGiant : public CreatureScript
{
    public:
        NpcRottingFrostGiant() : CreatureScript("npc_rotting_frost_giant") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return GetIcecrownCitadelAI<NpcRottingFrostGiantAI>(creature);
    }
};

class DeathPlagueTargetSelector
{
    public:
        explicit DeathPlagueTargetSelector(Unit* caster) : _caster(caster) {}

        bool operator()(WorldObject* object) const;

    private:
        Unit* _caster;
};

#endif