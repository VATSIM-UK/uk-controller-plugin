#include "NumberFormat.h"
#include "helper/HelperFunctions.h"

namespace UKControllerPlugin::Number {

    auto To1Dp(double number) -> std::string
    {
        char buffer[200];                  // NOLINT
        sprintf_s(buffer, "%.1f", number); // NOLINT
        return {buffer};
    }

    auto To1DpWide(double number) -> std::wstring
    {
        return HelperFunctions::ConvertToWideString(To1Dp(number));
    }
} // namespace UKControllerPlugin::Number
