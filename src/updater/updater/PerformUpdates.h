#pragma once

// Forward declarations
namespace UKControllerPlugin {
    namespace Api {
        class ApiInterface;
    } // namespace Api

    namespace Windows {
        class WinApiInterface;
    } // namespace Windows

    namespace Curl {
        class CurlInterface;
    } // namespace Curl
} // namespace UKControllerPlugin

bool CheckForUpdates(
    const UKControllerPlugin::Api::ApiInterface& api,
    UKControllerPlugin::Windows::WinApiInterface& windows,
    UKControllerPlugin::Curl::CurlInterface& curl,
    bool duplicatePlugin,
    const std::string& updateChannel);
bool UpdateRequired(UKControllerPlugin::Windows::WinApiInterface& windows, const nlohmann::json& versionDetails);
void PerformUpdates(
    UKControllerPlugin::Curl::CurlInterface& curl,
    UKControllerPlugin::Windows::WinApiInterface& windows,
    const nlohmann::json& versionDetails);
void MoveOldUpdaterBinary(UKControllerPlugin::Windows::WinApiInterface& windows);
std::wstring GetOldUpdaterLocation();
std::string GetVersionFromJson(const nlohmann::json& versionDetails);
void DisplayPostUpdateNotification(UKControllerPlugin::Windows::WinApiInterface& windows, std::wstring version);
bool DisplayPreUpdateConsentNotification(UKControllerPlugin::Windows::WinApiInterface& windows, std::wstring version);
