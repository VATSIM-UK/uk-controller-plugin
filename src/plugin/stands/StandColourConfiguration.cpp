#include "StandColourConfiguration.h"
#include "StandAssignmentSource.h"
#include "euroscope/UserSetting.h"
#include "helper/HelperFunctions.h"
#include <array>
#include <stdexcept>

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
                                        {{StandAssignment::Source::User, DEFAULT_USER_COLOUR},
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

        struct SourceColourDefault
        {
            StandAssignment::Source source;
            COLORREF defaultColour;
        };

        constexpr std::array<SourceColourDefault, 4> sourceColourDefaults = {{
            {StandAssignment::Source::User, DEFAULT_USER_COLOUR},
            {StandAssignment::Source::ReservationAllocator, DEFAULT_RESERVATION_COLOUR},
            {StandAssignment::Source::VaaAllocator, DEFAULT_VAA_COLOUR},
            {StandAssignment::Source::SystemAuto, DEFAULT_SYSTEM_COLOUR},
        }};

        for (const auto& entry : sourceColourDefaults) {
            const std::string key = std::string(SETTING_PREFIX) + std::string(StandAssignment::ToString(entry.source));
            if (this->userSetting->HasEntry(key)) {
                this->sourceColours[entry.source] = this->userSetting->GetColourEntry(key);
            }
        }

        LogInfo("Loaded stand assignment source colours from UserSettings");
    }

} // namespace UKControllerPlugin::Stands
