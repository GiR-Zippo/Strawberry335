#include "Npc_GreenDragonCombatTrigger.h"

void NpcGreenDragonCombatTriggerAI::Reset()
{
    _Reset();
    me->SetReactState(REACT_PASSIVE);
}

void NpcGreenDragonCombatTriggerAI::EnterCombat(Unit* target)
{
    if (!instance->CheckRequiredBosses(DATA_VALITHRIA_DREAMWALKER, target->ToPlayer()))
    {
        EnterEvadeMode();
        instance->DoCastSpellOnPlayers(LIGHT_S_HAMMER_TELEPORT);
        return;
    }

    me->setActive(true);
    DoZoneInCombat();
    instance->SetBossState(DATA_VALITHRIA_DREAMWALKER, IN_PROGRESS);
    if (Creature* valithria = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_VALITHRIA_DREAMWALKER)))
        valithria->AI()->DoAction(ACTION_ENTER_COMBAT);
}

void NpcGreenDragonCombatTriggerAI::AttackStart(Unit* target)
{
    if (target->GetTypeId() == TYPEID_PLAYER)
        BossAI::AttackStart(target);
}

bool NpcGreenDragonCombatTriggerAI::CanAIAttack(const Unit* target) const
{
    return target->GetTypeId() == TYPEID_PLAYER;
}

void NpcGreenDragonCombatTriggerAI::JustReachedHome()
{
    _JustReachedHome();
    DoAction(ACTION_DEATH);
}

void NpcGreenDragonCombatTriggerAI::DoAction(const int32 action)
{
    if (action == ACTION_DEATH)
    {
        instance->SetBossState(DATA_VALITHRIA_DREAMWALKER, NOT_STARTED);
        me->m_Events.AddEvent(new ValithriaDespawner(me), me->m_Events.CalculateTime(5000));
    }
}

void NpcGreenDragonCombatTriggerAI::UpdateAI(const uint32 )
{
    if (!me->isInCombat())
        return;

    std::list<HostileReference*> const& threatList = me->getThreatManager().getThreatList();
    if (threatList.empty())
    {
        EnterEvadeMode();
        return;
    }

    // check evade every second tick
    _evadeCheck ^= true;
    if (!_evadeCheck)
        return;

    // check if there is any player on threatlist, if not - evade
    for (std::list<HostileReference*>::const_iterator itr = threatList.begin(); itr != threatList.end(); ++itr)
        if (Unit* target = (*itr)->getTarget())
            if (target->GetTypeId() == TYPEID_PLAYER)
                return; // found any player, return

    EnterEvadeMode();
}