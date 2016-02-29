#include "Npc_FrostFreezeTrap.h"

///////////////////////////////
// Npc Valkyr Upper Spire AI //
///////////////////////////////

void NpcValkyrUpperSpireAI::Reset()
{
    _events.Reset();
    _events.ScheduleEvent(EVENT_SEVERED_ESSENCE, 3000);

    std::list<Creature*> creatures;
    GetCreatureListWithEntryInGrid(creatures, me, NPC_SEVERED_ESSENCE, 70.0f);
    for (std::list<Creature*>::iterator itr = creatures.begin(); itr != creatures.end(); ++itr)
    {
        (*itr)->DespawnOrUnsummon();
    }
}

void NpcValkyrUpperSpireAI::UpdateAI(uint32 const diff)
{
    if (!UpdateVictim())
        return;

    _events.Update(diff);

    if (me->HasUnitState(UNIT_STATE_CASTING))
        return;

    while (uint32 eventId = _events.ExecuteEvent())
    {
        switch (eventId)
        {
        case EVENT_SEVERED_ESSENCE:
            DoCastAOE(SPELL_SEVERED_ESSENCE);
            _events.ScheduleEvent(EVENT_SEVERED_ESSENCE, 10000);
            break;
        default:
            break;
        }
    }

    DoMeleeAttackIfReady();
}

void NpcSeveredEssenceUpperSpireAI::Reset()
{
    _events.Reset();
}

void NpcSeveredEssenceUpperSpireAI::UpdateAI(uint32 const diff)
{
    if (!UpdateVictim())
        return;

    _events.Update(diff);

    if (me->HasUnitState(UNIT_STATE_CASTING))
        return;

    while (uint32 eventId = _events.ExecuteEvent())
    {
        switch (eventId)
        {
        case EVENT_SET_CLASS:
            if (Player* master = me->ToTempSummon()->GetSummoner()->ToPlayer())
            {
                switch (master->getClass())
                {
                case CLASS_WARRIOR:
                    _events.ScheduleEvent(EVENT_WARRIOR, urand(6000, 15000));
                    break;
                case CLASS_MAGE:
                    _events.ScheduleEvent(EVENT_MAGE, 1000);
                    break;
                case CLASS_DRUID:
                    _events.ScheduleEvent(EVENT_DRUID, 1000);
                    break;
                case CLASS_PRIEST:
                    _events.ScheduleEvent(EVENT_PRIEST, 3000);
                    _events.ScheduleEvent(EVENT_PRIEST_HEAL, 6000);
                    break;
                case CLASS_ROGUE:
                    _events.ScheduleEvent(EVENT_ROGUE, 7000);
                    break;
                case CLASS_DEATH_KNIGHT:
                    _events.ScheduleEvent(EVENT_DEATHKNIGHT, 5000);
                    _events.ScheduleEvent(EVENT_DEATHKNIGHT_NECRO, urand(10000, 15000));
                    break;
                case CLASS_HUNTER:
                    _events.ScheduleEvent(EVENT_HUNTER, 1000);
                    break;
                case CLASS_SHAMAN:
                    _events.ScheduleEvent(EVENT_SHAMAN, 1000);
                    _events.ScheduleEvent(EVENT_SHAMAN_HEAL, urand(30000, 50000));
                    break;
                case CLASS_WARLOCK:
                    _events.ScheduleEvent(EVENT_WARLOCK_RAIN, urand(2000, 10000));
                    _events.ScheduleEvent(EVENT_WARLOCK_CORRUPTION, urand(2000, 8000));
                    _events.ScheduleEvent(EVENT_WARLOCK_SHADOWBOLT, 1000);
                    break;
                case CLASS_PALADIN:
                    _events.ScheduleEvent(EVENT_PALADIN, 1000);
                    break;
                default:
                    break;
                }
            }
            break;
        case EVENT_WARLOCK_RAIN:
            DoCastAOE(SPELL_RAIN_OF_CHAOS);
            _events.ScheduleEvent(EVENT_WARLOCK_RAIN, 16000);
            break;
        case EVENT_WARLOCK_CORRUPTION:
            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0, true, -SPELL_CORRUPTION))
                DoCast(target, SPELL_CORRUPTION, true);
            _events.ScheduleEvent(EVENT_WARLOCK_CORRUPTION, 4000);
            break;
        case EVENT_WARLOCK_SHADOWBOLT:
            DoCastVictim(SPELL_SHADOW_BOLT);
            _events.ScheduleEvent(EVENT_WARLOCK_SHADOWBOLT, urand(5000, 8000));
            break;
        case EVENT_PALADIN:
            DoCastAOE(SPELL_RADIANCE_AURA, true);
            break;
        case EVENT_PRIEST:
            if (Unit* valk = me->FindNearestCreature(37098, 40.0f, true))
                DoCast(valk, SPELL_RENEW, true);
            _events.ScheduleEvent(EVENT_PRIEST, 18000);
            break;
        case EVENT_PRIEST_HEAL:
            if (Unit* valk = me->FindNearestCreature(37098, 40.0f, true))
                DoCast(valk, SPELL_FLASH_HEAL);
            _events.ScheduleEvent(EVENT_PRIEST_HEAL, 5000);
            break;
        case EVENT_SHAMAN:
            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0, true))
                DoCast(target, SPELL_LIGHTNING_BOLT, true);
            _events.ScheduleEvent(EVENT_SHAMAN, urand(5000, 8000));
            break;
        case EVENT_SHAMAN_HEAL:
            DoCastAOE(SPELL_HEALING_RAIN);
            break;
        case EVENT_ROGUE:
            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0, true))
            {
                me->AddThreat(target, 99999999.0f);
                DoCast(target, SPELL_FOCUSED_ATTACKS);
            }
            _events.ScheduleEvent(EVENT_ROGUE, 50000);
            break;
        case EVENT_MAGE:
            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 0, true))
                DoCast(target, SPELL_FIREBALL, true);
            _events.ScheduleEvent(EVENT_MAGE, urand(5000, 8000));
            break;
        case EVENT_DEATHKNIGHT:
            DoCastVictim(SPELL_PLAGUE_STRIKE);
            _events.ScheduleEvent(EVENT_DEATHKNIGHT, urand(5000, 8000));
            break;
        case EVENT_DEATHKNIGHT_NECRO:
            DoCastVictim(SPELL_NECRO_STRIKE);
            _events.ScheduleEvent(EVENT_DEATHKNIGHT_NECRO, urand(10000, 15000));
            break;
        case EVENT_WARRIOR:
            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, -8.0f, true))
                DoCast(target, SPELL_HEROIC_LEAP);
            _events.ScheduleEvent(EVENT_WARRIOR, urand(45000, 55000));
            break;
        case EVENT_HUNTER:
            DoCastVictim(SPELL_SHOT);
            _events.ScheduleEvent(EVENT_HUNTER, 2000);
            break;
        case EVENT_DRUID:
            DoCastAOE(SPELL_CAT_FORM);
            _events.ScheduleEvent(EVENT_DRUID_RIP, urand(5000, 9000));
            break;
        case EVENT_DRUID_RIP:
            DoCastVictim(SPELL_RIP);
            _events.ScheduleEvent(EVENT_DRUID_RIP, urand(12000, 15000));
            break;
        default:
            break;
        }
    }

    DoMeleeAttackIfReady();
}

