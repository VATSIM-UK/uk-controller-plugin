#include "hold/AbstractHoldLevelRestriction.h"
#include "hold/DeemedSeparatedHold.h"
#include "hold/HoldingAircraft.h"
#include "hold/HoldManager.h"
#include "hold/HoldEventHandler.h"
#include "hold/HoldingData.h"
#include "hold/ProximityHold.h"
#include "plugin/PopupMenuItem.h"
#include "navaids/NavaidCollection.h"
#include "push/PushEventSubscription.h"
#include "push/PushEvent.h"
#include "sectorfile/SectorFileCoordinates.h"
#include "tag/TagData.h"
#include "time/SystemClock.h"

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;
using ::testing::Throw;
using UKControllerPlugin::Hold::HoldEventHandler;
using UKControllerPlugin::Hold::HoldingData;
using UKControllerPlugin::Hold::HoldManager;
using UKControllerPlugin::Hold::ProximityHold;
using UKControllerPlugin::Navaids::NavaidCollection;
using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPlugin::Push::PushEvent;
using UKControllerPlugin::Push::PushEventSubscription;
using UKControllerPlugin::SectorFile::ParseSectorFileCoordinates;
using UKControllerPlugin::Tag::TagData;
using UKControllerPlugin::Time::SetTestNow;
using UKControllerPlugin::Time::TimeNow;
using UKControllerPluginTest::Api::MockApiInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCRadarTargetInterface;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPluginTest::Euroscope::MockFlightplanRadarTargetPair;
using UKControllerPluginTest::TaskManager::MockTaskRunnerInterface;

namespace UKControllerPluginTest {
    namespace Hold {

        class HoldEventHandlerTest : public Test
        {
            public:
            HoldEventHandlerTest()
                : manager(mockApi, mockTaskRunner), handler(this->manager, this->navaids, this->mockPlugin),
                  tagData(
                      mockFlightplan,
                      mockRadarTarget,
                      1,
                      EuroScopePlugIn::TAG_DATA_CORRELATED,
                      itemString,
                      &euroscopeColourCode,
                      &tagColour,
                      &fontSize)
            {
                this->navaids.AddNavaid({1, "TIMBA", ParseSectorFileCoordinates("N050.56.44.000", "E000.15.42.000")});
                this->navaids.AddNavaid({2, "MAY", ParseSectorFileCoordinates("N051.01.02.000", "E000.06.58.000")});
                this->navaids.AddNavaid({3, "OLEVI", ParseSectorFileCoordinates("N051.11.17.400", "E000.06.11.300")});
                this->navaids.AddNavaid({4, "SAM", ParseSectorFileCoordinates("N050.57.18.900", "W001.20.42.200")});

                this->mockFlightplanPointer = std::make_shared<NiceMock<MockEuroScopeCFlightPlanInterface>>();
                ON_CALL(*this->mockFlightplanPointer, GetCallsign()).WillByDefault(Return("BAW123"));

                ON_CALL(this->mockFlightplan, GetCallsign()).WillByDefault(Return("BAW123"));

                this->manager.AssignAircraftToHold("BAW123", "TIMBA", false);

                SetTestNow(TimeNow());
            }

            void CreateFlightplanRadarTargetPair(std::string callsign, EuroScopePlugIn::CPosition position)
            {
                std::shared_ptr<NiceMock<MockEuroScopeCFlightPlanInterface>> flightplan =
                    std::make_shared<NiceMock<MockEuroScopeCFlightPlanInterface>>();

                std::shared_ptr<NiceMock<MockEuroScopeCRadarTargetInterface>> radarTarget =
                    std::make_shared<NiceMock<MockEuroScopeCRadarTargetInterface>>();

                ON_CALL(*flightplan, GetCallsign()).WillByDefault(Return(callsign));

                ON_CALL(*radarTarget, GetPosition()).WillByDefault(Return(position));

                this->mockPlugin.AddAllFlightplansItem({flightplan, radarTarget});
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
            HoldingData holdData = {1, "TIMBA", "TIMBA", 8000, 15000, 209, "left", {}};
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
            EXPECT_EQ("NOHOLD", this->tagData.GetItemString());
        }

