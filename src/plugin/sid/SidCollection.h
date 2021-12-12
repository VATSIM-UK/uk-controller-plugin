#pragma once

namespace UKControllerPlugin::Euroscope {
    class EuroScopeCFlightPlanInterface;
} // namespace UKControllerPlugin::Euroscope

namespace UKControllerPlugin::Sid {

    class StandardInstrumentDeparture;

    /*
        Stores SIDs and allows them to be searched
    */
    class SidCollection
    {
        public:
        void AddSid(const std::shared_ptr<StandardInstrumentDeparture>& sid);
        [[nodiscard]] auto CountSids() const -> size_t;
        [[nodiscard]] auto GetByAirfieldAndIdentifier(const std::string& airfield, const std::string& identifier) const
            -> std::shared_ptr<StandardInstrumentDeparture>;
        [[nodiscard]] auto GetForFlightplan(const Euroscope::EuroScopeCFlightPlanInterface& flightplan) const
            -> std::shared_ptr<StandardInstrumentDeparture>;

        private:
        [[nodiscard]] auto NormaliseIdentifier(const std::string& identifier) const -> std::string;

        // All the SIDs
        std::set<std::shared_ptr<StandardInstrumentDeparture>> sids;
    };
} // namespace UKControllerPlugin::Sid
