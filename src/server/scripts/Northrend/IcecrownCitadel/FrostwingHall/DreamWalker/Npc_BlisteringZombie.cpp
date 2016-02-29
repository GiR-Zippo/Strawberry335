#include "Npc_BlisteringZombie.h"

void NpcBlisteringZombieAI::EnterCombat(Unit* victim)
{
    DoCast(me, SPELL_CORRUSION);
    acidDone = false;
}

void NpcBlisteringZombieAI::DamageTaken(Unit* /*victim*/, uint32& damage)
{
    if (damage > me->GetHealth())
    {
        damage = 0;
        if (!acidDone)
            me->AI()->DoAction(ACTION_ACID_BURST_DOWN);
    }
}

void NpcBlisteringZombieAI::DoAction(const int32 action)
{
    switch (action)
    {
    case ACTION_ACID_BURST_DOWN:
        if (me->isAlive())
        {
            acidDone = true;
            DoCast(me, SPELL_ACID_BURST);
            me->SetFlag64(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
            me->DespawnOrUnsummon(1500);
        }
        break;
    default:
        break;
    }
}

void NpcBlisteringZombieAI::UpdateAI(const uint32 )
{
    if (!UpdateVictim())
        return;

    DoMeleeAttackIfReady();
}