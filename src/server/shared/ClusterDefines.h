#ifndef _CLUSTERDEFINES_H
#define _CLUSTERDEFINES_H

enum NodeTypes
{
    NODE_TYPE_SINGLE    = 0,            // Deprecated
    NODE_TYPE_MASTER    = 1,            // Master-Node (only this Node should be write data on our DB, if smt. is global or logon couldn't write it)
    NODE_TYPE_SLAVE     = 2,            // Slave-Node is a Map-Node
    NODE_TYPE_CUSTOM    = 3,            // World-Node provides an own World
    NODE_TYPE_POOL      = 4             // Pool-Node is a node for realmpools
};

//Defines at which point our init fails
enum NodeAck
{
    NODE_INIT_ACK_FAIL              = 0,
    NODE_INIT_ACK_ITEM_GUID_FAIL    = 1,
    NODE_INIT_ACK_MAIL_GUID_FAIL    = 2,
    NODE_INIT_ACK_CORPSE_GUID_FAIL  = 3,
    NODE_INIT_ACK_OK                = 254
};

enum AchievementData
{
    CL_DEF_COMPLETED_ACHIEVEMENT = 0,   //uint32(ID)
    CL_DEF_CRITERIA_UPDATE,             //uint32 entry, changeValue, uint8 ptype;
    CL_DEF_RESET_ACHIEVEMENTS,
    CL_DEF_RESET_ACHIEVEMENT_CRITERIA,  //uint32 type,miscvalue1,miscvalue2,bool evenIfCriteriaComplete;

    //Syncs L/N
    CL_DEF_GUID_SYNC,                   //Logon sends Highest GUID, Node sends if any GUID was taken
    CL_DEF_SHUTDOWN,                    //Sends shutdown in uint32 Seconds
    CL_DEF_GUILDBANK_ITEM_ADD,          //Add an item to GUILD-BANK
    CL_DEF_GUILDBANK_ITEM_REMOVE,       //remove an item from GUILD-BANK

    CL_DEF_TRANSFER_TO_NODE,           //uint32 (NODE)
    CL_DEF_TRANSFER_OK,
    CL_DEF_TRANSFER_ACC,

    CL_DEF_REP_SET_REPUTATION = 0,
    CL_DEF_REP,
    CL_DEF_TEST,
    CL_DEF_TELE,

};

enum EntityIDType
{
    ENTITYID_ITEM = 1,
    ENTITYID_MAIL,
    ENTITYID_CORPSE,
    ENTITYID_GROUP
};
#endif
