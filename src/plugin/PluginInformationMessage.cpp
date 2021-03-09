#include "pch/stdafx.h"
#include "plugin/PluginInformationMessage.h"
#include "update/PluginVersion.h"
#include "helper/HelperFunctions.h"

using UKControllerPlugin::Windows::WinApiInterface;
using UKControllerPlugin::HelperFunctions;

namespace UKControllerPlugin {
    namespace Plugin {

        PluginInformationMessage::PluginInformationMessage(WinApiInterface& winApi, int menuCallbackId)
            : winApi(winApi), menuCallbackId(menuCallbackId)
        {
        }

        void PluginInformationMessage::Configure(int functionId, std::string subject, RECT screenObjectArea)
        {
            this->ShowInformationMessage();
        }

        UKControllerPlugin::Plugin::PopupMenuItem PluginInformationMessage::GetConfigurationMenuItem(void) const
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

        bool PluginInformationMessage::ProcessCommand(std::string command)
        {
            if (command != this->command) {
                return false;
            }

            this->ShowInformationMessage();
            return true;
        }

        void PluginInformationMessage::ShowInformationMessage()
        {
            time_t currentTime = time(NULL);
            struct tm buf;
            gmtime_s(&buf, &currentTime);

            std::wstring message;
            message += L"UK Controller Plugin\r\n";
            message += L"Version " + HelperFunctions::ConvertToWideString(PluginVersion::version) + L"\r\n";
            message += L"Copyright \xa9 " + std::to_wstring(1900 + buf.tm_year) + L" VATSIM United Kingdom Division";

            this->winApi.OpenMessageBox(
                message.c_str(),
                L"About UKCP",
                MB_OK | MB_ICONINFORMATION
            );
        }
    }  // namespace Plugin
}  // namespace UKControllerPlugin
