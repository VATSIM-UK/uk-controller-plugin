#pragma once
#include "AbstractPrenote.h"

namespace UKControllerPlugin::Prenote {

    /*
        A class that corresponds to a prenote for a given standard instrument departure.
    */
    class DeparturePrenote : public UKControllerPlugin::Prenote::AbstractPrenote
    {
        public:
        DeparturePrenote(
            std::unique_ptr<UKControllerPlugin::Controller::ControllerPositionHierarchy> controllers,
            std::string airfield,
            std::string departure);
        [[nodiscard]] auto GetSummaryString() const -> std::string override;
        [[nodiscard]] auto
        IsApplicable(const UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightplan) const
            -> bool override;

        private:
        // The airfield this prenote associates with.
        std::string airfield;

        // The specific departure.
        std::string departure;
    };
} // namespace UKControllerPlugin::Prenote
