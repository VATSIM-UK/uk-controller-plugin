#include "pch/pch.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "mock/MockFlightPlanEventHandlerInterface.h"
#include "mock/MockEuroScopeCFlightplanInterface.h"
#include "mock/MockEuroScopeCRadarTargetInterface.h"

using UKControllerPlugin::Flightplan::FlightPlanEventHandlerCollection;
using UKControllerPluginTest::Flightplan::MockFlightPlanEventHandlerInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCRadarTargetInterface;

using ::testing::_;
using ::testing::StrictMock;

namespace UKControllerPluginTest {
    namespace EventHandler {

        TEST(FlightPlanEventHandlerCollection, TestFlightplanEventCallsTheCorrectHandlerMethod)
        {
            FlightPlanEventHandlerCollection collection;
            StrictMock<MockEuroScopeCFlightPlanInterface> mockFlightPlan;
            StrictMock<MockEuroScopeCRadarTargetInterface> mockRadarTarget;
            std::shared_ptr<StrictMock<MockFlightPlanEventHandlerInterface>> mockInterface(
                new StrictMock<MockFlightPlanEventHandlerInterface>
            );

            EXPECT_CALL(*mockInterface, FlightPlanEvent(_, _))
                .Times(1);

            collection.RegisterHandler(mockInterface);
            collection.FlightPlanEvent(mockFlightPlan, mockRadarTarget);
        }

        TEST(FlightPlanEventHandlerCollection, TestControllerFlightplanDataEventCallsTheCorrectHandlerMethod)
        {
            FlightPlanEventHandlerCollection collection;
            StrictMock<MockEuroScopeCFlightPlanInterface> mockFlightPlan;
            StrictMock<MockEuroScopeCRadarTargetInterface> mockRadarTarget;
            std::shared_ptr<StrictMock<MockFlightPlanEventHandlerInterface>> mockInterface(
                new StrictMock<MockFlightPlanEventHandlerInterface>
            );

            EXPECT_CALL(*mockInterface, ControllerFlightPlanDataEvent(_, EuroScopePlugIn::CTR_DATA_TYPE_SQUAWK))
                .Times(1);

            collection.RegisterHandler(mockInterface);
            collection.ControllerFlightPlanDataEvent(
                mockFlightPlan, mockRadarTarget,
                EuroScopePlugIn::CTR_DATA_TYPE_SQUAWK
            );
        }

        TEST(FlightPlanEventHandlerCollection, TestFlightplanDisconnectEventCallsTheCorrectHandlerMethod)
        {
            FlightPlanEventHandlerCollection collection;
            StrictMock<MockEuroScopeCFlightPlanInterface> mockFlightPlan;
            std::shared_ptr<StrictMock<MockFlightPlanEventHandlerInterface>> mockInterface(
                new StrictMock<MockFlightPlanEventHandlerInterface>
            );
            StrictMock<MockEuroScopeCRadarTargetInterface> mockRadarTarget;

            EXPECT_CALL(*mockInterface, FlightPlanDisconnectEvent(_))
                .Times(1);

            collection.RegisterHandler(mockInterface);
            collection.FlightPlanDisconnectEvent(mockFlightPlan);
        }

        TEST(FlightPlanEventHandlerCollection, CountHandlersReturnsTheNumberOfHandlers)
        {
            FlightPlanEventHandlerCollection collection;
            collection.RegisterHandler(std::shared_ptr<StrictMock<MockFlightPlanEventHandlerInterface>>(
                new StrictMock<MockFlightPlanEventHandlerInterface>)
            );
            EXPECT_EQ(1, collection.CountHandlers());
            collection.RegisterHandler(std::shared_ptr<StrictMock<MockFlightPlanEventHandlerInterface>>(
                new StrictMock<MockFlightPlanEventHandlerInterface>)
            );
            EXPECT_EQ(2, collection.CountHandlers());
            collection.RegisterHandler(std::shared_ptr<StrictMock<MockFlightPlanEventHandlerInterface>>(
                new StrictMock<MockFlightPlanEventHandlerInterface>)
            );
            EXPECT_EQ(3, collection.CountHandlers());
        }

        TEST(FlightPlanEventHandlerCollection, StartsEmpty)
        {
            FlightPlanEventHandlerCollection collection;
            EXPECT_EQ(0, collection.CountHandlers());
        }

        TEST(FlightPlanEventHandlerCollection, RegisterHandlerDoesNotAddDuplicates)
        {
            FlightPlanEventHandlerCollection collection;
            std::shared_ptr<StrictMock<MockFlightPlanEventHandlerInterface>> handler(
                new StrictMock<MockFlightPlanEventHandlerInterface>
            );
            collection.RegisterHandler(handler);
            EXPECT_EQ(1, collection.CountHandlers());
            collection.RegisterHandler(handler);
            EXPECT_EQ(1, collection.CountHandlers());
            collection.RegisterHandler(handler);
            EXPECT_EQ(1, collection.CountHandlers());
        }
    }  // namespace EventHandler
}  // namespace UKControllerPluginTest
