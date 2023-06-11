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
    struct ResolvedHandoff;

    /*
        Handles handoff events
    */
    class HandoffEventHandler : public Tag::TagItemInterface, public Flightplan::FlightPlanEventHandlerInterface
    {
        public:
        HandoffEventHandler(std::shared_ptr<DepartureHandoffResolver> resolver);

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
        // Resolves handoffs
        std::shared_ptr<DepartureHandoffResolver> resolver;
    };
} // namespace UKControllerPlugin::Handoff
