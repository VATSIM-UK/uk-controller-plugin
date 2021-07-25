#pragma once
#include "tag/TagItemInterface.h"
#include "flightplan/FlightPlanEventHandlerInterface.h"
#include "controller/ActiveCallsignCollection.h"
#include "handoff/HandoffCollection.h"
#include "handoff/CachedHandoff.h"
#include "controller/ActiveCallsignEventHandlerInterface.h"
#include "controller/ActiveCallsign.h"
#include "tag/TagData.h"
#include "integration/OutboundIntegrationEventHandler.h"

namespace UKControllerPlugin {
    namespace Handoff {

        /*
            Handles handoff events
        */
        class HandoffEventHandler
            : public Tag::TagItemInterface,
              public Flightplan::FlightPlanEventHandlerInterface,
              public Controller::ActiveCallsignEventHandlerInterface
        {
            public:

                HandoffEventHandler(
                    const HandoffCollection& handoffs,
                    const Controller::ActiveCallsignCollection& callsigns,
                    Integration::OutboundIntegrationEventHandler& outboundEvent
                );
                void AddCachedItem(std::string callsign, CachedHandoff handoff);
                size_t CountCachedItems(void) const;
                CachedHandoff GetCachedItem(std::string callsign) const;

                // Inherited via TagItemInterface
                std::string GetTagItemDescription(int tagItemId) const override;
                void SetTagItemData(Tag::TagData& tagData) override;

                // Inherited via FlightPlanEventHandlerInterface
                void FlightPlanEvent(
                    Euroscope::EuroScopeCFlightPlanInterface& flightPlan,
                    Euroscope::EuroScopeCRadarTargetInterface& radarTarget
                ) override;
                void FlightPlanDisconnectEvent(
                    Euroscope::EuroScopeCFlightPlanInterface& flightPlan
                ) override;
                void ControllerFlightPlanDataEvent(
                    Euroscope::EuroScopeCFlightPlanInterface& flightPlan,
                    int dataType
                ) override;

                // Inherited via ActiveCallsignEventHandlerInterface
                void ActiveCallsignAdded(
                    const Controller::ActiveCallsign& callsign,
                    bool userCallsign
                ) override;
                void ActiveCallsignRemoved(
                    const Controller::ActiveCallsign& callsign,
                    bool userCallsign
                ) override;
                void CallsignsFlushed(void) override;

                // The default values to return
                const CachedHandoff DEFAULT_TAG_VALUE = CachedHandoff("---.---", "");
                const CachedHandoff UNICOM_TAG_VALUE = CachedHandoff("122.800", "");

            private:
                void FireHandoffUpdatedEvent(std::string callsign);

                // The handoffs
                const HandoffCollection& handoffs;

                // The active callsigns
                const Controller::ActiveCallsignCollection& callsigns;

                // Maps callsign -> result so we can cache it.
                std::map<std::string, CachedHandoff> cache;

                // Allows us to push events to integrations
                Integration::OutboundIntegrationEventHandler& outboundEvent;
        };
    } // namespace Handoff
}  // namespace UKControllerPlugin
