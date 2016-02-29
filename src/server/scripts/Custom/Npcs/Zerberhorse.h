class ZerberhorseAI : public ScriptedAI
{
public:
    ZerberhorseAI(Creature* creature);
    
    void KilledUnit(Unit* who);
    void MoveInLineOfSight(Unit*);

    void Reset();
    void JustReachedHome();
    void JustDied(Unit* /*killer*/);
    void SpellHitTarget(Unit* /*target*/, SpellInfo const* /*spell*/);

    void EnterCombat(Unit* /*who*/);

    void UpdateAI(const uint32 diff);
private:
    void HandleEvent(const uint32 Event);
    int32 ModPower(int32 delta);

    bool intro;
    int32 power;
    int32 round;

    uint32 phase;

    EventMap events;
};

class Zerberhorse : public CreatureScript
{
public:
    Zerberhorse() : CreatureScript("custom_zerberhorse") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new ZerberhorseAI(creature);
    };
};
