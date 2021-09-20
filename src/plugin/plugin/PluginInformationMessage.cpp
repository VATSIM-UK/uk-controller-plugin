#include "PluginInformationMessage.h"
#include "helper/HelperFunctions.h"
#include "update/PluginVersion.h"
#include "windows/WinApiInterface.h"

using UKControllerPlugin::HelperFunctions;
using UKControllerPlugin::Windows::WinApiInterface;

namespace UKControllerPlugin::Plugin {

    PluginInformationMessage::PluginInformationMessage(WinApiInterface& winApi, int menuCallbackId)
        : menuCallbackId(menuCallbackId), winApi(winApi)
    {
    }

    void PluginInformationMessage::Configure(int functionId, std::string subject, RECT screenObjectArea)
    {
        this->ShowInformationMessage();
    }

    auto PluginInformationMessage::GetConfigurationMenuItem() const -> UKControllerPlugin::Plugin::PopupMenuItem
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

    auto PluginInformationMessage::ProcessCommand(std::string command) -> bool
    {
        if (command != this->command) {
            return false;
        }

        this->ShowInformationMessage();
        return true;
    }

    void PluginInformationMessage::ShowInformationMessage()
    {
        time_t currentTime = time(nullptr);
        struct tm buf
        {
        };
        gmtime_s(&buf, &currentTime);

        std::wstring message;
        message += L"UK Controller Plugin\r\n";
        message += L"Version " + HelperFunctions::ConvertToWideString(PluginVersion::version) + L"\r\n";
        message +=
            L"Copyright \xa9 " + std::to_wstring(DATE_BASE_YEAR + buf.tm_year) + L" VATSIM United Kingdom Division";

        this->winApi.OpenMessageBox(message.c_str(), L"About UKCP", MB_OK | MB_ICONINFORMATION);
    }
} // namespace UKControllerPlugin::Plugin
