#ifndef __NPC_CULT_ADHERENT_H
#define __NPC_CULT_ADHERENT_H

#include "LadyDeathwhisper.h"

class CultAdherentAI : public ScriptedAI
{
public:
    CultAdherentAI(Creature* creature) : ScriptedAI(creature) {}

    void Reset();
    void SpellHit(Unit* /*caster*/, SpellInfo const* spell);
    void UpdateAI(uint32 const diff);

protected:

    EventMap Events;
};

class CultAdherent : public CreatureScript
{
public:
    CultAdherent() : CreatureScript("npc_cult_adherent") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return GetIcecrownCitadelAI<CultAdherentAI>(creature);
    }

};

#endif