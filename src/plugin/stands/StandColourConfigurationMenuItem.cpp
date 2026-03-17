#include "StandColourConfigurationMenuItem.h"
#include "StandColourConfiguration.h"
#include "StandAssignmentSource.h"
#include "helper/HelperFunctions.h"
#include "plugin/PopupMenuItem.h"
#include <Windows.h>
#include <array>

using UKControllerPlugin::HelperFunctions;
using UKControllerPlugin::Plugin::PopupMenuItem;

namespace UKControllerPlugin::Stands {

    namespace {
        auto CALLBACK ChooseColorTitleHook(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) -> UINT_PTR
        {
            if (msg != WM_INITDIALOG) {
                return 0;
            }

            const auto* chooseColor = reinterpret_cast<CHOOSECOLOR*>(lParam);
            if (chooseColor == nullptr || chooseColor->lCustData == 0) {
                return 0;
            }

            const auto* title = reinterpret_cast<const char*>(chooseColor->lCustData);
            SetWindowTextA(hwnd, title);
            return 1;
        }
    } // namespace

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

        std::array<std::string, 4> sourceLabels = {"User", "Reservation Allocator", "VAA Allocator", "System"};

        // Allow user to pick colours for each source
        CHOOSECOLOR chooseColor{};
        chooseColor.lStructSize = sizeof(CHOOSECOLOR);
        chooseColor.hwndOwner = GetActiveWindow();
        chooseColor.lpCustColors = this->customColours.data();
        chooseColor.lpfnHook = ChooseColorTitleHook;
        chooseColor.Flags = CC_RGBINIT | CC_FULLOPEN | CC_ENABLEHOOK;

        for (size_t offset = 0; offset < sources.size(); ++offset) {
            const size_t i = (this->lastSelectedSourceIndex + offset) % sources.size();
            chooseColor.rgbResult = this->colourConfiguration->GetColourForSource(sources[i]);
            const auto dialogTitle = std::string("Stand Colour Configuration - ") + sourceLabels[i];
            chooseColor.lCustData = reinterpret_cast<LPARAM>(dialogTitle.c_str());

            if (ChooseColor(&chooseColor) == TRUE) {
                this->colourConfiguration->SetColourForSource(sources[i], chooseColor.rgbResult);
                LogInfo("Stand colour updated for source: " + sources[i]);
            } else {
                this->lastSelectedSourceIndex = i;
                // User cancelled, stop the colour selection process
                break;
            }

            if (offset == sources.size() - 1) {
                this->lastSelectedSourceIndex = 0;
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
