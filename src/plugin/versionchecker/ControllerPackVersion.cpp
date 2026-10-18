#include "ControllerPackVersion.h"
#include "curl/CurlInterface.h"
#include "curl/CurlRequest.h"
#include "log/LoggerFunctions.h"
#include "windows/WinApiInterface.h"

namespace UKControllerPlugin::VersionChecker {

    constexpr const char* WILDCARD_VERSION = "dev";

    const std::wstring VERSION_FILE_PATH = L"UK/Data/Sector/pack_version.txt";
    const std::string LATEST_VERSION_URL = "https://docs.vatsim.uk/General/Software%20Downloads/Files/pack_version.txt";

    ControllerPackVersion::ControllerPackVersion(Curl::CurlInterface& curl_, Windows::WinApiInterface& windows_)
        : curl(curl_), windows(windows_)
    {
    }

    auto ControllerPackVersion::GetCurrentVersion() -> std::string
    {
        if (!windows.FileExists(VERSION_FILE_PATH, false)) {
            LogWarning("Pack version file does not exist");
            return "unknown";
        }

        auto versionContent = windows.ReadFromFile(VERSION_FILE_PATH, false);
        return versionContent;
    }

    auto ControllerPackVersion::GetLatestVersion() -> std::string
    {
        Curl::CurlRequest request(LATEST_VERSION_URL, Curl::CurlRequest::METHOD_GET);
        auto response = curl.MakeCurlRequest(request);

        if (response.GetStatusCode() != 200) {
            LogWarning(std::format("Request for latest pack version returned status {}", response.GetStatusCode()));
            return "";
        }

        auto versionContent = response.GetResponse();
        return versionContent;
    }

    auto ControllerPackVersion::GetUpdateMessage() -> std::string
    {
        return "Your controller pack is outdated; please download the latest version from <https://docs.vatsim.uk/sf>.";
    }

    auto ControllerPackVersion::IsUpdateRequired(const std::string& current, const std::string& latest) -> bool
    {
        return current != WILDCARD_VERSION && current != latest;
    }

} // namespace UKControllerPlugin::VersionChecker
