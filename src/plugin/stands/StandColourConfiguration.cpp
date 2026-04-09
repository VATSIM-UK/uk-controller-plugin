#include "StandColourConfiguration.h"
#include "StandAssignmentSource.h"
#include "euroscope/UserSetting.h"
#include "helper/HelperFunctions.h"
#include "utils/log/LoggerFunctions.h"
#include <array>

namespace UKControllerPlugin::Stands {

    StandColourConfiguration::StandColourConfiguration(UKControllerPlugin::Euroscope::UserSetting& setting)
        : StandColourConfiguration(&setting)
    {
    }

    StandColourConfiguration::StandColourConfiguration() : StandColourConfiguration(nullptr)
    {
    }

    StandColourConfiguration::StandColourConfiguration(UKControllerPlugin::Euroscope::UserSetting* userSetting)
        : userSetting(userSetting), sourceColours(
                                        {{StandAssignment::Source::Unknown, DEFAULT_UNKNOWN_COLOUR},
                                         {StandAssignment::Source::User, DEFAULT_USER_COLOUR},
                                         {StandAssignment::Source::ReservationAllocator, DEFAULT_RESERVATION_COLOUR},
                                         {StandAssignment::Source::VaaAllocator, DEFAULT_VAA_COLOUR},
                                         {StandAssignment::Source::SystemAuto, DEFAULT_SYSTEM_COLOUR}})
    {
        LoadFromUserSettings();
    }

    auto StandColourConfiguration::GetColourForSource(StandAssignment::Source source) const -> COLORREF
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

        constexpr std::array<StandAssignment::Source, 4> sourceColourDefaults = {{
            StandAssignment::Source::Unknown,
            StandAssignment::Source::User,
            StandAssignment::Source::ReservationAllocator,
            StandAssignment::Source::VaaAllocator,
            StandAssignment::Source::SystemAuto,
        }};

        for (const auto source : sourceColourDefaults) {
            const std::string key = std::string(SETTING_PREFIX) + std::string(StandAssignment::ToString(source));
            if (this->userSetting->HasEntry(key)) {
                this->sourceColours[source] = this->userSetting->GetColourEntry(key);
            }
        }

        LogInfo("Loaded stand assignment source colours from UserSettings");
    }

} // namespace UKControllerPlugin::Stands
