#include "Npc_VengefulShade.h"

///////////////////////
// Vengeful Shade AI //
///////////////////////

void VengefulShadeAI::Reset()
{
    _events.Reset();
    _events.ScheduleEvent(EVENT_VENGEFUL_START, 1000);
    me->AddAura(SPELL_VENGEFUL_BLAST_PASSIVE, me);
}

void VengefulShadeAI::SpellHitTarget(Unit* /*target*/, SpellInfo const* spell)
{
    switch (spell->Id)
    {
    case SPELL_VENGEFUL_BLAST:
    case SPELL_VENGEFUL_BLAST_25N:
    case SPELL_VENGEFUL_BLAST_10H:
    case SPELL_VENGEFUL_BLAST_25H:
        me->Kill(me);
        break;
    default:
        break;
    }
}

void VengefulShadeAI::UpdateAI(const uint32 diff)
{
    if (!UpdateVictim())
        return;

    _events.Update(diff);

    while (uint32 eventId = _events.ExecuteEvent())
    {
        switch (eventId)
        {
        case EVENT_VENGEFUL_START:
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
            if (Unit* nearTarget = me->SelectNearbyTarget(0, 50.0f))
                me->AddThreat(nearTarget, 99999999.0f);
            break;
        }
    }

    DoMeleeAttackIfReady();
}
