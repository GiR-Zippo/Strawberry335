#include "BossValkyr.h"
#include "../TwinValkyr.h"

class OrbsDespawner : public BasicEvent
{
    public:
        explicit OrbsDespawner(Creature* creature) : _creature(creature)
        {
        }

        bool Execute(uint64 /*currTime*/, uint32 /*diff*/)
        {
            Trinity::CreatureWorker<OrbsDespawner> worker(_creature, *this);
            _creature->VisitNearbyGridObject(5000.0f, worker);
            return true;
        }

        void operator()(Creature* creature) const
        {
            switch (creature->GetEntry())
            {
                case NPC_BULLET_DARK:
                case NPC_BULLET_LIGHT:
                    creature->DespawnOrUnsummon();
                    return;
                default:
                    return;
            }
        }

    private:
        Creature* _creature;
};

BossValkyrAI::BossValkyrAI(Creature* creature) : ScriptedAI(creature), Summons(me)
{
    instance = creature->GetInstanceScript();
}

void BossValkyrAI::Reset()
{
    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NOT_SELECTABLE);
    me->SetReactState(REACT_PASSIVE);
    me->ModifyAuraState(m_uiAuraState, true);
    // Uncomment this once that they are flying above the ground
    /*me->SetHover(true);
    me->AddUnitMovementFlag(MOVEMENTFLAG_DISABLE_GRAVITY);*/
    m_bIsBerserk = false;

    m_uiSpecialAbilityTimer = MINUTE*IN_MILLISECONDS;
    m_uiSpikeTimer = 20*IN_MILLISECONDS;
    m_uiTouchTimer = urand(10, 15)*IN_MILLISECONDS;
    m_uiBerserkTimer = IsHeroic() ? 6*MINUTE*IN_MILLISECONDS : 10*MINUTE*IN_MILLISECONDS;

    Summons.DespawnAll();
}

void BossValkyrAI::JustReachedHome()
{
    Summons.DespawnAll();
    me->DespawnOrUnsummon();
}

void BossValkyrAI::MovementInform(uint32 uiType, uint32 uiId)
{
    if (uiType != POINT_MOTION_TYPE)
        return;

    switch (uiId)
    {
        case 1:
        {
            if (instance)
                instance->SetData(GO_MAIN_GATE_DOOR, CLOSE);
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NOT_SELECTABLE);
            me->SetReactState(REACT_AGGRESSIVE);
            me->SetInCombatWithZone();
            break;
        }
    }
}

void BossValkyrAI::KilledUnit(Unit* who)
{
    if (who->GetTypeId() == TYPEID_PLAYER)
        DoScriptText(urand(0, 1) ? SAY_KILL1 : SAY_KILL2, me);
}

void BossValkyrAI::JustSummoned(Creature* summoned)
{
    Summons.Summon(summoned);
}

void BossValkyrAI::SummonedCreatureDespawn(Creature* summoned)
{
    switch (summoned->GetEntry())
    {
        case NPC_LIGHT_ESSENCE:
            instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_LIGHT_ESSENCE_HELPER);
            instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_POWERING_UP_HELPER);
            instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_EMPOWERED_LIGHT_HELPER);
            break;
        case NPC_DARK_ESSENCE:
            instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_DARK_ESSENCE_HELPER);
            instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_POWERING_UP_HELPER);
            instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_EMPOWERED_DARK_HELPER);
            break;
        case NPC_BULLET_CONTROLLER:
            me->m_Events.AddEvent(new OrbsDespawner(me), me->m_Events.CalculateTime(100));
            break;
    }
    Summons.Despawn(summoned);
}

void BossValkyrAI::JustDied(Unit* /*killer*/)
{
    DoScriptText(SAY_DEATH, me);
    if (instance)
    {
        if (Creature* pSister = GetSister())
        {
            if (!pSister->isAlive())
            {
                me->SetFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
                pSister->SetFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);

                instance->SetData(TYPE_ENCOUNTER_STATUS, DONE);
                Summons.DespawnAll();
            }
            else
                me->RemoveFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
        }
    }
    Summons.DespawnAll();
}

