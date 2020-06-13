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
#include "sectorfile/SectorFileCoordinates.h"
#include "mock/MockFlightplanRadarTargetPair.h"

using UKControllerPlugin::Navaids::NavaidCollection;
using UKControllerPlugin::Hold::HoldingData;
using UKControllerPlugin::Hold::HoldManager;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCRadarTargetInterface;
using UKControllerPluginTest::Euroscope::MockFlightplanRadarTargetPair;
using UKControllerPluginTest::Api::MockApiInterface;
using UKControllerPluginTest::TaskManager::MockTaskRunnerInterface;
using UKControllerPlugin::Hold::HoldEventHandler;
using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPlugin::Websocket::WebsocketSubscription;
using UKControllerPlugin::Websocket::WebsocketMessage;
using UKControllerPlugin::SectorFile::ParseSectorFileCoordinates;
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
                    : handler(this->manager, this->navaids, this->mockPlugin, 1),
                    manager(mockApi, mockTaskRunner)
                {
                    this->navaids.AddNavaid(
                        {1, "TIMBA", ParseSectorFileCoordinates("N050.56.44.000", "E000.15.42.000")}
                    );
                    this->navaids.AddNavaid(
                        { 2, "MAY", ParseSectorFileCoordinates("N051.01.02.000", "E000.06.58.000") }
                    );
                    this->navaids.AddNavaid(
                        { 3, "OLEVI", ParseSectorFileCoordinates("N051.11.17.400", "E000.06.11.300") }
                    );
                    this->navaids.AddNavaid(
                        { 4, "SAM", ParseSectorFileCoordinates("N050.57.18.900", "W001.20.42.200") }
                    );

                    this->mockFlightplanPointer = std::make_shared<NiceMock<MockEuroScopeCFlightPlanInterface>>();
                    ON_CALL(*this->mockFlightplanPointer, GetCallsign())
                        .WillByDefault(Return("BAW123"));

                    ON_CALL(this->mockFlightplan, GetCallsign())
                        .WillByDefault(Return("BAW123"));

                    this->manager.AssignAircraftToHold("BAW123", "TIMBA", false);
                }

                void CreateFlightplanRadarTargetPair(
                    std::string callsign,
                    EuroScopePlugIn::CPosition position
                ) {
                    std::shared_ptr<NiceMock<MockEuroScopeCFlightPlanInterface>> flightplan =
                        std::make_shared<NiceMock<MockEuroScopeCFlightPlanInterface>>();

                    std::shared_ptr<NiceMock<MockEuroScopeCRadarTargetInterface>> radarTarget =
                        std::make_shared<NiceMock<MockEuroScopeCRadarTargetInterface>>();

                    ON_CALL(*flightplan, GetCallsign())
                        .WillByDefault(Return(callsign));

                    ON_CALL(*radarTarget, GetPosition())
                        .WillByDefault(Return(position));

                    this->mockPlugin.AddAllFlightplansItem({ flightplan, radarTarget });
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

        TEST_F(HoldEventHandlerTest, TimedEventAddsAircraftToProximityHoldsIfCloseEnough)
        {
            this->manager.UnassignAircraftFromHold("BAW123", "TIMBA");

            // Aircraft is at TIMBA
            this->CreateFlightplanRadarTargetPair(
                "RYR123",
                ParseSectorFileCoordinates("N050.56.44.000", "E000.15.42.000")
            );

            // Aircraft is at MAY
            this->CreateFlightplanRadarTargetPair(
                "EZY234",
                ParseSectorFileCoordinates("N051.01.02.000", "E000.06.58.000")
            );

            this->handler.TimedEventTrigger();

            std::set<std::string> expectedProximityHoldsEzy234({ "MAY", "OLEVI", "TIMBA" });
            EXPECT_EQ("EZY234", (*this->manager.GetAircraftForHold("TIMBA").cbegin())->GetCallsign());
            EXPECT_EQ("EZY234", (*this->manager.GetAircraftForHold("MAY").cbegin())->GetCallsign());
            EXPECT_EQ(expectedProximityHoldsEzy234, this->manager.GetHoldingAircraft("EZY234")->GetProximityHolds());

            std::set<std::string> expectedProximityHoldsRyr123({ "MAY", "TIMBA" });
            EXPECT_EQ("RYR123", (*++this->manager.GetAircraftForHold("TIMBA").cbegin())->GetCallsign());
            EXPECT_EQ("RYR123", (*++this->manager.GetAircraftForHold("MAY").cbegin())->GetCallsign());
            EXPECT_EQ(expectedProximityHoldsRyr123, this->manager.GetHoldingAircraft("RYR123")->GetProximityHolds());
        }

        TEST_F(HoldEventHandlerTest, TimedEventRemovesAircraftFromProximityHoldsIfNotCloseEnough)
        {
            // Aircraft is at SAM
            this->CreateFlightplanRadarTargetPair(
                "RYR123",
                ParseSectorFileCoordinates("N050.57.18.900", "W001.20.42.200")
            );

            this->manager.AddAircraftToProximityHold("RYR123", "OLEVI");
            this->manager.AddAircraftToProximityHold("RYR123", "MAY");

            this->handler.TimedEventTrigger();

            std::set<std::string> expectedProximityHolds({ "SAM", });
            EXPECT_EQ("RYR123", (*this->manager.GetAircraftForHold("SAM").cbegin())->GetCallsign());
            EXPECT_EQ(0, this->manager.GetAircraftForHold("OLEVI").size());
            EXPECT_EQ(0, this->manager.GetAircraftForHold("MAY").size());
            EXPECT_EQ(expectedProximityHolds, this->manager.GetHoldingAircraft("RYR123")->GetProximityHolds());
        }
    }  // namespace Hold
}  // namespace UKControllerPluginTest
