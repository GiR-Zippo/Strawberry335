#include "Kriffel.h"

class Kriffel_OrbAI : public ScriptedAI
{
public:
    Kriffel_OrbAI(Creature* creature) : ScriptedAI(creature)
    {
    }

    void Reset()
    {
        events.Reset();
    }

    void UpdateAI(const uint32 diff)
    {
        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_EXPLODE:
                    me->RemoveAllAuras();
                    DoCast(SPELL_SHADOW_EXPLO);
                    DoCast(SPELL_EXPLODE);
                    break;
                case EVENT_EXPLODE_BIG:
                    DoCast(SPELL_CAMERA_SHAKE);
                    DoCast(SPELL_SHADOW_EXPLO);
                    DoCast(SPELL_EXPLODE);
                    break;
                case EVENT_GLITZERN:
                    DoCast(SPELL_GLITZERN);
                    events.ScheduleEvent(EVENT_STOP_GLITZERN, 2000);
                    break;
                case EVENT_STOP_GLITZERN:
                    me->RemoveAllAuras();
                    break;
                default:
                    break;
            }
        }
    }

    void DoAction(const int32 action)
    {
        switch (action)
        {
            case ACTION_EXPLODE_START:
                DoCast(SPELL_VISUAL_EXPLODE);
                events.ScheduleEvent(EVENT_EXPLODE, 3500);
                break;
            case ACTION_EXPLODE_BIG:
                events.ScheduleEvent(EVENT_EXPLODE_BIG, 3500);
                break;
            case ACTION_WARN_EXPLODE:
                events.ScheduleEvent(EVENT_GLITZERN, 1500);
                break;
            default:
                break;
        }
    }

private:
    EventMap events;
};

class Kriffel_FrostorbAI : public ScriptedAI
{
public:
    Kriffel_FrostorbAI(Creature* creature) : ScriptedAI(creature)
    {
    }

    void DamageTaken(Unit* damageDealer, uint32& damage)
    {
        if (damageDealer == me)
            return;

        if (damageDealer->HasAura(SPELL_AURA_OF_DESIRE))
        {
            int32 bp0 = damage * 0.25;
            me->CastCustomSpell(damageDealer, SPELL_AURA_OF_DESIRE_DAMAGE, &bp0, NULL, NULL, true);
        }
    }

    void Reset()
    {
        float healthmod = 1.0f;
        if (Is25ManRaid())
            healthmod *= 4.0f;
        if (IsHeroic())
            healthmod *= 1.4f;
        me->SetMaxHealth(me->GetHealth() * healthmod);
        me->SetHealth(me->GetMaxHealth());
        events.Reset();
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED | UNIT_FLAG_DISABLE_MOVE);
        me->SetReactState(REACT_PASSIVE);
        DoZoneInCombat();

        me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);

        DoCast(SPELL_BLUE_PILLAR);

        events.ScheduleEvent(EVENT_FROSTORB_FROST, 1000);
        events.ScheduleEvent(EVENT_FROSTORB_DETO, 12000);
    }

    void JustDied(Unit* killer)
    {
        me->DespawnOrUnsummon(500);
    }

    void UpdateAI(const uint32 diff)
    {
        events.Update(diff);

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        while (uint32 eventId = events.ExecuteEvent())
        {
            switch (eventId)
            {
                case EVENT_FROSTORB_DETO:
                    DoCast(SPELL_FROSTEXPLOSION);
                    me->DespawnOrUnsummon(500);
                    break;
                case EVENT_FROSTORB_FROST:
                    DoCast(SPELL_PERMAFROST);
                    DoCast(SPELL_WACHSEN);
                    events.ScheduleEvent(EVENT_FROSTORB_FROST, 1500);
                    break;
                default:
                    break;
            }
        }
    }

private:
    EventMap events;
};


class Kriffel_Orb : public CreatureScript
{
public:
    Kriffel_Orb() : CreatureScript("custom_kriffel_orb") {}

    CreatureAI* GetAI(Creature* creature) const
    {
        return new Kriffel_OrbAI(creature);
    };
};

class Kriffel_Frostorb : public CreatureScript
{
public:
    Kriffel_Frostorb() : CreatureScript("custom_kriffel_frostorb") {}

    CreatureAI* GetAI(Creature* creature) const
    {
        return new Kriffel_FrostorbAI(creature);
    };
};

void LoadNpcKriffelOrb()
{
    new Kriffel_Orb();
    new Kriffel_Frostorb();
}
