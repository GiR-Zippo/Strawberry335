#ifndef __BOSS_MARROWGAR_H
#define __BOSS_MARROWGAR_H

#include "LordMarrowgar.h"
#include "ScriptedCreature.h"
#include "Creature.h"
#include "MapManager.h"


class BossLordMarrowgarAI : public BossAI
{
public:
    BossLordMarrowgarAI(Creature* creature) : BossAI(creature, DATA_LORD_MARROWGAR)
    {
        _boneStormDuration = RAID_MODE<uint32>(20000, 30000, 20000, 30000);
        _baseSpeed = creature->GetSpeedRate(MOVE_RUN);
        _coldflameLastPos.Relocate(creature);
        _introDone = false;
        _boneSlice = false;
    }

    void Reset();
    void EnterEvadeMode();
    void EnterCombat(Unit* /*who*/);
    void JustDied(Unit* /*killer*/);
    void KilledUnit(Unit* victim);
    void MoveInLineOfSight(Unit* who);
    void JustSummoned(Creature* summon);
    void UpdateAI(uint32 const diff);
    void MovementInform(uint32 type, uint32 id);
    Position const* GetLastColdflamePosition() const;
    uint64 GetGUID(int32 type/* = 0 */);
    void SetGUID(uint64 guid, int32 type/* = 0 */);

private:

    Position _coldflameLastPos;
    uint64 _coldflameTarget;
    uint32 _boneStormDuration;
    float _baseSpeed;
    bool _introDone;
    bool _boneSlice;

};

class BossLordMarrowgar : public CreatureScript
{
    public:
        BossLordMarrowgar() : CreatureScript("boss_lord_marrowgar") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return GetIcecrownCitadelAI<BossLordMarrowgarAI>(creature);
        }
};
#endif
