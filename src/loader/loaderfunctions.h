#pragma once

bool DoUpdate();
nlohmann::json GetUpdateDetails();
size_t CurlWriteFunction(void *contents, size_t size, size_t nmemb, void *outString);