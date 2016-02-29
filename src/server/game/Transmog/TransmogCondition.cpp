#include "TransmogCondition.h"

#include "ArenaTeam.h"
#include "World.h"

TransmogSubCondition::TransmogSubCondition(uint32 type, uint32 value) : _type(TransmogConditionType(type)), _value(value)
{
}

bool TransmogSubCondition::IsMatching(Player *player) const
{
    switch (_type)
    {
        case TRANSMOG_CONDITION_RACE:
            return player->getRace() == _value;
        case TRANSMOG_CONDITION_CLASS:
            return player->getClass() == _value;
        case TRANSMOG_CONDITION_RATING_2_VS_2:
            return player->GetArenaPersonalRating(ArenaTeam::GetSlotByType(ARENA_TEAM_2v2)) >= _value;
        case TRANSMOG_CONDITION_RATING_3_VS_3:
            return player->GetArenaPersonalRating(ArenaTeam::GetSlotByType(ARENA_TEAM_3v3)) >= _value;
        case TRANSMOG_CONDITION_QUEST:
            return player->GetQuestStatus(_value) == QUEST_STATUS_REWARDED;
        case TRANSMOG_CONDITION_ITEM:
            return player->HasItemCount(TRANSMOG_COST_ITEM, _value);
        default:
            return false;
    }
}

void TransmogCondition::AddSubCondition(uint32 id, TransmogSubCondition condition)
{
    _subConditions[id].push_back(condition);
}

bool TransmogCondition::IsMatchingCondition(Player *player, TransmogCondition const* condition)
{
    if (player)
        return condition ? condition->IsMatching(player) : true;

    return false;
}

bool TransmogCondition::IsMatching(Player *player) const
{
    bool result = false;

    for (SubConditionMap::const_iterator itr = _subConditions.begin(); !result && itr != _subConditions.end(); ++itr)
    {
        bool match = true;

        for (SubConditionList::const_iterator itr2 = itr->second.begin(); match && itr2 != itr->second.end(); ++itr2)
            match = match && itr2->IsMatching(player);

        result = result || match;
    }

    return result;
}
