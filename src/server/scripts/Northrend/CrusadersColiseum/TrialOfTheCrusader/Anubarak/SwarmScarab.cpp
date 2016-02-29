/*
 * Copyright (C) 2012 Zirkon
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "Anubarak.h"

enum Spells
{
    // Scarab
    SPELL_DETERMINATION     = 66092,
    SPELL_ACID_MANDIBLE     = 65774, //Passive - Triggered
};

class SwarmScarab : public CreatureScript
{
public:
    class SwarmScarabAI : public ScriptedAI
    {
    public:
        SwarmScarabAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
        }

        InstanceScript* instance;

        uint32 m_uiDeterminationTimer;

        void Reset()
        {
            me->SetCorpseDelay(0);
            m_uiDeterminationTimer = urand(5*IN_MILLISECONDS, 60*IN_MILLISECONDS);
            DoCast(me, SPELL_ACID_MANDIBLE);
            me->SetInCombatWithZone();
            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                me->AddThreat(target, 20000.0f);
            if (!me->isInCombat())
                me->DisappearAndDie();
        }

        void DoAction(const int32 actionId)
        {
            switch (actionId)
            {
                case ACTION_SCARAB_SUBMERGE:
                    DoCast(SPELL_SUBMERGE_EFFECT);
                    me->DespawnOrUnsummon(1000);
                    break;
            }
        }

        void JustDied(Unit* killer)
        {
            DoCast(killer, RAID_MODE<uint32>(SPELL_TRAITOR_KING_10, SPELL_TRAITOR_KING_25));
        }

        void UpdateAI(const uint32 uiDiff)
        {
            if (!UpdateVictim())
                return;

            /* Bosskillers don't recognize */
            if (m_uiDeterminationTimer <= uiDiff)
            {
                DoCast(me, SPELL_DETERMINATION);
                m_uiDeterminationTimer = urand(10*IN_MILLISECONDS, 60*IN_MILLISECONDS);
            } else m_uiDeterminationTimer -= uiDiff;

            DoMeleeAttackIfReady();
        }
    };
    
    SwarmScarab() : CreatureScript("mob_swarm_scarab") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new SwarmScarabAI(creature);
    };
};

class SpellSummonScarab : public SpellScriptLoader
{
public:
    SpellSummonScarab() : SpellScriptLoader("spell_summon_scarab") { }
    
    class SpellSummonScarabScript : public SpellScript
    {
        PrepareSpellScript(SpellSummonScarabScript);

        bool Validate(SpellInfo const* /*spellEntry*/)
        {
            return true;
        }

        bool Load()
        {
            if (GetCaster() && GetCaster()->GetMap() && GetCaster()->GetMap()->Is25ManRaid())
                GetSpell()->SetSpellValue(SPELLVALUE_MAX_TARGETS, 2);
            else
                GetSpell()->SetSpellValue(SPELLVALUE_MAX_TARGETS, 1);
            return true;
        }

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            for (std::list<WorldObject*>::iterator itr = targets.begin(); itr != targets.end();)
            {
                if ((*itr)->GetTypeId() == TYPEID_UNIT)
                    if (WorldObject* wob = *itr)
                        if (Unit* u = wob->ToUnit())
                            if (u->GetEntry() == NPC_BURROW)
                            {
                                itr++;
                                continue;
                            }
                targets.erase(itr++);
            }
        }

        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(SpellSummonScarabScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new SpellSummonScarabScript();
    }
};

void AddSwarmScarab()
{
    new SwarmScarab();
    new SpellSummonScarab();
}
