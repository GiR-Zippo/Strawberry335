enum Spells {
    SPELL_LIGHT_ESSENCE     =   65712,
    SPELL_DARK_ESSENCE      =   65684,
    SPELL_VISUAL_EXPLODE    =   22579,
    SPELL_EXPLODE           =   63548,
    SPELL_EXPLODE_BIG       =   63548,
    SPELL_KRACHEN           =   67038
};

enum Event {
    // Tlaloc
    EVENT_KRACHEN           =   1,
    EVENT_KRACHEN_2         =   2,
    EVENT_KRACHEN_3         =   3,
    EVENT_BIG_KRACHEN       =   4,
    EVENT_RESET             =   5,
    // Add
    EVENT_EXPLODE           =   6
};

enum Actions {
    ACTION_EXPLODE_START    = 1,
    ACTION_EXPLODE_BIG      = 2
};

enum Creatures {
    NPC_WATER_TRIGGER       =   1006005,
};

const float dir[6] =
{
    0.45f, 
    2.14f, 
    5.23f, 
    3.50f,
    6.05f,
    2.81f
};

const uint32 creatureKrachen[6] =
{
    1006006,
    1006007,
    1006008,
    1006009,
    1006012,
    1006013
};

class TlalocAI : public ScriptedAI
{
public:
    TlalocAI(Creature* creature) : ScriptedAI(creature)
    { }

    void KilledUnit(Unit* who);

    void Reset();
    void JustDied(Unit* /*killer*/);

    void EnterCombat(Unit* /*who*/);

    void UpdateAI(const uint32 diff);

private:
    EventMap events;
    uint32 phase;
    uint32 phase2;
    uint32 phase3;
    std::list<Creature*> creatures;
    std::list<Creature*> creatures2;
    std::list<Creature*> creatures3;
};

class Tlaloc : public CreatureScript
{
public:
    Tlaloc() : CreatureScript("custom_tlaloc") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new TlalocAI(creature);
    };
};

