#include "DepartureHandoffResolver.h"
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
        std::shared_ptr<FlightplanSidHandoffMapper> mapper, const Controller::ActiveCallsignCollection& activeCallsigns)
        : mapper(std::move(mapper)), activeCallsigns(activeCallsigns)
    {
    }

    auto DepartureHandoffResolver::Resolve(const Euroscope::EuroScopeCFlightPlanInterface& flightplan) const
        -> std::shared_ptr<ResolvedHandoff>
    {
        const auto handoff = this->mapper->MapForFlightplan(flightplan);
        if (!handoff) {
            return ResolveToUnicom(flightplan);
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

        return ResolveToUnicom(flightplan);
    }

    auto DepartureHandoffResolver::ResolveToUnicom(const Euroscope::EuroScopeCFlightPlanInterface& flightplan)
        -> std::shared_ptr<ResolvedHandoff>
    {
        return std::make_shared<ResolvedHandoff>(
            flightplan.GetCallsign(), UNICOM_FREQUENCY, std::make_shared<ControllerPositionHierarchy>());
    }
} // namespace UKControllerPlugin::Handoff
