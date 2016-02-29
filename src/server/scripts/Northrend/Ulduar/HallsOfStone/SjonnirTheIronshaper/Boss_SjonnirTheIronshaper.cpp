/*
* Copyright (C) 2013 Laenalith Private WoW <http://www.laenalith-wow.com//>
* Copyright (C) 2008-2013 TrinityCore <http://www.trinitycore.org/>
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

#include "Boss_SjonnirTheIronshaper.h"
#include "Npc_SjonnirTheIronshaper.h"
typedef boss_sjonnir_the_ironshaper::boss_sjonnir_the_ironshaperAI SjonnirAI;


void SjonnirAI::Reset()
{
    _Reset();
    abuseTheOoze = 0;

    messageSent[0] = true;
    for (uint8 i = 1; i < 4; ++i)
        messageSent[i] = false;

    CreateBoundaryMap();
};

void SjonnirAI::JustReachedHome()
{
    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NON_ATTACKABLE);
    me->SetVisible(true);
};

void SjonnirAI::EnterCombat(Unit* who)
{
    if (!instance->CheckRequiredBosses(DATA_SJONNIR, who->ToPlayer()))
    {
        EnterEvadeMode();
        return;
    }

    _EnterCombat();
    DoScriptText(SAY_AGGRO, me);

    events.ScheduleEvent(EVENT_CHAIN_LIGHTNING, urand(3*IN_MILLISECONDS, 8*IN_MILLISECONDS));
    events.ScheduleEvent(EVENT_LIGHTNING_SHIELD, urand(20*IN_MILLISECONDS, 25*IN_MILLISECONDS));
    events.ScheduleEvent(EVENT_STATIC_CHARGE, urand(20*IN_MILLISECONDS, 25*IN_MILLISECONDS));
    events.ScheduleEvent(EVENT_LIGHTNING_RING, urand(30*IN_MILLISECONDS, 35*IN_MILLISECONDS));
    events.ScheduleEvent(EVENT_SUMMON, 5*IN_MILLISECONDS);
    events.ScheduleEvent(EVENT_FRENZY, 300*IN_MILLISECONDS);
    events.ScheduleEvent(EVENT_CHECK_IN_ROOM, 5*IN_MILLISECONDS);

    Creature* pBrann = Creature::GetCreature((*me), instance->GetData64(DATA_BRANN_FINAL));
    if (pBrann)
    {
        pBrann->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_NONE);
        pBrann->AI()->AttackStart(me);
        pBrann->AddThreat(me, 0.0f);
    }
};

void SjonnirAI::JustSummoned(Creature* summon)
{
    summon->GetMotionMaster()->MovePoint(2, AddPointLocations[currentPipeLoc]);
    summons.Summon(summon);
};

void SjonnirAI::JustDied(Unit* killer)
{
    _JustDied();
    DoScriptText(SAY_DEATH, me);

    if (killer->GetTypeId()== TYPEID_PLAYER)
        killer->ToPlayer()->GroupEventHappens(QUEST_HALLS_OF_STONE, me);
    else if (Player* pPlayer = me->FindNearestPlayer(50.0f))
        pPlayer->GroupEventHappens(QUEST_HALLS_OF_STONE, me);

    if (!instance)
        return;

    Creature* pCreature = NULL;
    if (pCreature = me->GetCreature((*me), instance->GetData64(DATA_TRIGGER_ENTRANCE)))
        pCreature->DespawnOrUnsummon();
};

void SjonnirAI::KilledUnit(Unit* who)
{
    if (who->GetTypeId() == TYPEID_PLAYER)
        DoScriptText(SAY_DEATH, me);

    if (who->GetEntry() == CREATURE_BRANN_FINAL)
        EnterEvadeMode();
};

void SjonnirAI::DoAction(int32 const action)
{
    if (action == ACTION_OOZE_DEAD)
        ++abuseTheOoze;
};

uint32 SjonnirAI::GetData(uint32 type) const
{
    if (type == DATA_ABUSE_THE_OOZE)
        return abuseTheOoze;

    return 0;
};

void SjonnirAI::UpdateAI(uint32 const diff)
{
    if (!UpdateVictim())
        return;

    events.Update(diff);

    if (me->HasUnitState(UNIT_STATE_CASTING))
        return;

    ExecuteEvents();

    DoMeleeAttackIfReady();
};

void SjonnirAI::ExecuteEvents()
{
    while (uint32 eventId = events.ExecuteEvent())
    {
        switch (eventId)
        {
            case EVENT_CHAIN_LIGHTNING:
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                    DoCast(target, DUNGEON_MODE(SPELL_CHAIN_LIGHTING, H_SPELL_CHAIN_LIGHTING));
                events.ScheduleEvent(EVENT_CHAIN_LIGHTNING, urand(10*IN_MILLISECONDS, 15*IN_MILLISECONDS));
            }break;

            case EVENT_LIGHTNING_SHIELD:
            {
                DoCast(me, DUNGEON_MODE(SPELL_LIGHTING_SHIELD, H_SPELL_LIGHTING_SHIELD));
            }break;

            case EVENT_STATIC_CHARGE:
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                    DoCast(target, DUNGEON_MODE(SPELL_STATIC_CHARGE, H_SPELL_STATIC_CHARGE));
                events.ScheduleEvent(EVENT_STATIC_CHARGE, urand(20*IN_MILLISECONDS, 25*IN_MILLISECONDS));
            }break;

            case EVENT_LIGHTNING_RING:
            {
                DoCast(me, DUNGEON_MODE(SPELL_LIGHTING_RING, H_SPELL_LIGHTING_RING));
                events.ScheduleEvent(EVENT_LIGHTNING_RING, urand(30*IN_MILLISECONDS, 35*IN_MILLISECONDS));
            }break;

            case EVENT_SUMMON:
            {
                currentPipeLoc = urand(0, 1);
                uint32 timer = 20*IN_MILLISECONDS;

                if (HealthAbovePct(75))
                {
                    me->SummonCreature(NPC_FORGED_IRON_DWARF, PipeLocations[currentPipeLoc], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30*IN_MILLISECONDS);
                    if (!messageSent[0])
                    {
                        DoScriptText(RAND(SAY_RANDOM_1, SAY_RANDOM_2), me);
                        messageSent[0] = true;
                    }
                    else
                        messageSent[0] = false;
                }
                else if (HealthAbovePct(50))
                {
                    me->SummonCreature(NPC_FORGED_IRON_TROGG, PipeLocations[currentPipeLoc], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30*IN_MILLISECONDS);
                    if (Creature* pBrann = Creature::GetCreature((*me), instance->GetData64(DATA_BRANN_FINAL)))
                    {
                        if (!messageSent[1] && pBrann)
                        {
                            DoScriptText(SAY_BRANN_SPAWN_TROGG, pBrann);
                            messageSent[1] = true;
                        }
                    }
                    timer = 17*IN_MILLISECONDS;
                }
                else if (HealthAbovePct(25))
                {
                    me->SummonCreature(NPC_MALFORMED_OOZE, PipeLocations[currentPipeLoc], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30*IN_MILLISECONDS);
                    if (Creature* pBrann = Creature::GetCreature((*me), instance->GetData64(DATA_BRANN_FINAL)))
                    {
                        if (!messageSent[2] && pBrann->isAlive())
                        {
                            DoScriptText(SAY_BRANN_SPAWN_OOZE, pBrann);
                            messageSent[2] = true;
                        }
                    }
                    timer = 15*IN_MILLISECONDS;
                }
                else
                {
                    me->SummonCreature(NPC_EARTHEN_DWARF, PipeLocations[currentPipeLoc], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30*IN_MILLISECONDS);
                    if (Creature* pBrann = Creature::GetCreature((*me), instance->GetData64(DATA_BRANN_FINAL)))
                    {
                        if (!messageSent[3] && pBrann->isAlive())
                        {
                            DoScriptText(SAY_BRANN_SPAWN_EARTHEN, pBrann);
                            messageSent[3] = true;
                        }
                    }
                    timer = 15*IN_MILLISECONDS;
                }

                    events.ScheduleEvent(EVENT_SUMMON, timer);
            }break;

            case EVENT_FRENZY:
            {
                /// @todo: add emote - which emote?
                DoCast(me, SPELL_FRENZY, true);
            }break;

            case EVENT_CHECK_IN_ROOM:
            {
                if (CheckBoundaryMap(&_boundaryMap))
                    events.ScheduleEvent(EVENT_CHECK_IN_ROOM, 3*IN_MILLISECONDS);
                else
                {
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NON_ATTACKABLE);
                    me->SetVisible(false);
                    EnterEvadeMode();
                }
            }break;
        }
    }
};

void SjonnirAI::CreateBoundaryMap()
{
    _boundaryMap.clear();
    _boundaryMap[BOUNDARY_S] = BOUNDARY_POSITION_S;
    _boundaryMap[BOUNDARY_E] = BOUNDARY_POSITION_E;
    _boundaryMap[BOUNDARY_W] = BOUNDARY_POSITION_W;
};
