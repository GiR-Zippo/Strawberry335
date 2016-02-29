/**
 * @file
 * @author TehKevin for Laenalith-WoW
 * @date 13.10.2014
 *
 * Contact kevin.unegg@ainet.at
 * Website: http://www.laenalith-wow.com/
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @copyright Copyright 2014 Laenalith-WoW. All rights reserved.
 */

#include "Spell_HallsOfReflection.h"

using namespace SPELL_HALLSOFREFLECTION;


typedef Spell_HorStartQuest::Spell_HorStartQuest_SpellScript                QuestAESpellScript;
typedef Spell_HorGunshipCannonFire::Spell_HorGunshipCannonFire_AuraScript   CannonFireAuraScript;
typedef Spell_HorSummonReflections::Spell_HorSummonReflections_SpellScript  SummonReflectionsSpellScript;

void QuestAESpellScript::StartQuests(SpellEffIndex /*effIndex*/)
{
    if (Unit* pUnit = GetHitUnit())
    {
        if (Player* pPlayer = pUnit->ToPlayer())
        {
            if (pPlayer->GetTeam() == ALLIANCE)
                pPlayer->CastSpell(pPlayer, SPELL_START_HALLS_OF_REFLECTION_QUEST_A, true);
            else
                pPlayer->CastSpell(pPlayer, SPELL_START_HALLS_OF_REFLECTION_QUEST_H, true);
        }
    }
}

void QuestAESpellScript::Register()
{
    OnEffectHitTarget += SpellEffectFn(QuestAESpellScript::StartQuests, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
}


void CannonFireAuraScript::HandlePeriodic(AuraEffect const* /*aurEff*/)
{
    if (!urand(0, 2))
    {
        if (GetTarget()->GetEntry() == NPC_GUNSHIP_CANNON_HORDE)
            GetTarget()->CastSpell((Unit*)NULL, SPELL_GUNSHIP_CANNON_FIRE_MISSILE_HORDE, true);
        else
            GetTarget()->CastSpell((Unit*)NULL, SPELL_GUNSHIP_CANNON_FIRE_MISSILE_ALLIANCE, true);
    }
}

void CannonFireAuraScript::Register()
{
    OnEffectPeriodic += AuraEffectPeriodicFn(CannonFireAuraScript::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
}


void SummonReflectionsSpellScript::HandleEffectHit(SpellEffIndex /*effIndex*/)
{
    if (!GetCaster())
        return;

    if (Creature* pCreature = GetCaster()->ToCreature())
    {
        if (!pCreature->AI())
            return;

        std::list<Unit*> targetList;
        pCreature->AI()->SelectTargetList(targetList, 3, SELECT_TARGET_RANDOM);

        if (!targetList.empty())
        {
            for (std::list<Unit*>::const_iterator itr = targetList.begin(); itr != targetList.end(); ++itr)
            {
                Creature* pSummon = pCreature->SummonCreature(NPC_REFLECTION, pCreature->GetPosition(), TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 3000);
                pSummon->SetName((*itr)->GetName());

                (*itr)->CastSpell(pSummon, SPELL_CLONE_NAME, true);
                (*itr)->CastSpell(pSummon, SPELL_CLONE_MODEL, true);

                pSummon->setFaction(pCreature->getFaction());
                pSummon->AI()->AttackStart((*itr));
            }
        }
    }
}

void SummonReflectionsSpellScript::Register()
{
    OnEffectHitTarget += SpellEffectFn(SummonReflectionsSpellScript::HandleEffectHit, EFFECT_0, SPELL_EFFECT_DUMMY);
}
