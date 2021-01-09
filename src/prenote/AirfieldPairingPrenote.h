#pragma once
#include "prenote/AbstractPrenote.h"

namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroScopeCFlightPlanInterface;
    }  // namespace Euroscope
    namespace Controller {
        class ControllerPositionHierarchy;
    }  // namespace Controller
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Prenote {

        /*
            Represents a prenote where the conditions for prenoting
            are the origin and destination airfields (the pairing).
        */
        class AirfieldPairingPrenote : public UKControllerPlugin::Prenote::AbstractPrenote
        {
            public:
                AirfieldPairingPrenote(
                    std::unique_ptr<UKControllerPlugin::Controller::ControllerPositionHierarchy> controllers,
                    std::string origin,
                    std::string destination,
                    std::string flightRules
                );

                // Inherited via AbstractPrenote
            
                std::string GetSummaryString(void) const override;
                std::string GetFlightRules() const;
                bool IsApplicable(
                    const UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightplan
                ) const override;

                const std::string NO_FLIGHT_RULES = "";

            private:

                // The origin aerodrome
                std::string origin;

                // The destination aerodrome
                std::string destination;

                // The destination aerodrome
                std::string flightRules;
        };
    }  // namespace Prenote
}  // namespace UKControllerPlugin
