#include "Zerberhorse.h"

enum Spells{
    POWER       = 72525,
    EAT_PLAYER  = 69503,
    STUN        = 66758,
    EXPLODE     = 78723,
};

enum Event{
    EVT_EAT_PLAYER      = 1,
    EVT_EAT_BUFFS,
    EVT_EXPLODE_TIMER,
};

enum Timer{
    TMR_EAT_PLAYER      = 15000,
    TMR_EAT_BUFFS       = 7000,
    TMR_EXPLODE_TIMER   = 150,
};

//enum Texts{
//    SAY_INTRO   = 0,
//    SAY_EAT     = 0,
//    SAY_KILLED  = 0,
//    SAY_AGGRO   = 0,
//    SAY_DIE     = 0,
//    SAY_EXPLODE = 0,
//};

enum StackAdd{
    ADD_KILL    = 5,
    ADD_EAT     = 2,
    ADD_BUFF    = 1,
};

enum Phase{
    EATING_PHASE    = 0,
    RESTING_PHASE   = 1,
};

#define MAX_POWER 100
#define MAX_ROUNDS 5

#define SMALL_ZERBER 0.5f
#define SCALE_SMALL 1
#define SCALE_ROUND 20
#define POWER_ROUND 20
#define SPEED_ROUND 5

ZerberhorseAI::ZerberhorseAI(Creature* creature) : ScriptedAI(creature)
{
    intro = false;
    phase = EATING_PHASE;
    power = 0;
    round = 0;
}

void ZerberhorseAI::Reset()
{
    phase = EATING_PHASE;
    power = 0;
    round = 0;
    me->RemoveAura(POWER);
    me->SetObjectScale(SMALL_ZERBER);
}

void ZerberhorseAI::MoveInLineOfSight(Unit*)
{
    if (intro)
        return;

    intro = true;

    //DoScriptText(SAY_INTRO, me);
}

void ZerberhorseAI::JustReachedHome()
{
    intro = false;
}

void ZerberhorseAI::JustDied(Unit* /*killer*/)
{

}

int32 ZerberhorseAI::ModPower(int32 delta)
{
    if (delta == 0 || ( 0 < delta && phase == RESTING_PHASE))
        return power;

    power += delta;

    if (power <= 0)
        power = 0;

    if (power >= MAX_POWER)
    {
        power = MAX_POWER;
        if (phase == EATING_PHASE)
        {
            phase = RESTING_PHASE;
            round++;
            me->CastSpell(me, STUN);
            events.Reset();
            events.ScheduleEvent(EVT_EXPLODE_TIMER, TMR_EXPLODE_TIMER);
        }
    }

    if (Aura *aura = me->GetAura(POWER))
    {
        aura->GetEffect(EFFECT_0)->ChangeAmount(power + round * POWER_ROUND);
        aura->GetEffect(EFFECT_1)->ChangeAmount(round * SPEED_ROUND);
        aura->GetEffect(EFFECT_2)->ChangeAmount(power * SCALE_SMALL + power * SCALE_ROUND);
    }
    return power;
}

void ZerberhorseAI::KilledUnit(Unit* who)
{
    //DoScriptText(SAY_KILLED, me, who);

    ModPower(ADD_KILL);
}

void ZerberhorseAI::SpellHitTarget(Unit* target, SpellInfo const* spell)
{
    if (target->ToPlayer() && spell->Id == EAT_PLAYER)
        ModPower(ADD_EAT);
}

void ZerberhorseAI::EnterCombat(Unit* /*who*/)
{
    //DoScriptText(SAY_AGGRO, me);
    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
    me->SetInCombatWithZone();

    events.ScheduleEvent(EVT_EAT_PLAYER, TMR_EAT_PLAYER);
    events.ScheduleEvent(EVT_EAT_BUFFS, TMR_EAT_BUFFS);
}

void ZerberhorseAI::HandleEvent(const uint32 Event)
{
    switch (Event)
    {
        case EVT_EAT_PLAYER:
        {
            Unit *victim = SelectTarget(SELECT_TARGET_RANDOM, 0, 40, true);
            if (victim)
                me->CastSpell(victim, EAT_PLAYER, false);
            break;
        }
        case EVT_EAT_BUFFS:
        {
            std::list<Unit *> targets;
            SelectTargetList(targets, 5, SELECT_TARGET_RANDOM, 40, true);
            
            for (std::list<Unit *>::iterator itr = targets.begin(); itr != targets.end(); ++itr)
            {
                Unit *victim = *itr;
                DispelChargesList dispelList;
                victim->GetDispellableAuraList(me, DISPEL_MAGIC, dispelList);

                DispelChargesList::iterator ditr = dispelList.begin();
                std::advance(ditr, urand(0, dispelList.size() - 1));
                Aura *aura = ditr->first;
                if (aura)
                {
                    aura->Remove();
                    ModPower(ADD_BUFF);
                }
            }

            break;
        }
        case EVT_EXPLODE_TIMER:
        {
            if (ModPower(-1))
            {
                events.ScheduleEvent(EVT_EXPLODE_TIMER, TMR_EXPLODE_TIMER);
                break;
            }

            if (round >= MAX_ROUNDS)
            {
                const int32 bp0 = 14000;
                me->CastCustomSpell(me, EXPLODE, &bp0, NULL, NULL, false);
            }
            else
            {
                const int32 bp0 = 3500;
                me->CastCustomSpell(me, EXPLODE, &bp0, NULL, NULL, false);
            }

            events.ScheduleEvent(EVT_EAT_PLAYER, TMR_EAT_PLAYER);
            events.ScheduleEvent(EVT_EAT_BUFFS, TMR_EAT_BUFFS);
            phase = EATING_PHASE;
            break;
        }
    }
}

void ZerberhorseAI::UpdateAI(const uint32 diff)
{
    if (!UpdateVictim())
        return;

    if (me->HasUnitState(UNIT_STATE_CASTING))
        return;

    events.Update(diff);
    
    while (int Event = events.ExecuteEvent())
    {
        HandleEvent(Event);
        
        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;
    }

    DoMeleeAttackIfReady();
}
