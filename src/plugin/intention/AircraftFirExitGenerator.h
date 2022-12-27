#pragma once

namespace UKControllerPlugin::Euroscope {
    class EuroScopeCFlightPlanInterface;
} // namespace UKControllerPlugin::Euroscope

namespace UKControllerPlugin::IntentionCode
{
    class AircraftFirExit;

    /*
        Interface for classes that generate an aircrafts FIR exit point.
    */
    class AircraftFirExitGenerator
    {
        public:
            /*
                Generates the aircrafts FIR exit point data, or nullptr if there are none.
            */
            [[nodiscard]] virtual auto Generate(Euroscope::EuroScopeCFlightPlanInterface& flightplan) -> std::shared_ptr<AircraftFirExit> = 0;
    }
} // namespace UKControllerPlugin::IntentionCode