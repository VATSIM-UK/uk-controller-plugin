#include "pch/pch.h"
#include "stands/StandEventHandler.h"
#include "stands/CompareStands.h"
#include "stands/Stand.h"
#include "mock/MockEuroScopeCFlightplanInterface.h"
#include "mock/MockEuroScopeCRadarTargetInterface.h"
#include "tag/TagData.h"
#include "websocket/WebsocketSubscription.h"
#include "websocket/WebsocketMessage.h"
#include "mock/MockApiInterface.h"
#include "mock/MockTaskRunnerInterface.h"
#include "mock/MockEuroscopePluginLoopbackInterface.h"
#include "api/ApiException.h"
#include "plugin/PopupMenuItem.h"

using ::testing::Test;
using UKControllerPlugin::Api::ApiException;
using UKControllerPlugin::Stands::StandEventHandler;
using UKControllerPlugin::Stands::CompareStands;
using UKControllerPlugin::Stands::Stand;
using UKControllerPlugin::Tag::TagData;
using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPlugin::Websocket::WebsocketSubscription;
using UKControllerPluginTest::Api::MockApiInterface;
using UKControllerPluginTest::TaskManager::MockTaskRunnerInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCRadarTargetInterface;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPlugin::Websocket::WebsocketMessage;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Throw;
using ::testing::_;

namespace UKControllerPluginTest {
    namespace Stands {

        class StandEventHandlerTest : public Test
        {
            public:
                StandEventHandlerTest()
                    : handler(api, taskRunner, plugin, this->GetStands(), 1),
                    tagData(flightplan, radarTarget, 110, 1, itemString, &euroscopeColourCode, &tagColour, &fontSize)
                {
                    ON_CALL(this->flightplan, GetCallsign())
                        .WillByDefault(Return("BAW123"));
                }

                std::set<Stand, CompareStands> GetStands(void) const
                {
                    std::set<Stand, CompareStands> stands;
                    stands.insert({ 1, "EGKK", "1L" });
                    stands.insert({ 2, "EGKK", "55" });
                    stands.insert({ 3, "EGLL", "317" });
                    return stands;
                }

                double fontSize = 24.1;
                COLORREF tagColour = RGB(255, 255, 255);
                int euroscopeColourCode = EuroScopePlugIn::TAG_COLOR_ASSUMED;
                char itemString[16] = "Foooooo";
                NiceMock<MockApiInterface> api;
                NiceMock<MockEuroscopePluginLoopbackInterface> plugin;
                NiceMock<MockTaskRunnerInterface> taskRunner;
                NiceMock<MockEuroScopeCFlightPlanInterface> flightplan;
                NiceMock<MockEuroScopeCRadarTargetInterface> radarTarget;
                TagData tagData;
                StandEventHandler handler;
        };

        TEST_F(StandEventHandlerTest, ItSetsStandSelectedCallbackId)
        {
            EXPECT_EQ(1, this->handler.standSelectedCallbackId);
        }

        TEST_F(StandEventHandlerTest, ItStartsWithNoAssignments)
        {
            EXPECT_EQ(0, this->handler.CountStandAssignments());
        }

        TEST_F(StandEventHandlerTest, ItReturnsATagItemDescription)
        {
            EXPECT_EQ("Assigned Stand", this->handler.GetTagItemDescription(110));
        }

        TEST_F(StandEventHandlerTest, ItReturnsAssignedStand)
        {
            this->handler.SetAssignedStand("BAW123", 3);
            this->handler.SetTagItemData(this->tagData);
            EXPECT_EQ("317", this->tagData.GetItemString());
        }

        TEST_F(StandEventHandlerTest, ItReturnsNothingIfStandNotAssigned)
        {
            this->handler.SetTagItemData(this->tagData);
            EXPECT_EQ("Foooooo", this->tagData.GetItemString());
        }

        TEST_F(StandEventHandlerTest, ItSubscribesToChannels)
        {
            std::set<WebsocketSubscription> expectedSubscriptions;
            expectedSubscriptions.insert(
                {
                    WebsocketSubscription::SUB_TYPE_CHANNEL,
                    "private-stand-assignments"
                }
            );
            expectedSubscriptions.insert(
                {
                    WebsocketSubscription::SUB_TYPE_EVENT,
                    "pusher:connection_established"
                }
            );
            EXPECT_EQ(expectedSubscriptions, this->handler.GetSubscriptions());
        }

