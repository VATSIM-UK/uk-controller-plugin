#include "StandColourConfiguration.h"
#include "StandAssignmentSource.h"
#include "euroscope/UserSetting.h"
#include "helper/HelperFunctions.h"
#include <stdexcept>

namespace UKControllerPlugin::Stands {

    StandColourConfiguration::StandColourConfiguration(UKControllerPlugin::Euroscope::UserSetting& setting)
        : userSetting(&setting)
    {
        sourceColours[std::string(StandAssignmentSource::SOURCE_USER)] = DEFAULT_USER_COLOUR;
        sourceColours[std::string(StandAssignmentSource::SOURCE_RESERVATION_ALLOCATOR)] = DEFAULT_RESERVATION_COLOUR;
        sourceColours[std::string(StandAssignmentSource::SOURCE_VAA_ALLOCATOR)] = DEFAULT_VAA_COLOUR;
        sourceColours[std::string(StandAssignmentSource::SOURCE_SYSTEM)] = DEFAULT_SYSTEM_COLOUR;

        LoadFromUserSettings();
    }

    StandColourConfiguration::StandColourConfiguration() : userSetting(nullptr)
    {
        sourceColours[std::string(StandAssignmentSource::SOURCE_USER)] = DEFAULT_USER_COLOUR;
        sourceColours[std::string(StandAssignmentSource::SOURCE_RESERVATION_ALLOCATOR)] = DEFAULT_RESERVATION_COLOUR;
        sourceColours[std::string(StandAssignmentSource::SOURCE_VAA_ALLOCATOR)] = DEFAULT_VAA_COLOUR;
        sourceColours[std::string(StandAssignmentSource::SOURCE_SYSTEM)] = DEFAULT_SYSTEM_COLOUR;
    }

    auto StandColourConfiguration::GetColourForSource(const std::string& source) const -> COLORREF
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

        try {
            std::string key = std::string(SETTING_PREFIX) + std::string(StandAssignmentSource::SOURCE_USER);
            if (this->userSetting->HasEntry(key)) {
                sourceColours[std::string(StandAssignmentSource::SOURCE_USER)] =
                    this->userSetting->GetColourEntry(key, DEFAULT_USER_COLOUR);
            }

            key = std::string(SETTING_PREFIX) + std::string(StandAssignmentSource::SOURCE_RESERVATION_ALLOCATOR);
            if (this->userSetting->HasEntry(key)) {
                sourceColours[std::string(StandAssignmentSource::SOURCE_RESERVATION_ALLOCATOR)] =
                    this->userSetting->GetColourEntry(key, DEFAULT_RESERVATION_COLOUR);
            }

            key = std::string(SETTING_PREFIX) + std::string(StandAssignmentSource::SOURCE_VAA_ALLOCATOR);
            if (this->userSetting->HasEntry(key)) {
                sourceColours[std::string(StandAssignmentSource::SOURCE_VAA_ALLOCATOR)] =
                    this->userSetting->GetColourEntry(key, DEFAULT_VAA_COLOUR);
            }

            key = std::string(SETTING_PREFIX) + std::string(StandAssignmentSource::SOURCE_SYSTEM);
            if (this->userSetting->HasEntry(key)) {
                sourceColours[std::string(StandAssignmentSource::SOURCE_SYSTEM)] =
                    this->userSetting->GetColourEntry(key, DEFAULT_SYSTEM_COLOUR);
            }

            LogInfo("Loaded stand assignment source colours from UserSettings");
        } catch (const std::out_of_range& e) {
            LogWarning("Failed to load stand colours from UserSettings: " + std::string(e.what()));
        }
    }

} // namespace UKControllerPlugin::Stands
