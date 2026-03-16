#include "StandColourConfiguration.h"
#include "StandAssignmentSource.h"
#include "euroscope/UserSetting.h"
#include "helper/HelperFunctions.h"
#include <sstream>
#include <iomanip>

namespace UKControllerPlugin::Stands {

    StandColourConfiguration::StandColourConfiguration(UKControllerPlugin::Euroscope::UserSetting& userSetting)
        : userSetting(userSetting)
    {
        // Initialize with default colours
        sourceColours[std::string(StandAssignmentSource::SOURCE_USER)] = DEFAULT_USER_COLOUR;
        sourceColours[std::string(StandAssignmentSource::SOURCE_RESERVATION_ALLOCATOR)] = DEFAULT_RESERVATION_COLOUR;
        sourceColours[std::string(StandAssignmentSource::SOURCE_VAA_ALLOCATOR)] = DEFAULT_VAA_COLOUR;
        sourceColours[std::string(StandAssignmentSource::SOURCE_SYSTEM)] = DEFAULT_SYSTEM_COLOUR;

        // Try to load from UserSettings
        LoadFromUserSettings();
    }

    auto StandColourConfiguration::GetColourForSource(const std::string& source) const -> COLORREF
    {
        auto it = sourceColours.find(source);
        if (it != sourceColours.cend()) {
            return it->second;
        }

        return DEFAULT_COLOUR;
    }

    void StandColourConfiguration::LoadFromUserSettings()
    {
        try {
            std::string key(SETTING_PREFIX);

            // Load user colour
            key = std::string(SETTING_PREFIX) + std::string(StandAssignmentSource::SOURCE_USER);
            if (this->userSetting.HasEntry(key)) {
                sourceColours[std::string(StandAssignmentSource::SOURCE_USER)] =
                    this->userSetting.GetColourEntry(key, DEFAULT_USER_COLOUR);
            }

            // Load reservation allocator colour
            key = std::string(SETTING_PREFIX) + std::string(StandAssignmentSource::SOURCE_RESERVATION_ALLOCATOR);
            if (this->userSetting.HasEntry(key)) {
                sourceColours[std::string(StandAssignmentSource::SOURCE_RESERVATION_ALLOCATOR)] =
                    this->userSetting.GetColourEntry(key, DEFAULT_RESERVATION_COLOUR);
            }

            // Load VAA allocator colour
            key = std::string(SETTING_PREFIX) + std::string(StandAssignmentSource::SOURCE_VAA_ALLOCATOR);
            if (this->userSetting.HasEntry(key)) {
                sourceColours[std::string(StandAssignmentSource::SOURCE_VAA_ALLOCATOR)] =
                    this->userSetting.GetColourEntry(key, DEFAULT_VAA_COLOUR);
            }

            // Load system colour
            key = std::string(SETTING_PREFIX) + std::string(StandAssignmentSource::SOURCE_SYSTEM);
            if (this->userSetting.HasEntry(key)) {
                sourceColours[std::string(StandAssignmentSource::SOURCE_SYSTEM)] =
                    this->userSetting.GetColourEntry(key, DEFAULT_SYSTEM_COLOUR);
            }

            LogInfo("Loaded stand assignment source colours from UserSettings");
        } catch (const std::exception& e) {
            LogWarning("Failed to load stand colours from UserSettings: " + std::string(e.what()));
        }
    }

    void StandColourConfiguration::SaveToUserSettings()
    {
        try {
            for (const auto& [source, colour] : sourceColours) {
                const std::string key = std::string(SETTING_PREFIX) + source;
                this->userSetting.Save(key, "Stand assignment source colour for " + source, colour);
            }
            LogInfo("Saved stand assignment source colours to UserSettings");
        } catch (const std::exception& e) {
            LogWarning("Failed to save stand colours to UserSettings: " + std::string(e.what()));
        }
    }

    void StandColourConfiguration::SetColourForSource(const std::string& source, COLORREF colour)
    {
        sourceColours[source] = colour;
        // Immediately save to UserSettings
        try {
            const std::string key = std::string(SETTING_PREFIX) + source;
            this->userSetting.Save(key, "Stand assignment source colour for " + source, colour);
        } catch (const std::exception& e) {
            LogWarning("Failed to save stand colour for '" + source + "': " + std::string(e.what()));
        }
    }

    auto StandColourConfiguration::HexToColorref(const std::string& hex) -> COLORREF
    {
        std::string colourStr = hex;

        // Remove '#' if present
        if (!colourStr.empty() && colourStr[0] == '#') {
            colourStr = colourStr.substr(1);
        }

        // Validate hex string length
        if (colourStr.length() != 6) {
            throw std::invalid_argument("Hex colour must be 6 characters (without #)");
        }

        // Parse hex values
        try {
            // Windows COLORREF is BGR, not RGB, so we parse as RGB and convert
            const uint32_t rgbValue = std::stoul(colourStr, nullptr, 16);
            const auto r = static_cast<uint8_t>((rgbValue >> 16) & 0xFF);
            const auto g = static_cast<uint8_t>((rgbValue >> 8) & 0xFF);
            const auto b = static_cast<uint8_t>(rgbValue & 0xFF);

            return RGB(r, g, b);
        } catch (const std::exception& e) {
            throw std::invalid_argument(std::string("Invalid hex colour format: ") + e.what());
        }
    }

    auto StandColourConfiguration::ColourrefToHex(COLORREF colour) -> std::string
    {
        // COLORREF is BGR format, convert to RGB for hex
        const auto b = static_cast<uint8_t>((colour >> 0) & 0xFF);
        const auto g = static_cast<uint8_t>((colour >> 8) & 0xFF);
        const auto r = static_cast<uint8_t>((colour >> 16) & 0xFF);

        std::stringstream ss;
        ss << "#" << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(r) << std::setw(2)
           << static_cast<int>(g) << std::setw(2) << static_cast<int>(b);
        return ss.str();
    }
} // namespace UKControllerPlugin::Stands