        TEST_F(HoldEventHandlerTest, ItReturnsNoHoldIfAircraftNotAssignedToHold)
        {
            this->manager.UnassignAircraftFromHold("BAW123", false);
            this->manager.AddAircraftToProximityHold(std::make_shared<ProximityHold>("BAW123", "TIMBA"));
            handler.SetTagItemData(this->tagData);
            EXPECT_EQ("NOHOLD", this->tagData.GetItemString());
        }

        TEST_F(HoldEventHandlerTest, ItHasSubscriptionsToPushEvents)
        {
            std::set<PushEventSubscription> expectedSubscriptions;
            expectedSubscriptions.insert({PushEventSubscription::SUB_TYPE_CHANNEL, "private-hold-assignments"});
            EXPECT_EQ(expectedSubscriptions, this->handler.GetPushEventSubscriptions());
        }

        TEST_F(HoldEventHandlerTest, ItAssignsHoldsOnEvent)
        {
            PushEvent message{
                "App\\Events\\HoldAssignedEvent",
                "private-hold-assignments",
                {{"callsign", "BAW123"}, {"navaid", "WILLO"}}};

            ON_CALL(this->mockPlugin, GetFlightplanForCallsign("BAW123"))
                .WillByDefault(Return(this->mockFlightplanPointer));

            EXPECT_CALL(this->mockApi, AssignAircraftToHold(_, _)).Times(0);

            this->handler.ProcessPushEvent(message);
            EXPECT_EQ("WILLO", this->manager.GetHoldingAircraft("BAW123")->GetAssignedHold());
        }

        TEST_F(HoldEventHandlerTest, ItDoesNotAssignIfCallsignMissing)
        {
            PushEvent message{"App\\Events\\HoldAssignedEvent", "private-hold-assignments", {{"navaid", "WILLO"}}};

            ON_CALL(this->mockPlugin, GetFlightplanForCallsign("BAW123"))
                .WillByDefault(Return(this->mockFlightplanPointer));

            EXPECT_CALL(this->mockApi, AssignAircraftToHold(_, _)).Times(0);

            this->handler.ProcessPushEvent(message);
            EXPECT_EQ("TIMBA", this->manager.GetHoldingAircraft("BAW123")->GetAssignedHold());
        }

        TEST_F(HoldEventHandlerTest, ItDoesNotAssignIfCallsignNotString)
        {
            PushEvent message{
                "App\\Events\\HoldAssignedEvent", "private-hold-assignments", {{"callsign", 123}, {"navaid", "WILLO"}}};

            ON_CALL(this->mockPlugin, GetFlightplanForCallsign("BAW123"))
                .WillByDefault(Return(this->mockFlightplanPointer));

            EXPECT_CALL(this->mockApi, AssignAircraftToHold(_, _)).Times(0);

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
                }};

            ON_CALL(this->mockPlugin, GetFlightplanForCallsign("BAW123"))
                .WillByDefault(Return(this->mockFlightplanPointer));

            EXPECT_CALL(this->mockApi, AssignAircraftToHold(_, _)).Times(0);

            this->handler.ProcessPushEvent(message);
            EXPECT_EQ("TIMBA", this->manager.GetHoldingAircraft("BAW123")->GetAssignedHold());
        }

