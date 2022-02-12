#pragma once

namespace UKControllerPlugin::Euroscope {
    class EuroScopeCFlightPlanInterface;
} // namespace UKControllerPlugin::Euroscope

namespace UKControllerPlugin::Sid {
    class StandardInstrumentDeparture;

    /**
     * An interface for a class that maps flightplans to their respective SIDs.
     *
     * This saves us having to build up the relatively complicated mapper class
     * in all our tests.
     */
    class SidMapperInterface
    {
        public:
        virtual ~SidMapperInterface() = default;
        [[nodiscard]] virtual auto MapFlightplanToSid(const Euroscope::EuroScopeCFlightPlanInterface& flightplan) const
            -> std::shared_ptr<StandardInstrumentDeparture> = 0;
    };
} // namespace UKControllerPlugin::Sid
