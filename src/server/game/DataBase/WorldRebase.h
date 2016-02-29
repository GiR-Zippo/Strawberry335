#ifndef WORLD_REBASE_H
#define WORLD_REBASE_H

#include <ace/Singleton.h>

struct IDStruc
{
    uint64 oldID;
    uint64 newID;
};

typedef UNORDERED_MAP<uint64, IDStruc> CreatureList;

class WorldRebase
{
    public:
        WorldRebase();
        ~WorldRebase();
        void Creatures();

    private:
        CreatureList _CreatureList;
};
#define sWorldRebase ACE_Singleton<WorldRebase, ACE_Null_Mutex>::instance()
#endif