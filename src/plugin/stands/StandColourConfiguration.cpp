#include "StandColourConfiguration.h"
#include "StandAssignmentSource.h"
#include "euroscope/UserSetting.h"
#include "helper/HelperFunctions.h"
#include <array>
#include <stdexcept>

namespace UKControllerPlugin::Stands {

    namespace {
        struct SourceColourDefault
        {
            StandAssignment::Source source;
            COLORREF defaultColour;
        };

        constexpr std::array<SourceColourDefault, 4> sourceColourDefaults = {{
            {StandAssignment::Source::User, StandColourConfiguration::DEFAULT_USER_COLOUR},
            {StandAssignment::Source::ReservationAllocator, StandColourConfiguration::DEFAULT_RESERVATION_COLOUR},
            {StandAssignment::Source::VaaAllocator, StandColourConfiguration::DEFAULT_VAA_COLOUR},
            {StandAssignment::Source::SystemAuto, StandColourConfiguration::DEFAULT_SYSTEM_COLOUR},
        }};

        auto LoadColourFromUserSettings(
            UKControllerPlugin::Euroscope::UserSetting& userSetting,
            std::map<StandAssignment::Source, COLORREF>& sourceColours,
            StandAssignment::Source source,
            COLORREF defaultColour) -> void
        {
            const std::string key =
                std::string(StandColourConfiguration::SETTING_PREFIX) + std::string(StandAssignment::ToString(source));
            if (userSetting.HasEntry(key)) {
                sourceColours[source] = userSetting.GetColourEntry(key, defaultColour);
            }
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

        for (const auto& entry : sourceColourDefaults) {
            LoadColourFromUserSettings(*this->userSetting, this->sourceColours, entry.source, entry.defaultColour);
        }

        LogInfo("Loaded stand assignment source colours from UserSettings");
    }

} // namespace UKControllerPlugin::Stands
