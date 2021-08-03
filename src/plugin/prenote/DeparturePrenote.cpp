#include "pch/pch.h"
#include "prenote/DeparturePrenote.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "controller/ControllerPositionHierarchy.h"

using UKControllerPlugin::Controller::ControllerPositionHierarchy;
using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;

namespace UKControllerPlugin {
    namespace Prenote {

        DeparturePrenote::DeparturePrenote(
            std::unique_ptr<ControllerPositionHierarchy> controllers,
            std::string airfield,
            std::string departure
        )
            : AbstractPrenote(std::move(controllers)), airfield(airfield), departure(departure)
        {

        }

        /*
            Returns a summary of the prenote for display.
        */
        std::string DeparturePrenote::GetSummaryString(void) const
        {
            return this->airfield + ", " + this->departure;
        }

        /*
            Returns true if this prenote is applicable to a given flightplan.
        */
        bool DeparturePrenote::IsApplicable(const EuroScopeCFlightPlanInterface & flightplan) const
        {
            return flightplan.GetOrigin() == this->airfield && flightplan.GetSidName() == this->departure;
        }
    }  // namespace Prenote
}  // namespace UKControllerPlugin
