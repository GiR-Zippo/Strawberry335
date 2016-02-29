#include "Npc_BallOfFlame.h"

void NpcBallOfFlameAI::Reset()
{
    me->CastSpell(me, SPELL_BALL_OF_FLAMES_VISUAL, true);
    if (me->GetEntry() == NPC_BALL_OF_INFERNO_FLAME)
    {
        me->CastSpell(me, SPELL_BALL_OF_FLAMES_PROC, true);
        me->CastSpell(me, SPELL_BALL_OF_FLAMES_PERIODIC, true);
    }
}

void NpcBallOfFlameAI::MovementInform(uint32 type, uint32 id)
{
    if (type == CHASE_MOTION_TYPE && id == GUID_LOPART(_chaseGUID) && _chaseGUID)
    {
        me->RemoveAurasDueToSpell(SPELL_BALL_OF_FLAMES_PERIODIC);
        DoCast(me, SPELL_FLAMES);
        _despawnTimer = 1000;
        _chaseGUID = 0;
    }
}

void NpcBallOfFlameAI::SetGUID(uint64 guid, int32 )
{
    _chaseGUID = guid;
}

void NpcBallOfFlameAI::DoAction(const int32 action)
{
    if (action == ACTION_FLAME_BALL_CHASE)
        if (Player* target = ObjectAccessor::GetPlayer(*me, _chaseGUID))
        {
            // need to clear states now because this call is before AuraEffect is fully removed
            me->ClearUnitState(UNIT_STATE_CASTING | UNIT_STATE_STUNNED);
            if (target && me->Attack(target, true))
                me->GetMotionMaster()->MoveChase(target, 1.0f);
        }
}

void NpcBallOfFlameAI::DamageDealt(Unit* , uint32& damage, DamageEffectType damageType)
{
    if (damageType != SPELL_DIRECT_DAMAGE)
        return;

    if (damage > RAID_MODE<uint32>(23000, 25000, 23000, 25000))
        _instance->SetData(DATA_ORB_WHISPERER_ACHIEVEMENT, uint32(false));
}

void NpcBallOfFlameAI::UpdateAI(const uint32 diff)
{
    if (!_despawnTimer)
        return;

    if (_despawnTimer <= diff)
    {
        _despawnTimer = 0;
        DoCast(me, SPELL_FLAME_SPHERE_DEATH_EFFECT);
    }
    else
        _despawnTimer -= diff;
}