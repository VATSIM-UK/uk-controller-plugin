#include "pch/stdafx.h"
#include "hold/HoldDisplayFunctions.h"

namespace UKControllerPlugin {
    namespace Hold {

        /*
            Converts a C++ string to TCHAR so that we can use it in displays.
        */
        TCHAR * ConvertToTchar(std::string string)
        {
            TCHAR * param = new TCHAR[string.size() + 1];
            param[string.size()] = 0;
            std::copy(string.begin(), string.end(), param);
            return param;
        }

        /*
            Convert an integer to string then to TCHAR
        */
        TCHAR * ConvertToTchar(int value)
        {
            return ConvertToTchar(std::to_string(value));
        }

        /*
            Given an altitude, return its display value
        */
        std::wstring GetLevelDisplayString(unsigned int altitude)
        {
            std::wstring altString = std::to_wstring(altitude);
            size_t stringLength = altString.size();

            if (stringLength < 3) {
                return L"000";
            } else if(stringLength == 3) {
                return L"00" + altString.substr(0, 1);
            } else if (stringLength == 4) {
                return L"0" + altString.substr(0, 2);
            }

            return altString.substr(0, 3);
        }
    }  // namespace Hold
}  // namespace UKControllerPlugin
