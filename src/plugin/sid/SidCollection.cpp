#include "pch/pch.h"
#include "sid/SidCollection.h"
#include "sid/StandardInstrumentDeparture.h"

namespace UKControllerPlugin {
    namespace Sid {

        void SidCollection::AddSid(std::shared_ptr<StandardInstrumentDeparture> sid)
        {
            this->sids.insert(sid);
        }

        size_t SidCollection::CountSids() const
        {
            return this->sids.size();
        }

        const std::shared_ptr<StandardInstrumentDeparture> SidCollection::GetByAirfieldAndIdentifier(
            std::string airfield,
            std::string identifier
        ) const
        {
            auto sid = std::find_if(
                this->sids.cbegin(),
                this->sids.cend(),
                [&airfield, &identifier](const std::shared_ptr<StandardInstrumentDeparture>& sid) -> bool
                {
                    return sid->Airfield() == airfield && sid->Identifier() == identifier;
                }
            );

            return sid == this->sids.cend() ? nullptr : *sid;
        }
    }  // namespace Sid
}  // namespace UKControllerPlugin
