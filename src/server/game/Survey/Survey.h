#ifndef __SURVEY
#define __SURVEY

using SurveyName = std::pair<std::string, uint32>;
using SurveyNames = std::vector<SurveyName>;

using SurveyOption = std::pair<std::string, uint32>;
using SurveyOptions = std::vector<SurveyOption>;

struct SurveyDescription {
    uint32 survey_text;
    SurveyOptions options;
};

struct CastVote {
    uint32 account;
    uint32 survey;
    uint32 option;
};

class Survey {
  public:
    static Survey &instance() {
        static Survey survey;
        return survey;
    }

    void load_from_db();
    void reload();

    SurveyNames get_survey_names() const { return survey_names_; }
    SurveyNames get_active_survey_names() const { return active_survey_names_; }

    SurveyDescription get_survey_description(uint32 survey) const;
    uint32 get_survey_option_description(uint32 survey, uint32 option) const;

    void cast_vote(uint32 account, uint32 survey, uint32 option);

  private:
    SurveyNames survey_names_;
    SurveyNames active_survey_names_;
    std::map<uint32, SurveyDescription> survey_descriptions_;
    std::map<std::pair<uint32, uint32>, uint32> survey_option_descriptions_;

    std::vector<CastVote> cast_votes_;
};

#define sSurvey Survey::instance()

#endif __SURVEY
