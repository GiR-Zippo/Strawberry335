#include "Npc_DreamCloud.h"

void NpcDreamCloudAI::Reset()
{
    _events.Reset();
    _events.ScheduleEvent(EVENT_CHECK_PLAYER, 1000);
    if (IsHeroic())
        _events.ScheduleEvent(EVENT_DAMAGE, 1000);
    me->SetCorpseDelay(0);  // remove corpse immediately
    me->LoadCreaturesAddon(true);
}

void NpcDreamCloudAI::UpdateAI(const uint32 diff)
{
    // trigger
    if (_instance->GetBossState(DATA_VALITHRIA_DREAMWALKER) != IN_PROGRESS)
        return;

    _events.Update(diff);

    while (uint32 eventId = _events.ExecuteEvent())
    {
        switch (eventId)
        {
            case EVENT_CHECK_PLAYER:
            {
                Player* player = NULL;
                Trinity::AnyPlayerInObjectRangeCheck check(me, 5.0f);
                Trinity::PlayerSearcher<Trinity::AnyPlayerInObjectRangeCheck> searcher(me, player, check);
                me->VisitNearbyWorldObject(7.5f, searcher);
                _events.ScheduleEvent(player ? EVENT_EXPLODE : EVENT_CHECK_PLAYER, 1000);
                break;
            }
            case EVENT_EXPLODE:
                me->GetMotionMaster()->MoveIdle();
                // must use originalCaster the same for all clouds to allow stacking
                me->CastSpell(me, EMERALD_VIGOR, false, NULL, NULL, _instance->GetData64(DATA_VALITHRIA_DREAMWALKER));
                me->DespawnOrUnsummon(1000);
                break;
            case EVENT_DAMAGE:
                DoCast(SPELL_NIGHTMARE_CLOUD);
            default:
                break;
        }
    }
}