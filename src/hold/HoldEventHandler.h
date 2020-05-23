#pragma once
#include "flightplan/FlightPlanEventHandlerInterface.h"
#include "radarscreen/ConfigurableDisplayInterface.h"
#include "command/CommandHandlerInterface.h"
#include "tag/TagItemInterface.h"
#include "timedevent/AbstractTimedEvent.h"
#include "navaids/NavaidCollection.h"

namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroscopePluginLoopbackInterface;
    }  // namespace Euroscope
    namespace Hold {
        class HoldManager;
    }  // namespace Hold
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Hold {

        /*
            Handles events that relate to holding - namely the periodic
            update of holding data.
        */
        class HoldEventHandler : public UKControllerPlugin::Flightplan::FlightPlanEventHandlerInterface,
            public UKControllerPlugin::Tag::TagItemInterface,
            public UKControllerPlugin::TimedEvent::AbstractTimedEvent
        {
            public:
                // Inherited via FlightPlanEventHandlerInterface
            HoldEventHandler(
                    UKControllerPlugin::Hold::HoldManager& holdManager,
                    const UKControllerPlugin::Navaids::NavaidCollection& navaids,
                    UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface & plugin,
                    const int popupMenuItemId
                );
                void FlightPlanEvent(
                    UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightPlan,
                    UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface & radarTarget
                ) override;
                void FlightPlanDisconnectEvent(
                    UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightPlan
                ) override;
                void ControllerFlightPlanDataEvent(
                    UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightPlan,
                    int dataType
                ) override;

                // Inherited via TagItemInterface
                std::string GetTagItemDescription(void) const override;
                std::string GetTagItemData(
                    UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightPlan,
                    UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface & radarTarget
                ) override;


                // The string to display when an aircraft is not holding
                const std::string noHold = "NOHOLD";

                // The id of this handlers popup menu item
                const int popupMenuItemId;

                // How far from a navaid an aircraft can be before its considered in proximity of the hold
                const double proximityDistance = 12.0;

            private:
                
                // Navaids against which holds are based
                const UKControllerPlugin::Navaids::NavaidCollection& navaids;

                // Gives access to the plugin
                UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface & plugin;

                // Manages holds
                UKControllerPlugin::Hold::HoldManager & holdManager;

                // Inherited via AbstractTimedEvent
                virtual void TimedEventTrigger(void) override;
        };
    }  // namespace Hold
}  // namespace UKControllerPlugin
