/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "DatabaseEnv.h"
#include "Transaction.h"

//- Append a raw ad-hoc query to the transaction
void Transaction::Append(const char* sql)
{
    SQLElementData data;
    data.type = SQL_ELEMENT_RAW;
    data.element.query = strdup(sql);
    m_queries.push_back(data);
}

void Transaction::PAppend(const char* sql, ...)
{
    va_list ap;
    char szQuery [MAX_QUERY_LEN];
    va_start(ap, sql);
    vsnprintf(szQuery, MAX_QUERY_LEN, sql, ap);
    va_end(ap);

    Append(szQuery);
}

//- Append a prepared statement to the transaction
void Transaction::Append(PreparedStatement* stmt)
{
    SQLElementData data;
    data.type = SQL_ELEMENT_PREPARED;
    data.element.stmt = stmt;
    m_queries.push_back(data);
}

std::string Transaction::QueriesToText() const
{
    std::stringstream ss;

    for (std::list<SQLElementData>::const_iterator itr = m_queries.begin(); itr != m_queries.end(); ++itr)
    {
        if (itr->type == SQL_ELEMENT_RAW)
        {
            ss << ("[SQL_ELEMENT_RAW]");
            ss << itr->element.query;
        }
        else
        {
            ss << "[SQL_ELEMENT_PREPARED]" << itr->element.stmt->m_index;
            for (std::vector<PreparedStatementData>::const_iterator itr2 = itr->element.stmt->statement_data.begin();
                itr2 != itr->element.stmt->statement_data.end(); ++itr2)
            {
                ss << " - ";
                switch (itr2->type)
                {
                    case TYPE_BOOL:
                        ss << itr2->data.boolean ? "true" : "false";
                        break;
                    case TYPE_UI8:
                        ss << static_cast<uint32>(itr2->data.ui8);
                        break;
                    case TYPE_UI16:
                        ss << itr2->data.ui16;
                        break;
                    case TYPE_UI32:
                        ss << itr2->data.ui32;
                        break;
                    case TYPE_UI64:
                        ss << itr2->data.ui64;
                        break;
                    case TYPE_I8:
                        ss << static_cast<int32>(itr2->data.i8);
                        break;
                    case TYPE_I16:
                        ss << itr2->data.i16;
                        break;
                    case TYPE_I32:
                        ss << itr2->data.i32;
                        break;
                    case TYPE_I64:
                        ss << itr2->data.i64;
                        break;
                    case TYPE_FLOAT:
                        ss << itr2->data.f;
                        break;
                    case TYPE_DOUBLE:
                        ss << itr2->data.d;
                        break;
                    case TYPE_STRING:
                        ss << itr2->str.c_str();
                        break;
                }
            }
        }
        ss << "\n";
    }

    return ss.str();
}

void Transaction::Cleanup()
{
    // This might be called by explicit calls to Cleanup or by the auto-destructor
    if (_cleanedUp)
        return;

    while (!m_queries.empty())
    {
        SQLElementData const &data = m_queries.front();
        switch (data.type)
        {
            case SQL_ELEMENT_PREPARED:
                delete data.element.stmt;
            break;
            case SQL_ELEMENT_RAW:
                free((void*)(data.element.query));
            break;
        }

        m_queries.pop_front();
    }

    _cleanedUp = true;
}

bool TransactionTask::Execute()
{
    switch (m_conn->ExecuteTransaction(m_trans))
    {
        case SQL_ERROR_RESCHEDULE:
            if (m_trans->GetSize())
            {
                sLog->outSQLDriver("[Warning] Execute: Transaction rescheduled. %u queries not executed. :\n %s",
                    m_trans->GetSize(), m_trans->QueriesToText().c_str());
            }
            return false; // We return false to reschedule on errors that can be handled that way
        case SQL_ERROR_ABORT:
            if (m_trans->GetSize())
            {
                sLog->outSQLDriver("[Error] Execute: Transaction aborted. %u queries not executed. :\n %s",
                    m_trans->GetSize(), m_trans->QueriesToText().c_str());
            }
        case SQL_ERROR_OK:
        default:
            return true; // We return true to stop rescheduling on errors that cannot be handled
    }
}
