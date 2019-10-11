#include "pch/stdafx.h"
#include "helper/HelperFunctions.h"
#include "windows/WinApiInterface.h"

using UKControllerPlugin::Windows::WinApiInterface;

namespace UKControllerPlugin {

    /*
        Convert vector to a delimited string
    */
    std::string HelperFunctions::VectorToDelimetedString(std::vector<std::string> vector, std::string delimiter)
    {
        std::ostringstream imploded;
        std::copy(vector.begin(), vector.end(),
            std::ostream_iterator<std::string>(imploded, delimiter.c_str()));

        return imploded.str().substr(0, imploded.str().size() - 1);
    }

    /*
        Given a comma separated colour string, convert it into a COLORREF.
        Used to convert settings data to a colour.
    */
    COLORREF HelperFunctions::GetColourFromSettingString(std::string setting)
    {
        if (!IsValidSettingsColour(setting)) {
            return RGB(255, 255, 255);
        }

        std::vector<std::string> tokens = HelperFunctions::TokeniseString(',', setting);

        return RGB(std::stoi(tokens[0]), std::stoi(tokens[1]), std::stoi(tokens[2]));
    }

    /*
        Get convert COLORREF to RGB string
    */
    std::string HelperFunctions::GetColourString(COLORREF color)
    {
        return std::to_string(GetRValue(color)) + ","
            + std::to_string(GetGValue(color)) + ","
            + std::to_string(GetBValue(color));
    }

    /*
        Get a time point today with the time given
    */
    std::chrono::system_clock::time_point HelperFunctions::GetTimeFromNumberString(std::string numberString)
    {
        if (numberString.empty()) {
            return (std::chrono::system_clock::time_point::max)();
        }

        // Incase someone puts the Z time on the end
        if (numberString.at(numberString.size() - 1) == 'Z' || numberString.at(numberString.size() - 1) == 'z') {
            numberString = numberString.substr(0, numberString.size() - 1);
        }

        // Incase of missing prefix 0
        while (numberString.size() < 4) {
            numberString = "0" + numberString;
        }

        if (numberString.size() != 4 ||
            !HelperFunctions::IsAnInteger(numberString)
        ) {
            return (std::chrono::system_clock::time_point::max)();
        }

        int hours = std::stoi(numberString.substr(0, 2));
        int minutes = std::stoi(numberString.substr(2, 2));

        if (
            (hours < 0 || hours > 23) ||
            (minutes < 0 || minutes > 59)
        ) {
            return (std::chrono::system_clock::time_point::max)();
        }

        // Get the current time, then set the hours and minutes based on the input
        time_t timeNow = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::tm tm;
        gmtime_s(&tm, &timeNow);
        tm.tm_hour = hours;
        tm.tm_min = minutes;
        tm.tm_sec = 0;

        return std::chrono::system_clock::from_time_t(std::mktime(&tm));
    }

    /*
        Takes a String and checks if that string is in fact
        an integer. Useful for pulling number-style settings from ASR files and checking that
        they are in fact numbers.
    */
    bool HelperFunctions::IsAnInteger(std::string string)
    {
        return !string.empty() && std::find_if(string.begin(),
            string.end(), [](char c) { return !std::isdigit(c); }) == string.end();
    }

    /*
        Takes a String and checks if that string is in fact
        a float. Useful for pulling number-style settings from ASR files and checking that
        they are in fact numbers.
    */
    bool HelperFunctions::IsFloat(std::string string)
    {
        return !string.empty() && std::find_if(string.begin(),
            string.end(), [](char c) { return !std::isdigit(c) && c != '.'; }) == string.end() &&
            (std::count(string.begin(), string.end(), '.') == 1 || std::count(string.begin(), string.end(), '.') == 0);
    }

    /*
        Returns true if a comma separated string is a valid colour.
        Values should be Red,Green,Blue. Used to check data that comes
        from the settings files.
    */
    bool HelperFunctions::IsValidSettingsColour(std::string setting)
    {
        std::istringstream ss(setting);
        std::vector<std::string> tokens;
        std::string token;

        while (std::getline(ss, token, ',')) {
            tokens.push_back(token);
        }

        return tokens.size() == 3 &&
            IsAnInteger(tokens[0]) &&
            IsAnInteger(tokens[1]) &&
            IsAnInteger(tokens[2]) &&
            stoi(tokens[0]) >= 0 &&
            stoi(tokens[0]) <= 255 &&
            stoi(tokens[1]) >= 0 &&
            stoi(tokens[1]) <= 255 &&
            stoi(tokens[2]) >= 0 &&
            stoi(tokens[2]) <= 255;
    }

    /*
        Returns true if a string is a boolean value, false otherwise.
    */
    bool HelperFunctions::IsBoolean(std::string string)
    {
        return string == std::string("0") || string == std::string("1");
    }

    /*
        Returns a string with all quotes removed.
    */
    std::string HelperFunctions::StripQuotesFromJsonString(std::string string)
    {
        // Return the original string if empty string.
        if (string.begin() == string.end()) {
            return string;
        }

        std::string::iterator startChar = string.begin();
        if (*startChar == '"') {
            string.erase(startChar);
        }


        // We've deleted the last character...
        if (string.begin() == string.end()) {
            return string;
        }

        std::string::iterator endChar = --string.end();
        if (*endChar == '"') {
            string.erase(endChar);
        }

        return string;
    }

    /*
        Splits a string based on a delimeter. Ignores tokens that occur twice in succession and treats them as a single token.
    */
    std::vector<std::string> HelperFunctions::TokeniseString(const char delimiter, std::string original)
    {
        std::istringstream ss(original);
        std::vector<std::string> tokens;
        std::string token;

        while (std::getline(ss, token, delimiter)) {
            if (token.compare("") == 0) {
                continue;
            }

            tokens.push_back(token);
        }

        return tokens;
    }

    /*
        Convert a standard string to wide.
    */
    std::wstring HelperFunctions::ConvertToWideString(std::string string)
    {
        return std::wstring(string.cbegin(), string.cend());
    }

    /*
        Convert a widestring to regular
    */
    std::string HelperFunctions::ConvertToRegularString(std::wstring wstring)
    {
        return std::string(wstring.cbegin(), wstring.cend());
    }
}  // namespace UKControllerPlugin
