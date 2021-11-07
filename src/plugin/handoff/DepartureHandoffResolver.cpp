#include "DepartureHandoffResolver.h"
#include "FlightplanSidHandoffMapper.h"
#include "controller/ActiveCallsignCollection.h"

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
    }
} // namespace UKControllerPlugin::Handoff
