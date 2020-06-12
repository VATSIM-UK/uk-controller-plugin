#include "pch/pch.h"
#include "hold/HoldManager.h"
#include "hold/HoldEventHandler.h"
#include "mock/MockEuroscopePluginLoopbackInterface.h"
#include "mock/MockEuroScopeCFlightplanInterface.h"
#include "mock/MockEuroScopeCRadarTargetInterface.h"
#include "plugin/PopupMenuItem.h"
#include "navaids/NavaidCollection.h"
#include "hold/HoldingData.h"
#include "mock/MockApiInterface.h"
#include "mock/MockTaskRunnerInterface.h"
#include "websocket/WebsocketSubscription.h"
#include "websocket/WebsocketMessage.h"

using UKControllerPlugin::Navaids::NavaidCollection;
using UKControllerPlugin::Hold::HoldingData;
using UKControllerPlugin::Hold::HoldManager;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCRadarTargetInterface;
using UKControllerPluginTest::Api::MockApiInterface;
using UKControllerPluginTest::TaskManager::MockTaskRunnerInterface;
using UKControllerPlugin::Hold::HoldEventHandler;
using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPlugin::Websocket::WebsocketSubscription;
using UKControllerPlugin::Websocket::WebsocketMessage;
using ::testing::Return;
using ::testing::NiceMock;
using ::testing::Test;
using ::testing::Throw;
using ::testing::_;

namespace UKControllerPluginTest {
    namespace Hold {

        class HoldEventHandlerTest : public Test
        {
            public:
                HoldEventHandlerTest(void)
                    : handler(this->manager,this->navaids, this->mockPlugin, 1),
                    manager(mockApi, mockTaskRunner)
                {
                    this->mockFlightplanPointer = std::make_shared<NiceMock<MockEuroScopeCFlightPlanInterface>>();
                    ON_CALL(*this->mockFlightplanPointer, GetCallsign())
                        .WillByDefault(Return("BAW123"));

                    ON_CALL(this->mockFlightplan, GetCallsign())
                        .WillByDefault(Return("BAW123"));

                    this->manager.AssignAircraftToHold(mockFlightplan, "TIMBA", false);
                }

                NiceMock<MockEuroScopeCFlightPlanInterface> mockFlightplan;
                NiceMock<MockEuroScopeCRadarTargetInterface> mockRadarTarget;
                NiceMock<MockApiInterface> mockApi;
                NiceMock<MockTaskRunnerInterface> mockTaskRunner;
                NavaidCollection navaids;
                HoldingData holdData = { 1, "TIMBA", "TIMBA", 8000, 15000, 209, "left", {} };
                NiceMock<MockEuroscopePluginLoopbackInterface> mockPlugin;
                HoldManager manager;
                HoldEventHandler handler;
                std::shared_ptr<NiceMock<MockEuroScopeCFlightPlanInterface>> mockFlightplanPointer;
        };

        TEST_F(HoldEventHandlerTest, ItHasATagItemDescription)
        {
            EXPECT_TRUE("Selected Hold" == this->handler.GetTagItemDescription());
        }

        TEST_F(HoldEventHandlerTest, ItReturnsTheSelectedHoldForAnAircraft)
        {
            EXPECT_TRUE("HTIMBA" == this->handler.GetTagItemData(this->mockFlightplan, this->mockRadarTarget));
        }

        TEST_F(HoldEventHandlerTest, ItReturnsNoHoldIfAircraftNotInHold)
        {
            this->manager.UnassignAircraftFromHold("BAW123", false);
            EXPECT_TRUE(
                this->handler.noHold == this->handler.GetTagItemData(this->mockFlightplan, this->mockRadarTarget)
            );
        }

        TEST_F(HoldEventHandlerTest, ItHasSubscriptionsToWebsocketEvents)
        {
            std::set<WebsocketSubscription> expectedSubscriptions;
            expectedSubscriptions.insert(
                {
                    WebsocketSubscription::SUB_TYPE_CHANNEL,
                    "private-hold-assignments"
                }
            );
            EXPECT_EQ(expectedSubscriptions, this->handler.GetSubscriptions());
        }

