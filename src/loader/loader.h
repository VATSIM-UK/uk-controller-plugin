#pragma once

void CheckForUpdates();
bool UpdateRequired(const nlohmann::json& versionDetails);
bool LockfileOutdated(std::string latestVersion);
bool VersionDetailsValid(const nlohmann::json& versionDetails);
void PerformUpdates(const nlohmann::json& versionDetails);
std::string GetLibsDownload(const nlohmann::json& versionDetails);
std::string GetLoaderDownload(const nlohmann::json& versionDetails);
nlohmann::json GetUpdateDetails();
nlohmann::json ParseUpdateResponse(const std::string& responseData, const uint64_t& responseCode, CURLcode result);
CURLcode PerformWebRequest(std::string url, std::string& response, uint64_t& responseCode);
size_t CurlWriteFunction(void* contents, size_t size, size_t nmemb, void* outString);
std::wstring GetUkcpRootDirectory();
std::wstring GetUkcpLibsLocation();
std::wstring GetVersionLockfileLocation();
void WriteLibsDataToFilesystem(std::string data);
void WriteLoaderDataToFilesystem(std::string data);
void UpdateLockfileVersion(const nlohmann::json& versionDetails);
void WriteToFilesystem(std::string data, std::wstring path);
void CreateUkcpRoot();
bool VersionsMatch(std::string latest, std::string lockfile);
std::string GetVersionFromJson(const nlohmann::json& versionDetails);
std::string GetLibsDownloadUrlFromJson(const nlohmann::json& versionDetails);
std::string GetLoaderDownloadUrlFromJson(const nlohmann::json& versionDetails);
