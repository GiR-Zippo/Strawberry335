#include "Boss_Festergut.h"

///////////////////////
// Boss Festergut AI //
///////////////////////

void FestergutAI::Reset()
{
    _Reset();
    me->SetReactState(REACT_DEFENSIVE);
    events.ScheduleEvent(EVENT_BERSERK, 300000);
    events.ScheduleEvent(EVENT_INHALE_BLIGHT, urand(25000, 30000));
    events.ScheduleEvent(EVENT_GAS_SPORE, urand(20000, 25000));
    events.ScheduleEvent(EVENT_GASTRIC_BLOAT, urand(12500, 15000));
    _maxInoculatedStack = 0;
    _inhaleCounter = 0;
    me->RemoveAurasDueToSpell(SPELL_BERSERK2);
    if (Creature* gasDummy = me->FindNearestCreature(NPC_GAS_DUMMY, 100.0f, true))
    {
        _gasDummyGUID = gasDummy->GetGUID();
        for (uint8 i = 0; i < 3; ++i)
        {
            me->RemoveAurasDueToSpell(gaseousBlight[i]);
            gasDummy->RemoveAurasDueToSpell(gaseousBlightVisual[i]);
        }
    }
}

void FestergutAI::EnterCombat(Unit* who)
{
    if (me->isDead())
    {
        EnterEvadeMode();
        return;
    }

    if (!instance->CheckRequiredBosses(DATA_FESTERGUT, who->ToPlayer()))
    {
        EnterEvadeMode();
        instance->DoCastSpellOnPlayers(LIGHT_S_HAMMER_TELEPORT);
        return;
    }

    me->setActive(true);
    Talk(SAY_AGGRO);
    if (Creature* gasDummy = me->FindNearestCreature(NPC_GAS_DUMMY, 100.0f, true))
        _gasDummyGUID = gasDummy->GetGUID();
    if (Creature* professor = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_PROFESSOR_PUTRICIDE)))
        professor->AI()->DoAction(ACTION_FESTERGUT_COMBAT);
    DoZoneInCombat();
}

void FestergutAI::JustDied(Unit* /*killer*/)
{
    _JustDied();
    Talk(SAY_DEATH);
    if (Creature* professor = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_PROFESSOR_PUTRICIDE)))
        professor->AI()->DoAction(ACTION_FESTERGUT_DEATH);

    RemoveBlight();
}

void FestergutAI::JustReachedHome()
{
    _JustReachedHome();
    instance->SetBossState(DATA_FESTERGUT, FAIL);
}

void FestergutAI::EnterEvadeMode()
{
    ScriptedAI::EnterEvadeMode();
    if (Creature* professor = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_PROFESSOR_PUTRICIDE)))
        professor->AI()->EnterEvadeMode();
    _DespawnAtEvade();
}

void FestergutAI::KilledUnit(Unit* victim)
{
    if (victim->GetTypeId() == TYPEID_PLAYER)
        Talk(SAY_KILL);
}

void FestergutAI::SpellHitTarget(Unit* target, SpellInfo const* spell)
{
    if (spell->Id == PUNGENT_BLIGHT_HELPER)
        target->RemoveAurasDueToSpell(INOCULATED_HELPER);
}

void FestergutAI::UpdateAI(uint32 const diff)
{
    if (!UpdateVictim() || !CheckInRoom())
        return;

    events.Update(diff);

    if (me->HasUnitState(UNIT_STATE_CASTING))
        return;

    while (uint32 eventId = events.ExecuteEvent())
    {
        switch (eventId)
        {
        case EVENT_INHALE_BLIGHT:
            {
                RemoveBlight();
                if (_inhaleCounter == 3)
                {
                    Talk(EMOTE_WARN_PUNGENT_BLIGHT);
                    Talk(SAY_PUNGENT_BLIGHT);
                    DoCast(me, PUNGENT_BLIGHT_HELPER);
                    _inhaleCounter = 0;
                    if (Creature* professor = ObjectAccessor::GetCreature(*me, instance->GetData64(DATA_PROFESSOR_PUTRICIDE)))
                        professor->AI()->DoAction(ACTION_FESTERGUT_GAS);
                    events.RescheduleEvent(EVENT_GAS_SPORE, urand(20000, 25000));
                }
                else
                {
                    DoCast(me, SPELL_INHALE_BLIGHT);
                    // just cast and dont bother with target, conditions will handle it
                    ++_inhaleCounter;
                    if (_inhaleCounter < 3)
                        me->CastSpell(me, gaseousBlight[_inhaleCounter], true, NULL, NULL, me->GetGUID());
                }

                events.ScheduleEvent(EVENT_INHALE_BLIGHT, urand(33500, 35000));
                break;
            }
        case EVENT_VILE_GAS:
            {
                std::list<Unit*> ranged, melee;
                uint32 minTargets = RAID_MODE<uint32>(3, 8, 3, 8);
                SelectTargetList(ranged, 25, SELECT_TARGET_RANDOM, -5.0f, true);
                SelectTargetList(melee, 25, SELECT_TARGET_RANDOM, 5.0f, true);
                while (ranged.size() < minTargets)
                {
                    if (melee.empty())
                        break;

                    Unit* target = Trinity::Containers::SelectRandomContainerElement(melee);
                    ranged.push_back(target);
                    melee.remove(target); 
                }

                if (!ranged.empty())
                {
                    Trinity::Containers::RandomResizeList(ranged, RAID_MODE<uint32>(1, 3, 1, 3));
                    for (std::list<Unit*>::iterator itr = ranged.begin(); itr != ranged.end(); ++itr)
                        DoCast(*itr, VILE_GAS_HELPER);
                }
                break;
            }
        case EVENT_GAS_SPORE:
            Talk(EMOTE_WARN_GAS_SPORE);
            Talk(EMOTE_GAS_SPORE);
            me->CastCustomSpell(GAS_SPORE_HELPER, SPELLVALUE_MAX_TARGETS, RAID_MODE<int32>(2, 3, 2, 3), me);
            events.ScheduleEvent(EVENT_GAS_SPORE, urand(40000, 45000));
            events.RescheduleEvent(EVENT_VILE_GAS, urand(28000, 35000));
            break;
        case EVENT_GASTRIC_BLOAT:
            DoCastVictim(GASTRIC_BLOAT_HELPER);
            events.ScheduleEvent(EVENT_GASTRIC_BLOAT, urand(15000, 17500));
            break;
        case EVENT_BERSERK:
            DoCast(me, SPELL_BERSERK2);
            Talk(SAY_BERSERK);
            break;
        default:
            break;
        }
    }

    DoMeleeAttackIfReady();
}

void FestergutAI::SetData(uint32 type, uint32 data)
{
    if (type == DATA_INOCULATED_STACK && data > _maxInoculatedStack)
        _maxInoculatedStack = data;
}

uint32 FestergutAI::GetData(uint32 type)
{
    if (type == DATA_INOCULATED_STACK)
        return uint32(_maxInoculatedStack);

    return 0;
}

void FestergutAI::RemoveBlight()
{
    if (Creature* gasDummy = ObjectAccessor::GetCreature(*me, _gasDummyGUID))
        for (uint8 i = 0; i < 3; ++i)
        {
            me->RemoveAurasDueToSpell(gaseousBlight[i]);
            gasDummy->RemoveAurasDueToSpell(gaseousBlightVisual[i]);
        }
}
