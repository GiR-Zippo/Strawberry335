#include "Npc_IceTomb.h"

void NpcIceTombAI::Reset()
{
    me->SetReactState(REACT_PASSIVE);
}

void NpcIceTombAI::SetGUID(uint64 guid, int32 type)
{
    if (type == DATA_TRAPPED_PLAYER)
    {
        _trappedPlayerGUID = guid;
        _existenceCheckTimer = 1000;
    }
}

void NpcIceTombAI::DoAction(const int32 action)
{
    if (action == ACTION_TRIGGER_ASPHYXIATION)
        if (Player* player = ObjectAccessor::GetPlayer(*me, _trappedPlayerGUID))
            player->CastSpell(player, SPELL_ASPHYXIATION, true);
}

void NpcIceTombAI::JustDied(Unit* )
{
    if (GameObject* go = me->GetGameObject(SPELL_ICE_TOMB_DAMAGE))
    {
        go->SetGoState(GO_STATE_ACTIVE);
        go->EnableCollision(false);
        go->RemoveFromWorld();
    }
    if (Player* player = ObjectAccessor::GetPlayer(*me, _trappedPlayerGUID))
    {
        _trappedPlayerGUID = 0;
        player->RemoveAurasDueToSpell(SPELL_ICE_TOMB_DAMAGE);
        player->RemoveAurasDueToSpell(SPELL_ASPHYXIATION);
        player->RemoveAurasDueToSpell(SPELL_ICE_TOMB_UNTARGETABLE);
        player->RemoveAura(SPELL_ICE_TOMB_DAMAGE);
        player->RemoveAura(SPELL_ASPHYXIATION);
        player->RemoveAura(SPELL_ICE_TOMB_UNTARGETABLE);
        player->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
    }
}

void NpcIceTombAI::UpdateAI(const uint32 diff)
{
    if (!_trappedPlayerGUID)
        return;

    if (_existenceCheckTimer <= diff)
    {
        Player* player = ObjectAccessor::GetPlayer(*me, _trappedPlayerGUID);
        if (!player || player->isDead() || !player->HasAura(SPELL_ICE_TOMB_DAMAGE))
        {
            // Remove object
            JustDied(me);
            me->DespawnOrUnsummon();
            return;
        }
        _existenceCheckTimer = 1000;
    }
    else
        _existenceCheckTimer -= diff;
}