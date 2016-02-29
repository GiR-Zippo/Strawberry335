
enum
{
    SPELL_CONTROLLER_PERIODIC   = 66149
};

struct NpcBulletControlAI : public Scripted_NoMovementAI
{
    public:
        NpcBulletControlAI(Creature* creature) : Scripted_NoMovementAI(creature)
        {
            Reset();
        }

        void Reset()
        {
            DoCastAOE(SPELL_CONTROLLER_PERIODIC);
        }

        void UpdateAI(const uint32 /*uiDiff*/)
        {
            UpdateVictim();
        }
};

class NpcBulletControl : public CreatureScript
{
    public:
        NpcBulletControl() : CreatureScript("mob_bullet_controller") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new NpcBulletControlAI(creature);
        }
};

void AddNpcBulletControl()
{
    new NpcBulletControl();
}
