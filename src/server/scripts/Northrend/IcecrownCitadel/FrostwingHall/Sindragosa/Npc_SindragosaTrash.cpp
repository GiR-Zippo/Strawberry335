#include "Npc_SindragosaTrash.h"

void NpcSindragosaTrashAI::InitializeAI()
{
    _frostwyrmId = (me->GetHomePosition().GetPositionY() < 2484.35f) ? DATA_RIMEFANG : DATA_SPINESTALKER;
    // Increase add count
    if (!me->isDead())
    {
        if (me->GetEntry() == NPC_FROSTWING_WHELP)
            _instance->SetData(_frostwyrmId, me->GetDBTableGUIDLow());  // this cannot be in Reset because reset also happens on evade
        Reset();
    }
}

void NpcSindragosaTrashAI::Reset()
{
    // This is shared AI for handler and whelps
    if (me->GetEntry() == NPC_FROSTWARDEN_HANDLER)
    {
        _events.ScheduleEvent(EVENT_FROSTWARDEN_ORDER_WHELP, 3000);
        _events.ScheduleEvent(EVENT_CONCUSSIVE_SHOCK, urand(8000, 10000));
    }

    _isTaunted = false;
}

void NpcSindragosaTrashAI::JustRespawned()
{
    ScriptedAI::JustRespawned();

    // Increase add count
    if (me->GetEntry() == NPC_FROSTWING_WHELP)
        _instance->SetData(_frostwyrmId, me->GetDBTableGUIDLow());  // this cannot be in Reset because reset also happens on evade
}

void NpcSindragosaTrashAI::SetData(uint32 type, uint32 data)
{
    if (type == DATA_WHELP_MARKER)
        _isTaunted = data != 0;
}

uint32 NpcSindragosaTrashAI::GetData(uint32 type)
{
    if (type == DATA_FROSTWYRM_OWNER)
        return _frostwyrmId;
    else if (type == DATA_WHELP_MARKER)
        return uint32(_isTaunted);
    return 0;
}

void NpcSindragosaTrashAI::UpdateAI(const uint32 diff)
{
    if (!UpdateVictim())
        return;

    _events.Update(diff);

    if (me->HasUnitState(UNIT_STATE_CASTING))
        return;

    while (uint32 eventId = _events.ExecuteEvent())
    {
        switch (eventId)
        {
            case EVENT_FROSTWARDEN_ORDER_WHELP:
                DoCast(me, SPELL_ORDER_WHELP);
                _events.ScheduleEvent(EVENT_FROSTWARDEN_ORDER_WHELP, 3000);
                break;
            case EVENT_CONCUSSIVE_SHOCK:
                DoCast(me, SPELL_CONCUSSIVE_SHOCK);
                _events.ScheduleEvent(EVENT_CONCUSSIVE_SHOCK, urand(10000, 13000));
                break;
            default:
                break;
        }
    }

    DoMeleeAttackIfReady();
}