        TEST_F(StandEventHandlerTest, ItAssignsStandsFromWebsocketMessage)
        {
            WebsocketMessage message{
                "App\\Events\\StandAssignedEvent",
                "private-stand-assignments",
                nlohmann::json {
                    {"callsign", "BAW123"},
                    {"stand_id", 1},
                }
            };

            this->handler.ProcessWebsocketMessage(message);
            ASSERT_EQ(1, this->handler.GetAssignedStandForCallsign("BAW123"));
        }

        TEST_F(StandEventHandlerTest, ItDoesntAssignStandFromWebsocketMessageIfNoCallsign)
        {
            WebsocketMessage message{
                "App\\Events\\StandAssignedEvent",
                "private-stand-assignments",
                nlohmann::json {
                    {"stand_id", 1},
                }
            };

            this->handler.ProcessWebsocketMessage(message);
            ASSERT_EQ(this->handler.noStandAssigned, this->handler.GetAssignedStandForCallsign("BAW123"));
        }

        TEST_F(StandEventHandlerTest, ItDoesntAssignStandFromWebsocketMessageIfCallsignInvalid)
        {
            WebsocketMessage message{
                "App\\Events\\StandAssignedEvent",
                "private-stand-assignments",
                nlohmann::json {
                    {"callsign", 123},
                    {"stand_id", 1},
                }
            };

            this->handler.ProcessWebsocketMessage(message);
            ASSERT_EQ(this->handler.noStandAssigned, this->handler.GetAssignedStandForCallsign("BAW123"));
        }

        TEST_F(StandEventHandlerTest, ItDoesntAssignStandFromWebsocketMessageIfNoStandId)
        {
            WebsocketMessage message{
                "App\\Events\\StandAssignedEvent",
                "private-stand-assignments",
                nlohmann::json {
                    {"callsign", "BAW123"},
                }
            };

            this->handler.ProcessWebsocketMessage(message);
            ASSERT_EQ(this->handler.noStandAssigned, this->handler.GetAssignedStandForCallsign("BAW123"));
        }

        TEST_F(StandEventHandlerTest, ItDoesntAssignStandFromWebsocketMessageIfStandIdInvalid)
        {
            WebsocketMessage message{
                "App\\Events\\StandAssignedEvent",
                "private-stand-assignments",
                nlohmann::json {
                    {"callsign", "BAW123"},
                    {"stand_id", "1"},
                }
            };

            this->handler.ProcessWebsocketMessage(message);
            ASSERT_EQ(this->handler.noStandAssigned, this->handler.GetAssignedStandForCallsign("BAW123"));
        }

        TEST_F(StandEventHandlerTest, ItDoesntAssignStandFromWebsocketMessageIfStandIdNotRealStand)
        {
            WebsocketMessage message{
                "App\\Events\\StandAssignedEvent",
                "private-stand-assignments",
                nlohmann::json {
                    {"callsign", "BAW123"},
                    {"stand_id", -999},
                }
            };

            this->handler.ProcessWebsocketMessage(message);
            ASSERT_EQ(this->handler.noStandAssigned, this->handler.GetAssignedStandForCallsign("BAW123"));
        }

        TEST_F(StandEventHandlerTest, ItUnassignsStandsFromWebsocketMessage)
        {
            this->handler.SetAssignedStand("BAW123", 3);
            WebsocketMessage message{
                "App\\Events\\StandUnassignedEvent",
                "private-stand-assignments",
                nlohmann::json {
                    {"callsign", "BAW123"}
                }
            };

            this->handler.ProcessWebsocketMessage(message);
            ASSERT_EQ(this->handler.noStandAssigned, this->handler.GetAssignedStandForCallsign("BAW123"));
        }

        TEST_F(StandEventHandlerTest, ItDoesntUnassignStandFromWebsocketMessageIfCallsignMissing)
        {
            this->handler.SetAssignedStand("BAW123", 3);
            WebsocketMessage message{
                "App\\Events\\StandUnassignedEvent",
                "private-stand-assignments",
                nlohmann::json {
                    {"notcallsign", "BAW123"}
                }
            };

            this->handler.ProcessWebsocketMessage(message);
            ASSERT_EQ(3, this->handler.GetAssignedStandForCallsign("BAW123"));
        }

        TEST_F(StandEventHandlerTest, ItDoesntUnassignStandFromWebsocketMessageIfCallsignInvalid)
        {
            this->handler.SetAssignedStand("BAW123", 3);
            WebsocketMessage message{
                "App\\Events\\StandUnassignedEvent",
                "private-stand-assignments",
                nlohmann::json {
                    {"callsign", 123}
                }
            };

            this->handler.ProcessWebsocketMessage(message);
            ASSERT_EQ(3, this->handler.GetAssignedStandForCallsign("BAW123"));
        }

