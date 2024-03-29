#include "hold/HoldDisplayFunctions.h"

namespace UKControllerPlugin {
    namespace Hold {

        /*
            The minimum offset from a holding level that an aircraft must be
            before it is considered to be going into the next level if climbing
            or descending.
        */
        const int minimumLevelOffset = 400;

        /*
            The minimum vertical speed that an aircraft must have in order to be
            considered to be climbing or descending.
        */
        const int minimumVerticalSpeed = 300;

        /*
            Converts a C++ string to TCHAR so that we can use it in displays.
        */
        std::wstring ConvertToTchar(std::string string)
        {
            std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
            return converter.from_bytes(string);
        }

        /*
            Convert an integer to string then to TCHAR
        */
        std::wstring ConvertToTchar(int value)
        {
            return ConvertToTchar(std::to_string(value));
        }

        /*
            Convert from TCHAR to a standard string.
        */
        std::string ConvertFromTchar(TCHAR* string)
        {
            std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
            return converter.to_bytes(string);
        }

        /*
            Given an altitude, return its display value
        */
        std::wstring GetLevelDisplayString(int altitude)
        {
            // Round altitude to nearest 100
            altitude = ((altitude + 50) / 100) * 100;
            std::wstring altString = std::to_wstring(altitude);
            size_t stringLength = altString.size();

            if (stringLength < 3) {
                return L"000";
            } else if (stringLength == 3) {
                return L"00" + altString.substr(0, 1);
            } else if (stringLength == 4) {
                return L"0" + altString.substr(0, 2);
            }

            return altString.substr(0, 3);
        }

        /*
            Returns the occupied level in the hold, based on altitude and vertical
            speed
        */
        unsigned int GetOccupiedLevel(int altitude, int verticalSpeed)
        {
            const int nearestThousand = ((altitude + 500) / 1000) * 1000;
            const int difference = altitude - nearestThousand;

            if (std::abs(verticalSpeed) < minimumVerticalSpeed) {
                return nearestThousand;
            }

            if (verticalSpeed < 0) {
                return difference <= (-minimumLevelOffset) ? nearestThousand - 1000 : nearestThousand;
            } else {
                return difference >= minimumLevelOffset ? nearestThousand + 1000 : nearestThousand;
            }
        }

        /*
            Returns the row in the hold manager that an aircraft should be displayed in, given its
            occupied level
        */
        unsigned int GetDisplayRow(int holdMax, int occupiedLevel)
        {
            return (holdMax - occupiedLevel) / 1000;
        }

        /*
            Returns the time in the hold as minutes
        */
        std::wstring GetTimeInHoldDisplayString(const std::chrono::system_clock::time_point& entryTime)
        {
            int64_t minutes =
                std::chrono::duration_cast<std::chrono::minutes>(std::chrono::system_clock::now() - entryTime).count();

            return std::to_wstring(minutes) + L"m";
        }

        int GetVerticalSpeedDirection(int verticalSpeed)
        {
            if (std::abs(verticalSpeed) < minimumVerticalSpeed) {
                return 0;
            }

            return verticalSpeed > 0 ? 1 : -1;
        }

        auto FormatOutboundLegValue(double value) -> std::wstring
        {
            std::wstringstream stream;
            stream.setf(std::ios::fixed);
            stream.precision(1);
            stream << value;

            return stream.str();
        }
    } // namespace Hold
} // namespace UKControllerPlugin
