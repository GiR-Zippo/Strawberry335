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

#include "LogDatabase.h"

void LogDatabaseConnection::DoPrepareStatements()
{
    if (!m_reconnecting)
        m_stmts.resize(MAX_LOGDATABASE_STATEMENTS);

    // GM Quality Manager
    PrepareStatement(LOG_SEL_GM_LOG_TICKETID, "SELECT ticketId FROM gm_log ORDER BY ticketId DESC", CONNECTION_SYNCH);
    PrepareStatement(LOG_INS_GM_LOG, "INSERT INTO gm_log VALUES(?, ?, ?, ?, ?, ?, ?, ?)", CONNECTION_ASYNC);
    PrepareStatement(LOG_INS_GM_TICKET_LOG, "INSERT INTO gm_ticket_log VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)", CONNECTION_ASYNC);
    PrepareStatement(LOG_INS_GM_CHAT_LOG, "INSERT INTO gm_chat_log VALUES(?, ?, ?, ?, ?, ?, ?)", CONNECTION_ASYNC);
}
