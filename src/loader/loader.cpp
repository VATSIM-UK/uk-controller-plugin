#include "pch.h"
#include "loader/loader.h"
#include "helper/HelperFunctions.h"

using UKControllerPlugin::HelperFunctions;

void CheckForUpdates()
{
    try {
        const nlohmann::json versionDetails = GetUpdateDetails();
        if (UpdateRequired(versionDetails)) {
            PerformUpdates(versionDetails);
        }
    } catch (std::exception exception) {
        std::wstring message = std::wstring(HelperFunctions::ConvertToWideString(exception.what())) + L"\r\n";
        message += L"Plugin will attempt to load with previously downloaded version.";

        MessageBox(GetActiveWindow(), message.c_str(), L"UKCP Automatic Update Failed", MB_OK | MB_ICONSTOP);
    }
}

void PerformUpdates(const nlohmann::json& versionDetails)
{
    WriteLibsDataToFilesystem(GetLibsDownload(versionDetails));
    WriteLoaderDataToFilesystem(GetLoaderDownload(versionDetails));
    UpdateLockfileVersion(versionDetails);


    // TODO: Move LibCurl to Chocolatey on build
}

std::string GetLibsDownload(const nlohmann::json& versionDetails)
{
    std::string pluginLibData;
    uint64_t responseCode;
    const CURLcode downloadCurlCode = PerformWebRequest(
        GetLibsDownloadUrlFromJson(versionDetails),
        pluginLibData,
        responseCode
    );

    if (downloadCurlCode != CURLE_OK) {
        throw std::exception(
            std::string(
                "A cURL error occurred when downloading the UKCP libraries binary: "
                + std::to_string(downloadCurlCode)
            ).c_str()
        );
    }
    if(responseCode != 200) {
        throw std::exception(
            std::string("An error occurred when downloading the UKCP libraries binary, response code: "
                + std::to_string(downloadCurlCode)
            ).c_str()
        );
    }

    return pluginLibData;
}

std::string GetLoaderDownload(const nlohmann::json& versionDetails)
{
    std::string pluginLoaderData;
    uint64_t responseCode;
    const CURLcode downloadCurlCode = PerformWebRequest(
        GetLoaderDownloadUrlFromJson(versionDetails),
        pluginLoaderData,
        responseCode
    );

    if (downloadCurlCode != CURLE_OK) {
        throw std::exception(
            std::string(
                "A cURL error occurred when downloading the UKCP loader binary: "
                + std::to_string(downloadCurlCode)
            ).c_str()
        );
    }
    if (responseCode != 200) {
        throw std::exception(
            std::string("An error occurred when downloading the UKCP loader binary, response code: "
                + std::to_string(downloadCurlCode)
            ).c_str()
        );
    }

    return pluginLoaderData;
}

bool UpdateRequired(const nlohmann::json& versionDetails)
{
    if (versionDetails.is_null())
    {
        return false;
    }

    if (!VersionDetailsValid(versionDetails))
    {
        throw std::exception("Unable to check for UK Controller Plugin updates. Invalid JSON returned.");
    }

    return LockfileOutdated(GetVersionFromJson(versionDetails));
}


bool LockfileOutdated(std::string latestVersion)
{
    if (!std::filesystem::exists(GetVersionLockfileLocation())) {
        return true;
    }

    std::ifstream file(GetVersionLockfileLocation(), std::ifstream::in);
    std::string lockfileVersion;
    file.exceptions(std::ifstream::badbit);
    if (file.is_open()) {
        lockfileVersion.assign((std::istreambuf_iterator<char>(file)),
                               (std::istreambuf_iterator<char>()));
        file.close();
    }

    return VersionsMatch(std::move(latestVersion), lockfileVersion);
}

bool VersionDetailsValid(const nlohmann::json& versionDetails)
{
    return versionDetails.is_object() &&
        versionDetails.contains("version") &&
        versionDetails.at("version").is_string() &&
        versionDetails.contains("libs_download_url") &&
        versionDetails.at("libs_download_url").is_string() &&
        versionDetails.contains("plugin_download_url") &&
        versionDetails.at("plugin_download_url").is_string();
}

nlohmann::json GetUpdateDetails()
{
    std::string responseBody;
    uint64_t responseCode;
    return ParseUpdateResponse(
        responseBody,
        responseCode,
        PerformWebRequest("https://ukcp.vatsim.uk/version/latest/github", responseBody, responseCode)
    );
}

