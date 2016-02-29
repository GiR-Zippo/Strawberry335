#include "Npc_SpiritWarden.h"

void NpcSpiritWardenAI::Reset()
{
    _events.Reset();
    _events.ScheduleEvent(EVENT_SOUL_RIP, urand(12000, 15000));
    DoCast(SPELL_DARK_HUNGER);
}

void NpcSpiritWardenAI::JustDied ( Unit* )
{
    if (Creature* terenas = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_TERENAS_MENETHIL)))
        terenas->AI()->DoAction(ACTION_TELEPORT_BACK);
}

void NpcSpiritWardenAI::UpdateAI ( const uint32 diff )
{
    if (!UpdateVictim())
        return;

    _events.Update(diff);

    while (uint32 eventId = _events.ExecuteEvent())
    {
        switch (eventId)
        {
            case EVENT_SOUL_RIP:
                DoCastVictim(SPELL_SOUL_RIP);
                _events.ScheduleEvent(EVENT_SOUL_RIP, urand(23000, 27000));
                break;
            default:
                break;
        }
    }

    DoMeleeAttackIfReady();
}