#pragma once

namespace UKControllerPlugin {
    namespace Airfield {
        class AirfieldCollection;
    } // namespace Airfield
    namespace Euroscope {
        class EuroScopeCFlightPlanInterface;
    } // namespace Euroscope
    namespace FlightRules {
        class FlightRuleCollection;
    } // namespace FlightRules
    namespace Sid {
        class SidCollection;
    } // namespace Sid
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Prenote {
    struct PublishedPrenote;
    class PublishedPrenoteCollection;
    /**
     * Given a flightplan, map to the relevant prenotes
     */
    class PublishedPrenoteMapper
    {
        public:
        PublishedPrenoteMapper(
            const PublishedPrenoteCollection& publishedPrenotes,
            const Airfield::AirfieldCollection& airfields,
            const Sid::SidCollection& sids,
            const FlightRules::FlightRuleCollection& flightRules);

        [[nodiscard]] auto MapForFlightplan(const Euroscope::EuroScopeCFlightPlanInterface& flightplan) const
            -> std::set<std::shared_ptr<PublishedPrenote>>;

        private:
        void MapAirfieldPairingPrenotes(
            const Euroscope::EuroScopeCFlightPlanInterface& flightplan,
            std::set<std::shared_ptr<PublishedPrenote>>& prenotes) const;
        void MapSidPrenotes(
            const Euroscope::EuroScopeCFlightPlanInterface& flightplan,
            std::set<std::shared_ptr<PublishedPrenote>>& prenotes) const;

        // All the published prenotes
        const PublishedPrenoteCollection& publishedPrenotes;

        // The airfields for airfield pairings
        const Airfield::AirfieldCollection& airfields;

        // The sids for specific sid prenotes
        const Sid::SidCollection& sids;

        // For mapping flight rules
        const FlightRules::FlightRuleCollection& flightRules;
    };
} // namespace UKControllerPlugin::Prenote
