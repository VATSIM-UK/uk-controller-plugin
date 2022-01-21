#include "FlightplanSidHandoffMapper.h"
#include "HandoffCollection.h"
#include "sid/SidMapperInterface.h"
#include "sid/StandardInstrumentDeparture.h"

namespace UKControllerPlugin::Handoff {

    FlightplanSidHandoffMapper::FlightplanSidHandoffMapper(
        const HandoffCollection& handoffs, const Sid::SidMapperInterface& sidMapper)
        : handoffs(handoffs), sidMapper(sidMapper)
    {
    }

    auto FlightplanSidHandoffMapper::MapForFlightplan(const Euroscope::EuroScopeCFlightPlanInterface& flightplan) const
        -> std::shared_ptr<HandoffOrder>
    {
        const auto sid = this->sidMapper.MapFlightplanToSid(flightplan);
        if (!sid || !sid->HasHandoff()) {
            return nullptr;
        }

        return this->handoffs.Get(sid->HandoffId());
    }
} // namespace UKControllerPlugin::Handoff
