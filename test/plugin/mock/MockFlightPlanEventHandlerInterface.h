#pragma once
#include "flightplan/FlightPlanEventHandlerInterface.h"

namespace UKControllerPluginTest {
    namespace Euroscope {
        class EuroScopeCRadarTargetInterface;
        class EuroScopeCFlightPlanInterface;
    }  // namespace Euroscope
}  // namespace UKControllerPluginTest

namespace UKControllerPluginTest {
    namespace Flightplan {

        class MockFlightPlanEventHandlerInterface :
            public UKControllerPlugin::Flightplan::FlightPlanEventHandlerInterface
        {
            public:
                MOCK_METHOD2(
                    FlightPlanEvent,
                    void(
                        UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightPlan,
                        UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface & target
                    )
                );
                MOCK_METHOD1(
                    FlightPlanDisconnectEvent,
                    void(UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightPlan)
                );
                MOCK_METHOD2(
                    ControllerFlightPlanDataEvent,
                    void(UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightPlan, int dataType)
                );
        };
    }  // namespace Flightplan
}  // namespace UKControllerPluginTest
