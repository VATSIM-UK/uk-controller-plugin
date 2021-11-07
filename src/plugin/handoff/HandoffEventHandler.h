#pragma once
#include "ResolvedHandoff.h"
#include "controller/ActiveCallsignEventHandlerInterface.h"
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
    class HandoffCollection;
    class HandoffOrder;

    /*
        Handles handoff events
    */
    class HandoffEventHandler : public Tag::TagItemInterface,
                                public Flightplan::FlightPlanEventHandlerInterface,
                                public Controller::ActiveCallsignEventHandlerInterface
    {
        public:
        HandoffEventHandler(
            const HandoffCollection& handoffs,
            const Sid::SidCollection& sids,
            const Controller::ActiveCallsignCollection& callsigns,
            Integration::OutboundIntegrationEventHandler& outboundEvent);
        void AddCachedItem(const std::string& callsign, ResolvedHandoff handoff);
        [[nodiscard]] auto GetCachedItem(const std::string& callsign) const -> ResolvedHandoff;

        // Inherited via TagItemInterface
        [[nodiscard]] auto GetTagItemDescription(int tagItemId) const -> std::string override;
        void SetTagItemData(Tag::TagData& tagData) override;

        // Inherited via FlightPlanEventHandlerInterface
        void FlightPlanEvent(
            Euroscope::EuroScopeCFlightPlanInterface& flightPlan,
            Euroscope::EuroScopeCRadarTargetInterface& radarTarget) override;
        void FlightPlanDisconnectEvent(Euroscope::EuroScopeCFlightPlanInterface& flightPlan) override;
        void ControllerFlightPlanDataEvent(Euroscope::EuroScopeCFlightPlanInterface& flightPlan, int dataType) override;

        // Inherited via ActiveCallsignEventHandlerInterface
        void ActiveCallsignAdded(const Controller::ActiveCallsign& callsign) override;
        void ActiveCallsignRemoved(const Controller::ActiveCallsign& callsign) override;
        void CallsignsFlushed() override;

        private:
        // The default values to return
        const ResolvedHandoff DEFAULT_TAG_VALUE = ResolvedHandoff("---.---", "");
        const ResolvedHandoff UNICOM_TAG_VALUE = ResolvedHandoff("122.800", "");

        void FireHandoffUpdatedEvent(const std::string& callsign);
        auto MapSidToHandoffOrder(const Euroscope::EuroScopeCFlightPlanInterface& flightplan) const
            -> std::shared_ptr<HandoffOrder>;

        // The handoffs
        const HandoffCollection& handoffs;

        // All the SIDS that we have
        const Sid::SidCollection& sids;

        // The active callsigns
        const Controller::ActiveCallsignCollection& callsigns;

        // Maps callsign -> result so we can cache it.
        std::map<std::string, ResolvedHandoff> cache;

        // Allows us to push events to integrations
        Integration::OutboundIntegrationEventHandler& outboundEvent;
    };
} // namespace UKControllerPlugin::Handoff
