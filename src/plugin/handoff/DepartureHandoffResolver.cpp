#include "DepartureHandoffResolver.h"
#include "FlightplanAirfieldHandoffMapper.h"
#include "FlightplanSidHandoffMapper.h"
#include "HandoffOrder.h"
#include "ResolvedHandoff.h"
#include "controller/ActiveCallsignCollection.h"
#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionHierarchy.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"

using UKControllerPlugin::Controller::ControllerPositionHierarchy;

namespace UKControllerPlugin::Handoff {

    DepartureHandoffResolver::DepartureHandoffResolver(
        const FlightplanSidHandoffMapper& sidMapper,
        const FlightplanAirfieldHandoffMapper& airfieldMapper,
        const Controller::ActiveCallsignCollection& activeCallsigns)
        : sidMapper(sidMapper), airfieldMapper(airfieldMapper), activeCallsigns(activeCallsigns)
    {
    }

    auto DepartureHandoffResolver::Resolve(const Euroscope::EuroScopeCFlightPlanInterface& flightplan) const
        -> std::shared_ptr<ResolvedHandoff>
    {
        const auto handoff = this->ResolveHandoff(flightplan);
        if (!handoff) {
            return ResolveToUnicom(flightplan, std::make_shared<ControllerPositionHierarchy>());
        }

        for (const auto& controller : *handoff->order) {
            if (!this->activeCallsigns.PositionActive(controller->GetCallsign())) {
                continue;
            }

            // If the controller is the user, no handoff frequency here!
            if (this->activeCallsigns.GetLeadCallsignForPosition(controller->GetCallsign()).GetIsUser()) {
                break;
            }

            return std::make_shared<ResolvedHandoff>(
                flightplan.GetCallsign(), controller->GetFrequency(), handoff->order);
        }

        return ResolveToUnicom(flightplan, handoff->order);
    }

    auto DepartureHandoffResolver::ResolveToUnicom(
        const Euroscope::EuroScopeCFlightPlanInterface& flightplan,
        std::shared_ptr<Controller::ControllerPositionHierarchy> handoffOrder) -> std::shared_ptr<ResolvedHandoff>
    {
        return std::make_shared<ResolvedHandoff>(flightplan.GetCallsign(), UNICOM_FREQUENCY, handoffOrder);
    }

    auto DepartureHandoffResolver::ResolveHandoff(const Euroscope::EuroScopeCFlightPlanInterface& flightplan) const
        -> std::shared_ptr<HandoffOrder>
    {
        const auto sidHandoff = this->sidMapper.MapForFlightplan(flightplan);
        return sidHandoff != nullptr ? sidHandoff : this->airfieldMapper.MapForFlightplan(flightplan);
    }
} // namespace UKControllerPlugin::Handoff
