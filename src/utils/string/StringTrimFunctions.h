#pragma once

namespace UKControllerPluginUtils::String {
    const std::string DEFAULT_CHARS_TO_TRIM = " \n\r\t\f\v";

    [[nodiscard]] auto ltrim(const std::string& string, const std::string& charactersToTrim = DEFAULT_CHARS_TO_TRIM)
        -> std::string;
    [[nodiscard]] auto rtrim(const std::string& string, const std::string& charactersToTrim = DEFAULT_CHARS_TO_TRIM)
        -> std::string;
    [[nodiscard]] auto trim(const std::string& string, const std::string& charactersToTrim = DEFAULT_CHARS_TO_TRIM)
        -> std::string;
} // namespace UKControllerPluginUtils::String
