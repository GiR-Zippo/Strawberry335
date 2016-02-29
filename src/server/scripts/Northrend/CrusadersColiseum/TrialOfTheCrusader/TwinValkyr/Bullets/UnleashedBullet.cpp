#include "UnleashedBullet.h"
#include "../TwinValkyr.h"

#define SPELL_UNLEASHED_DARK_HELPER RAID_MODE<uint32>(65808, 67172, 67173, 67174)
#define SPELL_UNLEASHED_LIGHT_HELPER RAID_MODE<uint32>(65795, 67238, 67239, 67240)

UnleashedBulletAI::UnleashedBulletAI(Creature* creature, AiType type) : ScriptedAI(creature), m_Type(type)
{
    m_Instance = creature->GetInstanceScript();
}

void UnleashedBulletAI::Reset()
{
    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_NPC | UNIT_FLAG_NOT_SELECTABLE);
    me->SetReactState(REACT_PASSIVE);
    me->setFaction(14);
    //me->SetDisableGravity(true);
    //me->SetCanFly(true);
    SetCombatMovement(false);
    DoMovement();
    m_RangeCheckTimer = TIME_RANGE_CHECK;
    m_MovementTimer = TIME_MOVEMENT_WAIT;
}

void UnleashedBulletAI::MovementInform(uint32 type, uint32 id)
{
    if (type != POINT_MOTION_TYPE)
        return;

    if (id == 0)
    {
        m_MovementTimer = TIME_MOVEMENT_WAIT;
    }
}

void UnleashedBulletAI::SpellHitTarget(Unit* who, SpellInfo const* spell)
{
    if (m_Type == DARK_AI && (spell->Id != SPELL_UNLEASHED_DARK_HELPER || !who->HasAura(SPELL_DARK_ESSENCE_HELPER)))
        return;

    if (m_Type == LIGHT_AI && (spell->Id != SPELL_UNLEASHED_LIGHT_HELPER|| !who->HasAura(SPELL_LIGHT_ESSENCE_HELPER)))
        return;

    if (who->HasAura(SPELL_POWERING_UP))
        who->CastCustomSpell(SPELL_POWERING_UP, SPELLVALUE_AURA_STACK, 5, who, TRIGGERED_FULL_MASK);
    else
        who->CastCustomSpell(SPELL_POWERING_UP, SPELLVALUE_AURA_STACK, 6, who, TRIGGERED_FULL_MASK);
}

void UnleashedBulletAI::UpdateAI(const uint32 diff)
{
    if (m_MovementTimer && m_MovementTimer <= diff)
    {
        m_MovementTimer = 0;
        DoMovement();
    }
    else
        m_MovementTimer -= diff;

    if (m_RangeCheckTimer <= diff)
    {
        m_RangeCheckTimer = TIME_RANGE_CHECK;
        if (me->SelectNearestPlayer(0.2f))
        {
            if (m_Type == DARK_AI)
                DoCastAOE(SPELL_UNLEASHED_DARK);
            else
                DoCastAOE(SPELL_UNLEASHED_LIGHT);

            me->DespawnOrUnsummon(1000);
        }
    }
    else
        m_RangeCheckTimer -= diff;
}

void UnleashedBulletAI::DoMovement()
{
    float mx = TrialCrusaderLoc[0].GetPositionX();
    float my = TrialCrusaderLoc[0].GetPositionY();

    float x = me->GetPositionX();
    float y = me->GetPositionY();

    float dx = mx - x;
    float dy = my - y;

    float a = 0.f;

    while (a < 0.0001f)
    {
        dx += frand(-5.0f, 5.0f);
        dy += frand(-5.0f, 5.0f);

        a = dx * dx + dy * dy;
    }

    float b = (dx * (x - mx) + dy * (y - my));
    float c = (x - mx) * (x - mx) + (y - my) * (y - my) - 3844.0f;

    c = b * b - a * c;

    if (c < 0.f)
    {
        me->GetMotionMaster()->MovePoint(0, dx + x, dy + y, me->GetPositionZ());
        return;
    }

    if (c == 0.f)
        a = (-b) / a;
    else
    {
        c = sqrt(c);
        a = std::max(-b + c, -b - c) / a;
    }

    me->GetMotionMaster()->MovePoint(0, a * dx + x, a * dy + y, me->GetPositionZ());
}

void AddNpcBulletControl();
void AddSpellBulletControl();

void AddBullets()
{
    AddNpcBulletControl();
    AddSpellBulletControl();
    new NpcLightBullet();
    new NpcDarkBullet();
}
