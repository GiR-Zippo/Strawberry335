
class SpellBulletControl_AuraScript : public AuraScript
{
    public:
        PrepareAuraScript(SpellBulletControl_AuraScript);
    
        SpellBulletControl_AuraScript() : 
            m_SpellLast(0),
            m_SpellCount(0)
        {
            ;
        }

        void HandleEffect(AuraEffect const* /*aurEff*/)
        {
            m_SpellCount++;
            if (m_SpellCount < 6)
                return;

            uint32 num1, num2;

            if (m_SpellCount == 6 || m_SpellCount == 13 + m_SpellLast)
            {
                uint32 max = GetBulletCount() / 2;
                num1 = urand(1, max);
                num2 = urand(1, max);
                m_SpellLast = m_SpellCount;
            }
            else
            {
                num1 = 2;
                num2 = 2;
            }

            GetCaster()->CastCustomSpell(66152, SPELLVALUE_MAX_TARGETS, num1, GetTarget(), TRIGGERED_FULL_MASK);
            GetCaster()->CastCustomSpell(66153, SPELLVALUE_MAX_TARGETS, num2, GetTarget(), TRIGGERED_FULL_MASK);

        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(SpellBulletControl_AuraScript::HandleEffect, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        }

    private:
        uint32 m_SpellCount;
        uint32 m_SpellLast;

        uint32 GetBulletCount()
        {
            switch (GetCaster()->GetMap()->GetDifficulty())
            {
                case 0:
                    return 10;
                case 1:
                    return 15;
                case 2:
                    return 15;
                default:
                    return 20;
            }
        }
};

class SpellBulletControl : public SpellScriptLoader
{
    public:
        SpellBulletControl() : SpellScriptLoader("spell_valkyr_bullet_control") { }

        AuraScript* GetAuraScript() const
        {
            return new SpellBulletControl_AuraScript();
        }
};

void AddSpellBulletControl()
{
    new SpellBulletControl();
}
