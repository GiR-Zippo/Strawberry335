#include "Npc_StrangulateVehicle.h"

void NpcStrangulateVehicleAI::IsSummonedBy(Unit* summoner)
{
    me->SetFacingToObject(summoner);
    DoCast(summoner, SPELL_HARVEST_SOUL_VEHICLE);
    _events.Reset();
    _events.ScheduleEvent(EVENT_MOVE_TO_LICH_KING, 2000);
    _events.ScheduleEvent(EVENT_TELEPORT, 6000);

    // this will let us easily access all creatures of this entry on heroic mode when its time to teleport back
    if (Creature* lichKing = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_THE_LICH_KING)))
        lichKing->AI()->JustSummoned(me);
}

void NpcStrangulateVehicleAI::DoAction(const int32 action)
{
    if (action != ACTION_TELEPORT_BACK)
        return;

    if (TempSummon* summ = me->ToTempSummon())
    {
        if (Unit* summoner = summ->GetSummoner())
        {
            DoCast(summoner, SPELL_HARVEST_SOUL_TELEPORT_BACK);
            summoner->RemoveAurasDueToSpell(SPELL_HARVEST_SOUL_DAMAGE_AURA);
        }
    }

    if (Creature* lichKing = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_THE_LICH_KING)))
        lichKing->AI()->SummonedCreatureDespawn(me);
}

void NpcStrangulateVehicleAI::UpdateAI(const uint32 diff)
{
    UpdateVictim();

    _events.Update(diff);

    while (uint32 eventId = _events.ExecuteEvent())
    {
        switch (eventId)
        {
            case EVENT_TELEPORT:
                me->GetMotionMaster()->Clear(false);
                me->GetMotionMaster()->MoveIdle();
                if (TempSummon* summ = me->ToTempSummon())
                {
                    if (Unit* summoner = summ->GetSummoner())
                    {
                        summoner->CastSpell((Unit*)NULL, SPELL_HARVEST_SOUL_VISUAL, true);
                        summoner->ExitVehicle(summoner);
                        if (!IsHeroic())
                            summoner->CastSpell(summoner, SPELL_HARVEST_SOUL_TELEPORT, true);
                        else
                        {
                            summoner->CastSpell(summoner, SPELL_HARVEST_SOULS_TELEPORT, true);
                            summoner->RemoveAurasDueToSpell(HARVEST_SOUL, 0, 0, AURA_REMOVE_BY_EXPIRE);
                        }
                    }
                }

                _events.ScheduleEvent(EVENT_DESPAWN_SELF, 65000);
                break;
            case EVENT_MOVE_TO_LICH_KING:
                if (Creature* lichKing = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_THE_LICH_KING)))
                {
                    if (me->GetExactDist(lichKing) > 10.0f)
                    {
                        Position pos;
                        lichKing->GetNearPosition(pos, float(rand_norm()) * 5.0f  + 7.5f, lichKing->GetAngle(me));
                        me->GetMotionMaster()->MovePoint(0, pos);
                    }
                }
                break;
            case EVENT_DESPAWN_SELF:
                if (Creature* lichKing = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_THE_LICH_KING)))
                    lichKing->AI()->SummonedCreatureDespawn(me);
                me->DespawnOrUnsummon(1);
                break;
            default:
                break;
        }
    }
}