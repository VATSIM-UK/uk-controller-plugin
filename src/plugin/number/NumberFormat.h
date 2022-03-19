#pragma once

namespace UKControllerPlugin::Number {
    [[nodiscard]] auto To1Dp(double number) -> std::string;
    [[nodiscard]] auto To1DpWide(double number) -> std::wstring;
} //
