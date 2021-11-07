#include "DepartureHandoffResolver.h"
#include "FlightplanSidHandoffMapper.h"
#include "HandoffOrder.h"
#include "ResolvedHandoff.h"
#include "controller/ActiveCallsignCollection.h"
#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionHierarchy.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"

namespace UKControllerPlugin::Handoff {

    DepartureHandoffResolver::DepartureHandoffResolver(
        std::shared_ptr<FlightplanSidHandoffMapper> mapper, const Controller::ActiveCallsignCollection& activeCallsigns)
        : mapper(std::move(mapper)), activeCallsigns(activeCallsigns)
    {
    }

    auto DepartureHandoffResolver::Resolve(Euroscope::EuroScopeCFlightPlanInterface& flightplan) const
        -> std::shared_ptr<ResolvedHandoff>
    {
        const auto handoff = this->mapper->MapForFlightplan(flightplan);
        if (!handoff) {
            return nullptr;
        }

        for (const auto& controller : *handoff->order) {
            if (!this->activeCallsigns.PositionActive(controller.get().GetCallsign())) {
                continue;
            }

            // If the controller is the user, no handoff frequency here!
            if (this->activeCallsigns.GetLeadCallsignForPosition(controller.get().GetCallsign()).GetIsUser()) {
                break;
            }

            return std::make_shared<ResolvedHandoff>(flightplan.GetCallsign(), controller.get().GetFrequency());
        }

        return nullptr;
    }
} // namespace UKControllerPlugin::Handoff
