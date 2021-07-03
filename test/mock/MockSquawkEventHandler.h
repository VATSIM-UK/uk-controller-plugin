#pragma once
#include "squawk/SquawkEventHandler.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"
#include "squawk/SquawkGenerator.h"
#include "controller/ActiveCallsignCollection.h"
#include "squawk/SquawkAssignment.h"
#include "flightplan/StoredFlightplanCollection.h"
#include "login/Login.h"
#include "timedevent/DeferredEventHandler.h"

namespace UKControllerPluginTest {
    namespace Squawk {

        class MockSquawkEventHandler : public UKControllerPlugin::Squawk::SquawkEventHandler
        {
            public:
                MockSquawkEventHandler() : SquawkEventHandler(
                    *squawkGen,
                    *callsigns,
                    *plans,
                    *pluginLoopback,
                    *login,
                    false
                ) {
                }
                MOCK_METHOD2(
                    FlightPlanEvent,
                    void(
                        UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightPlan,
                        UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface & target)
                    );
                MOCK_METHOD1(
                    FlightPlanDisconnectEvent,
                    void(UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightPlan)
                );
                MOCK_METHOD1(
                    ControllerFlightPlanDataEvent,
                    void(UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightPlan)
                );

            private:

                std::unique_ptr<UKControllerPlugin::Controller::ActiveCallsignCollection> callsigns;
                std::unique_ptr<UKControllerPlugin::Squawk::SquawkGenerator> squawkGen;
                std::unique_ptr<UKControllerPlugin::Squawk::SquawkAssignment> squawkAssignment;
                std::unique_ptr<UKControllerPlugin::Flightplan::StoredFlightplanCollection> plans;
                std::unique_ptr<UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface> pluginLoopback;
                std::unique_ptr<UKControllerPlugin::Controller::Login> login;
        };
    }  // namespace Squawk
}  // namespace UKControllerPluginTest
