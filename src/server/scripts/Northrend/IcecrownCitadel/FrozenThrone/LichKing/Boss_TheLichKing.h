#ifndef __BOSS_THELICHKING_H_
#define __BOSS_THELICHKING_H_

#include "TheLichKing.h"

#define LICH_KING_BOUNDARY_POSITION_N   577.79730f
#define LICH_KING_BOUNDARY_POSITION_E   -2197.76538f
#define LICH_KING_BOUNDARY_POSITION_S   456.60931f
#define LICH_KING_BOUNDARY_POSITION_W   -2051.26782f


class BossTheLichKingAI : public BossAI
{
public:

    BossTheLichKingAI(Creature* creature) : BossAI(creature, DATA_THE_LICH_KING)
    {
    }

    void Reset();
    void JustDied(Unit* /*killer*/);
    void EnterCombat(Unit* target);
    void JustReachedHome();
    bool CanAIAttack(Unit const* target) const;
    void EnterEvadeMode();
    void KilledUnit(Unit* victim);
    void DoAction(int32 const action);
    uint32 GetData(uint32 type);
    void SetData(uint32 type, uint32 value);
    void DamageTaken(Unit* /*attacker*/, uint32& /*damage*/);
    void JustSummoned(Creature* summon);
    void SummonedCreatureDies(Creature* summon, Unit* /*killer*/);
    void SpellHit(Unit* /*caster*/, SpellInfo const* spell);
    void SpellHitTarget(Unit* /*target*/, SpellInfo const* spell);
    void MovementInform(uint32 type, uint32 pointId);
    void UpdateAI(uint32 const diff);

private:
    void SendMusicToPlayers(uint32 musicId) const;
    void SendLightOverride(uint32 overrideId, uint32 fadeInTime) const;
    void SendWeather(WeatherState weather) const;
    // Send packet to all players in The Frozen Throne
    void SendPacketToPlayers(WorldPacket const* data) const;

    void createBoundaryMap();

    BossBoundaryMap boundaryMap;
    uint32 _necroticPlagueStack;
    uint32 _vileSpiritExplosions;

    bool secondTransition;
};

class BossTheLichKing : public CreatureScript
{
    public:
        BossTheLichKing() : CreatureScript("boss_the_lich_king") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return GetIcecrownCitadelAI<BossTheLichKingAI>(creature);
    }
};

#endif