        TEST_F(HoldEventHandlerTest, ItAssignsHoldsOnEvent)
        {
            WebsocketMessage message{
                "App\\Events\\HoldAssignedEvent",
                "private-hold-assignments",
                {
                    {"callsign", "BAW123"},
                    {"navaid", "WILLO"}
                }
            };

            ON_CALL(this->mockPlugin, GetFlightplanForCallsign("BAW123"))
                .WillByDefault(Return(this->mockFlightplanPointer));

            EXPECT_CALL(this->mockApi, AssignAircraftToHold(_, _))
                .Times(0);

            this->handler.ProcessWebsocketMessage(message);
            EXPECT_EQ("WILLO", this->manager.GetHoldingAircraft("BAW123")->GetAssignedHold());
        }

        TEST_F(HoldEventHandlerTest, ItHandlesFlightplansNotBeingFoundOnAssignment)
        {
            WebsocketMessage message{
                "App\\Events\\HoldAssignedEvent",
                "private-hold-assignments",
                {
                    {"callsign", "BAW123"},
                    {"navaid", "WILLO"}
                }
            };

            ON_CALL(this->mockPlugin, GetFlightplanForCallsign("BAW123"))
                .WillByDefault(Throw(std::invalid_argument("Test")));

            EXPECT_CALL(this->mockApi, AssignAircraftToHold(_, _))
                .Times(0);

            this->handler.ProcessWebsocketMessage(message);
            EXPECT_EQ("TIMBA", this->manager.GetHoldingAircraft("BAW123")->GetAssignedHold());
        }

        TEST_F(HoldEventHandlerTest, ItDoesNotAssignIfCallsignMissing)
        {
            WebsocketMessage message{
                "App\\Events\\HoldAssignedEvent",
                "private-hold-assignments",
                {
                    {"navaid", "WILLO"}
                }
            };

            ON_CALL(this->mockPlugin, GetFlightplanForCallsign("BAW123"))
                .WillByDefault(Return(this->mockFlightplanPointer));

            EXPECT_CALL(this->mockApi, AssignAircraftToHold(_, _))
                .Times(0);

            this->handler.ProcessWebsocketMessage(message);
            EXPECT_EQ("TIMBA", this->manager.GetHoldingAircraft("BAW123")->GetAssignedHold());
        }

        TEST_F(HoldEventHandlerTest, ItDoesNotAssignIfCallsignNotString)
        {
            WebsocketMessage message{
                "App\\Events\\HoldAssignedEvent",
                "private-hold-assignments",
                {
                    {"callsign", 123},
                    {"navaid", "WILLO"}
                }
            };

            ON_CALL(this->mockPlugin, GetFlightplanForCallsign("BAW123"))
                .WillByDefault(Return(this->mockFlightplanPointer));

            EXPECT_CALL(this->mockApi, AssignAircraftToHold(_, _))
                .Times(0);

            this->handler.ProcessWebsocketMessage(message);
            EXPECT_EQ("TIMBA", this->manager.GetHoldingAircraft("BAW123")->GetAssignedHold());
        }

        TEST_F(HoldEventHandlerTest, ItDoesNotAssignIfNavaidMissing)
        {
            WebsocketMessage message{
                "App\\Events\\HoldAssignedEvent",
                "private-hold-assignments",
                {
                    {"callsign", "BAW123"},
                }
            };

            ON_CALL(this->mockPlugin, GetFlightplanForCallsign("BAW123"))
                .WillByDefault(Return(this->mockFlightplanPointer));

            EXPECT_CALL(this->mockApi, AssignAircraftToHold(_, _))
                .Times(0);

            this->handler.ProcessWebsocketMessage(message);
            EXPECT_EQ("TIMBA", this->manager.GetHoldingAircraft("BAW123")->GetAssignedHold());
        }

        TEST_F(HoldEventHandlerTest, ItDoesNotAssignIfNavaidNotString)
        {
            WebsocketMessage message{
                "App\\Events\\HoldAssignedEvent",
                "private-hold-assignments",
                {
                    {"callsign", "BAW123"},
                    {"navaid", 123}
                }
            };

            ON_CALL(this->mockPlugin, GetFlightplanForCallsign("BAW123"))
                .WillByDefault(Return(this->mockFlightplanPointer));

            EXPECT_CALL(this->mockApi, AssignAircraftToHold(_, _))
                .Times(0);

            this->handler.ProcessWebsocketMessage(message);
            EXPECT_EQ("TIMBA", this->manager.GetHoldingAircraft("BAW123")->GetAssignedHold());
        }