        TEST_F(StandEventHandlerTest, ItHandlesUnassignStandFromWebsocketMessageIfStandNotAssigned)
        {
            WebsocketMessage message{
                "App\\Events\\StandUnassignedEvent",
                "private-stand-assignments",
                nlohmann::json {
                    {"callsign", "BAW123"}
                }
            };

            this->handler.ProcessWebsocketMessage(message);
            ASSERT_EQ(this->handler.noStandAssigned, this->handler.GetAssignedStandForCallsign("BAW123"));
        }

        TEST_F(StandEventHandlerTest, ItLoadsAssignmentsOnWebsocketConnection)
        {
            nlohmann::json assignments = nlohmann::json::array();
            assignments.push_back({
                {"callsign", "BAW123"},
                {"stand_id", 1},
            });
            assignments.push_back({
                {"callsign", "VIR245"},
                {"stand_id", 2},
            });
            WebsocketMessage message{
                "pusher:connection_established",
                "bla",
                nlohmann::json(),
            };

            EXPECT_CALL(this->api, GetAssignedStands())
                .Times(1)
                .WillOnce(Return(assignments));

            this->handler.ProcessWebsocketMessage(message);
            ASSERT_EQ(1, this->handler.GetAssignedStandForCallsign("BAW123"));
            ASSERT_EQ(2, this->handler.GetAssignedStandForCallsign("VIR245"));
        }

        TEST_F(StandEventHandlerTest, ItHandlesNonArrayStandAssignments)
        {
            nlohmann::json assignments = nlohmann::json::object();
            WebsocketMessage message{
                "pusher:connection_established",
                "bla",
                nlohmann::json(),
            };

            EXPECT_CALL(this->api, GetAssignedStands())
                .Times(1)
                .WillOnce(Return(assignments));

            this->handler.ProcessWebsocketMessage(message);
            ASSERT_EQ(this->handler.noStandAssigned, this->handler.GetAssignedStandForCallsign("BAW123"));
            ASSERT_EQ(this->handler.noStandAssigned, this->handler.GetAssignedStandForCallsign("VIR245"));
        }

        TEST_F(StandEventHandlerTest, ItHandlesNonArrayAssignmentsOnWebsocketConnection)
        {
            nlohmann::json assignments = nlohmann::json::array();
            assignments.push_back({
                {"callsign", "BAW123"},
                {"stand_id", 1},
            });
            assignments.push_back(nlohmann::json::object());
            WebsocketMessage message{
                "pusher:connection_established",
                "bla",
                nlohmann::json(),
            };

            EXPECT_CALL(this->api, GetAssignedStands())
                .Times(1)
                .WillOnce(Return(assignments));

            this->handler.ProcessWebsocketMessage(message);
            ASSERT_EQ(1, this->handler.GetAssignedStandForCallsign("BAW123"));
            ASSERT_EQ(this->handler.noStandAssigned, this->handler.GetAssignedStandForCallsign("VIR245"));
        }

        TEST_F(StandEventHandlerTest, ItHandlesAssignmentsWithMissingCallsignOnWebsocketConnection)
        {
            nlohmann::json assignments = nlohmann::json::array();
            assignments.push_back({
                {"callsign", "BAW123"},
                {"stand_id", 1},
            });
            assignments.push_back({
                {"stand_id", 2},
            });
            WebsocketMessage message{
                "pusher:connection_established",
                "bla",
                nlohmann::json(),
            };

            EXPECT_CALL(this->api, GetAssignedStands())
                .Times(1)
                .WillOnce(Return(assignments));

            this->handler.ProcessWebsocketMessage(message);
            ASSERT_EQ(1, this->handler.GetAssignedStandForCallsign("BAW123"));
            ASSERT_EQ(this->handler.noStandAssigned, this->handler.GetAssignedStandForCallsign("VIR245"));
        }

        TEST_F(StandEventHandlerTest, ItHandlesAssignmentsWithInvalidCallsignOnWebsocketConnection)
        {
            nlohmann::json assignments = nlohmann::json::array();
            assignments.push_back({
                {"callsign", "BAW123"},
                {"stand_id", 1},
            });
            assignments.push_back({
                {"callsign", 123},
                {"stand_id", 2},
            });
            WebsocketMessage message{
                "pusher:connection_established",
                "bla",
                nlohmann::json(),
            };

            EXPECT_CALL(this->api, GetAssignedStands())
                .Times(1)
                .WillOnce(Return(assignments));

            this->handler.ProcessWebsocketMessage(message);
            ASSERT_EQ(1, this->handler.GetAssignedStandForCallsign("BAW123"));
            ASSERT_EQ(this->handler.noStandAssigned, this->handler.GetAssignedStandForCallsign("VIR245"));
        }

