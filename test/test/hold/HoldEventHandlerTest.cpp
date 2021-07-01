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
#include "push/PushEventSubscription.h"
#include "push/PushEvent.h"
#include "sectorfile/SectorFileCoordinates.h"
#include "mock/MockFlightplanRadarTargetPair.h"
#include "tag/TagData.h"

using UKControllerPlugin::Tag::TagData;
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
using UKControllerPlugin::Push::PushEventSubscription;
using UKControllerPlugin::Push::PushEvent;
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
                    manager(mockApi, mockTaskRunner),
                    tagData(
                        mockFlightplan,
                        mockRadarTarget,
                        1,
                        EuroScopePlugIn::TAG_DATA_CORRELATED,
                        itemString,
                        &euroscopeColourCode,
                        &tagColour,
                        &fontSize
                    )
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

                double fontSize = 24.1;
                COLORREF tagColour = RGB(255, 255, 255);
                int euroscopeColourCode = EuroScopePlugIn::TAG_COLOR_ASSUMED;
                char itemString[16] = "Foooooo";
                NiceMock<MockEuroScopeCFlightPlanInterface> mockFlightplan;
                NiceMock<MockEuroScopeCRadarTargetInterface> mockRadarTarget;
                NiceMock<MockApiInterface> mockApi;
                NiceMock<MockTaskRunnerInterface> mockTaskRunner;
                NavaidCollection navaids;
                HoldingData holdData = { 1, "TIMBA", "TIMBA", 8000, 15000, 209, "left", {} };
                NiceMock<MockEuroscopePluginLoopbackInterface> mockPlugin;
                HoldManager manager;
                HoldEventHandler handler;
                TagData tagData;
                std::shared_ptr<NiceMock<MockEuroScopeCFlightPlanInterface>> mockFlightplanPointer;
        };

        TEST_F(HoldEventHandlerTest, ItHasATagItemDescription)
        {
            EXPECT_TRUE("Selected Hold" == this->handler.GetTagItemDescription(0));
        }

        TEST_F(HoldEventHandlerTest, ItReturnsTheSelectedHoldForAnAircraft)
        {
            handler.SetTagItemData(this->tagData);
            EXPECT_EQ("HTIMBA", this->tagData.GetItemString());
        }

        TEST_F(HoldEventHandlerTest, ItReturnsNoHoldIfAircraftNotInHold)
        {
            this->manager.UnassignAircraftFromHold("BAW123", false);
            handler.SetTagItemData(this->tagData);
            EXPECT_EQ(this->handler.noHold, this->tagData.GetItemString());
        }

        TEST_F(HoldEventHandlerTest, ItReturnsNoHoldIfAircraftNotAssignedToHold)
        {
            this->manager.UnassignAircraftFromHold("BAW123", false);
            this->manager.AddAircraftToProximityHold("BAW123", "TIMBA");
            handler.SetTagItemData(this->tagData);
            EXPECT_EQ(this->handler.noHold, this->tagData.GetItemString());
        }

        TEST_F(HoldEventHandlerTest, ItHasSubscriptionsToPushEvents)
        {
            std::set<PushEventSubscription> expectedSubscriptions;
            expectedSubscriptions.insert(
                {
                    PushEventSubscription::SUB_TYPE_CHANNEL,
                    "private-hold-assignments"
                }
            );
            EXPECT_EQ(expectedSubscriptions, this->handler.GetPushEventSubscriptions());
        }

        TEST_F(HoldEventHandlerTest, ItAssignsHoldsOnEvent)
        {
            PushEvent message{
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

            this->handler.ProcessPushEvent(message);
            EXPECT_EQ("WILLO", this->manager.GetHoldingAircraft("BAW123")->GetAssignedHold());
        }

        TEST_F(HoldEventHandlerTest, ItDoesNotAssignIfCallsignMissing)
        {
            PushEvent message{
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

            this->handler.ProcessPushEvent(message);
            EXPECT_EQ("TIMBA", this->manager.GetHoldingAircraft("BAW123")->GetAssignedHold());
        }

        TEST_F(HoldEventHandlerTest, ItDoesNotAssignIfCallsignNotString)
        {
            PushEvent message{
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

            this->handler.ProcessPushEvent(message);
            EXPECT_EQ("TIMBA", this->manager.GetHoldingAircraft("BAW123")->GetAssignedHold());
        }

        TEST_F(HoldEventHandlerTest, ItDoesNotAssignIfNavaidMissing)
        {
            PushEvent message{
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

            this->handler.ProcessPushEvent(message);
            EXPECT_EQ("TIMBA", this->manager.GetHoldingAircraft("BAW123")->GetAssignedHold());
        }

        TEST_F(HoldEventHandlerTest, ItDoesNotAssignIfNavaidNotString)
        {
            PushEvent message{
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

            this->handler.ProcessPushEvent(message);
            EXPECT_EQ("TIMBA", this->manager.GetHoldingAircraft("BAW123")->GetAssignedHold());
        }

        TEST_F(HoldEventHandlerTest, ItDoesNotAssignIfDataNotObject)
        {
            PushEvent message{
                "App\\Events\\HoldAssignedEvent",
                "private-hold-assignments",
                nlohmann::json::array({"callsign", "BAW123", "navaid", "WILLO"}),
            };

            ON_CALL(this->mockPlugin, GetFlightplanForCallsign("BAW123"))
                .WillByDefault(Return(this->mockFlightplanPointer));

            EXPECT_CALL(this->mockApi, AssignAircraftToHold(_, _))
                .Times(0);

            this->handler.ProcessPushEvent(message);
            EXPECT_EQ("TIMBA", this->manager.GetHoldingAircraft("BAW123")->GetAssignedHold());
        }

        TEST_F(HoldEventHandlerTest, ItUnassignsHoldsOnEvent)
        {
            PushEvent message{
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

            this->handler.ProcessPushEvent(message);
            EXPECT_EQ(this->manager.invalidAircraft, this->manager.GetHoldingAircraft("BAW123"));
        }

        TEST_F(HoldEventHandlerTest, ItDoesntUnassignHoldOnMissingCallsign)
        {
            PushEvent message{
                "App\\Events\\HoldUnassignedEvent",
                "private-hold-assignments",
                {
                }
            };

            ON_CALL(this->mockPlugin, GetFlightplanForCallsign("BAW123"))
                .WillByDefault(Return(this->mockFlightplanPointer));

            EXPECT_CALL(this->mockApi, UnassignAircraftHold(_))
                .Times(0);

            this->handler.ProcessPushEvent(message);
            EXPECT_EQ("TIMBA", this->manager.GetHoldingAircraft("BAW123")->GetAssignedHold());
        }

        TEST_F(HoldEventHandlerTest, ItDoesntUnassignHoldOnCallsignNotString)
        {
            PushEvent message{
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

            this->handler.ProcessPushEvent(message);
            EXPECT_EQ("TIMBA", this->manager.GetHoldingAircraft("BAW123")->GetAssignedHold());
        }

        TEST_F(HoldEventHandlerTest, ItDoesntUnassignHoldOnDataNotObject)
        {
            PushEvent message{
                "App\\Events\\HoldUnassignedEvent",
                "private-hold-assignments",
                nlohmann::json::array({"callsign", "BAW123"})
            };

            ON_CALL(this->mockPlugin, GetFlightplanForCallsign("BAW123"))
                .WillByDefault(Return(this->mockFlightplanPointer));

            EXPECT_CALL(this->mockApi, UnassignAircraftHold(_))
                .Times(0);

            this->handler.ProcessPushEvent(message);
            EXPECT_EQ("TIMBA", this->manager.GetHoldingAircraft("BAW123")->GetAssignedHold());
        }

        TEST_F(HoldEventHandlerTest, ItHandlesInvalidEvent)
        {
            PushEvent message{
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

            this->handler.ProcessPushEvent(message);
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
