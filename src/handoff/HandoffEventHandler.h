#pragma once
#include "tag/TagItemInterface.h"
#include "flightplan/FlightPlanEventHandlerInterface.h"
#include "controller/ActiveCallsignCollection.h"
#include "handoff/HandoffCollection.h"
#include "handoff/CachedHandoff.h"
#include "controller/ActiveCallsignEventHandlerInterface.h"
#include "controller/ActiveCallsign.h"

namespace UKControllerPlugin {
    namespace Handoff {

        /*
            Handles handoff events
        */
        class HandoffEventHandler
            : public UKControllerPlugin::Tag::TagItemInterface,
            public UKControllerPlugin::Flightplan::FlightPlanEventHandlerInterface,
            public UKControllerPlugin::Controller::ActiveCallsignEventHandlerInterface
        {
            public:

                HandoffEventHandler(
                    const UKControllerPlugin::Handoff::HandoffCollection& handoffs,
                    const UKControllerPlugin::Controller::ActiveCallsignCollection& callsigns
                );
                void AddCachedItem(std::string callsign, CachedHandoff handoff);
                size_t CountCachedItems(void) const;
                CachedHandoff GetCachedItem(std::string callsign) const;

                // Inherited via TagItemInterface
                std::string GetTagItemDescription(void) const override;
                std::string GetTagItemData(
                    UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightPlan,
                    UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& radarTarget
                ) override;

                // Inherited via FlightPlanEventHandlerInterface
                void FlightPlanEvent(
                    UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightPlan,
                    UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& radarTarget
                ) override;
                void FlightPlanDisconnectEvent(
                    UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightPlan
                ) override;
                void ControllerFlightPlanDataEvent(
                    UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightPlan,
                    int dataType
                ) override;

                // Inherited via ActiveCallsignEventHandlerInterface
                void ActiveCallsignAdded(
                    const UKControllerPlugin::Controller::ActiveCallsign& callsign,
                    bool userCallsign
                ) override;
                void ActiveCallsignRemoved(
                    const UKControllerPlugin::Controller::ActiveCallsign& callsign,
                    bool userCallsign
                ) override;
                void CallsignsFlushed(void) override;

                // The default values to return
                const CachedHandoff DEFAULT_TAG_VALUE = CachedHandoff("---.---", "");
                const CachedHandoff UNICOM_TAG_VALUE = CachedHandoff("122.800", "");

            private:

                // The handoffs
                const UKControllerPlugin::Handoff::HandoffCollection& handoffs;

                // The active callsigns
                const UKControllerPlugin::Controller::ActiveCallsignCollection& callsigns;

                // Maps callsign -> result so we can cache it.
                std::map<std::string, UKControllerPlugin::Handoff::CachedHandoff> cache;
        };

    }  // namespace Handoff
}  // namespace UKControllerPlugin
