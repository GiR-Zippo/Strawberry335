#include "TransmogCost.h"

#include "ArenaTeam.h"
#include "World.h"

TransmogSubCost::TransmogSubCost(uint32 type, uint32 value) : _type(TransmogCostType(type)), _value(value)
{
}

bool TransmogSubCost::CanBePaidBy(Player *player) const
{
    switch (_type)
    {
        case TRANSMOG_COST_ARENAPOINTS:
            return player->GetArenaPoints() >= _value;
        case TRANSMOG_COST_HONORPOINTS:
            return player->GetHonorPoints() >= _value;
        case TRANSMOG_COST_PVE_MARKEN:
            return player->GetItemCount(TRANSMOG_COST_ITEM) >= _value;
        default:
            return false;
    }
}

void TransmogSubCost::PayBy(Player *player) const
{
    switch (_type)
    {
        case TRANSMOG_COST_ARENAPOINTS:
        {
            player->ModifyArenaPoints(-static_cast<int32>(_value));
            break;
        }
        case TRANSMOG_COST_HONORPOINTS:
        {
            player->ModifyHonorPoints(-static_cast<int32>(_value));
            break;
        }
        case TRANSMOG_COST_PVE_MARKEN:
        {
            player->DestroyItemCount(TRANSMOG_COST_ITEM, _value, true);
            break;
        }
    }
}

std::string TransmogSubCost::ToString() const
{
    std::stringstream ss;

    switch (_type)
    {
        case TRANSMOG_COST_ARENAPOINTS:
        {
            ss << _value << " Arena Punkte";
            break;
        }
        case TRANSMOG_COST_HONORPOINTS:
        {
            ss << _value << " Ehren Punkte";
            break;
        }
        case TRANSMOG_COST_PVE_MARKEN:
            ss << _value << " Transmog Marken";
            break;
    }

    return ss.str();
}

std::set<uint32> TransmogCost::GetCostOptions(TransmogCost const* cost)
{
    std::set<uint32> result;

    if (cost)
        return cost->GetCostOptions();

    result.insert(0);

    return result;
}

bool TransmogCost::CanBePaid(Player *player, TransmogCost const* cost, uint32 id)
{
    if (player)
        return cost ? cost->CanBePaidBy(player, id) : true;

    return false;
}

bool TransmogCost::Pay(Player *player, TransmogCost const* cost, uint32 id)
{
    if (player)
        return cost ? cost->PayBy(player, id) : true;

    return false;
}

void TransmogCost::AddSubCost(uint32 id, TransmogSubCost cost)
{
    _subCosts[id].push_back(cost);
}

TransmogCost::SubCostList const* TransmogCost::GetSubCostList(uint32 id) const
{
    SubCostMap::const_iterator itr = _subCosts.find(id);

    if (itr == _subCosts.end())
        return NULL;

    return &itr->second;
}

std::string TransmogCost::SubCostToText(uint32 id) const
{
    std::stringstream ss;

    if (SubCostList const* subCosts = GetSubCostList(id))
    {
        for (SubCostList::const_iterator itr = subCosts->begin(); itr != subCosts->end(); ++itr)
        {
            if (itr != subCosts->begin())
                ss << ", ";

            ss << (*itr).ToString();
        }
    }

    return ss.str();
}

std::set<uint32> TransmogCost::GetCostOptions() const
{
    std::set<uint32> result;

    for (SubCostMap::const_iterator itr = _subCosts.begin(); itr != _subCosts.end(); ++itr)
        result.insert(itr->first);

    return result;
}

bool TransmogCost::CanBePaidBy(Player *player, uint32 id) const
{
    if (SubCostList const* subCosts = GetSubCostList(id))
    {
        for (SubCostList::const_iterator itr = subCosts->begin(); itr != subCosts->end(); ++itr)
            if (!itr->CanBePaidBy(player))
                return false;
    }

    return true;
}

bool TransmogCost::PayBy(Player *player, uint32 id) const
{
    if (!CanBePaidBy(player, id))
        return false;

    if (SubCostList const* subCosts = GetSubCostList(id))
    {
        for (SubCostList::const_iterator itr = subCosts->begin(); itr != subCosts->end(); ++itr)
            itr->PayBy(player);
    }

    return true;
}