        TEST_F(HoldEventHandlerTest, ItDoesNotAssignIfNavaidNotString)
        {
            PushEvent message{
                "App\\Events\\HoldAssignedEvent",
                "private-hold-assignments",
                {{"callsign", "BAW123"}, {"navaid", 123}}};

            ON_CALL(this->mockPlugin, GetFlightplanForCallsign("BAW123"))
                .WillByDefault(Return(this->mockFlightplanPointer));

            EXPECT_CALL(this->mockApi, AssignAircraftToHold(_, _)).Times(0);

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

            EXPECT_CALL(this->mockApi, AssignAircraftToHold(_, _)).Times(0);

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
                }};

            ON_CALL(this->mockPlugin, GetFlightplanForCallsign("BAW123"))
                .WillByDefault(Return(this->mockFlightplanPointer));

            EXPECT_CALL(this->mockApi, UnassignAircraftHold(_)).Times(0);

            this->handler.ProcessPushEvent(message);
            EXPECT_EQ(this->manager.invalidAircraft, this->manager.GetHoldingAircraft("BAW123"));
        }

        TEST_F(HoldEventHandlerTest, ItDoesntUnassignHoldOnMissingCallsign)
        {
            PushEvent message{"App\\Events\\HoldUnassignedEvent", "private-hold-assignments", {}};

            ON_CALL(this->mockPlugin, GetFlightplanForCallsign("BAW123"))
                .WillByDefault(Return(this->mockFlightplanPointer));

            EXPECT_CALL(this->mockApi, UnassignAircraftHold(_)).Times(0);

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
                }};

            ON_CALL(this->mockPlugin, GetFlightplanForCallsign("BAW123"))
                .WillByDefault(Return(this->mockFlightplanPointer));

            EXPECT_CALL(this->mockApi, UnassignAircraftHold(_)).Times(0);

            this->handler.ProcessPushEvent(message);
            EXPECT_EQ("TIMBA", this->manager.GetHoldingAircraft("BAW123")->GetAssignedHold());
        }

        TEST_F(HoldEventHandlerTest, ItDoesntUnassignHoldOnDataNotObject)
        {
            PushEvent message{
                "App\\Events\\HoldUnassignedEvent",
                "private-hold-assignments",
                nlohmann::json::array({"callsign", "BAW123"})};

            ON_CALL(this->mockPlugin, GetFlightplanForCallsign("BAW123"))
                .WillByDefault(Return(this->mockFlightplanPointer));

            EXPECT_CALL(this->mockApi, UnassignAircraftHold(_)).Times(0);

            this->handler.ProcessPushEvent(message);
            EXPECT_EQ("TIMBA", this->manager.GetHoldingAircraft("BAW123")->GetAssignedHold());
        }

        TEST_F(HoldEventHandlerTest, ItHandlesInvalidEvent)
        {
            PushEvent message{
                "App\\Events\\Uwut", "private-hold-assignments", {{"callsign", "BAW123"}, {"navaid", "WILLO"}}};

            EXPECT_CALL(this->mockPlugin, GetFlightplanForCallsign("BAW123")).Times(0);

            EXPECT_CALL(this->mockApi, AssignAircraftToHold(_, _)).Times(0);

            this->handler.ProcessPushEvent(message);
            EXPECT_EQ("TIMBA", this->manager.GetHoldingAircraft("BAW123")->GetAssignedHold());
        }

        TEST_F(HoldEventHandlerTest, TimedEventAddsAircraftToProximityHoldsIfCloseEnough)
        {
            this->manager.UnassignAircraftFromHold("BAW123", false);

            // Aircraft is at TIMBA
            this->CreateFlightplanRadarTargetPair(
                "RYR123", ParseSectorFileCoordinates("N050.56.44.000", "E000.15.42.000"));

            // Aircraft is at MAY
            this->CreateFlightplanRadarTargetPair(
                "EZY234", ParseSectorFileCoordinates("N051.01.02.000", "E000.06.58.000"));

            this->handler.TimedEventTrigger();

            // EZY234
            EXPECT_EQ("EZY234", (*this->manager.GetAircraftForHold("TIMBA").cbegin())->GetCallsign());
            EXPECT_EQ("EZY234", (*this->manager.GetAircraftForHold("MAY").cbegin())->GetCallsign());

            EXPECT_EQ(3, this->manager.GetHoldingAircraft("EZY234")->GetProximityHolds().size());
            auto mayfieldEzy234 = this->manager.GetHoldingAircraft("EZY234")->GetProximityHold("MAY");
            EXPECT_NE(nullptr, mayfieldEzy234);
            EXPECT_EQ("EZY234", mayfieldEzy234->Callsign());
            EXPECT_EQ("MAY", mayfieldEzy234->Navaid());
            EXPECT_TRUE(mayfieldEzy234->HasEntered());

            auto oleviEzy234 = this->manager.GetHoldingAircraft("EZY234")->GetProximityHold("OLEVI");
            EXPECT_NE(nullptr, oleviEzy234);
            EXPECT_EQ("EZY234", oleviEzy234->Callsign());
            EXPECT_EQ("OLEVI", oleviEzy234->Navaid());
            EXPECT_FALSE(oleviEzy234->HasEntered());

            auto timbaEzy234 = this->manager.GetHoldingAircraft("EZY234")->GetProximityHold("TIMBA");
            EXPECT_NE(nullptr, timbaEzy234);
            EXPECT_EQ("EZY234", timbaEzy234->Callsign());
            EXPECT_EQ("TIMBA", timbaEzy234->Navaid());
            EXPECT_FALSE(oleviEzy234->HasEntered());

            // RYR123
            EXPECT_EQ("RYR123", (*++this->manager.GetAircraftForHold("TIMBA").cbegin())->GetCallsign());
            EXPECT_EQ("RYR123", (*++this->manager.GetAircraftForHold("MAY").cbegin())->GetCallsign());

            EXPECT_EQ(2, this->manager.GetHoldingAircraft("RYR123")->GetProximityHolds().size());
            auto mayfieldRyr123 = this->manager.GetHoldingAircraft("RYR123")->GetProximityHold("MAY");
            EXPECT_NE(nullptr, mayfieldRyr123);
            EXPECT_EQ("RYR123", mayfieldRyr123->Callsign());
            EXPECT_EQ("MAY", mayfieldRyr123->Navaid());
            EXPECT_FALSE(mayfieldRyr123->HasEntered());

            auto timbaRyr123 = this->manager.GetHoldingAircraft("RYR123")->GetProximityHold("TIMBA");
            EXPECT_NE(nullptr, timbaRyr123);
            EXPECT_EQ("RYR123", timbaRyr123->Callsign());
            EXPECT_EQ("TIMBA", timbaRyr123->Navaid());
            EXPECT_TRUE(timbaRyr123->HasEntered());
        }

        TEST_F(HoldEventHandlerTest, ItDoesntResetTheHoldEnteredTimer)
        {
            this->manager.UnassignAircraftFromHold("BAW123", false);

            // Aircraft is at TIMBA
            this->CreateFlightplanRadarTargetPair(
                "RYR123", ParseSectorFileCoordinates("N050.56.44.000", "E000.15.42.000"));

            this->handler.TimedEventTrigger();

            auto timeBefore = TimeNow();
            EXPECT_EQ(timeBefore, this->manager.GetHoldingAircraft("RYR123")->GetProximityHold("TIMBA")->EnteredAt());
            SetTestNow(TimeNow() + std::chrono::seconds(10));
            this->handler.TimedEventTrigger();
            EXPECT_EQ(timeBefore, this->manager.GetHoldingAircraft("RYR123")->GetProximityHold("TIMBA")->EnteredAt());
        }

        TEST_F(HoldEventHandlerTest, TimedEventRemovesAircraftFromProximityHoldsIfNotCloseEnough)
        {
            // Aircraft is at SAM
            this->CreateFlightplanRadarTargetPair(
                "RYR123", ParseSectorFileCoordinates("N050.57.18.900", "W001.20.42.200"));

            this->manager.AddAircraftToProximityHold(std::make_shared<ProximityHold>("RYR123", "OLEVI"));
            this->manager.AddAircraftToProximityHold(std::make_shared<ProximityHold>("RYR123", "MAY"));

            this->handler.TimedEventTrigger();

            EXPECT_EQ("RYR123", (*this->manager.GetAircraftForHold("SAM").cbegin())->GetCallsign());
            EXPECT_EQ(0, this->manager.GetAircraftForHold("OLEVI").size());
            EXPECT_EQ(0, this->manager.GetAircraftForHold("MAY").size());
            EXPECT_EQ(1, this->manager.GetHoldingAircraft("RYR123")->GetProximityHolds().size());
            EXPECT_NE(nullptr, this->manager.GetHoldingAircraft("RYR123")->GetProximityHold("SAM"));
        }
    } // namespace Hold
} // namespace UKControllerPluginTest
