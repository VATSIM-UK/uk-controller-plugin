#pragma once

namespace UKControllerPlugin {
    namespace Sid {

        class StandardInstrumentDeparture;

        /*
            Stores SIDs and allows them to be searched
        */
        class SidCollection
        {
            public:

                SidCollection();
                void AddSid(std::shared_ptr<StandardInstrumentDeparture> sid);
                size_t CountSids() const;
                const std::shared_ptr<StandardInstrumentDeparture> GetByAirfieldAndIdentifier(
                    std::string airfield,
                    std::string identifier
                ) const;

            private:
                // All the SIDs
                std::set<std::shared_ptr<StandardInstrumentDeparture>> sids;
        };
    }  // namespace Sid
}  // namespace UKControllerPlugin
