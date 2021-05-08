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

void CheckForUpdates(
    const UKControllerPlugin::Api::ApiInterface& api,
    UKControllerPlugin::Windows::WinApiInterface& windows,
    UKControllerPlugin::Curl::CurlInterface& curl,
    bool duplicatePlugin
);
bool UpdateRequired(UKControllerPlugin::Windows::WinApiInterface& windows, const nlohmann::json& versionDetails);
void PerformUpdates(
    UKControllerPlugin::Curl::CurlInterface& curl,
    UKControllerPlugin::Windows::WinApiInterface& windows,
    const nlohmann::json& versionDetails
);
void MoveOldUpdaterBinary(UKControllerPlugin::Windows::WinApiInterface& windows);
std::wstring GetOldUpdaterLocation();
void UpdateLockfile(UKControllerPlugin::Windows::WinApiInterface& windows, std::string version);
std::string GetVersionFromJson(const nlohmann::json& versionDetails);
void DisplayUpdateNotification(UKControllerPlugin::Windows::WinApiInterface& windows, std::wstring version);
