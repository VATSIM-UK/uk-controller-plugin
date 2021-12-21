#include "FlightplanSidHandoffMapper.h"
#include "HandoffCollection.h"
#include "sid/SidCollection.h"
#include "sid/StandardInstrumentDeparture.h"

namespace UKControllerPlugin::Handoff {

    FlightplanSidHandoffMapper::FlightplanSidHandoffMapper(
        const HandoffCollection& handoffs, const Sid::SidCollection& sids)
        : handoffs(handoffs), sids(sids)
    {
    }

    auto FlightplanSidHandoffMapper::MapForFlightplan(const Euroscope::EuroScopeCFlightPlanInterface& flightplan) const
        -> std::shared_ptr<HandoffOrder>
    {
        const auto sid = this->sids.GetForFlightplan(flightplan);

        if (!sid || !sid->HasHandoff()) {
            return nullptr;
        }

        return this->handoffs.Get(sid->HandoffId());
    }
} // namespace UKControllerPlugin::Handoff
