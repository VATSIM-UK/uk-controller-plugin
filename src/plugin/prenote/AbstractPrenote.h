#pragma once

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
            Represents a pre-note to a controller. Provides methods
            to check whether a pre-note is appropriate given a flightplan
            and which controllers require contacting.
        */
        class AbstractPrenote
        {
            public:
                explicit AbstractPrenote(
                    std::unique_ptr<UKControllerPlugin::Controller::ControllerPositionHierarchy> controllers
                );
                virtual ~AbstractPrenote();
                const UKControllerPlugin::Controller::ControllerPositionHierarchy & GetControllers(void) const;
                virtual std::string GetSummaryString(void) const = 0;
                virtual bool IsApplicable(
                    const UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightplan
                ) const = 0;

            private:
                std::unique_ptr<UKControllerPlugin::Controller::ControllerPositionHierarchy> controllers;
        };
    }  // namespace Prenote
}  // namespace UKControllerPlugin
