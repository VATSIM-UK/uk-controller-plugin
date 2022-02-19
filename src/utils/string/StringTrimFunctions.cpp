#include "StringTrimFunctions.h"

namespace UKControllerPluginUtils::String {

    auto ltrim(const std::string& string, const std::string& charactersToTrim) -> std::string
    {
        size_t start = string.find_first_not_of(charactersToTrim);
        return (start == std::string::npos) ? "" : string.substr(start);
    }

    auto rtrim(const std::string& string, const std::string& charactersToTrim) -> std::string
    {
        size_t end = string.find_last_not_of(charactersToTrim);
        return (end == std::string::npos) ? "" : string.substr(0, end + 1);
    }

    auto trim(const std::string& string, const std::string& charactersToTrim) -> std::string
    {
        return ltrim(rtrim(string, charactersToTrim), charactersToTrim);
    }
} // namespace UKControllerPluginUtils::String
