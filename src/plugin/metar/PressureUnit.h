#pragma once

namespace UKControllerPlugin::Metar {
    /**
     * The units in which pressure canbe recorded.
     */
    enum class PressureUnit : unsigned int
    {
        Hectopascals = 0,
        InHg = 1,
    };
} // namespace UKControllerPlugin::Metar
