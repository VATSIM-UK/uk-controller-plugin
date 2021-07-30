#include "pch/pch.h"
#include "prenote/AirfieldPairingPrenote.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "controller/ControllerPositionHierarchy.h"

using UKControllerPlugin::Controller::ControllerPositionHierarchy;
using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;

namespace UKControllerPlugin {
    namespace Prenote {

        AirfieldPairingPrenote::AirfieldPairingPrenote(
            std::unique_ptr<ControllerPositionHierarchy> controllers,
            std::string origin,
            std::string destination,
            std::string flightRules
        )
            : AbstractPrenote(std::move(controllers)), origin(origin), destination(destination),
                flightRules(flightRules)
        {

        }

        std::string AirfieldPairingPrenote::GetSummaryString(void) const
        {
            return this->origin + " -> " + this->destination;
        }

        std::string AirfieldPairingPrenote::GetFlightRules() const
        {
            return this->flightRules;
        }

        bool AirfieldPairingPrenote::IsApplicable(const EuroScopeCFlightPlanInterface & flightplan) const
        {
            return flightplan.GetOrigin() == this->origin &&
                flightplan.GetDestination() == this->destination &&
                (this->flightRules == this->NO_FLIGHT_RULES || flightplan.GetFlightRules() == this->flightRules);
        }
    }  // namespace Prenote
}  // namespace UKControllerPlugin
