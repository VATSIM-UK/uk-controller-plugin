#include "DefaultDepartureHandoffResolutionStrategy.h"
#include "FlightplanAirfieldHandoffMapper.h"
#include "FlightplanSidHandoffMapper.h"
#include "HandoffOrder.h"
#include "ResolvedHandoff.h"
#include "controller/ActiveCallsign.h"
#include "controller/ActiveCallsignCollection.h"
#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionHierarchy.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"

using UKControllerPlugin::Controller::ControllerPositionHierarchy;

namespace UKControllerPlugin::Handoff {

    DefaultDepartureHandoffResolutionStrategy::DefaultDepartureHandoffResolutionStrategy(
        const FlightplanSidHandoffMapper& sidMapper,
        const FlightplanAirfieldHandoffMapper& airfieldMapper,
        const Controller::ActiveCallsignCollection& activeCallsigns)
        : sidMapper(sidMapper), airfieldMapper(airfieldMapper), activeCallsigns(activeCallsigns),
          unicomController(std::make_shared<Controller::ControllerPosition>(
              -1, "UNICOM", 122.800, std::vector<std::string>{}, false, false))
    {
    }

    /**
     * Try and map this flightplan to a given handoff order, even if that means Unicom. If there's absolutely
     * nothing to map to, resolve to unicom anyway... We'll need a flightplan change to be able to attempt
     * it again.
     */
    auto
    DefaultDepartureHandoffResolutionStrategy::Resolve(const Euroscope::EuroScopeCFlightPlanInterface& flightplan) const
        -> std::shared_ptr<ResolvedHandoff>
    {
        std::shared_ptr<Controller::ControllerPosition> controller = nullptr;

        /*
         * Resolve the handoff for the SID. If we find one and a controller on that handoff
         * is active, that's the handoff controller.
         */
        const auto sidHandoff = this->sidMapper.MapForFlightplan(flightplan);
        if (sidHandoff != nullptr) {
            const auto resolvedSidController = this->ResolveController(*sidHandoff);
            if (resolvedSidController) {
                controller = resolvedSidController;
            }
        }

        /*
         * Resolve the handoff for the airfield as a fallback.
         *
         * If there isn't already a controller the SID, try to find one for the airport.
         */
        const auto airfieldHandoff = this->airfieldMapper.MapForFlightplan(flightplan);
        if (controller == nullptr && airfieldHandoff != nullptr) {
            const auto resolvedAirfieldController = this->ResolveController(*airfieldHandoff);
            if (resolvedAirfieldController) {
                controller = resolvedAirfieldController;
            }
        }

        return std::make_shared<ResolvedHandoff>(
            flightplan.GetCallsign(),
            controller != nullptr ? controller : this->unicomController,
            sidHandoff != nullptr ? sidHandoff->order : std::make_shared<ControllerPositionHierarchy>(),
            airfieldHandoff != nullptr ? airfieldHandoff->order : std::make_shared<ControllerPositionHierarchy>());
    }

    /**
     * For a given handoff, resolve it to a particular controller position.
     */
    auto DefaultDepartureHandoffResolutionStrategy::ResolveController(const HandoffOrder& handoff) const
        -> std::shared_ptr<Controller::ControllerPosition>
    {
        for (auto& controller : *handoff.order) {
            if (!this->activeCallsigns.PositionActive(controller->GetCallsign())) {
                continue;
            }

            // If the controller is us, then it's unicom.
            if (this->activeCallsigns.GetLeadCallsignForPosition(controller->GetCallsign()).GetIsUser()) {
                return unicomController;
            }

            return controller;
        }

        return nullptr;
    }
} // namespace UKControllerPlugin::Handoff
