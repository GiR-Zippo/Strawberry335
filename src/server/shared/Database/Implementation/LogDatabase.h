/**
 * @file
 * @author TehKevin for Laenalith-WoW
 * @date 25.03.2015
 *
 * Contact kevin.unegg@ainet.at
 * Website: http://www.laenalith-wow.com/
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @copyright Copyright 2015 Laenalith-WoW. All rights reserved.
 */

#ifndef _LOGDATABASE_H
#define _LOGDATABASE_H

#include "DatabaseWorkerPool.h"
#include "MySQLConnection.h"

class LogDatabaseConnection : public MySQLConnection
{
    public:
        //- Constructors for sync and async connections
        LogDatabaseConnection(MySQLConnectionInfo& connInfo) : MySQLConnection(connInfo) {}
        LogDatabaseConnection(ACE_Activation_Queue* q, MySQLConnectionInfo& connInfo) : MySQLConnection(q, connInfo) {}

        //- Loads database type specific prepared statements
        void DoPrepareStatements();
};

typedef DatabaseWorkerPool<LogDatabaseConnection> LogDatabaseWorkerPool;

enum LogDatabaseStatements
{
    /*  Naming standard for defines:
        {DB}_{SEL/INS/UPD/DEL/REP}_{Summary of data changed}
        When updating more than one field, consider looking at the calling function
        name for a suiting suffix.
    */

    // GM Quality Manager
    LOG_SEL_GM_LOG_TICKETID,
    LOG_INS_GM_LOG,
    LOG_INS_GM_TICKET_LOG,
    LOG_INS_GM_CHAT_LOG,

    MAX_LOGDATABASE_STATEMENTS,
};

#endif
