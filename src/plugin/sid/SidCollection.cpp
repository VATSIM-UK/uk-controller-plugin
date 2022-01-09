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
    
    auto SidCollection::GetById(int id) const -> std::shared_ptr<StandardInstrumentDeparture>
    {
        auto sid = std::find_if(
            this->sids.cbegin(),
            this->sids.cend(),
            [&id](const std::shared_ptr<StandardInstrumentDeparture>& sid) -> bool {
                return sid->Id() == id;
            });
        
        return sid == this->sids.cend() ? nullptr : *sid;
    }

    auto SidCollection::GetByRunwayIdAndIdentifier(int runwayId, const std::string& identifier) const
        -> std::shared_ptr<StandardInstrumentDeparture>
    {
        const auto normalisedIdentifier = NormaliseIdentifier(identifier);
        auto sid = std::find_if(
            this->sids.cbegin(),
            this->sids.cend(),
            [&runwayId, &normalisedIdentifier](const std::shared_ptr<StandardInstrumentDeparture>& sid) -> bool {
                return sid->RunwayId() == runwayId && sid->Identifier() == normalisedIdentifier;
            });

        return sid == this->sids.cend() ? nullptr : *sid;
    }

    /**
     * Some SIDs in the sectorfile are marked as "deprecated" in order for them to show up still
     * when pilots with outdated airacs file them. These are denoted by a "#" at the start of
     * the identifier.
     */
    auto SidCollection::NormaliseIdentifier(const std::string& identifier) const -> std::string
    {
        return !identifier.empty() && identifier.substr(0, 1) == "#" ? identifier.substr(1) : identifier;
    }
} // namespace UKControllerPlugin::Sid
