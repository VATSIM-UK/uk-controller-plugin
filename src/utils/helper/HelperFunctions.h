#pragma once

namespace UKControllerPlugin {

    /*
        Class containing utility / helper functions that
        don't really fit in anywhere else.
    */
    class HelperFunctions
    {
        public:
        static COLORREF GetColourFromSettingString(std::string setting);
        static std::string GetColourString(COLORREF color);
        static std::chrono::system_clock::time_point GetTimeFromNumberString(std::string numberString);
        static bool IsAnInteger(std::string string);
        static bool IsValidSettingsColour(std::string setting);
        static bool IsBoolean(std::string string);
        static bool IsFloat(std::string string);
        static std::string StripQuotesFromJsonString(std::string string);
        static std::vector<std::string> TokeniseString(const char delimiter, std::string original);
        static std::wstring ConvertToWideString(std::string string);
        static std::string ConvertToRegularString(std::wstring wstring);
        static std::string VectorToDelimetedString(std::vector<std::string> vector, std::string delimiter);
    };
} // namespace UKControllerPlugin
