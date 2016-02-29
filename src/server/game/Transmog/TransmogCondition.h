#ifndef __TRANSMOG_CONDITION
#define __TRANSMOG_CONDITION

enum TransmogConditionType
{
    TRANSMOG_CONDITION_RACE = 0,
    TRANSMOG_CONDITION_CLASS = 1,
    TRANSMOG_CONDITION_RATING_2_VS_2 = 2,
    TRANSMOG_CONDITION_RATING_3_VS_3 = 3,
    //TRANSMOG_CONDITION_SEASON = 4, -- Disabled and Deprecated
    TRANSMOG_CONDITION_QUEST = 5,
    TRANSMOG_CONDITION_ITEM = 6,
    MAX_TRANSMOG_CONDITION
};

class TransmogSubCondition
{
public:
    TransmogSubCondition(uint32 type, uint32 value);
    bool IsMatching(Player *player) const;

private:
    TransmogConditionType _type;
    uint32 _value;
};

class TransmogCondition
{
public:
    void AddSubCondition(uint32 id, TransmogSubCondition condition);

    static bool IsMatchingCondition(Player *player, TransmogCondition const* condition);

    bool IsMatching(Player *player) const;

private:
    typedef std::list<TransmogSubCondition> SubConditionList;
    typedef std::map<uint32, SubConditionList> SubConditionMap;

    SubConditionMap _subConditions;
};


#endif /* __TRANSMOG_CONDITION */
