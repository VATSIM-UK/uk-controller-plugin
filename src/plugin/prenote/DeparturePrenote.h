#pragma once
#include "prenote/AbstractPrenote.h"

namespace UKControllerPlugin {
    namespace Prenote {

        /*
            A class that corresponds to a prenote for a given standard instrument departure.
        */
        class DeparturePrenote : public UKControllerPlugin::Prenote::AbstractPrenote
        {
            public:
                DeparturePrenote(
                    std::unique_ptr<UKControllerPlugin::Controller::ControllerPositionHierarchy> controllers,
                    std::string airfield,
                    std::string departure
                );
                std::string GetSummaryString(void) const;
                bool IsApplicable(
                    const UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightplan
                ) const override;

            private:

                // The airfield this prenote associates with.
                std::string airfield;

                // The specific departure.
                std::string departure;
        };
    }  // namespace Prenote
}  // namespace UKControllerPlugin