        TEST_F(StandEventHandlerTest, ItHandlesAssignmentsWithMissingStandOnWebsocketConnection)
        {
            nlohmann::json assignments = nlohmann::json::array();
            assignments.push_back({
                {"callsign", "BAW123"},
                {"stand_id", 1},
                });
            assignments.push_back({
                {"callsign", "VIR245"},
            });
            WebsocketMessage message{
                "pusher:connection_established",
                "bla",
                nlohmann::json(),
            };

            EXPECT_CALL(this->api, GetAssignedStands())
                .Times(1)
                .WillOnce(Return(assignments));

            this->handler.ProcessWebsocketMessage(message);
            ASSERT_EQ(1, this->handler.GetAssignedStandForCallsign("BAW123"));
            ASSERT_EQ(this->handler.noStandAssigned, this->handler.GetAssignedStandForCallsign("VIR245"));
        }

        TEST_F(StandEventHandlerTest, ItHandlesAssignmentsWithInvalidStandOnWebsocketConnection)
        {
            nlohmann::json assignments = nlohmann::json::array();
            assignments.push_back({
                {"callsign", "BAW123"},
                {"stand_id", 1},
            });
            assignments.push_back({
                {"callsign", "VIR245"},
                {"stand_id", "2"},
            });
            WebsocketMessage message{
                "pusher:connection_established",
                "bla",
                nlohmann::json(),
            };

            EXPECT_CALL(this->api, GetAssignedStands())
                .Times(1)
                .WillOnce(Return(assignments));

            this->handler.ProcessWebsocketMessage(message);
            ASSERT_EQ(1, this->handler.GetAssignedStandForCallsign("BAW123"));
            ASSERT_EQ(this->handler.noStandAssigned, this->handler.GetAssignedStandForCallsign("VIR245"));
        }

        TEST_F(StandEventHandlerTest, ItHandlesAssignmentsWithNonExistentStandOnWebsocketConnection)
        {
            nlohmann::json assignments = nlohmann::json::array();
            assignments.push_back({
                {"callsign", "BAW123"},
                {"stand_id", 1},
            });
            assignments.push_back({
                {"callsign", "VIR245"},
                {"stand_id", -55},
            });
            WebsocketMessage message{
                "pusher:connection_established",
                "bla",
                nlohmann::json(),
            };

            EXPECT_CALL(this->api, GetAssignedStands())
                .Times(1)
                .WillOnce(Return(assignments));

            this->handler.ProcessWebsocketMessage(message);
            ASSERT_EQ(1, this->handler.GetAssignedStandForCallsign("BAW123"));
            ASSERT_EQ(this->handler.noStandAssigned, this->handler.GetAssignedStandForCallsign("VIR245"));
        }

        TEST_F(StandEventHandlerTest, ItHandlesApiExceptionsWhenFetchingStandsOnWebsocketConnection)
        {
            WebsocketMessage message{
                "pusher:connection_established",
                "bla",
                nlohmann::json(),
            };

            EXPECT_CALL(this->api, GetAssignedStands())
                .Times(1)
                .WillOnce(Throw(ApiException("Foo")));

            EXPECT_NO_THROW(this->handler.ProcessWebsocketMessage(message));
            ASSERT_EQ(this->handler.noStandAssigned, this->handler.GetAssignedStandForCallsign("BAW123"));
            ASSERT_EQ(this->handler.noStandAssigned, this->handler.GetAssignedStandForCallsign("VIR245"));
        }