Creature* BossValkyrAI::GetSister()
{
    return Unit::GetCreature((*me), instance->GetData64(m_uiSisterNpcId));
}

void BossValkyrAI::EnterCombat(Unit* /*who*/)
{
    me->SetInCombatWithZone();
    if (instance)
    {
        if (Creature* pSister = GetSister())
        {
            me->AddAura(m_uiMyEmphatySpellId, pSister);
            pSister->SetInCombatWithZone();
        }
    }

    DoScriptText(SAY_AGGRO, me);
    DoCast(me, m_uiSurgeSpellId);
}

void BossValkyrAI::DoAction(const int32 action)
{
    switch (action)
    {
        case ACTION_VORTEX:
            m_uiStage = me->GetEntry() == NPC_LIGHTBANE ? 2 : 1;
            break;
        case ACTION_PACT:
            m_uiStage = me->GetEntry() == NPC_LIGHTBANE ? 1 : 2;
            break;
    }
}

void BossValkyrAI::EnableDualWield(bool mode)
{
    SetEquipmentSlots(false, m_uiWeapon, mode ? m_uiWeapon : int32(EQUIP_UNEQUIP), EQUIP_UNEQUIP);
    me->SetCanDualWield(mode);
    me->UpdateDamagePhysical(mode ? OFF_ATTACK : BASE_ATTACK);
}

void BossValkyrAI::UpdateAI(const uint32 uiDiff)
{
    if (!instance || !UpdateVictim())
        return;

    switch (m_uiStage)
    {
        case 0:
            break;
        case 1: // Vortex
            if (m_uiSpecialAbilityTimer <= uiDiff)
            {
                if (Creature* pSister = GetSister())
                    pSister->AI()->DoAction(ACTION_VORTEX);
                DoScriptText(m_uiVortexEmote, me);
                DoScriptText(m_uiVortexSay, me);
                DoCastAOE(m_uiVortexSpellId);
                m_uiStage = 0;
                m_uiSpecialAbilityTimer = MINUTE*IN_MILLISECONDS;
            }
            else
                m_uiSpecialAbilityTimer -= uiDiff;
            break;
        case 2: // Shield+Pact
            if (m_uiSpecialAbilityTimer <= uiDiff)
            {
                DoScriptText(EMOTE_SHIELD, me);
                DoScriptText(SAY_SHIELD, me);
                if (Creature* pSister = GetSister())
                {
                    pSister->AI()->DoAction(ACTION_PACT);
                    pSister->CastSpell(pSister, SPELL_POWER_TWINS, false);
                }
                DoCast(me, m_uiShieldSpellId);
                DoCast(me, m_uiTwinPactSpellId);
                m_uiStage = 0;
                m_uiSpecialAbilityTimer = MINUTE*IN_MILLISECONDS;
            }
            else
                m_uiSpecialAbilityTimer -= uiDiff;
            break;
        default:
            break;
    }

    if (m_uiSpikeTimer <= uiDiff)
    {
        DoCastVictim(m_uiSpikeSpellId);
        m_uiSpikeTimer = 20*IN_MILLISECONDS;
    }
    else
        m_uiSpikeTimer -= uiDiff;

    if (IsHeroic() && m_uiTouchTimer <= uiDiff)
    {
        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 200, true, m_uiOtherEssenceSpellId))
            me->CastCustomSpell(m_uiTouchSpellId, SPELLVALUE_MAX_TARGETS, 1, target, TRIGGERED_FULL_MASK);
        m_uiTouchTimer = urand(10, 15)*IN_MILLISECONDS;
    }
    else
        m_uiTouchTimer -= uiDiff;

    if (!m_bIsBerserk && m_uiBerserkTimer <= uiDiff)
    {
        DoCast(me, SPELL_BERSERK);
        DoScriptText(SAY_BERSERK, me);
        m_bIsBerserk = true;
    }
    else
        m_uiBerserkTimer -= uiDiff;

    DoMeleeAttackIfReady();
}

void AddBossEydis();
void AddBossFjola();
void AddSpellTwinShield();
void AddSpellTwinPower();

void AddBossTwinValkyr()
{
    AddBossEydis();
    AddBossFjola();
    AddSpellTwinShield();
    AddSpellTwinPower();
}
