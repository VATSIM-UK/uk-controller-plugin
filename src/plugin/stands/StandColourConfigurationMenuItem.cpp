#include "StandColourConfigurationMenuItem.h"
#include "StandColourConfiguration.h"
#include "StandAssignmentSource.h"
#include "helper/HelperFunctions.h"
#include "plugin/PopupMenuItem.h"
#include <Windows.h>
#include <array>

using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPlugin::HelperFunctions;

namespace UKControllerPlugin::Stands {

    StandColourConfigurationMenuItem::StandColourConfigurationMenuItem(
        std::shared_ptr<StandColourConfiguration> colourConfiguration, int callbackFunctionId)
        : colourConfiguration(std::move(colourConfiguration)), callbackFunctionId(callbackFunctionId)
    {
    }

    void StandColourConfigurationMenuItem::Configure(int functionId, std::string subject, RECT area)
    {
        if (!this->colourConfiguration) {
            LogWarning("StandColourConfigurationMenuItem: Colour configuration is null");
            return;
        }

        // Define the assignment sources to configure
        std::array<std::string, 4> sources = {
            std::string(StandAssignmentSource::SOURCE_USER),
            std::string(StandAssignmentSource::SOURCE_RESERVATION_ALLOCATOR),
            std::string(StandAssignmentSource::SOURCE_VAA_ALLOCATOR),
            std::string(StandAssignmentSource::SOURCE_SYSTEM)};

        // Allow user to pick colours for each source
        CHOOSECOLOR chooseColor{};
        COLORREF customColours[16] = {};
        chooseColor.lStructSize = sizeof(CHOOSECOLOR);
        chooseColor.hwndOwner = GetActiveWindow();
        chooseColor.lpCustColors = customColours;
        chooseColor.Flags = CC_RGBINIT | CC_FULLOPEN;

        for (size_t i = 0; i < sources.size(); ++i) {
            chooseColor.rgbResult = this->colourConfiguration->GetColourForSource(sources[i]);

            if (ChooseColor(&chooseColor) == TRUE) {
                this->colourConfiguration->SetColourForSource(sources[i], chooseColor.rgbResult);
                LogInfo("Stand colour updated for source: " + sources[i]);
            } else {
                // User cancelled, stop the colour selection process
                break;
            }
        }

        // Save the new colours to user settings
        this->colourConfiguration->SaveToUserSettings();
        LogInfo("Stand colours saved to user settings");
    }

    auto StandColourConfigurationMenuItem::GetConfigurationMenuItem() const -> PopupMenuItem
    {
        PopupMenuItem returnVal;
        returnVal.firstValue = this->itemDescription;
        returnVal.secondValue = "";
        returnVal.callbackFunctionId = this->callbackFunctionId;
        returnVal.checked = 2; // No checkbox
        returnVal.disabled = false;
        returnVal.fixedPosition = false;
        return returnVal;
    }
} // namespace UKControllerPlugin::Stands
