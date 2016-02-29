#include "ScriptedCreature.h"
#include "ScriptMgr.h"
#include "ScriptedGossip.h"

#include "Player.h"
#include "Survey.h"

#define VOTE_TEXT "Unverbindlich voten!"
#define BACK_TEXT "Back"

enum TextIds {
    // Available ids = 60050 - 60059 -> Surveys from 60060
    NPC_TEXT_ID_SURVEYS         = 60050,
    NPC_TEXT_ID_SURVEYS_EMPTY   = 60051,
    NPC_TEXT_ID_ERROR           = 60052,
    NPC_TEXT_ID_VOTE_SUCCESS    = 60053
};

using PlayerState = std::pair<uint32, bool>;

class npc_survey : public CreatureScript {
    std::map<std::pair<uint32, uint64>, PlayerState> player_states;

  public:
    npc_survey() : CreatureScript("npc_survey") {}

    bool OnGossipHello(Player *pPlayer, Creature *pCreature) {
        auto creature_guid = pCreature->GetGUID();

        auto &player_state =
            player_states[std::make_pair(pPlayer->GetGUIDLow(), creature_guid)];

        auto &choosen_survey = player_state.first;
        auto &submit = player_state.second;

        choosen_survey = -1;
        submit = false;

        SurveyNames surveys = sSurvey.get_active_survey_names();
        if (!surveys.empty()) {
            // Load surveys from manager
            for (auto survey : surveys)
                pPlayer->ADD_GOSSIP_ITEM(
                    GOSSIP_ICON_CHAT, survey.first.c_str(), GOSSIP_SENDER_MAIN,
                    survey.second + GOSSIP_ACTION_INFO_DEF + 1);

            // Display available text with surveys
            pPlayer->SEND_GOSSIP_MENU(NPC_TEXT_ID_SURVEYS, creature_guid);
            return true;
        }

        // No surveys available - display empty text
        pPlayer->SEND_GOSSIP_MENU(NPC_TEXT_ID_SURVEYS_EMPTY, creature_guid);
        return true;
    }

    bool OnGossipSelect(Player *pPlayer, Creature *pCreature, uint32 /*sender*/,
                        uint32 action) {

        auto creature_guid = pCreature->GetGUID();

        auto &player_state =
            player_states[std::make_pair(pPlayer->GetGUIDLow(), creature_guid)];

        auto &choosen_survey = player_state.first;
        auto &submit = player_state.second;

        pPlayer->PlayerTalkClass->ClearMenus();

        // Back option
        if (action == GOSSIP_ACTION_INFO_DEF) {
          // Reset saved states
          submit = false;
          uint32 tmp_choosen_survey = choosen_survey;
          choosen_survey = -1;

          OnGossipSelect(pPlayer, pCreature, GOSSIP_SENDER_MAIN,
                         tmp_choosen_survey + GOSSIP_ACTION_INFO_DEF + 1);
          return true;
        }

        action -= (GOSSIP_ACTION_INFO_DEF + 1);

        if (choosen_survey != static_cast<uint32>(-1)) {
          if (submit) // Vote
          {
            if (auto session = pPlayer->GetSession()) {
              sSurvey.cast_vote(session->GetAccountId(), choosen_survey,
                                action);

              // Display success text
              pPlayer->SEND_GOSSIP_MENU(NPC_TEXT_ID_VOTE_SUCCESS,
                                        creature_guid);
              return true;
            }
          } else // Confirm
          {
            uint32 survey_option_text =
                sSurvey.get_survey_option_description(choosen_survey, action);

            if (survey_option_text != static_cast<uint32>(-1)) {
              // Save state
              submit = true;

              // Add option to submit
              pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, VOTE_TEXT,
                                       GOSSIP_SENDER_MAIN,
                                       action + GOSSIP_ACTION_INFO_DEF + 1);

              // Add option to go back
              pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, BACK_TEXT,
                                       GOSSIP_SENDER_MAIN,
                                       GOSSIP_ACTION_INFO_DEF);

              // Display option text with submit button
              pPlayer->SEND_GOSSIP_MENU(survey_option_text, creature_guid);
              return true;
            }
          }
        } else // Choose option
        {
          SurveyDescription surveyDescription =
              sSurvey.get_survey_description(action);
          if (surveyDescription.survey_text != static_cast<uint32>(-1)) {
            // Save state
            choosen_survey = action;

            // Load survey description from manager
            for (auto surveyOption : surveyDescription.options)
              pPlayer->ADD_GOSSIP_ITEM(
                  GOSSIP_ICON_CHAT, surveyOption.first.c_str(),
                  GOSSIP_SENDER_MAIN,
                  surveyOption.second + GOSSIP_ACTION_INFO_DEF + 1);

            // Display survey text with options
            pPlayer->SEND_GOSSIP_MENU(surveyDescription.survey_text,
                                      creature_guid);
            return true;
          }
        }

        // Nothing found - display error text
        pPlayer->SEND_GOSSIP_MENU(NPC_TEXT_ID_ERROR, creature_guid);
        return true;
    }
};

void AddSC_npc_survey() {
    new npc_survey();
}
