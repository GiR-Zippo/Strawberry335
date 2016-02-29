#include "Npc_BoneSpike.h"

void NpcBoneSpikeAI::IsSummonedBy(Unit* summoner)
{
    DoCast(summoner, SPELL_IMPALED);
    summoner->CastSpell(me, SPELL_RIDE_VEHICLE, true);
    _events.ScheduleEvent(EVENT_FAIL_BONED, 8000);
    _hasTrappedUnit = true;
}

void NpcBoneSpikeAI::JustDied(Unit* )
{
    if (TempSummon* summ = me->ToTempSummon())
    {
        if (Unit* trapped = summ->GetSummoner())
        {
            trapped->RemoveAurasDueToSpell(SPELL_RIDE_VEHICLE);
            trapped->RemoveAurasDueToSpell(SPELL_IMPALED);
        }
    }

    me->DespawnOrUnsummon();
}

void NpcBoneSpikeAI::KilledUnit(Unit* victim)
{
    me->DespawnOrUnsummon();
    victim->RemoveAurasDueToSpell(SPELL_IMPALED);
}

void NpcBoneSpikeAI::UpdateAI(const uint32 diff)
{
    if (!_hasTrappedUnit)
        return;

    _events.Update(diff);

    if (_events.ExecuteEvent() == EVENT_FAIL_BONED)
        if (InstanceScript* instance = me->GetInstanceScript())
            instance->SetData(DATA_BONED_ACHIEVEMENT, uint32(false));
}