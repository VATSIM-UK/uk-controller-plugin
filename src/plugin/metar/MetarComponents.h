#pragma once

namespace UKControllerPlugin::Metar {
    class PressureComponent;

    /**
     * Struct containing the individual components of the METAR
     */
    using MetarComponents = struct MetarComponents
    {
        std::shared_ptr<PressureComponent> pressure;
    };
} // namespace UKControllerPlugin::Metar
