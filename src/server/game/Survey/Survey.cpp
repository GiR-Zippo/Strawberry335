#include "Survey.h"

void Survey::reload() {
    survey_names_.clear();
    active_survey_names_.clear();
    survey_descriptions_.clear();
    survey_option_descriptions_.clear();
    cast_votes_.clear();

    load_from_db();
}

void Survey::load_from_db() {
    if (auto result = WorldDatabase.Query(
            "SELECT id, name, description, active FROM surveys")) {

        do {
            auto fields = result->Fetch();

            auto id = fields[0].GetUInt32();
            auto name = fields[1].GetString();
            auto description = fields[2].GetUInt32();

            survey_names_.push_back(SurveyName{name, id});

            survey_descriptions_[id] = SurveyDescription{description};

            if (auto active = fields[3].GetBool()) {
                active_survey_names_.push_back(SurveyName{name, id});
            }
        } while (result->NextRow());
    }

    if (auto result = WorldDatabase.Query(
            "SELECT id, survey, name, description FROM survey_option")) {

        do {
            auto fields = result->Fetch();

            auto id = fields[0].GetUInt32();
            auto survey = fields[1].GetUInt32();
            auto name = fields[2].GetString();
            auto description = fields[3].GetUInt32();

            auto &survey_description = survey_descriptions_[survey];

            survey_description.options.push_back(SurveyOption{name, id});

            survey_option_descriptions_[std::make_pair(survey, id)] =
                description;
        } while (result->NextRow());
    }

    if (auto result = CharacterDatabase.Query(
            "SELECT account, survey, option FROM character_survey_votes")) {
        do {
            auto fields = result->Fetch();

            auto account = fields[0].GetUInt32();
            auto survey = fields[1].GetUInt32();
            auto option = fields[2].GetUInt32();

            cast_votes_.push_back(CastVote{account, survey, option});
        } while (result->NextRow());
    }
}

SurveyDescription Survey::get_survey_description(uint32 survey) const {
    auto itr = survey_descriptions_.find(survey);

    if (itr != survey_descriptions_.end())
        return itr->second;

    return SurveyDescription{-1};
}

uint32 Survey::get_survey_option_description(uint32 survey,
                                             uint32 option) const {
    auto itr = survey_option_descriptions_.find(std::make_pair(survey, option));

    if (itr != survey_option_descriptions_.end())
        return itr->second;

    return -1;
}

void Survey::cast_vote(uint32 account, uint32 survey, uint32 option) {

    if (option == static_cast<uint32>(-1))
        return;

    auto vote = std::find_if(std::begin(cast_votes_), std::end(cast_votes_),
                             [&](CastVote const &vote) {
                                 return vote.account == account &&
                                        vote.account == survey;
                             });

    if (vote == cast_votes_.end())
        cast_votes_.push_back(CastVote{account, survey, option});
    else
        vote->option = option;

    CharacterDatabase.PExecute("REPLACE INTO character_survey_votes(account, "
                               "survey, option) VALUES (%u, %u, %u)",
                               account, survey, option);
}
