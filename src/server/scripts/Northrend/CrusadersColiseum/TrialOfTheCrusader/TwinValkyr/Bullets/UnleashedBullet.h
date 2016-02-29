
enum AiType
{
    DARK_AI,
    LIGHT_AI
};

enum
{
    // Timers
    TIME_RANGE_CHECK            = 250,     // increase this, in case of performance issues !
    TIME_MOVEMENT_WAIT          = 500,

    // Spells
    SPELL_POWERING_UP           = 67590,
    SPELL_UNLEASHED_DARK        = 65808,
    SPELL_UNLEASHED_LIGHT       = 65795
};

class UnleashedBulletAI : public ScriptedAI
{
    public:
        UnleashedBulletAI(Creature* creature, AiType type);

        void Reset();

        void MovementInform(uint32 type, uint32 id);

        void SpellHitTarget(Unit* who, SpellInfo const* spell);

        void UpdateAI(uint32 const diff);

    private:
        InstanceScript* m_Instance;
        uint32 m_RangeCheckTimer;
        uint32 m_MovementTimer;

        AiType m_Type;

        void DoMovement();
};

class NpcLightBullet : public CreatureScript
{
    public:
        NpcLightBullet() : CreatureScript("mob_unleashed_light") { }
    
        CreatureAI* GetAI(Creature* creature) const
        {
            return new UnleashedBulletAI(creature, LIGHT_AI);
        }
};

class NpcDarkBullet : public CreatureScript
{
    public:
        NpcDarkBullet() : CreatureScript("mob_unleashed_dark") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new UnleashedBulletAI(creature, DARK_AI);
        }
};
