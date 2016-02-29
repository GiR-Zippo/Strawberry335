#include "BossValkyr.h"
#include "../TwinValkyr.h"

enum
{
    EQUIP_MAIN_2                = 37377,

    EMOTE_DARK_VORTEX           = -1649049,
    SAY_DARK_VORTEX             = -1649050,

    SPELL_DARK_TWIN_SPIKE       = 66069,
    SPELL_DARK_SURGE            = 65768,
    SPELL_DARK_SHIELD           = 65874,
    SPELL_DARK_TWIN_PACT        = 65875,
    SPELL_DARK_VORTEX           = 66058,
    SPELL_DARK_TOUCH            = 67282,
    SPELL_TWIN_EMPATHY_LIGHT    = 66133
};

class BossEydisAI : public BossValkyrAI
{
    public:

    BossEydisAI(Creature* creature) : BossValkyrAI(creature)
    {
        m_uiWeapon = EQUIP_MAIN_2;
        m_uiAuraState = AURA_STATE_UNKNOWN19;
        m_uiVortexEmote = EMOTE_DARK_VORTEX;
        m_uiVortexSay = SAY_DARK_VORTEX;
        m_uiSisterNpcId = NPC_LIGHTBANE;
        m_uiMyEmphatySpellId = SPELL_TWIN_EMPATHY_LIGHT;
        m_uiOtherEssenceSpellId = SPELL_LIGHT_ESSENCE_HELPER;
        m_uiSurgeSpellId = SPELL_DARK_SURGE;
        m_uiVortexSpellId = SPELL_DARK_VORTEX;
        m_uiShieldSpellId = SPELL_DARK_SHIELD;
        m_uiTwinPactSpellId = SPELL_DARK_TWIN_PACT;
        m_uiTouchSpellId = SPELL_DARK_TOUCH;
        m_uiSpikeSpellId = SPELL_DARK_TWIN_SPIKE;
    }

    void Reset()
    {
        BossValkyrAI::Reset();
        SetEquipmentSlots(false, EQUIP_MAIN_2, EQUIP_UNEQUIP, EQUIP_NO_CHANGE);
        m_uiStage = 1;
    }
};

class BossEydis : public CreatureScript
{
    public:
        BossEydis() : CreatureScript("boss_eydis") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new BossEydisAI(creature);
        }

};

void AddBossEydis()
{
    new BossEydis();
}
