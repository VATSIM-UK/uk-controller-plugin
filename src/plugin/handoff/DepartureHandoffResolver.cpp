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

    /**
     * Try and map this flightplan to a given handoff order, even if that means Unicom. If there's absolutely
     * nothing to map to, resolve to unicom anyway... We'll need a flightplan change to be able to attempt
     * it again.
     */
    auto DepartureHandoffResolver::Resolve(const Euroscope::EuroScopeCFlightPlanInterface& flightplan) const
        -> std::shared_ptr<ResolvedHandoff>
    {
        const auto resolved = this->ResolveHandoff(flightplan);
        return resolved != nullptr ? resolved
                                   : ResolveToUnicom(flightplan, std::make_shared<ControllerPositionHierarchy>());
    }

    auto DepartureHandoffResolver::ResolveToUnicom(
        const Euroscope::EuroScopeCFlightPlanInterface& flightplan,
        std::shared_ptr<Controller::ControllerPositionHierarchy> handoffOrder) -> std::shared_ptr<ResolvedHandoff>
    {
        return std::make_shared<ResolvedHandoff>(flightplan.GetCallsign(), UNICOM_FREQUENCY, handoffOrder);
    }

    /**
     * Work through the flightplan and use the resolvers in orde to resolve the handoff.
     */
    auto DepartureHandoffResolver::ResolveHandoff(const Euroscope::EuroScopeCFlightPlanInterface& flightplan) const
        -> std::shared_ptr<ResolvedHandoff>
    {
        std::shared_ptr<ResolvedHandoff> resolved = nullptr;

        // First, try the SID, as these orders are most specific.
        const auto sidHandoff = this->sidMapper.MapForFlightplan(flightplan);
        if (sidHandoff != nullptr) {
            resolved = this->ResolveForHandoff(flightplan, *sidHandoff);
        }

        // If we've resolved to something that's not unicom, stop here.
        if (resolved && !ResolvedToUnicom(*resolved)) {
            return resolved;
        }

        // As a last attempt, try to resolve to the default airfield handoff order.
        const auto airfieldHandoff = this->airfieldMapper.MapForFlightplan(flightplan);
        if (airfieldHandoff != nullptr) {
            resolved = this->ResolveForHandoff(flightplan, *airfieldHandoff);
        }

        return resolved;
    }

    /**
     * Given a particular handoff, resolve it to a controller and thus make it a resolved
     * handoff. The handoff frequency may be unicom if the controller is providing top-down,
     * but should still use the resolved handoffs order for it incase another controller
     * comes along.
     */
    auto DepartureHandoffResolver::ResolveForHandoff(
        const Euroscope::EuroScopeCFlightPlanInterface& flightplan, const HandoffOrder& handoff) const
        -> std::shared_ptr<ResolvedHandoff>
    {
        const auto controller = this->ResolveController(handoff);
        if (!controller) {
            return ResolveToUnicom(flightplan, handoff.order);
        }

        return this->activeCallsigns.GetLeadCallsignForPosition(controller->GetCallsign()).GetIsUser()
                   ? ResolveToUnicom(flightplan, handoff.order)
                   : std::make_shared<ResolvedHandoff>(
                         flightplan.GetCallsign(), controller->GetFrequency(), handoff.order);
    }

    /**
     * For a given handoff, resolve it to a particular controller position.
     */
    auto DepartureHandoffResolver::ResolveController(const HandoffOrder& handoff) const
        -> std::shared_ptr<const Controller::ControllerPosition>
    {
        for (const auto& controller : *handoff.order) {
            if (!this->activeCallsigns.PositionActive(controller->GetCallsign())) {
                continue;
            }

            return controller;
        }

        return nullptr;
    }

    auto DepartureHandoffResolver::ResolvedToUnicom(const ResolvedHandoff& resolved) -> bool
    {
        return resolved.frequency == UNICOM_FREQUENCY;
    }
} // namespace UKControllerPlugin::Handoff
