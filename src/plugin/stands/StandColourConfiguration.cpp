#include "StandColourConfiguration.h"
#include "StandAssignmentSource.h"
#include "euroscope/UserSetting.h"
#include "helper/HelperFunctions.h"
#include <stdexcept>

namespace UKControllerPlugin::Stands {

    namespace {
        auto DefaultSourceColours() -> std::map<StandAssignmentSource::Source, COLORREF>
        {
            return {
                {StandAssignmentSource::SOURCE_USER, RGB(255, 255, 255)},
                {StandAssignmentSource::SOURCE_RESERVATION_ALLOCATOR, RGB(255, 255, 0)},
                {StandAssignmentSource::SOURCE_VAA_ALLOCATOR, RGB(0, 255, 255)},
                {StandAssignmentSource::SOURCE_SYSTEM, RGB(180, 180, 180)}};
        }
    } // namespace

    StandColourConfiguration::StandColourConfiguration(UKControllerPlugin::Euroscope::UserSetting& setting)
        : StandColourConfiguration(&setting)
    {
    }

    StandColourConfiguration::StandColourConfiguration() : StandColourConfiguration(nullptr)
    {
    }

    StandColourConfiguration::StandColourConfiguration(UKControllerPlugin::Euroscope::UserSetting* userSetting)
        : userSetting(userSetting), sourceColours(DefaultSourceColours())
    {
        LoadFromUserSettings();
    }

    auto StandColourConfiguration::GetColourForSource(StandAssignmentSource::Source source) const -> COLORREF
    {
        if (auto it = sourceColours.find(source); it != sourceColours.cend()) {
            return it->second;
        }

        return DEFAULT_COLOUR;
    }

    void StandColourConfiguration::LoadFromUserSettings()
    {
        if (!this->userSetting) {
            return;
        }

        std::string key = std::string(SETTING_PREFIX) +
                          std::string(StandAssignmentSource::ToString(StandAssignmentSource::SOURCE_USER));
        if (this->userSetting->HasEntry(key)) {
            sourceColours[StandAssignmentSource::SOURCE_USER] =
                this->userSetting->GetColourEntry(key, DEFAULT_USER_COLOUR);
        }

        key = std::string(SETTING_PREFIX) +
              std::string(StandAssignmentSource::ToString(StandAssignmentSource::SOURCE_RESERVATION_ALLOCATOR));
        if (this->userSetting->HasEntry(key)) {
            sourceColours[StandAssignmentSource::SOURCE_RESERVATION_ALLOCATOR] =
                this->userSetting->GetColourEntry(key, DEFAULT_RESERVATION_COLOUR);
        }

        key = std::string(SETTING_PREFIX) +
              std::string(StandAssignmentSource::ToString(StandAssignmentSource::SOURCE_VAA_ALLOCATOR));
        if (this->userSetting->HasEntry(key)) {
            sourceColours[StandAssignmentSource::SOURCE_VAA_ALLOCATOR] =
                this->userSetting->GetColourEntry(key, DEFAULT_VAA_COLOUR);
        }

        key = std::string(SETTING_PREFIX) +
              std::string(StandAssignmentSource::ToString(StandAssignmentSource::SOURCE_SYSTEM));
        if (this->userSetting->HasEntry(key)) {
            sourceColours[StandAssignmentSource::SOURCE_SYSTEM] =
                this->userSetting->GetColourEntry(key, DEFAULT_SYSTEM_COLOUR);
        }

        LogInfo("Loaded stand assignment source colours from UserSettings");
    }

} // namespace UKControllerPlugin::Stands
