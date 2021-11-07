#include "SidCollection.h"
#include "StandardInstrumentDeparture.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"

namespace UKControllerPlugin::Sid {
    void SidCollection::AddSid(const std::shared_ptr<StandardInstrumentDeparture>& sid)
    {
        this->sids.insert(sid);
    }

    auto SidCollection::CountSids() const -> size_t
    {
        return this->sids.size();
    }

    auto SidCollection::GetByAirfieldAndIdentifier(std::string airfield, std::string identifier) const
        -> std::shared_ptr<StandardInstrumentDeparture>
    {
        auto sid = std::find_if(
            this->sids.cbegin(),
            this->sids.cend(),
            [&airfield, &identifier](const std::shared_ptr<StandardInstrumentDeparture>& sid) -> bool {
                return sid->Airfield() == airfield && sid->Identifier() == identifier;
            });

        return sid == this->sids.cend() ? nullptr : *sid;
    }

    auto SidCollection::GetForFlightplan(const Euroscope::EuroScopeCFlightPlanInterface& flightplan) const
        -> std::shared_ptr<StandardInstrumentDeparture>
    {
        return this->GetByAirfieldAndIdentifier(flightplan.GetOrigin(), flightplan.GetSidName());
    }
} // namespace UKControllerPlugin::Sid
