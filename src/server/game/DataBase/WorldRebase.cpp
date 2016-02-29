#include "WorldRebase.h"
#include "Log.h"
#include "DatabaseEnv.h"
#include "Log.h"

WorldRebase::WorldRebase()
{
    _CreatureList.clear();
}
WorldRebase::~WorldRebase()
{
    _CreatureList.clear();
}

void WorldRebase::Creatures()
{
    sLog->outString("Rebase Creatures");
    std::ostringstream ss;
    ss << "CREATE TABLE creature_old SELECT * FROM creature";
    WorldDatabase.Execute(ss.str().c_str());



}