        TEST_F(StandEventHandlerTest, ItTriggersTheStandSelectionMenuWithDepartureAirportIfWithinLimit)
        {
            PopupMenuItem menuItemStand1;
            menuItemStand1.firstValue = "1L";
            menuItemStand1.secondValue = "";
            menuItemStand1.callbackFunctionId = 1;
            menuItemStand1.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
            menuItemStand1.disabled = false;
            menuItemStand1.fixedPosition = false;

            PopupMenuItem menuItemStand2;
            menuItemStand2.firstValue = "55";
            menuItemStand2.secondValue = "";
            menuItemStand2.callbackFunctionId = 1;
            menuItemStand2.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
            menuItemStand2.disabled = false;
            menuItemStand2.fixedPosition = false;

            PopupMenuItem menuItemStandNone;
            menuItemStandNone.firstValue = "--";
            menuItemStandNone.secondValue = "";
            menuItemStandNone.callbackFunctionId = 1;
            menuItemStandNone.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
            menuItemStandNone.disabled = false;
            menuItemStandNone.fixedPosition = true;

            // Tracked by user
            ON_CALL(this->flightplan, IsTracked())
                .WillByDefault(Return(true));

            ON_CALL(this->flightplan, IsTrackedByUser())
                .WillByDefault(Return(true));

            ON_CALL(this->flightplan, GetCallsign())
                .WillByDefault(Return("BAW123"));

            ON_CALL(this->flightplan, GetOrigin())
                .WillByDefault(Return("EGKK"));

            ON_CALL(this->flightplan, GetDistanceFromOrigin())
                .WillByDefault(Return(4));

            RECT expectedArea = { 0, 0, 400, 600 };
            EXPECT_CALL(this->plugin, TriggerPopupList(RectEq(expectedArea), "Assign Stand at EGKK", 2))
                .Times(1);

            EXPECT_CALL(this->plugin, AddItemToPopupList(menuItemStand1))
                .Times(1);

            EXPECT_CALL(this->plugin, AddItemToPopupList(menuItemStand2))
                .Times(1);

            EXPECT_CALL(this->plugin, AddItemToPopupList(menuItemStandNone))
                .Times(1);

            this->handler.DisplayStandSelectionMenu(this->flightplan, this->radarTarget, "", { 0, 0 });
        }

        TEST_F(StandEventHandlerTest, ItTriggersTheStandSelectionMenuWithArrivalAirportIfNotCloseToDeparture)
        {
            PopupMenuItem menuItemStand;
            menuItemStand.firstValue = "317";
            menuItemStand.secondValue = "";
            menuItemStand.callbackFunctionId = 1;
            menuItemStand.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
            menuItemStand.disabled = false;
            menuItemStand.fixedPosition = false;

            PopupMenuItem menuItemStandNone;
            menuItemStandNone.firstValue = "--";
            menuItemStandNone.secondValue = "";
            menuItemStandNone.callbackFunctionId = 1;
            menuItemStandNone.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
            menuItemStandNone.disabled = false;
            menuItemStandNone.fixedPosition = true;

            // Not tracked by anyone
            ON_CALL(this->flightplan, IsTracked())
                .WillByDefault(Return(false));

            ON_CALL(this->flightplan, IsTrackedByUser())
                .WillByDefault(Return(false));

            ON_CALL(this->flightplan, GetCallsign())
                .WillByDefault(Return("BAW123"));

            ON_CALL(this->flightplan, GetOrigin())
                .WillByDefault(Return("EGKK"));

            ON_CALL(this->flightplan, GetDestination())
                .WillByDefault(Return("EGLL"));

            ON_CALL(this->flightplan, GetDistanceFromOrigin())
                .WillByDefault(Return(8));

            RECT expectedArea = { 0, 0, 400, 600 };
            EXPECT_CALL(this->plugin, TriggerPopupList(RectEq(expectedArea), "Assign Stand at EGLL", 2))
                .Times(1);

            EXPECT_CALL(this->plugin, AddItemToPopupList(menuItemStand))
                .Times(1);

            EXPECT_CALL(this->plugin, AddItemToPopupList(menuItemStandNone))
                .Times(1);

            this->handler.DisplayStandSelectionMenu(this->flightplan, this->radarTarget, "", { 0, 0 });
        }

        TEST_F(StandEventHandlerTest, ItDoesntTriggerTheSelectionMenuIfFlightplanTrackedBySomeoneElse)
        {
            PopupMenuItem menuItemStand1;
            menuItemStand1.firstValue = "1L";
            menuItemStand1.secondValue = "";
            menuItemStand1.callbackFunctionId = 1;
            menuItemStand1.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
            menuItemStand1.disabled = false;
            menuItemStand1.fixedPosition = false;

            PopupMenuItem menuItemStand2;
            menuItemStand2.firstValue = "55";
            menuItemStand2.secondValue = "";
            menuItemStand2.callbackFunctionId = 1;
            menuItemStand2.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
            menuItemStand2.disabled = false;
            menuItemStand2.fixedPosition = false;

            PopupMenuItem menuItemStandNone;
            menuItemStandNone.firstValue = "--";
            menuItemStandNone.secondValue = "";
            menuItemStandNone.callbackFunctionId = 1;
            menuItemStandNone.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
            menuItemStandNone.disabled = false;
            menuItemStandNone.fixedPosition = true;

            // Not tracked by user
            ON_CALL(this->flightplan, IsTracked())
                .WillByDefault(Return(true));

            ON_CALL(this->flightplan, IsTrackedByUser())
                .WillByDefault(Return(false));

            ON_CALL(this->flightplan, GetCallsign())
                .WillByDefault(Return("BAW123"));

            ON_CALL(this->flightplan, GetOrigin())
                .WillByDefault(Return("EGKK"));

            ON_CALL(this->flightplan, GetDistanceFromOrigin())
                .WillByDefault(Return(4));

            RECT expectedArea = { 0, 0, 400, 600 };
            EXPECT_CALL(this->plugin, TriggerPopupList(RectEq(expectedArea), "Assign Stand at EGKK", 2))
                .Times(0);

            EXPECT_CALL(this->plugin, AddItemToPopupList(_))
                .Times(0);

            this->handler.DisplayStandSelectionMenu(this->flightplan, this->radarTarget, "", { 0, 0 });
        }

