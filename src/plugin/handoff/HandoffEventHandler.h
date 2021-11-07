#pragma once
#include "CachedHandoff.h"
#include "HandoffCollection.h"
#include "controller/ActiveCallsign.h"
#include "controller/ActiveCallsignCollection.h"
#include "controller/ActiveCallsignEventHandlerInterface.h"
#include "flightplan/FlightPlanEventHandlerInterface.h"
#include "integration/OutboundIntegrationEventHandler.h"
#include "tag/TagData.h"
#include "tag/TagItemInterface.h"

namespace UKControllerPlugin::Handoff {

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
            const Controller::ActiveCallsignCollection& callsigns,
            Integration::OutboundIntegrationEventHandler& outboundEvent);
        void AddCachedItem(const std::string& callsign, CachedHandoff handoff);
        [[nodiscard]] auto GetCachedItem(const std::string& callsign) const -> CachedHandoff;

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
        const CachedHandoff DEFAULT_TAG_VALUE = CachedHandoff("---.---", "");
        const CachedHandoff UNICOM_TAG_VALUE = CachedHandoff("122.800", "");

        void FireHandoffUpdatedEvent(const std::string& callsign);

        // The handoffs
        const HandoffCollection& handoffs;

        // The active callsigns
        const Controller::ActiveCallsignCollection& callsigns;

        // Maps callsign -> result so we can cache it.
        std::map<std::string, CachedHandoff> cache;

        // Allows us to push events to integrations
        Integration::OutboundIntegrationEventHandler& outboundEvent;
    };
} // namespace UKControllerPlugin::Handoff
