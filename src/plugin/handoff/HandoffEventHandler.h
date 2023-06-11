#pragma once
#include "flightplan/FlightPlanEventHandlerInterface.h"
#include "integration/OutboundIntegrationEventHandler.h"
#include "tag/TagItemInterface.h"

namespace UKControllerPlugin {
    namespace Controller {
        class ActiveCallsign;
        class ActiveCallsignCollection;
    } // namespace Controller
    namespace Sid {
        class SidCollection;
    } // namespace Sid
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Handoff {
    class DepartureHandoffResolver;
    class HandoffCache;
    struct ResolvedHandoff;

    /*
        Handles handoff events
    */
    class HandoffEventHandler : public Tag::TagItemInterface, public Flightplan::FlightPlanEventHandlerInterface
    {
        public:
        HandoffEventHandler(
            std::shared_ptr<DepartureHandoffResolver> resolver,
            std::shared_ptr<HandoffCache> cache,
            Integration::OutboundIntegrationEventHandler& outboundEvent);
        [[nodiscard]] auto GetCache() const -> std::shared_ptr<HandoffCache>;

        // Inherited via TagItemInterface
        [[nodiscard]] auto GetTagItemDescription(int tagItemId) const -> std::string override;
        void SetTagItemData(Tag::TagData& tagData) override;

        // Inherited via FlightPlanEventHandlerInterface
        void FlightPlanEvent(
            Euroscope::EuroScopeCFlightPlanInterface& flightPlan,
            Euroscope::EuroScopeCRadarTargetInterface& radarTarget) override;
        void FlightPlanDisconnectEvent(Euroscope::EuroScopeCFlightPlanInterface& flightPlan) override;
        void ControllerFlightPlanDataEvent(Euroscope::EuroScopeCFlightPlanInterface& flightPlan, int dataType) override;

        private:
        void FireHandoffUpdatedEvent(const std::string& callsign);
        [[nodiscard]] static auto FormatFrequency(const std::shared_ptr<ResolvedHandoff>& handoff) -> std::string;
        [[nodiscard]] auto ResolveHandoffAndCache(const Euroscope::EuroScopeCFlightPlanInterface& flightplan) const
            -> std::shared_ptr<ResolvedHandoff>;

        // Resolves handoffs
        const std::shared_ptr<DepartureHandoffResolver> resolver;

        // Caches resolved handoffs
        const std::shared_ptr<HandoffCache> cache;

        // Allows us to push events to integrations
        Integration::OutboundIntegrationEventHandler& outboundEvent;

        // How big to make the buffer
        inline static const int FREQUENCY_BUFFER_LENGTH = 8;
    };
} // namespace UKControllerPlugin::Handoff
