#include "Common.h"
#include "DBCEnums.h"
#include "ObjectMgr.h"
#include "ArenaTeamMgr.h"
#include "GuildMgr.h"
#include "World.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "DatabaseEnv.h"
#include "AchievementMgr.h"
#include "ArenaTeam.h"
#include "CellImpl.h"
#include "GameEventMgr.h"
#include "GridNotifiersImpl.h"
#include "Guild.h"
#include "Language.h"
#include "Player.h"
#include "SpellMgr.h"
#include "DisableMgr.h"
#include "ScriptMgr.h"
#include "MapManager.h"
#include "Battleground.h"
#include "BattlegroundAB.h"
#include "Map.h"
#include "InstanceScript.h"
#include "ClusterDefines.h"

void AchievementMgr::CompletedAchievementAddition(AchievementEntry const* achievement)
{
    sLog->outDetail("AchievementMgr::CompletedAchievement(%u)", achievement->ID);

    // disable for gamemasters with GM-mode enabled
    if (m_player->isGameMaster())
        return;

    if (achievement->flags & ACHIEVEMENT_FLAG_COUNTER || HasAchieved(achievement->ID))
        return;

    SendAchievementEarned(achievement);
    CompletedAchievementData& ca = m_completedAchievements[achievement->ID];
    ca.date = time(NULL);
    ca.changed = true;

    // don't insert for ACHIEVEMENT_FLAG_REALM_FIRST_KILL since otherwise only the first group member would reach that achievement
    // TODO: where do set this instead?
    if (!(achievement->flags & ACHIEVEMENT_FLAG_REALM_FIRST_KILL))
        sAchievementMgr->SetRealmCompleted(achievement);

    UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_ACHIEVEMENT);
    UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_EARN_ACHIEVEMENT_POINTS, achievement->points);

    WorldPacket data(NODE_PLAYER_DATA);
    data << uint32(CL_DEF_COMPLETED_ACHIEVEMENT);
    data << uint32(achievement->ID);
    GetPlayer()->SendDirectMessage(&data);

    // reward items and titles if any
    AchievementReward const* reward = sAchievementMgr->GetAchievementReward(achievement);

    // no rewards
    if (!reward)
        return;

    // titles
    //! Currently there's only one achievement that deals with gender-specific titles.
    //! Since no common attributes were found, (not even in titleRewardFlags field)
    //! we explicitly check by ID. Maybe in the future we could move the achievement_reward
    //! condition fields to the condition system.
    if (uint32 titleId = reward->titleId[achievement->ID == 1793 ? GetPlayer()->getGender() : (GetPlayer()->GetTeam() == ALLIANCE ? 0 : 1)])
        if (CharTitlesEntry const* titleEntry = sCharTitlesStore.LookupEntry(titleId))
            GetPlayer()->SetTitle(titleEntry);

    // mail
    if (reward->sender)
    {
        Item* item = reward->itemId ? Item::CreateItem(reward->itemId, 1, GetPlayer ()) : NULL;

        int loc_idx = GetPlayer()->GetSession()->GetSessionDbLocaleIndex();

        // subject and text
        std::string subject = reward->subject;
        std::string text = reward->text;
        if (loc_idx >= 0)
        {
            if (AchievementRewardLocale const* loc = sAchievementMgr->GetAchievementRewardLocale(achievement))
            {
                ObjectMgr::GetLocaleString(loc->subject, loc_idx, subject);
                ObjectMgr::GetLocaleString(loc->text, loc_idx, text);
            }
        }

        MailDraft draft(subject, text);

        SQLTransaction trans = CharacterDatabase.BeginTransaction();
        if (item)
        {
            // save new item before send
            item->SaveToDB(trans);                               // save for prevent lost at next mail load, if send fail then item will deleted

            // item
            draft.AddItem(item);
        }

        draft.SendMailTo(trans, GetPlayer(), MailSender(MAIL_CREATURE, reward->sender));
        CharacterDatabase.CommitTransaction(trans);
    }
}

