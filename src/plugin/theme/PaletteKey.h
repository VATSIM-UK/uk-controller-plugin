#pragma once

namespace UKControllerPlugin::Theme {

    enum class PaletteKey : std::size_t
    {
        Background = 0,
        Border,
        Header,
        HeaderHighlight,
        Text,
        TextHighlight,
        AircraftText,
        AircraftTextHighlight,
        TimerGreen,
        TimerYellow,
        TimerRed,
        _Count,
    };

    constexpr const auto PALETTE_KEY_COUNT = static_cast<std::size_t>(PaletteKey::_Count);

} // namespace UKControllerPlugin::Theme
