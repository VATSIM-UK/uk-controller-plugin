#include "FileStatusModule.h"
#include "bootstrap/PersistenceContainer.h"
#include "windows/WinApiInterface.h"
#include "data/PluginDataLocations.h"
#include "log/LoggerFunctions.h"
#include "curl/CurlInterface.h"
#include "curl/CurlRequest.h"

#include <fstream>
#include <sstream>

namespace UKControllerPlugin::FileStatus {

    const std::string PACK_VERSION_URL = "http://docs.vatsim.uk/General/Software%20Downloads/Files/pack_version.txt";
    const std::string PACK_VERSION_PATH = "./UK/Data/Sector/pack_version.txt"; // local
    const std::string SECTOR_FILE_DOWNLOADER_PATH = "./UK/Data/Sector/VATUK_SectorFileProviderDescriptor.txt";
    const std::string SECTOR_FILE_DOWNLOADER_LINK =
        "http://docs.vatsim.uk/General/Software%20Downloads/Files/VATUK_Euroscope_files.txt";

    std::string FetchGithubVersion(UKControllerPlugin::Curl::CurlInterface& curl)
    {
        try {
            UKControllerPlugin::Curl::CurlRequest request(
                PACK_VERSION_URL, UKControllerPlugin::Curl::CurlRequest::METHOD_GET);

            auto response = curl.MakeCurlRequest(request);

            if (response.GetStatusCode() != 200) {
                LogError(
                    "Failed to fetch version from docs.vatsim.uk, status code: " +
                    std::to_string(response.GetStatusCode()));
                return "";
            }
            std::string version = response.GetResponse();
            version.erase(
                std::remove_if(version.begin(), version.end(), [](unsigned char c) { return std::isspace(c); }),
                version.end());

            return version;

        } catch (const std::exception& e) {
            LogError("Exception when fetching GitHub version: " + std::string(e.what()));
            return "";
        }
    }

    void CheckPackVersion(UKControllerPlugin::Bootstrap::PersistenceContainer& container)
    {
        try { // check pack is up to date

            std::ifstream versionFile(PACK_VERSION_PATH);

            if (!std::filesystem::exists(PACK_VERSION_PATH)) {
                LogError("Version file not found at: " + PACK_VERSION_PATH);
                container.windows->OpenMessageBox(
                    L"You must update your controller pack",
                    L"Out of Date Controller Pack Detected",
                    MB_OK | MB_ICONWARNING);
                return;
            }

            if (!versionFile.is_open()) {
                LogError("Could not open file when checking controller pack version");
                return;
            }

            std::stringstream buffer;
            buffer << versionFile.rdbuf();
            std::string versionContent = buffer.str();
            versionFile.close();

            std::string githubVersion = FetchGithubVersion(*container.curl);
            if (githubVersion.empty()) {
                LogError("Could not fetch pack sha");
                return;
            }

            if (versionContent != githubVersion) {
                LogWarning("Out of date pack detected");
                container.windows->OpenMessageBox(
                    L"You must update your controller pack",
                    L"Out of Date Controller Pack Detected",
                    MB_OK | MB_ICONWARNING);
                return;
            }

        } catch (const std::exception& e) {
            LogError("Exeption when checking UKCPa version");
        }
    }

    void CheckSectorFileProviderFile(UKControllerPlugin::Bootstrap::PersistenceContainer& container)
    {
        try {

            std::ifstream downloadFile(SECTOR_FILE_DOWNLOADER_PATH);

            if (!std::filesystem::exists(SECTOR_FILE_DOWNLOADER_PATH)) {
                LogError("File not found at: " + SECTOR_FILE_DOWNLOADER_PATH);
            }

            if (!downloadFile.is_open()) {
                LogError("Could not open Sector File Provider File when checking");
            }

            std::stringstream buffer;
            buffer << downloadFile.rdbuf();
            std::string sfDownloadFile = buffer.str();

            std::string extractedUrl;
            std::istringstream fileStream(sfDownloadFile);
            std::string line;

            while (std::getline(fileStream, line)) {
                if (line.substr(0, 4) == "URL:") {
                    extractedUrl = line.substr(4);
                    break;
                }
            }
            if (!extractedUrl.empty()) {
                LogInfo("Extracted sector file provider URL: " + extractedUrl);
            } else {
                LogError("Could not find URL in sector file provider");
            }

            if (extractedUrl != SECTOR_FILE_DOWNLOADER_LINK) {
                LogWarning("Out of date sector file provider URL detected");
                std::wstring message =
                    L"Your controller pack needs updating. The sector file downloader URL is outdated.\n\n"
                    L"Current URL: " +
                    std::wstring(extractedUrl.begin(), extractedUrl.end()) + L"\nExpected URL: " +
                    std::wstring(SECTOR_FILE_DOWNLOADER_LINK.begin(), SECTOR_FILE_DOWNLOADER_LINK.end()) +
                    L"\n\nPlease download the latest controller pack from VATSIM UK";

                container.windows->OpenMessageBox(
                    message.c_str(), L"Outdated Sector File Configuration", MB_OK | MB_ICONWARNING);
                return;
            }

        } catch (const std::exception& e) {
            LogError("Exeption when checking Sector File Download Link");
        }
    }

    void FileStatusModule::BootstrapPlugin(UKControllerPlugin::Bootstrap::PersistenceContainer& container)
    {
        CheckPackVersion(container);
        CheckSectorFileProviderFile(container);
    }

} // namespace UKControllerPlugin::FileStatus