void AchievementMgr::ResetAchievementCriteriaAddition(AchievementCriteriaTypes type, uint32 miscvalue1, uint32 miscvalue2, bool evenIfCriteriaComplete)
{
    sLog->outDebug(LOG_FILTER_ACHIEVEMENTSYS, "AchievementMgr::ResetAchievementCriteria(%u, %u, %u)", type, miscvalue1, miscvalue2);

    // disable for gamemasters with GM-mode enabled
    if (m_player->isGameMaster())
        return;

    WorldPacket data(NODE_PLAYER_DATA);
    data << uint32(CL_DEF_RESET_ACHIEVEMENT_CRITERIA);
    data << uint32(type);
    data << uint32(miscvalue1);
    data << uint32(miscvalue2);
    data << bool(evenIfCriteriaComplete);
    GetPlayer()->SendDirectMessage(&data);

    AchievementCriteriaEntryList const& achievementCriteriaList = sAchievementMgr->GetAchievementCriteriaByType(type);
    for (AchievementCriteriaEntryList::const_iterator i = achievementCriteriaList.begin(); i != achievementCriteriaList.end(); ++i)
    {
        AchievementCriteriaEntry const* achievementCriteria = (*i);

        AchievementEntry const* achievement = sAchievementStore.LookupEntry(achievementCriteria->referredAchievement);
        if (!achievement)
            continue;

        // don't update already completed criteria if not forced or achievement already complete
        if ((IsCompletedCriteria(achievementCriteria, achievement) && !evenIfCriteriaComplete) || HasAchieved(achievement->ID))
            continue;

        for (uint8 j = 0; j < MAX_CRITERIA_REQUIREMENTS; ++j)
            if (achievementCriteria->additionalRequirements[j].additionalRequirement_type == miscvalue1 &&
                (!achievementCriteria->additionalRequirements[j].additionalRequirement_value ||
                achievementCriteria->additionalRequirements[j].additionalRequirement_value == miscvalue2))
            {
                RemoveCriteriaProgress(achievementCriteria);
                break;
            }
    }
}

void AchievementMgr::ResetAddition()
{
    WorldPacket data(NODE_PLAYER_DATA);
    data << uint32(CL_DEF_RESET_ACHIEVEMENTS);
    m_player->SendDirectMessage(&data);

    for (CompletedAchievementMap::const_iterator iter = m_completedAchievements.begin(); iter != m_completedAchievements.end(); ++iter)
    {
        WorldPacket data(SMSG_ACHIEVEMENT_DELETED, 4);
        data << uint32(iter->first);
        m_player->SendDirectMessage(&data);
    }

    for (CriteriaProgressMap::const_iterator iter = m_criteriaProgress.begin(); iter != m_criteriaProgress.end(); ++iter)
    {
        WorldPacket data(SMSG_CRITERIA_DELETED, 4);
        data << uint32(iter->first);
        m_player->SendDirectMessage(&data);
    }

    m_completedAchievements.clear();
    m_criteriaProgress.clear();
    DeleteFromDB(m_player->GetGUIDLow());

    // re-fill data
    CheckAllAchievementCriteria();
}

void AchievementMgr::SetCriteriaProgressAddition(AchievementCriteriaEntry const* entry, uint32 changeValue, ProgressType ptype)
{
    // Don't allow to cheat - doing timed achievements without timer active
    TimedAchievementMap::iterator timedIter = m_timedAchievements.find(entry->ID);
    if (entry->timeLimit && timedIter == m_timedAchievements.end())
        return;

    sLog->outDebug(LOG_FILTER_ACHIEVEMENTSYS, "AchievementMgr::SetCriteriaProgress(%u, %u) for (GUID:%u)", entry->ID, changeValue, m_player->GetGUIDLow());

    CriteriaProgress* progress = GetCriteriaProgress(entry);
    if (!progress)
    {
        // not create record for 0 counter but allow it for timed achievements
        // we will need to send 0 progress to client to start the timer
        if (changeValue == 0 && !entry->timeLimit)
            return;

        progress = &m_criteriaProgress[entry->ID];
        progress->counter = changeValue;
    }
    else
    {
        uint32 newValue = 0;
        switch (ptype)
        {
            case PROGRESS_SET:
                newValue = changeValue;
                break;
            case PROGRESS_ACCUMULATE:
            {
                // avoid overflow
                uint32 max_value = std::numeric_limits<uint32>::max();
                newValue = max_value - progress->counter > changeValue ? progress->counter + changeValue : max_value;
                break;
            }
            case PROGRESS_HIGHEST:
                newValue = progress->counter < changeValue ? changeValue : progress->counter;
                break;
        }

        // not update (not mark as changed) if counter will have same value
        if (progress->counter == newValue && !entry->timeLimit)
            return;

        progress->counter = newValue;
    }

    progress->changed = true;
    progress->date = time(NULL); // set the date to the latest update.

    uint32 timeElapsed = 0;
    bool timedCompleted = false;

    if (entry->timeLimit)
    {
        // has to exist else we wouldn't be here
        timedCompleted = IsCompletedCriteria(entry, sAchievementStore.LookupEntry(entry->referredAchievement));
        // Client expects this in packet
        timeElapsed = entry->timeLimit - (timedIter->second/IN_MILLISECONDS);

        // Remove the timer, we wont need it anymore
        if (timedCompleted)
            m_timedAchievements.erase(timedIter);
    }

    SendCriteriaUpdate(entry, progress, timeElapsed, timedCompleted);

    WorldPacket data(NODE_PLAYER_DATA);
    data << uint32(CL_DEF_CRITERIA_UPDATE);
    data << uint32(entry->ID);
    data << uint32(changeValue);
    data << uint8(ptype);
    GetPlayer()->SendDirectMessage(&data);
}
