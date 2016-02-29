enum
{
    SAY_KILL1                   = -1649045,
    SAY_KILL2                   = -1649046,
    SAY_AGGRO                   = -1649040,
    SAY_DEATH                   = -1649041,
    SAY_BERSERK                 = -1649042,
    EMOTE_SHIELD                = -1649043,
    SAY_SHIELD                  = -1649044,

    SPELL_DARK_ESSENCE          = 65684,
    SPELL_LIGHT_ESSENCE         = 65686,
    SPELL_POWER_TWINS           = 65879,
    SPELL_BERSERK               = 64238,

    NPC_BULLET_DARK             = 34628,
    NPC_BULLET_LIGHT            = 34630,
    NPC_BULLET_CONTROLLER       = 34743,

    ACTION_VORTEX               = 1,
    ACTION_PACT                 = 2
};

class BossValkyrAI : public ScriptedAI
{
    public:
        BossValkyrAI(Creature* creature);

        InstanceScript* instance;
        SummonList Summons;

        AuraStateType  m_uiAuraState;

        uint8  m_uiStage;
        bool   m_bIsBerserk;
        uint32 m_uiWeapon;
        uint32 m_uiSpecialAbilityTimer;
        uint32 m_uiSpikeTimer;
        uint32 m_uiTouchTimer;
        uint32 m_uiBerserkTimer;

        int32 m_uiVortexSay;
        int32 m_uiVortexEmote;
        uint32 m_uiSisterNpcId;
        uint32 m_uiMyEmphatySpellId;
        uint32 m_uiOtherEssenceSpellId;
        uint32 m_uiSurgeSpellId;
        uint32 m_uiVortexSpellId;
        uint32 m_uiShieldSpellId;
        uint32 m_uiTwinPactSpellId;
        uint32 m_uiSpikeSpellId;
        uint32 m_uiTouchSpellId;

        void Reset();

        void JustReachedHome();

        void MovementInform(uint32 uiType, uint32 uiId);

        void KilledUnit(Unit* who);

        void JustSummoned(Creature* summoned);

        void SummonedCreatureDespawn(Creature* summoned);

        void JustDied(Unit* /*killer*/);

        Creature* GetSister();

        void EnterCombat(Unit* /*who*/);

        void DoAction(const int32 action);

        void EnableDualWield(bool mode = true);

        void UpdateAI(const uint32 diff);
};
