#pragma once

bool CheckForUpdates();
bool DoUpdate(const nlohmann::json &versionDetails);
bool RequiresUpdate(std::string latestVersion);
bool VersionDetailsValid(const nlohmann::json & versionDetails);
nlohmann::json GetUpdateDetails();
size_t CurlWriteFunction(void *contents, size_t size, size_t nmemb, void *outString);