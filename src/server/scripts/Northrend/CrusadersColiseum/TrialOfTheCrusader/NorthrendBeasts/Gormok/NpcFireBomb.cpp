#include "../NorthrendBeasts.h"

enum Data
{
    SPELL_DOT_TRIGGER                       = 66318,
};

class NpcFireBombAI : public ScriptedAI
{
    public:
        NpcFireBombAI(Creature* creature) : ScriptedAI(creature) {}

        void Reset() 
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
            SetCombatMovement(false);
            DoCast(SPELL_DOT_TRIGGER);
            me->SetReactState(REACT_PASSIVE);
            me->SetDisplayId(me->GetCreatureTemplate()->Modelid2);
        }

        void UpdateAI(uint32 const dist) 
        {
            if (InstanceScript* Instance = me->GetInstanceScript())
                if (Instance->GetData(TYPE_ENCOUNTER_STATUS) != IN_PROGRESS)
                    me->DespawnOrUnsummon();
        }
};

class NpcFireBomb : public CreatureScript
{
    public:
        NpcFireBomb() : CreatureScript("npc_snobold_fire_bomb") {}

        CreatureAI* GetAI(Creature* creature) const
        {
            return new NpcFireBombAI(creature);
        }
};

void AddNpcFireBomb()
{
    new NpcFireBomb();
}
