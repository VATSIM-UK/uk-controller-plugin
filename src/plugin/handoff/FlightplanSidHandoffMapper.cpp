#include "FlightplanSidHandoffMapper.h"
#include "HandoffCollection.h"
#include "sid/SidCollection.h"
#include "sid/StandardInstrumentDeparture.h"

namespace UKControllerPlugin::Handoff {

    FlightplanSidHandoffMapper::FlightplanSidHandoffMapper(
        std::shared_ptr<const HandoffCollection> handoffs, std::shared_ptr<const Sid::SidCollection> sids)
        : handoffs(std::move(handoffs)), sids(std::move(sids))
    {
    }

    auto FlightplanSidHandoffMapper::MapForFlightplan(Euroscope::EuroScopeCFlightPlanInterface& flightplan) const
        -> std::shared_ptr<HandoffOrder>
    {
        const auto sid = this->sids->GetForFlightplan(flightplan);

        if (!sid || !sid->HasHandoff()) {
            return nullptr;
        }

        return this->handoffs->Get(sid->HandoffId());
    }
} // namespace UKControllerPlugin::Handoff