        TEST_F(StandEventHandlerTest, ItDoesntMakeAStandAssignmentRequestIfFlightplanTrackedBySomeoneElse)
        {
            // Trigger the menu first to set the last airport
            ON_CALL(this->flightplan, IsTracked())
                .WillByDefault(Return(false));

            ON_CALL(this->flightplan, IsTrackedByUser())
                .WillByDefault(Return(false));

            ON_CALL(this->flightplan, GetCallsign())
                .WillByDefault(Return("BAW123"));

            ON_CALL(this->flightplan, GetOrigin())
                .WillByDefault(Return("EGKK"));

            ON_CALL(this->flightplan, GetDistanceFromOrigin())
                .WillByDefault(Return(4));

            this->handler.DisplayStandSelectionMenu(this->flightplan, this->radarTarget, "", { 0, 0 });

            std::shared_ptr<NiceMock<MockEuroScopeCFlightPlanInterface>> pluginReturnedFp
                = std::make_shared<NiceMock<MockEuroScopeCFlightPlanInterface>>();

            ON_CALL(*pluginReturnedFp, IsTracked())
                .WillByDefault(Return(true));

            ON_CALL(*pluginReturnedFp, IsTrackedByUser())
                .WillByDefault(Return(false));

            ON_CALL(*pluginReturnedFp, GetCallsign())
                .WillByDefault(Return("BAW123"));

            ON_CALL(this->plugin, GetSelectedFlightplan())
                .WillByDefault(Return(pluginReturnedFp));

            EXPECT_CALL(this->api, AssignStandToAircraft("BAW123", _))
                .Times(0);

            this->handler.StandSelected(1, "55", {});
            EXPECT_EQ(this->handler.noStandAssigned, this->handler.GetAssignedStandForCallsign("BAW123"));
        }

        TEST_F(StandEventHandlerTest, ItMakesAStandAssignmentRequestWhenStandSelected)
        {
            // Trigger the menu first to set the last airport
            ON_CALL(this->flightplan, IsTracked())
                .WillByDefault(Return(false));

            ON_CALL(this->flightplan, IsTrackedByUser())
                .WillByDefault(Return(false));

            ON_CALL(this->flightplan, GetCallsign())
                .WillByDefault(Return("BAW123"));

            ON_CALL(this->flightplan, GetOrigin())
                .WillByDefault(Return("EGKK"));

            ON_CALL(this->flightplan, GetDistanceFromOrigin())
                .WillByDefault(Return(4));

            this->handler.DisplayStandSelectionMenu(this->flightplan, this->radarTarget, "", { 0, 0 });

            std::shared_ptr<NiceMock<MockEuroScopeCFlightPlanInterface>> pluginReturnedFp
                = std::make_shared<NiceMock<MockEuroScopeCFlightPlanInterface>>();

            ON_CALL(*pluginReturnedFp, IsTracked())
                .WillByDefault(Return(false));

            ON_CALL(*pluginReturnedFp, IsTrackedByUser())
                .WillByDefault(Return(false));

            ON_CALL(*pluginReturnedFp, GetCallsign())
                .WillByDefault(Return("BAW123"));

            ON_CALL(this->plugin, GetSelectedFlightplan())
                .WillByDefault(Return(pluginReturnedFp));

            EXPECT_CALL(this->api, AssignStandToAircraft("BAW123", 2))
                .Times(1);

            this->handler.StandSelected(1, "55", {});
            EXPECT_EQ(2, this->handler.GetAssignedStandForCallsign("BAW123"));
        }

