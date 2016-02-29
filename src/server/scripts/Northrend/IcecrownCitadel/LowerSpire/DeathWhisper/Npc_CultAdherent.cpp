#include "Npc_CultAdherent.h"

//////////////////////
// Cult Adherent AI //
//////////////////////

void CultAdherentAI::Reset()
{
    Events.Reset();
    Events.ScheduleEvent(EVENT_ADHERENT_FROST_FEVER, urand(10000, 12000));
    Events.ScheduleEvent(EVENT_ADHERENT_DEATHCHILL, urand(14000, 16000));
    Events.ScheduleEvent(EVENT_ADHERENT_CURSE_OF_TORPOR, urand(14000, 16000));
    Events.ScheduleEvent(EVENT_ADHERENT_SHORUD_OF_THE_OCCULT, urand(32000, 39000));
    if (me->GetEntry() == NPC_CULT_ADHERENT)
        Events.ScheduleEvent(EVENT_CULTIST_DARK_MARTYRDOM, urand(18000, 32000));
}

void CultAdherentAI::SpellHit(Unit* /*caster*/, SpellInfo const* spell)
{
    if (spell->Id == SPELL_DARK_EMPOWERMENT)
        me->UpdateEntry(NPC_EMPOWERED_ADHERENT);
    else if (spell->Id == SPELL_DARK_EMPOWERMENT_T)
    {
        Events.CancelEvent(EVENT_CULTIST_DARK_MARTYRDOM);
        me->InterruptNonMeleeSpells(true);
        DoCast(me, SPELL_DARK_EMPOWERMENT);
    }
}

void CultAdherentAI::UpdateAI(uint32 const diff)
{
    if (!UpdateVictim())
        return;

    Events.Update(diff);

    if (me->HasUnitState(UNIT_STATE_CASTING))
        return;

    while (uint32 eventId = Events.ExecuteEvent())
    {
        switch (eventId)
        {
        case EVENT_ADHERENT_FROST_FEVER:
            DoCastVictim(FROST_FEVER_HELPER);
            Events.ScheduleEvent(EVENT_ADHERENT_FROST_FEVER, urand(9000, 13000));
            break;
        case EVENT_ADHERENT_DEATHCHILL:
            if (me->GetEntry() == NPC_EMPOWERED_ADHERENT)
                DoCastVictim(DEATHCHILL_BLAST_HELPER);
            else
                DoCastVictim(DEATHCHILL_BOLT_HELPER);
            Events.ScheduleEvent(EVENT_ADHERENT_DEATHCHILL, urand(9000, 13000));
            break;
        case EVENT_ADHERENT_CURSE_OF_TORPOR:
            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1))
                DoCast(target, SPELL_CURSE_OF_TORPOR);
            Events.ScheduleEvent(EVENT_ADHERENT_CURSE_OF_TORPOR, urand(9000, 13000));
            break;
        case EVENT_ADHERENT_SHORUD_OF_THE_OCCULT:
            DoCast(me, SPELL_SHORUD_OF_THE_OCCULT);
            Events.ScheduleEvent(EVENT_ADHERENT_SHORUD_OF_THE_OCCULT, urand(27000, 32000));
            break;
        case EVENT_CULTIST_DARK_MARTYRDOM:
            DoCast(me, DARK_MARTYRDOM_ADHERENT_HELPER);
            Events.ScheduleEvent(EVENT_CULTIST_DARK_MARTYRDOM, urand(16000, 21000));
            break;
        }
    }

    DoMeleeAttackIfReady();
}
