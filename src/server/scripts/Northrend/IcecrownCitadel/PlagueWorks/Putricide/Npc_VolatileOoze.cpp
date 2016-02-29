#include "Npc_VolatileOoze.h"

//////////////////////
// Volatile Ooze AI //
//////////////////////

void VolatileOozeAI::Reset()
{
    me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
    me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK_DEST, true);
}

void VolatileOozeAI::EnterCombat(Unit* /*attacker*/)
{
    eventMap.ScheduleEvent(EVENT_OOZE_ICE_BLOCK, 250);
}

void VolatileOozeAI::SpellHitTarget(Unit* /*target*/, SpellInfo const* spell)
{
    if (!eventMap.IsEventActive(EVENT_OOZE_SELECT_TARGET) && spell->Id == sSpellMgr->GetSpellIdForDifficulty(SPELL_OOZE_ERUPTION, me))
        eventMap.ScheduleEvent(EVENT_OOZE_SELECT_TARGET, 5 * IN_MILLISECONDS);
}

void VolatileOozeAI::SpellHit(Unit* /*caster*/, SpellInfo const* spell)
{
    if (spell->Id == SPELL_TEAR_GAS_CREATURE)
        eventMap.ScheduleEvent(EVENT_OOZE_SELECT_TARGET, 5 * IN_MILLISECONDS);
}

void VolatileOozeAI::UpdateAI(uint32 const diff)
{
    if (me->HasUnitState(UNIT_STATE_CASTING))
        return;

    if (!me->getVictim() && !eventMap.IsEventActive(EVENT_OOZE_SELECT_TARGET))
    {
        me->CastSpell(me, SPELL_VOLATILE_OOZE_ADHESIVE, false);
        return;
    }

    eventMap.Update(diff);

    if (uint32 eventID = eventMap.ExecuteEvent())
        executeEvent(eventID);
}

void VolatileOozeAI::executeEvent(uint32 eventID)
{
    uint32 timer = 0;

    switch (eventID)
    {
        case EVENT_OOZE_SELECT_TARGET:
        {
            me->CastSpell(me, SPELL_VOLATILE_OOZE_ADHESIVE, false);
        }break;

        case EVENT_OOZE_ICE_BLOCK:
        {
            if (Unit* pUnit = me->getVictim())
            {
                if (pUnit->HasAura(SPELL_AURA_ICE_BLOCK) && !me->HasUnitState(UNIT_STATE_ROOT | UNIT_STATE_STUNNED))
                    me->AddUnitState(UNIT_STATE_ROOT | UNIT_STATE_STUNNED);
                else if (me->HasUnitState(UNIT_STATE_ROOT | UNIT_STATE_STUNNED))
                    me->ClearUnitState(UNIT_STATE_ROOT | UNIT_STATE_STUNNED);
            }

            timer = 250;
        }break;
    }

    if (eventID && timer)
        eventMap.ScheduleEvent(eventID, timer);
}
