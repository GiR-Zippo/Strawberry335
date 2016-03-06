#ifndef TRINITY_BATTLEGROUNDMGR_H
#define TRINITY_BATTLEGROUNDMGR_H

#include "SharedDefines.h"

struct BGTemplate
{
    uint8 Entry;
    uint8 MinPlayer;
    uint8 MaxPlayer;
    uint8 MinLevel;
    uint8 MaxLevel;

};
typedef std::unordered_map<uint32, BGTemplate> BGTemplateContainer;


class BattlegroundMgr
{
    public:
        void Initialize();

    private:
        BGTemplateContainer _bgTemplateContainer;

};
#define sBattlegroundMgr ACE_Singleton<BattlegroundMgr, ACE_Null_Mutex>::instance()
#endif