void NpcSeveredEssenceUpperSpireAI::DoAction(const int32 action)
{
    switch (action)
    {
    case ACTION_CLONE_OWNER:
        if (Player* master = me->ToTempSummon()->GetSummoner()->ToPlayer())
        {
            master->CastSpell(me, SPELL_CLONE_IMAGE, true);
            master->CastSpell(me, SPELL_CLONE_WEAPON, true);
            master->CastSpell(me, SPELL_CLONE_OFFHAND, true);
            me->SetInCombatWithZone();
        }
        _events.ScheduleEvent(EVENT_SET_CLASS, 500);
        break;
    default:
        break;
    }
}

class spell_severed_essence_icc : public SpellScriptLoader
{
public:
    spell_severed_essence_icc() : SpellScriptLoader("spell_severed_essence_icc") { }

    class spell_severed_essence_icc_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_severed_essence_icc_SpellScript);

        void OnHit (SpellEffIndex effIndex)
        {
            if (Player* target = GetHitPlayer())
                if (Creature* essence = target->SummonCreature(NPC_SEVERED_ESSENCE, target->GetPosition(), TEMPSUMMON_DEAD_DESPAWN))
                    essence->AI()->DoAction(ACTION_CLONE_OWNER);
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_severed_essence_icc_SpellScript::OnHit, EFFECT_1, SPELL_EFFECT_FORCE_CAST);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_severed_essence_icc_SpellScript();
    }
};

void LoadTrashUpperSpire()
{
    new spell_severed_essence_icc();
    new NpcValkyrUpperSpire();
    new NpcSeveredEssenceUpperSpire();
    new Npc_FrostFreezeTrap();
}