        TEST_F(StandEventHandlerTest, ItHandlesApiExceptionOnStandAssignment)
        {
            // Trigger the menu first to set the last airport
            ON_CALL(this->flightplan, IsTracked())
                .WillByDefault(Return(false));

            ON_CALL(this->flightplan, IsTrackedByUser())
                .WillByDefault(Return(false));

            ON_CALL(this->flightplan, GetCallsign())
                .WillByDefault(Return("BAW123"));

            ON_CALL(this->flightplan, GetOrigin())
                .WillByDefault(Return("EGKK"));

            ON_CALL(this->flightplan, GetDistanceFromOrigin())
                .WillByDefault(Return(4));

            this->handler.DisplayStandSelectionMenu(this->flightplan, this->radarTarget, "", { 0, 0 });

            std::shared_ptr<NiceMock<MockEuroScopeCFlightPlanInterface>> pluginReturnedFp
                = std::make_shared<NiceMock<MockEuroScopeCFlightPlanInterface>>();

            ON_CALL(*pluginReturnedFp, IsTracked())
                .WillByDefault(Return(false));

            ON_CALL(*pluginReturnedFp, IsTrackedByUser())
                .WillByDefault(Return(false));

            ON_CALL(*pluginReturnedFp, GetCallsign())
                .WillByDefault(Return("BAW123"));

            ON_CALL(this->plugin, GetSelectedFlightplan())
                .WillByDefault(Return(pluginReturnedFp));

            EXPECT_CALL(this->api, AssignStandToAircraft("BAW123", 2))
                .Times(1)
                .WillOnce(Throw(ApiException("Foo")));

            EXPECT_NO_THROW(this->handler.StandSelected(1, "55", {}));
            EXPECT_EQ(2, this->handler.GetAssignedStandForCallsign("BAW123"));
        }

        TEST_F(StandEventHandlerTest, ItDoesntAssignStandFromTheWrongAirfield)
        {
            // Trigger the menu first to set the last airport
            ON_CALL(this->flightplan, IsTracked())
                .WillByDefault(Return(false));

            ON_CALL(this->flightplan, IsTrackedByUser())
                .WillByDefault(Return(false));

            ON_CALL(this->flightplan, GetCallsign())
                .WillByDefault(Return("BAW123"));

            ON_CALL(this->flightplan, GetOrigin())
                .WillByDefault(Return("EGKK"));

            ON_CALL(this->flightplan, GetDistanceFromOrigin())
                .WillByDefault(Return(4));

            this->handler.DisplayStandSelectionMenu(this->flightplan, this->radarTarget, "", { 0, 0 });

            std::shared_ptr<NiceMock<MockEuroScopeCFlightPlanInterface>> pluginReturnedFp
                = std::make_shared<NiceMock<MockEuroScopeCFlightPlanInterface>>();

            ON_CALL(*pluginReturnedFp, IsTracked())
                .WillByDefault(Return(false));

            ON_CALL(*pluginReturnedFp, IsTrackedByUser())
                .WillByDefault(Return(false));

            ON_CALL(*pluginReturnedFp, GetCallsign())
                .WillByDefault(Return("BAW123"));

            ON_CALL(this->plugin, GetSelectedFlightplan())
                .WillByDefault(Return(pluginReturnedFp));

            EXPECT_CALL(this->api, AssignStandToAircraft("BAW123", _))
                .Times(0);

            this->handler.StandSelected(1, "317", {});
            EXPECT_EQ(this->handler.noStandAssigned, this->handler.GetAssignedStandForCallsign("BAW123"));
        }

        TEST_F(StandEventHandlerTest, ItDeletesStandAssignmentsIfSet)
        {
            this->handler.SetAssignedStand("BAW123", 2);

            // Trigger the menu first to set the last airport
            ON_CALL(this->flightplan, IsTracked())
                .WillByDefault(Return(false));

            ON_CALL(this->flightplan, IsTrackedByUser())
                .WillByDefault(Return(false));

            ON_CALL(this->flightplan, GetCallsign())
                .WillByDefault(Return("BAW123"));

            ON_CALL(this->flightplan, GetOrigin())
                .WillByDefault(Return("EGKK"));

            ON_CALL(this->flightplan, GetDistanceFromOrigin())
                .WillByDefault(Return(4));

            this->handler.DisplayStandSelectionMenu(this->flightplan, this->radarTarget, "", { 0, 0 });

            std::shared_ptr<NiceMock<MockEuroScopeCFlightPlanInterface>> pluginReturnedFp
                = std::make_shared<NiceMock<MockEuroScopeCFlightPlanInterface>>();

            ON_CALL(*pluginReturnedFp, IsTracked())
                .WillByDefault(Return(false));

            ON_CALL(*pluginReturnedFp, IsTrackedByUser())
                .WillByDefault(Return(false));

            ON_CALL(*pluginReturnedFp, GetCallsign())
                .WillByDefault(Return("BAW123"));

            ON_CALL(this->plugin, GetSelectedFlightplan())
                .WillByDefault(Return(pluginReturnedFp));

            EXPECT_CALL(this->api, DeleteStandAssignmentForAircraft("BAW123"))
                .Times(1);

            this->handler.StandSelected(1, "--", {});
            EXPECT_EQ(this->handler.noStandAssigned, this->handler.GetAssignedStandForCallsign("BAW123"));
        }

