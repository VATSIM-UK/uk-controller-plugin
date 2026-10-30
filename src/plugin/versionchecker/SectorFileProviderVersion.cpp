#include "SectorFileProviderVersion.h"
#include "log/LoggerFunctions.h"
#include "windows/WinApiInterface.h"

namespace UKControllerPlugin::VersionChecker {

    constexpr const char* LATEST_VERSION =
        "http://docs.vatsim.uk/General/Software%20Downloads/Files/VATUK_Euroscope_files.txt";

    const std::wstring VERSION_FILE_PATH = L"UK/Data/Sector/VATUK_SectorFileProviderDescriptor.txt";

    SectorFileProviderVersion::SectorFileProviderVersion(Windows::WinApiInterface& windows_) : windows(windows_)
    {
    }

    auto SectorFileProviderVersion::GetCurrentVersion() -> std::string
    {
        if (!windows.FileExists(VERSION_FILE_PATH, false)) {
            LogWarning("Sector file provider descriptor does not exist");
            return "unknown";
        }

        std::istringstream fileContent(windows.ReadFromFile(VERSION_FILE_PATH, false));
        std::string line;

        while (std::getline(fileContent, line)) {
            if (line.starts_with("URL:")) {
                line.erase(0, 4);
                return line;
            }
        }

        LogWarning("Sector file provider descriptor is missing URL entry");
        return "unknown";
    }

    auto SectorFileProviderVersion::GetLatestVersion() -> std::string
    {
        return LATEST_VERSION;
    }

    auto SectorFileProviderVersion::GetUpdateMessage() -> std::string
    {
        return "Your sector file provider URL is outdated; please download the latest controller pack from "
               "<https://docs.vatsim.uk/sf>.";
    }

} // namespace UKControllerPlugin::VersionChecker