nlohmann::json ParseUpdateResponse(const std::string& responseData, const uint64_t& responseCode, CURLcode result)
{
    // If we get a cURL error, nothing more to do
    if (result != CURLE_OK) {
        std::string message = "Unable to check for UK Controller Plugin updates. cURL error.\r\n";
        message += "Response code was: " + std::to_string(responseCode);
        throw std::exception(message.c_str());
    }

    // Parse the json response
    try
    {
        return nlohmann::json::parse(responseData);
    } catch (...) {
        throw std::exception("Unable to check for UK Controller Plugin updates, response was not JSON.");
    }
}

CURLcode PerformWebRequest(const std::string url, std::string& response, uint64_t& responseCode)
{
    CURL* curlObject;
    CURLcode result;

    // Set CURL params.
    curlObject = curl_easy_init();
    curl_easy_setopt(curlObject, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curlObject, CURLOPT_CUSTOMREQUEST, "GET");

    // Add headers
    std::string outBuffer;
    curl_easy_setopt(curlObject, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curlObject, CURLOPT_CONNECTTIMEOUT, 4);
    curl_easy_setopt(curlObject, CURLOPT_TIMEOUT, 10);
    curl_easy_setopt(curlObject, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curlObject, CURLOPT_WRITEFUNCTION, &CurlWriteFunction);

    result = curl_easy_perform(curlObject);
    curl_easy_getinfo(curlObject, CURLINFO_RESPONSE_CODE, &responseCode);
    curl_easy_cleanup(curlObject);
    return result;
}

size_t CurlWriteFunction(void *contents, size_t size, size_t nmemb, void *outString)
{
    // For Curl, we should assume that the data is not null terminated, so add a null terminator on the end
    static_cast<std::string*>(outString)->append(reinterpret_cast<char*>(contents) + '\0', size * nmemb);
    return size * nmemb;
}

std::wstring GetUkcpRootDirectory()
{
    TCHAR* folderPath = nullptr;
    HRESULT result = SHGetKnownFolderPath(FOLDERID_LocalAppData, KF_FLAG_SIMPLE_IDLIST, nullptr, &folderPath);

    std::wstring widePath(folderPath);
    std::replace(widePath.begin(), widePath.end(), L'\\', L'/');
    CoTaskMemFree(folderPath);
    return widePath + L"/UKControllerPlugin";
}

std::wstring GetUkcpLibsLocation()
{
    return GetUkcpRootDirectory() + L"/UKControllerPluginLibs.dll";
}

std::wstring GetVersionLockfileLocation()
{
    return GetUkcpRootDirectory() + L"/version.lock";
}

void WriteLibsDataToFilesystem(std::string data)
{
    WriteToFilesystem(
        data,
        GetUkcpLibsLocation()
    );
}

void WriteLoaderDataToFilesystem(std::string data)
{
    WriteToFilesystem(
        data,
        GetUkcpLibsLocation()
    );
}

void WriteToFilesystem(std::string data, std::wstring path)
{
    std::ofstream file(
        path,
        std::ofstream::out | std::ofstream::trunc
    );
    file.exceptions(std::ofstream::badbit);
    if (file.is_open()) {
        file << data;
        file.close();
    } else {
        throw std::exception(
            std::string("Unable to write to file: " + HelperFunctions::ConvertToRegularString(path)).c_str()
        );
    }
}

void CreateUkcpRoot()
{
    std::filesystem::create_directories(GetUkcpRootDirectory());
}

bool VersionsMatch(std::string latest, std::string lockfile)
{
    return latest == lockfile;
}

std::string GetVersionFromJson(const nlohmann::json& versionDetails)
{
    return versionDetails.at("version").get<std::string>();
}

std::string GetLibsDownloadUrlFromJson(const nlohmann::json& versionDetails)
{
    return versionDetails.at("libs_download_url").get<std::string>();
}

std::string GetLoaderDownloadUrlFromJson(const nlohmann::json& versionDetails)
{
    return versionDetails.at("plugin_download_url").get<std::string>();
}

void UpdateLockfileVersion(std::string version)
{
    WriteToFilesystem(std::move(version), GetVersionLockfileLocation());
}
