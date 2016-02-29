#include "WorldPacket.h"
#include "Opcodes.h"
#include "ClientSession.h"
#include "BattlegroundMgr.h"

void BattlegroundMgr::Initialize()
{
    uint32 oldMSTime = getMSTime();
    QueryResult result = WorldDatabase.Query("SELECT id, MinPlayersPerTeam, MaxPlayersPerTeam, MinLvl, MaxLvl "
                                             "FROM battleground_template;");

    if (!result)
    {
        sLog->outString(">> Loaded 0 creature template definitions. DB table `battleground_template` is empty.");
        sLog->outString();
        return;
    }

    _bgTemplateContainer.rehash(result->GetRowCount());
    uint32 count = 0;
    do
    {    uint32 oldMSTime = getMSTime();
        Field* fields = result->Fetch();

        uint32 entry = fields[0].GetUInt32();

        BGTemplate& bgTemplate = _bgTemplateContainer[entry];
        bgTemplate.Entry = entry;
        bgTemplate.MinPlayer = fields[1].GetUInt32();
        bgTemplate.MaxPlayer = fields[2].GetUInt32();
        bgTemplate.MinLevel = fields[3].GetUInt32();
        bgTemplate.MaxLevel = fields[4].GetUInt32();


        count++;

    }while (result->NextRow());
    sLog->outString(">> Loaded %u creature templates in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}
