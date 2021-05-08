#include "pch/stdafx.h"
#include "plugin/ForceUpdate.h"
#include "data/PluginDataLocations.h"

using UKControllerPlugin::Windows::WinApiInterface;

namespace UKControllerPlugin {
    namespace Plugin {

        ForceUpdate::ForceUpdate(WinApiInterface& winApi, int menuCallbackId)
            : winApi(winApi), menuCallbackId(menuCallbackId)
        {
        }

        void ForceUpdate::Configure(int functionId, std::string subject, RECT screenObjectArea)
        {
            this->ForcePluginUpdate();
        }

        PopupMenuItem ForceUpdate::GetConfigurationMenuItem(void) const
        {
            PopupMenuItem returnVal;
            returnVal.firstValue = this->menuItemDescription;
            returnVal.secondValue = "";
            returnVal.callbackFunctionId = this->menuCallbackId;
            returnVal.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
            returnVal.disabled = false;
            returnVal.fixedPosition = false;
            return returnVal;
        }

        bool ForceUpdate::ProcessCommand(std::string command)
        {
            if (command != this->command) {
                return false;
            }

            this->ForcePluginUpdate();
            return true;
        }

        void ForceUpdate::ForcePluginUpdate() const
        {
            int response = this->winApi.OpenMessageBox(
                L"Are you sure you want to force the UK Controller Plugin to update?",
                L"UKCP Forced Update",
                MB_YESNO | MB_ICONEXCLAMATION
            );

            if (response != IDYES) {
                return;
            }

            this->winApi.DeleteGivenFile(GetUpdaterBinaryRelativePath());
            this->winApi.DeleteGivenFile(GetVersionLockfileLocation());
            this->winApi.OpenMessageBox(
                L"The plugin will force an update next time it is loaded, provided no other instances are running.",
                L"UKCP Update Information",
                MB_OK | MB_ICONINFORMATION
            );
        }
    }  // namespace Plugin
}  // namespace UKControllerPlugin