        TEST_F(StandEventHandlerTest, ItHandlesApiExceptionsInAssignmentDeletion)
        {
            this->handler.SetAssignedStand("BAW123", 2);

            // Trigger the menu first to set the last airport
            ON_CALL(this->flightplan, IsTracked())
                .WillByDefault(Return(false));

            ON_CALL(this->flightplan, IsTrackedByUser())
                .WillByDefault(Return(false));

            ON_CALL(this->flightplan, GetCallsign())
                .WillByDefault(Return("BAW123"));

            ON_CALL(this->flightplan, GetOrigin())
                .WillByDefault(Return("EGKK"));

            ON_CALL(this->flightplan, GetDistanceFromOrigin())
                .WillByDefault(Return(4));

            this->handler.DisplayStandSelectionMenu(this->flightplan, this->radarTarget, "", { 0, 0 });

            std::shared_ptr<NiceMock<MockEuroScopeCFlightPlanInterface>> pluginReturnedFp
                = std::make_shared<NiceMock<MockEuroScopeCFlightPlanInterface>>();

            ON_CALL(*pluginReturnedFp, IsTracked())
                .WillByDefault(Return(false));

            ON_CALL(*pluginReturnedFp, IsTrackedByUser())
                .WillByDefault(Return(false));

            ON_CALL(*pluginReturnedFp, GetCallsign())
                .WillByDefault(Return("BAW123"));

            ON_CALL(this->plugin, GetSelectedFlightplan())
                .WillByDefault(Return(pluginReturnedFp));

            EXPECT_CALL(this->api, DeleteStandAssignmentForAircraft("BAW123"))
                .Times(1)
                .WillOnce(Throw(ApiException("Foo")));

            EXPECT_NO_THROW(this->handler.StandSelected(1, "--", {}));
            EXPECT_EQ(this->handler.noStandAssigned, this->handler.GetAssignedStandForCallsign("BAW123"));
        }

        TEST_F(StandEventHandlerTest, ItDoesntMakeDeletionRequestIfNoStandAssigned)
        {
            // Trigger the menu first to set the last airport
            ON_CALL(this->flightplan, IsTracked())
                .WillByDefault(Return(false));

            ON_CALL(this->flightplan, IsTrackedByUser())
                .WillByDefault(Return(false));

            ON_CALL(this->flightplan, GetCallsign())
                .WillByDefault(Return("BAW123"));

            ON_CALL(this->flightplan, GetOrigin())
                .WillByDefault(Return("EGKK"));

            ON_CALL(this->flightplan, GetDistanceFromOrigin())
                .WillByDefault(Return(4));

            this->handler.DisplayStandSelectionMenu(this->flightplan, this->radarTarget, "", { 0, 0 });

            std::shared_ptr<NiceMock<MockEuroScopeCFlightPlanInterface>> pluginReturnedFp
                = std::make_shared<NiceMock<MockEuroScopeCFlightPlanInterface>>();

            ON_CALL(*pluginReturnedFp, IsTracked())
                .WillByDefault(Return(false));

            ON_CALL(*pluginReturnedFp, IsTrackedByUser())
                .WillByDefault(Return(false));

            ON_CALL(*pluginReturnedFp, GetCallsign())
                .WillByDefault(Return("BAW123"));

            ON_CALL(this->plugin, GetSelectedFlightplan())
                .WillByDefault(Return(pluginReturnedFp));

            EXPECT_CALL(this->api, DeleteStandAssignmentForAircraft(_))
                .Times(0);

            this->handler.StandSelected(1, "--", {});
            EXPECT_EQ(this->handler.noStandAssigned, this->handler.GetAssignedStandForCallsign("BAW123"));
        }
    }  // namespace Stands
}  // namespace UKControllerPluginTest
