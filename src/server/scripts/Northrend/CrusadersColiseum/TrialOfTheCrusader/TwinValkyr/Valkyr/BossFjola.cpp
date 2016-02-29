#include "BossValkyr.h"
#include "../TwinValkyr.h"

enum
{
    EQUIP_MAIN_1                = 9423,

    EMOTE_LIGHT_VORTEX          = -1649047,
    SAY_LIGHT_VORTEX            = -1649048,

    SPELL_LIGHT_TWIN_SPIKE      = 66075,
    SPELL_LIGHT_SURGE           = 65766,
    SPELL_LIGHT_SHIELD          = 65858,
    SPELL_LIGHT_TWIN_PACT       = 65876,
    SPELL_LIGHT_VORTEX          = 66046,
    SPELL_LIGHT_TOUCH           = 67297,
    SPELL_TWIN_EMPATHY_DARK     = 66132
};

class BossFjolaAI : public BossValkyrAI
{
    public:
        BossFjolaAI(Creature* creature) : BossValkyrAI(creature)
        {
            instance = creature->GetInstanceScript();
            m_uiWeapon = EQUIP_MAIN_1;
            m_uiAuraState = AURA_STATE_UNKNOWN22;
            m_uiVortexEmote = EMOTE_LIGHT_VORTEX;
            m_uiVortexSay = SAY_LIGHT_VORTEX;
            m_uiSisterNpcId = NPC_DARKBANE;
            m_uiMyEmphatySpellId = SPELL_TWIN_EMPATHY_DARK;
            m_uiOtherEssenceSpellId = SPELL_DARK_ESSENCE_HELPER;
            m_uiSurgeSpellId = SPELL_LIGHT_SURGE;
            m_uiVortexSpellId = SPELL_LIGHT_VORTEX;
            m_uiShieldSpellId = SPELL_LIGHT_SHIELD;
            m_uiTwinPactSpellId = SPELL_LIGHT_TWIN_PACT;
            m_uiTouchSpellId = SPELL_LIGHT_TOUCH;
            m_uiSpikeSpellId = SPELL_LIGHT_TWIN_SPIKE;
        }

        void Reset()
        {
            BossValkyrAI::Reset();
            SetEquipmentSlots(false, EQUIP_MAIN_1, EQUIP_UNEQUIP, EQUIP_NO_CHANGE);
            m_uiStage = 0;

            if (instance)
                instance->DoStopTimedAchievement(ACHIEVEMENT_TIMED_TYPE_EVENT,  EVENT_START_TWINS_FIGHT);
        }

        void EnterCombat(Unit* who)
        {
            if (instance)
            {
                instance->SetData(TYPE_ENCOUNTER_STATUS, IN_PROGRESS);
                instance->DoStartTimedAchievement(ACHIEVEMENT_TIMED_TYPE_EVENT,  EVENT_START_TWINS_FIGHT);
            }

            if (Creature* temp = me->SummonCreature(NPC_BULLET_CONTROLLER, TrialCrusaderLoc[0]))
            {
                temp->SetVisible(false);
                temp->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                temp->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            }
            BossValkyrAI::EnterCombat(who);
        }

        void EnterEvadeMode()
        {
            if (instance)
                instance->SetData(GO_MAIN_GATE_DOOR, OPEN);
            BossValkyrAI::EnterEvadeMode();
        }

        void JustReachedHome()
        {
            if (instance)
                instance->SetData(GO_MAIN_GATE_DOOR, CLOSE);

            BossValkyrAI::JustReachedHome();
        }

    private:
        InstanceScript* instance;
};

class BossFjola : public CreatureScript
{
    public:
        BossFjola() : CreatureScript("boss_fjola") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new BossFjolaAI(creature);
        }
};


void AddBossFjola()
{
    new BossFjola();
}
