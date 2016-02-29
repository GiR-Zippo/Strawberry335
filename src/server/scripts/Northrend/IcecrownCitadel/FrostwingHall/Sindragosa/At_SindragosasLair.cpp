#include "At_SindragosasLair.h"

bool AtSindragosaLair::OnTrigger(Player* player, const AreaTriggerEntry* )
{
    if (InstanceScript* instance = player->GetInstanceScript())
    {
        if (!instance->GetData(DATA_SPINESTALKER))
            if (Creature* spinestalker = ObjectAccessor::GetCreature(*player, instance->GetData64(DATA_SPINESTALKER)))
                spinestalker->AI()->DoAction(ACTION_START_FROSTWYRM);

        if (!instance->GetData(DATA_RIMEFANG))
            if (Creature* rimefang = ObjectAccessor::GetCreature(*player, instance->GetData64(DATA_RIMEFANG)))
                rimefang->AI()->DoAction(ACTION_START_FROSTWYRM);

        if (!instance->GetData(DATA_SINDRAGOSA_FROSTWYRMS) && !instance->GetData64(DATA_SINDRAGOSA) && instance->GetBossState(DATA_SINDRAGOSA) != DONE)
        {
            if (player->GetMap()->IsHeroic() && !instance->GetData(DATA_HEROIC_ATTEMPTS))
                return true;

            player->GetMap()->LoadGrid(SindragosaSpawnPos.GetPositionX(), SindragosaSpawnPos.GetPositionY());
            if (Creature* sindragosa = player->GetMap()->SummonCreature(NPC_SINDRAGOSA, SindragosaSpawnPos))
                sindragosa->AI()->DoAction(ACTION_START_FROSTWYRM);
        }
    }

    return true;
}