#include "pch/stdafx.h"
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
            std::string destination
        )
            : AbstractPrenote(std::move(controllers)), origin(origin), destination(destination)
        {

        }

        std::string AirfieldPairingPrenote::GetSummaryString(void) const
        {
            return this->origin + " -> " + this->destination;
        }

        bool AirfieldPairingPrenote::IsApplicable(const EuroScopeCFlightPlanInterface & flightplan) const
        {
            return flightplan.GetOrigin() == this->origin && flightplan.GetDestination() == this->destination;
        }
    }  // namespace Prenote
}  // namespace UKControllerPlugin
