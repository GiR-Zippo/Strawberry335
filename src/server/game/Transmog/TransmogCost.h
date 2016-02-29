#ifndef __TRANSMOG_COST
#define __TRANSMOG_COST

enum TransmogCostType
{
    TRANSMOG_COST_ARENAPOINTS = 0,
    TRANSMOG_COST_HONORPOINTS = 1,
    TRANSMOG_COST_PVE_MARKEN  = 2,
    MAX_TRANSMOG_COST
};

enum TransmogCostItems
{
    TRANSMOG_COST_ITEM = 42425,
};

class TransmogSubCost
{
public:
    TransmogSubCost(uint32 type, uint32 value);

    bool CanBePaidBy(Player *player) const;
    void PayBy(Player *player) const;

    std::string ToString() const;

private:
    TransmogCostType _type;
    uint32 _value;
};

class TransmogCost
{
public:
    typedef std::list<TransmogSubCost> SubCostList;

    static std::set<uint32> GetCostOptions(TransmogCost const* cost);
    static bool CanBePaid(Player *player, TransmogCost const* cost, uint32 id);
    static bool Pay(Player *player, TransmogCost const* cost, uint32 id);

    void AddSubCost(uint32 id, TransmogSubCost cost);
    SubCostList const* GetSubCostList(uint32 id) const;

    std::string SubCostToText(uint32 id) const;

    std::set<uint32> GetCostOptions() const;
    bool CanBePaidBy(Player *player, uint32 id) const;

    bool PayBy(Player *player, uint32 id) const;

private:
    typedef std::map<uint32, SubCostList> SubCostMap;

    SubCostMap _subCosts;
};


#endif /* __TRANSMOG_COST */
