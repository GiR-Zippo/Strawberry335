#ifndef __NPC_CANNON_GUNSHIPBATTLE_H_
#define __NPC_CANNON_GUNSHIPBATTLE_H_

#include "GunShipBattle.h"
#include "ScriptedCreature.h"
#include "Creature.h"
#include "MapManager.h"

class NpcCannonGunshipAI : public ScriptedAI
{
public:
    NpcCannonGunshipAI(Creature *creature) : ScriptedAI(creature) { }

    void Reset();
    void EnterCombat(Unit* /*who*/);
    void SpellHit(Unit* /*caster*/, SpellInfo const* spellEntry);
    void DamageTaken(Unit* attacker, uint32& damage);
    void UpdateAI(const uint32 diff);
};

class NpcCannonGunship : public CreatureScript
{
    public:
        NpcCannonGunship() : CreatureScript("npc_gunship_cannon") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return GetIcecrownCitadelAI<NpcCannonGunshipAI>(pCreature);
    }
};

#endif