        TEST_F(HoldEventHandlerTest, ItDoesNotAssignIfDataNotObject)
        {
            WebsocketMessage message{
                "App\\Events\\HoldAssignedEvent",
                "private-hold-assignments",
                nlohmann::json::array({"callsign", "BAW123", "navaid", "WILLO"}),
            };

            ON_CALL(this->mockPlugin, GetFlightplanForCallsign("BAW123"))
                .WillByDefault(Return(this->mockFlightplanPointer));

            EXPECT_CALL(this->mockApi, AssignAircraftToHold(_, _))
                .Times(0);

            this->handler.ProcessWebsocketMessage(message);
            EXPECT_EQ("TIMBA", this->manager.GetHoldingAircraft("BAW123")->GetAssignedHold());
        }

        TEST_F(HoldEventHandlerTest, ItUnassignsHoldsOnEvent)
        {
            WebsocketMessage message{
                "App\\Events\\HoldUnassignedEvent",
                "private-hold-assignments",
                {
                    {"callsign", "BAW123"},
                }
            };

            ON_CALL(this->mockPlugin, GetFlightplanForCallsign("BAW123"))
                .WillByDefault(Return(this->mockFlightplanPointer));

            EXPECT_CALL(this->mockApi, UnassignAircraftHold(_))
                .Times(0);

            this->handler.ProcessWebsocketMessage(message);
            EXPECT_EQ(this->manager.invalidAircraft, this->manager.GetHoldingAircraft("BAW123"));
        }

        TEST_F(HoldEventHandlerTest, ItDoesntUnassignHoldOnMissingCallsign)
        {
            WebsocketMessage message{
                "App\\Events\\HoldUnassignedEvent",
                "private-hold-assignments",
                {
                }
            };

            ON_CALL(this->mockPlugin, GetFlightplanForCallsign("BAW123"))
                .WillByDefault(Return(this->mockFlightplanPointer));

            EXPECT_CALL(this->mockApi, UnassignAircraftHold(_))
                .Times(0);

            this->handler.ProcessWebsocketMessage(message);
            EXPECT_EQ("TIMBA", this->manager.GetHoldingAircraft("BAW123")->GetAssignedHold());
        }

        TEST_F(HoldEventHandlerTest, ItDoesntUnassignHoldOnCallsignNotString)
        {
            WebsocketMessage message{
                "App\\Events\\HoldUnassignedEvent",
                "private-hold-assignments",
                {
                    {"callsign", 123},
                }
            };

            ON_CALL(this->mockPlugin, GetFlightplanForCallsign("BAW123"))
                .WillByDefault(Return(this->mockFlightplanPointer));

            EXPECT_CALL(this->mockApi, UnassignAircraftHold(_))
                .Times(0);

            this->handler.ProcessWebsocketMessage(message);
            EXPECT_EQ("TIMBA", this->manager.GetHoldingAircraft("BAW123")->GetAssignedHold());
        }

        TEST_F(HoldEventHandlerTest, ItDoesntUnassignHoldOnDataNotObject)
        {
            WebsocketMessage message{
                "App\\Events\\HoldUnassignedEvent",
                "private-hold-assignments",
                nlohmann::json::array({"callsign", "BAW123"})
            };

            ON_CALL(this->mockPlugin, GetFlightplanForCallsign("BAW123"))
                .WillByDefault(Return(this->mockFlightplanPointer));

            EXPECT_CALL(this->mockApi, UnassignAircraftHold(_))
                .Times(0);

            this->handler.ProcessWebsocketMessage(message);
            EXPECT_EQ("TIMBA", this->manager.GetHoldingAircraft("BAW123")->GetAssignedHold());
        }

        TEST_F(HoldEventHandlerTest, ItHandlesInvalidEvent)
        {
            WebsocketMessage message{
                "App\\Events\\Uwut",
                "private-hold-assignments",
                {
                    {"callsign", "BAW123"},
                    {"navaid", "WILLO"}
                }
            };

            EXPECT_CALL(this->mockPlugin, GetFlightplanForCallsign("BAW123"))
                .Times(0);

            EXPECT_CALL(this->mockApi, AssignAircraftToHold(_, _))
                .Times(0);

            this->handler.ProcessWebsocketMessage(message);
            EXPECT_EQ("TIMBA", this->manager.GetHoldingAircraft("BAW123")->GetAssignedHold());
        }
    }  // namespace Hold
}  // namespace UKControllerPluginTest
