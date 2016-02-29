#ifndef __BOSS_BLOODQUEENLANATHEL_H_
#define __BOSS_BLOODQUEENLANATHEL_H_

#include "BloodQueenLanaThel.h"

class BossBloodQueenLanaThelAI : public BossAI
{
public:
    BossBloodQueenLanaThelAI(Creature* creature) : BossAI(creature, DATA_BLOOD_QUEEN_LANA_THEL)
    {
    }

    void Reset();
    void EnterCombat(Unit* who);
    void JustDied(Unit* killer);
    void CleanAuras();
    void DoAction(int32 const action);
    void EnterEvadeMode();
    void JustReachedHome();
    void KilledUnit(Unit* victim);
    void SetGUID(uint64 guid, int32 type = 0);
    void MovementInform(uint32 type, uint32 id);
    void UpdateAI(uint32 const diff);
    bool WasVampire(uint64 guid);
    bool WasBloodbolted(uint64 guid);

private:
    // offtank for this encounter is the player standing closest to main tank
    Player* SelectRandomTarget(bool includeOfftank, std::list<Player*>* targetList = NULL, bool bloodbolt = false);

    std::set<uint64> _vampires;
    std::set<uint64> _bloodboltedPlayers;
    Player* _offtank;
    bool _creditBloodQuickening;
    bool _killMinchar;
    bool _isAirPhase;
};

class BossBloodQueenLanaThel : public CreatureScript
{
    public:
        BossBloodQueenLanaThel() : CreatureScript("boss_blood_queen_lana_thel") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return GetIcecrownCitadelAI<BossBloodQueenLanaThelAI>(creature);
    }
};

#endif