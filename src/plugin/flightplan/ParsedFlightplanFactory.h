#pragma once

namespace UKControllerPlugin::Euroscope {
    class EuroscopeExtractedRouteInterface;
} // namespace UKControllerPlugin::Euroscope

namespace UKControllerPlugin::Flightplan {
    class ParsedFlightplan;

    [[nodiscard]] auto ParseFlightplanFromEuroscope(Euroscope::EuroscopeExtractedRouteInterface& euroscopePlan)
        -> std::shared_ptr<ParsedFlightplan>;
} // namespace UKControllerPlugin::Flightplan
