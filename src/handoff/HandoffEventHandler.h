#pragma once
#include "tag/TagItemInterface.h"
#include "flightplan/FlightPlanEventHandlerInterface.h"
#include "controller/ActiveCallsignCollection.h"
#include "handoff/HandoffCollection.h"

namespace UKControllerPlugin {
    namespace Handoff {

        /*
            Handles handoff events
        */
        class HandoffEventHandler 
            : public UKControllerPlugin::Tag::TagItemInterface,
            public UKControllerPlugin::Flightplan::FlightPlanEventHandlerInterface
        {
            public: 

                HandoffEventHandler(
                    const UKControllerPlugin::Handoff::HandoffCollection& handoffs,
                    const UKControllerPlugin::Controller::ActiveCallsignCollection& callsigns
                );
                void AddCachedItem(std::string callsign, std::string item);
                std::string GetCachedItem(std::string callsign) const;

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

                // The default value to return
                const std::string DEFAULT_TAG_VALUE = "-";

            private:

                // The handoffs
                const UKControllerPlugin::Handoff::HandoffCollection& handoffs;

                // The active callsigns
                const UKControllerPlugin::Controller::ActiveCallsignCollection& callsigns;
                
                // Maps callsign -> result so we can cache it.
                std::map<std::string, std::string> cache;
        };

    }  // namespace Handoff
}  // namespace UKControllerPlugin
