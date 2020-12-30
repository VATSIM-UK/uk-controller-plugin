#include "pch/pch.h"
#include "stands/StandEventHandler.h"
#include "stands/CompareStands.h"
#include "stands/Stand.h"
#include "mock/MockEuroScopeCFlightplanInterface.h"
#include "mock/MockEuroScopeCRadarTargetInterface.h"
#include "mock/MockEuroScopeCControllerInterface.h"
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
using UKControllerPluginTest::Euroscope::MockEuroScopeCControllerInterface;
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

                    this->mockController = std::make_shared<NiceMock<MockEuroScopeCControllerInterface>>();
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
                std::shared_ptr<NiceMock<MockEuroScopeCControllerInterface>> mockController;
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

            std::shared_ptr<NiceMock<MockEuroScopeCFlightPlanInterface>> pluginReturnedFp
                = std::make_shared<NiceMock<MockEuroScopeCFlightPlanInterface>>();

            ON_CALL(this->plugin, GetFlightplanForCallsign("BAW123"))
                .WillByDefault(Return(pluginReturnedFp));

            EXPECT_CALL(*pluginReturnedFp, AnnotateFlightStrip(3, "1L"))
                .Times(1);

            this->handler.ProcessWebsocketMessage(message);
            ASSERT_EQ(1, this->handler.GetAssignedStandForCallsign("BAW123"));
        }

        TEST_F(StandEventHandlerTest, ItHandlesNoFlightplanForAnnotations)
        {
            WebsocketMessage message{
                "App\\Events\\StandAssignedEvent",
                "private-stand-assignments",
                nlohmann::json {
                    {"callsign", "BAW123"},
                    {"stand_id", 1},
                }
            };

            ON_CALL(this->plugin, GetFlightplanForCallsign("BAW123"))
                .WillByDefault(Return(nullptr));


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

        TEST_F(StandEventHandlerTest, ItHandlesNoFlightplanForUnassignmentRemovalOfAnnotations)
        {
            this->handler.SetAssignedStand("BAW123", 3);
            WebsocketMessage message{
                "App\\Events\\StandUnassignedEvent",
                "private-stand-assignments",
                nlohmann::json {
                    {"callsign", "BAW123"}
                }
            };

            ON_CALL(this->plugin, GetFlightplanForCallsign("BAW123"))
                .WillByDefault(Return(nullptr));

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

            std::shared_ptr<NiceMock<MockEuroScopeCFlightPlanInterface>> pluginReturnedFp
                = std::make_shared<NiceMock<MockEuroScopeCFlightPlanInterface>>();

            ON_CALL(this->plugin, GetFlightplanForCallsign("BAW123"))
                .WillByDefault(Return(pluginReturnedFp));

            EXPECT_CALL(*pluginReturnedFp, AnnotateFlightStrip(3, ""))
                .Times(1);

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

            std::shared_ptr<NiceMock<MockEuroScopeCFlightPlanInterface>> pluginReturnedFp1
                = std::make_shared<NiceMock<MockEuroScopeCFlightPlanInterface>>();

            ON_CALL(this->plugin, GetFlightplanForCallsign("BAW123"))
                .WillByDefault(Return(pluginReturnedFp1));

            EXPECT_CALL(*pluginReturnedFp1, AnnotateFlightStrip(3, "1L"))
                .Times(1);

            std::shared_ptr<NiceMock<MockEuroScopeCFlightPlanInterface>> pluginReturnedFp2
                = std::make_shared<NiceMock<MockEuroScopeCFlightPlanInterface>>();

            ON_CALL(this->plugin, GetFlightplanForCallsign("VIR245"))
                .WillByDefault(Return(pluginReturnedFp2));

            EXPECT_CALL(*pluginReturnedFp2, AnnotateFlightStrip(3, "55"))
                .Times(1);

            EXPECT_CALL(this->api, GetAssignedStands())
                .Times(1)
                .WillOnce(Return(assignments));

            this->handler.ProcessWebsocketMessage(message);
            ASSERT_EQ(1, this->handler.GetAssignedStandForCallsign("BAW123"));
            ASSERT_EQ(2, this->handler.GetAssignedStandForCallsign("VIR245"));
        }

        TEST_F(StandEventHandlerTest, ItClearsPreviousAssignmentsOnWebsocketConnection)
        {
            nlohmann::json assignments = nlohmann::json::array();
            assignments.push_back({
                {"callsign", "BAW123"},
                {"stand_id", 1},
            });
            WebsocketMessage message{
                "pusher:connection_established",
                "bla",
                nlohmann::json(),
            };

            EXPECT_CALL(this->api, GetAssignedStands())
                .Times(1)
                .WillOnce(Return(assignments));

            this->handler.SetAssignedStand("RYR234", 3);
            this->handler.ProcessWebsocketMessage(message);
            ASSERT_EQ(this->handler.noStandAssigned, this->handler.GetAssignedStandForCallsign("RYR234"));
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

            std::shared_ptr<NiceMock<MockEuroScopeCFlightPlanInterface>> pluginReturnedFp
                = std::make_shared<NiceMock<MockEuroScopeCFlightPlanInterface>>();

            ON_CALL(this->plugin, GetFlightplanForCallsign("BAW123"))
                .WillByDefault(Return(pluginReturnedFp));

            EXPECT_CALL(*pluginReturnedFp, AnnotateFlightStrip(3, "1L"))
                .Times(1);

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

            std::shared_ptr<NiceMock<MockEuroScopeCFlightPlanInterface>> pluginReturnedFp
                = std::make_shared<NiceMock<MockEuroScopeCFlightPlanInterface>>();

            ON_CALL(this->plugin, GetFlightplanForCallsign("BAW123"))
                .WillByDefault(Return(pluginReturnedFp));

            EXPECT_CALL(*pluginReturnedFp, AnnotateFlightStrip(3, "1L"))
                .Times(1);

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

            std::shared_ptr<NiceMock<MockEuroScopeCFlightPlanInterface>> pluginReturnedFp
                = std::make_shared<NiceMock<MockEuroScopeCFlightPlanInterface>>();

            ON_CALL(this->plugin, GetFlightplanForCallsign("BAW123"))
                .WillByDefault(Return(pluginReturnedFp));

            EXPECT_CALL(*pluginReturnedFp, AnnotateFlightStrip(3, "1L"))
                .Times(1);

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

            std::shared_ptr<NiceMock<MockEuroScopeCFlightPlanInterface>> pluginReturnedFp
                = std::make_shared<NiceMock<MockEuroScopeCFlightPlanInterface>>();

            ON_CALL(this->plugin, GetFlightplanForCallsign("BAW123"))
                .WillByDefault(Return(pluginReturnedFp));

            EXPECT_CALL(*pluginReturnedFp, AnnotateFlightStrip(3, "1L"))
                .Times(1);

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

            std::shared_ptr<NiceMock<MockEuroScopeCFlightPlanInterface>> pluginReturnedFp
                = std::make_shared<NiceMock<MockEuroScopeCFlightPlanInterface>>();

            ON_CALL(this->plugin, GetFlightplanForCallsign("BAW123"))
                .WillByDefault(Return(pluginReturnedFp));

            EXPECT_CALL(*pluginReturnedFp, AnnotateFlightStrip(3, "1L"))
                .Times(1);

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

            std::shared_ptr<NiceMock<MockEuroScopeCFlightPlanInterface>> pluginReturnedFp
                = std::make_shared<NiceMock<MockEuroScopeCFlightPlanInterface>>();

            ON_CALL(this->plugin, GetFlightplanForCallsign("BAW123"))
                .WillByDefault(Return(pluginReturnedFp));

            EXPECT_CALL(*pluginReturnedFp, AnnotateFlightStrip(3, "1L"))
                .Times(1);

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
            menuItemStand1.secondValue = "EGKK";
            menuItemStand1.callbackFunctionId = 1;
            menuItemStand1.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
            menuItemStand1.disabled = false;
            menuItemStand1.fixedPosition = false;

            PopupMenuItem menuItemStand2;
            menuItemStand2.firstValue = "55";
            menuItemStand2.secondValue = "EGKK";
            menuItemStand2.callbackFunctionId = 1;
            menuItemStand2.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
            menuItemStand2.disabled = false;
            menuItemStand2.fixedPosition = false;

            PopupMenuItem menuItemStandNone;
            menuItemStandNone.firstValue = "--";
            menuItemStandNone.secondValue = "None";
            menuItemStandNone.callbackFunctionId = 1;
            menuItemStandNone.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
            menuItemStandNone.disabled = false;
            menuItemStandNone.fixedPosition = true;

            // Tracked by user
            ON_CALL(this->flightplan, IsTracked())
                .WillByDefault(Return(true));

            ON_CALL(this->flightplan, IsTrackedByUser())
                .WillByDefault(Return(true));

            ON_CALL(this->plugin, GetUserControllerObject())
                .WillByDefault(Return(this->mockController));

            ON_CALL(*this->mockController, IsVatsimRecognisedController())
                .WillByDefault(Return(true));

            ON_CALL(this->flightplan, GetCallsign())
                .WillByDefault(Return("BAW123"));

            ON_CALL(this->flightplan, GetOrigin())
                .WillByDefault(Return("EGKK"));

            ON_CALL(this->flightplan, GetDistanceFromOrigin())
                .WillByDefault(Return(4));

            RECT expectedArea = { 0, 0, 400, 600 };
            EXPECT_CALL(this->plugin, TriggerPopupList(RectEq(expectedArea), "Assign Stand", 2))
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
            menuItemStand.secondValue = "EGLL";
            menuItemStand.callbackFunctionId = 1;
            menuItemStand.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
            menuItemStand.disabled = false;
            menuItemStand.fixedPosition = false;

            PopupMenuItem menuItemStandNone;
            menuItemStandNone.firstValue = "--";
            menuItemStandNone.secondValue = "None";
            menuItemStandNone.callbackFunctionId = 1;
            menuItemStandNone.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
            menuItemStandNone.disabled = false;
            menuItemStandNone.fixedPosition = true;

            // Not tracked by anyone
            ON_CALL(this->flightplan, IsTracked())
                .WillByDefault(Return(false));

            ON_CALL(this->flightplan, IsTrackedByUser())
                .WillByDefault(Return(false));

            ON_CALL(this->plugin, GetUserControllerObject())
                .WillByDefault(Return(this->mockController));

            ON_CALL(*this->mockController, IsVatsimRecognisedController())
                .WillByDefault(Return(true));

            ON_CALL(this->flightplan, GetCallsign())
                .WillByDefault(Return("BAW123"));

            ON_CALL(this->flightplan, GetOrigin())
                .WillByDefault(Return("EGKK"));

            ON_CALL(this->flightplan, GetDestination())
                .WillByDefault(Return("EGLL"));

            ON_CALL(this->flightplan, GetDistanceFromOrigin())
                .WillByDefault(Return(8));

            RECT expectedArea = { 0, 0, 400, 600 };
            EXPECT_CALL(this->plugin, TriggerPopupList(RectEq(expectedArea), "Assign Stand", 2))
                .Times(1);

            EXPECT_CALL(this->plugin, AddItemToPopupList(menuItemStand))
                .Times(1);

            EXPECT_CALL(this->plugin, AddItemToPopupList(menuItemStandNone))
                .Times(1);

            this->handler.DisplayStandSelectionMenu(this->flightplan, this->radarTarget, "", { 0, 0 });
        }

        TEST_F(StandEventHandlerTest, ItDoesntTriggerTheSelectionMenuIfFlightplanTrackedBySomeoneElse)
        {
            // Not tracked by user
            ON_CALL(this->flightplan, IsTracked())
                .WillByDefault(Return(true));

            ON_CALL(this->flightplan, IsTrackedByUser())
                .WillByDefault(Return(false));

            ON_CALL(this->plugin, GetUserControllerObject())
                .WillByDefault(Return(this->mockController));

            ON_CALL(*this->mockController, IsVatsimRecognisedController())
                .WillByDefault(Return(true));

            ON_CALL(this->flightplan, GetCallsign())
                .WillByDefault(Return("BAW123"));

            ON_CALL(this->flightplan, GetOrigin())
                .WillByDefault(Return("EGKK"));

            ON_CALL(this->flightplan, GetDistanceFromOrigin())
                .WillByDefault(Return(4));

            RECT expectedArea = { 0, 0, 400, 600 };
            EXPECT_CALL(this->plugin, TriggerPopupList(RectEq(expectedArea), "Assign Stand", 2))
                .Times(0);

            EXPECT_CALL(this->plugin, AddItemToPopupList(_))
                .Times(0);

            this->handler.DisplayStandSelectionMenu(this->flightplan, this->radarTarget, "", { 0, 0 });
        }

        TEST_F(StandEventHandlerTest, ItDoesntTriggerTheSelectionMenuIfNotVatsimRecognisedController)
        {
            // Not tracked by user
            ON_CALL(this->flightplan, IsTracked())
                .WillByDefault(Return(false));

            ON_CALL(this->flightplan, IsTrackedByUser())
                .WillByDefault(Return(false));

            ON_CALL(this->plugin, GetUserControllerObject())
                .WillByDefault(Return(this->mockController));

            ON_CALL(*this->mockController, IsVatsimRecognisedController())
                .WillByDefault(Return(false));

            ON_CALL(this->flightplan, GetCallsign())
                .WillByDefault(Return("BAW123"));

            ON_CALL(this->flightplan, GetOrigin())
                .WillByDefault(Return("EGKK"));

            ON_CALL(this->flightplan, GetDistanceFromOrigin())
                .WillByDefault(Return(4));

            RECT expectedArea = { 0, 0, 400, 600 };
            EXPECT_CALL(this->plugin, TriggerPopupList(RectEq(expectedArea), "Assign Stand", 2))
                .Times(0);

            EXPECT_CALL(this->plugin, AddItemToPopupList(_))
                .Times(0);

            this->handler.DisplayStandSelectionMenu(this->flightplan, this->radarTarget, "", { 0, 0 });
        }

        TEST_F(StandEventHandlerTest, ItTriggersTheStandEditBoxWithDepartureAirportIfWithinLimit)
        {
            // Tracked by user
            ON_CALL(this->flightplan, IsTracked())
                .WillByDefault(Return(true));

            ON_CALL(this->flightplan, IsTrackedByUser())
                .WillByDefault(Return(true));

            ON_CALL(this->plugin, GetUserControllerObject())
                .WillByDefault(Return(this->mockController));

            ON_CALL(*this->mockController, IsVatsimRecognisedController())
                .WillByDefault(Return(true));

            ON_CALL(this->flightplan, GetCallsign())
                .WillByDefault(Return("BAW123"));

            ON_CALL(this->flightplan, GetOrigin())
                .WillByDefault(Return("EGKK"));

            ON_CALL(this->flightplan, GetDistanceFromOrigin())
                .WillByDefault(Return(4));

            RECT expectedArea = { 0, 0, 80, 25 };
            EXPECT_CALL(this->plugin, ShowTextEditPopup(RectEq(expectedArea), 1, ""))
                .Times(1);

            this->handler.DisplayStandAssignmentEditBox(this->flightplan, this->radarTarget, "", { 0, 0 });
            EXPECT_EQ("EGKK", this->handler.GetLastAirfield());
        }

        TEST_F(StandEventHandlerTest, ItTriggersTheStandEditBoxWithArrivalAirportIfNotCloseToDeparture)
        {
            // Not tracked by anyone
            ON_CALL(this->flightplan, IsTracked())
                .WillByDefault(Return(false));

            ON_CALL(this->flightplan, IsTrackedByUser())
                .WillByDefault(Return(false));

            ON_CALL(this->plugin, GetUserControllerObject())
                .WillByDefault(Return(this->mockController));

            ON_CALL(*this->mockController, IsVatsimRecognisedController())
                .WillByDefault(Return(true));

            ON_CALL(this->flightplan, GetCallsign())
                .WillByDefault(Return("BAW123"));

            ON_CALL(this->flightplan, GetOrigin())
                .WillByDefault(Return("EGKK"));

            ON_CALL(this->flightplan, GetDestination())
                .WillByDefault(Return("EGLL"));

            ON_CALL(this->flightplan, GetDistanceFromOrigin())
                .WillByDefault(Return(8));

            RECT expectedArea = { 0, 0, 80, 25 };
            EXPECT_CALL(this->plugin, ShowTextEditPopup(RectEq(expectedArea), 1, ""))
                .Times(1);

            this->handler.DisplayStandAssignmentEditBox(this->flightplan, this->radarTarget, "", { 0, 0 });
            EXPECT_EQ("EGLL", this->handler.GetLastAirfield());
        }

        TEST_F(StandEventHandlerTest, ItDoesntTriggerTheEditBoxIfFlightplanTrackedBySomeoneElse)
        {
            // Not tracked by user
            ON_CALL(this->flightplan, IsTracked())
                .WillByDefault(Return(true));

            ON_CALL(this->flightplan, IsTrackedByUser())
                .WillByDefault(Return(false));

            ON_CALL(this->plugin, GetUserControllerObject())
                .WillByDefault(Return(this->mockController));

            ON_CALL(*this->mockController, IsVatsimRecognisedController())
                .WillByDefault(Return(true));

            ON_CALL(this->flightplan, GetCallsign())
                .WillByDefault(Return("BAW123"));

            ON_CALL(this->flightplan, GetOrigin())
                .WillByDefault(Return("EGKK"));

            ON_CALL(this->flightplan, GetDistanceFromOrigin())
                .WillByDefault(Return(4));

            EXPECT_CALL(this->plugin, ShowTextEditPopup(_, _, _))
                .Times(0);

            this->handler.DisplayStandAssignmentEditBox(this->flightplan, this->radarTarget, "", { 0, 0 });
            EXPECT_EQ("", this->handler.GetLastAirfield());
        }

        TEST_F(StandEventHandlerTest, ItDoesntTriggerTheEditBoxIfNotVatsimRecognisedController)
        {
            // Not tracked by user
            ON_CALL(this->flightplan, IsTracked())
                .WillByDefault(Return(true));

            ON_CALL(this->flightplan, IsTrackedByUser())
                .WillByDefault(Return(false));

            ON_CALL(this->plugin, GetUserControllerObject())
                .WillByDefault(Return(this->mockController));

            ON_CALL(*this->mockController, IsVatsimRecognisedController())
                .WillByDefault(Return(false));

            ON_CALL(this->flightplan, GetCallsign())
                .WillByDefault(Return("BAW123"));

            ON_CALL(this->flightplan, GetOrigin())
                .WillByDefault(Return("EGKK"));

            ON_CALL(this->flightplan, GetDistanceFromOrigin())
                .WillByDefault(Return(4));

            EXPECT_CALL(this->plugin, ShowTextEditPopup(_, _, _))
                .Times(0);

            this->handler.DisplayStandAssignmentEditBox(this->flightplan, this->radarTarget, "", { 0, 0 });
            EXPECT_EQ("", this->handler.GetLastAirfield());
        }

        TEST_F(StandEventHandlerTest, ItTriggersTheStandEditBoxWithCurrentStandIfOneAssigned)
        {
            this->handler.SetAssignedStand("BAW123", 1);

            // Not tracked by anyone
            ON_CALL(this->flightplan, IsTracked())
                .WillByDefault(Return(false));

            ON_CALL(this->flightplan, IsTrackedByUser())
                .WillByDefault(Return(false));

            ON_CALL(this->plugin, GetUserControllerObject())
                .WillByDefault(Return(this->mockController));

            ON_CALL(*this->mockController, IsVatsimRecognisedController())
                .WillByDefault(Return(true));

            ON_CALL(this->flightplan, GetCallsign())
                .WillByDefault(Return("BAW123"));

            ON_CALL(this->flightplan, GetOrigin())
                .WillByDefault(Return("EGKK"));

            ON_CALL(this->flightplan, GetDestination())
                .WillByDefault(Return("EGLL"));

            ON_CALL(this->flightplan, GetDistanceFromOrigin())
                .WillByDefault(Return(8));

            RECT expectedArea = { 0, 0, 80, 25 };
            EXPECT_CALL(this->plugin, ShowTextEditPopup(RectEq(expectedArea), 1, "1L"))
                .Times(1);

            this->handler.DisplayStandAssignmentEditBox(this->flightplan, this->radarTarget, "", { 0, 0 });
        }

        TEST_F(StandEventHandlerTest, ItDoesntMakeAStandAssignmentRequestIfFlightplanTrackedBySomeoneElse)
        {
            // Trigger the menu first to set the last airport
            ON_CALL(this->flightplan, IsTracked())
                .WillByDefault(Return(true));

            ON_CALL(this->flightplan, IsTrackedByUser())
                .WillByDefault(Return(false));

            ON_CALL(this->plugin, GetUserControllerObject())
                .WillByDefault(Return(this->mockController));

            ON_CALL(*this->mockController, IsVatsimRecognisedController())
                .WillByDefault(Return(true));

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

        TEST_F(StandEventHandlerTest, ItDoesntMakeAStandAssignmentRequestIfNotVatsimRecognisedController)
        {
            // Trigger the menu first to set the last airport
            ON_CALL(this->flightplan, IsTracked())
                .WillByDefault(Return(false));

            ON_CALL(this->flightplan, IsTrackedByUser())
                .WillByDefault(Return(false));

            ON_CALL(this->plugin, GetUserControllerObject())
                .WillByDefault(Return(this->mockController));

            ON_CALL(*this->mockController, IsVatsimRecognisedController())
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

        TEST_F(StandEventHandlerTest, ItDoesntMakeAStandAssignmentRequestIfMenuOrEditBoxWasntPreviouslyTriggered)
        {
            ON_CALL(this->plugin, GetUserControllerObject())
                .WillByDefault(Return(this->mockController));

            ON_CALL(*this->mockController, IsVatsimRecognisedController())
                .WillByDefault(Return(true));

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

            ON_CALL(this->plugin, GetUserControllerObject())
                .WillByDefault(Return(this->mockController));

            ON_CALL(*this->mockController, IsVatsimRecognisedController())
                .WillByDefault(Return(true));

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

            ON_CALL(this->plugin, GetFlightplanForCallsign("BAW123"))
                .WillByDefault(Return(pluginReturnedFp));

            EXPECT_CALL(*pluginReturnedFp, AnnotateFlightStrip(3, "55"))
                .Times(1);

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

            ON_CALL(this->plugin, GetUserControllerObject())
                .WillByDefault(Return(this->mockController));

            ON_CALL(*this->mockController, IsVatsimRecognisedController())
                .WillByDefault(Return(true));

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

            ON_CALL(this->plugin, GetFlightplanForCallsign("BAW123"))
                .WillByDefault(Return(pluginReturnedFp));

            EXPECT_CALL(*pluginReturnedFp, AnnotateFlightStrip(3, "55"))
                .Times(1);

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

            ON_CALL(this->plugin, GetUserControllerObject())
                .WillByDefault(Return(this->mockController));

            ON_CALL(*this->mockController, IsVatsimRecognisedController())
                .WillByDefault(Return(true));

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

            ON_CALL(this->plugin, GetUserControllerObject())
                .WillByDefault(Return(this->mockController));

            ON_CALL(*this->mockController, IsVatsimRecognisedController())
                .WillByDefault(Return(true));

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

            ON_CALL(this->plugin, GetFlightplanForCallsign("BAW123"))
                .WillByDefault(Return(pluginReturnedFp));

            EXPECT_CALL(this->api, DeleteStandAssignmentForAircraft("BAW123"))
                .Times(1);

            EXPECT_CALL(*pluginReturnedFp, AnnotateFlightStrip(3, ""))
                .Times(1);

            this->handler.StandSelected(1, "--", {});
            EXPECT_EQ(this->handler.noStandAssigned, this->handler.GetAssignedStandForCallsign("BAW123"));
        }

        TEST_F(StandEventHandlerTest, ItDeletesStandAssignmentsViaTheEditBoxIfSet)
        {
            this->handler.SetAssignedStand("BAW123", 2);

            // Trigger the menu first to set the last airport
            ON_CALL(this->flightplan, IsTracked())
                .WillByDefault(Return(false));

            ON_CALL(this->flightplan, IsTrackedByUser())
                .WillByDefault(Return(false));

            ON_CALL(this->plugin, GetUserControllerObject())
                .WillByDefault(Return(this->mockController));

            ON_CALL(*this->mockController, IsVatsimRecognisedController())
                .WillByDefault(Return(true));

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

            ON_CALL(this->plugin, GetFlightplanForCallsign("BAW123"))
                .WillByDefault(Return(pluginReturnedFp));

            EXPECT_CALL(this->api, DeleteStandAssignmentForAircraft("BAW123"))
                .Times(1);

            EXPECT_CALL(*pluginReturnedFp, AnnotateFlightStrip(3, ""))
                .Times(1);

            this->handler.StandSelected(1, "", {});
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

            ON_CALL(this->plugin, GetUserControllerObject())
                .WillByDefault(Return(this->mockController));

            ON_CALL(*this->mockController, IsVatsimRecognisedController())
                .WillByDefault(Return(true));

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

        TEST_F(StandEventHandlerTest, ItDoesntDeleteStandAssignmentIfNotDoneViaMenuOrEditBox)
        {
            this->handler.SetAssignedStand("BAW123", 2);

            ON_CALL(this->plugin, GetUserControllerObject())
                .WillByDefault(Return(this->mockController));

            ON_CALL(*this->mockController, IsVatsimRecognisedController())
                .WillByDefault(Return(true));

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
                .Times(0);

            this->handler.StandSelected(1, "", {});
            EXPECT_EQ(2, this->handler.GetAssignedStandForCallsign("BAW123"));
        }

        TEST_F(StandEventHandlerTest, ItDoesntMakeDeletionRequestIfNoStandAssigned)
        {
            // Trigger the menu first to set the last airport
            ON_CALL(this->flightplan, IsTracked())
                .WillByDefault(Return(false));

            ON_CALL(this->flightplan, IsTrackedByUser())
                .WillByDefault(Return(false));

            ON_CALL(this->plugin, GetUserControllerObject())
                .WillByDefault(Return(this->mockController));

            ON_CALL(*this->mockController, IsVatsimRecognisedController())
                .WillByDefault(Return(true));

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

        TEST_F(StandEventHandlerTest, ItDoesntMakeDeletionRequestIfFlightplanTrackedBySomeoneElse)
        {
            this->handler.SetAssignedStand("BAW123", 2);

            // Trigger the menu first to set the last airport
            ON_CALL(this->flightplan, IsTracked())
                .WillByDefault(Return(false));

            ON_CALL(this->flightplan, IsTrackedByUser())
                .WillByDefault(Return(false));

            ON_CALL(this->plugin, GetUserControllerObject())
                .WillByDefault(Return(this->mockController));

            ON_CALL(*this->mockController, IsVatsimRecognisedController())
                .WillByDefault(Return(true));

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

            EXPECT_CALL(this->api, DeleteStandAssignmentForAircraft(_))
                .Times(0);

            this->handler.StandSelected(1, "--", {});
            EXPECT_EQ(2, this->handler.GetAssignedStandForCallsign("BAW123"));
        }

        TEST_F(StandEventHandlerTest, ItDoesntMakeDeletionRequestIfNotVatsimRecognisedController)
        {
            this->handler.SetAssignedStand("BAW123", 2);

            // Trigger the menu first to set the last airport
            ON_CALL(this->flightplan, IsTracked())
                .WillByDefault(Return(false));

            ON_CALL(this->flightplan, IsTrackedByUser())
                .WillByDefault(Return(false));

            ON_CALL(this->plugin, GetUserControllerObject())
                .WillByDefault(Return(this->mockController));

            ON_CALL(*this->mockController, IsVatsimRecognisedController())
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
            EXPECT_EQ(2, this->handler.GetAssignedStandForCallsign("BAW123"));
        }

        TEST_F(StandEventHandlerTest, ItUpdatesFlightplanAnnotationsOnEvent)
        {
            this->handler.SetAssignedStand("BAW123", 2);

            ON_CALL(this->flightplan, GetCallsign())
                .WillByDefault(Return("BAW123"));

            ON_CALL(this->flightplan, GetAnnotation(3))
                .WillByDefault(Return("LOL"));

            EXPECT_CALL(this->flightplan, AnnotateFlightStrip(3, "55"))
                .Times(1);

            this->handler.FlightPlanEvent(this->flightplan, this->radarTarget);
        }

        TEST_F(StandEventHandlerTest, ItDoesntUpdateFlightplanAnnotationsIfAlreadySet)
        {
            this->handler.SetAssignedStand("BAW123", 2);

            ON_CALL(this->flightplan, GetCallsign())
                .WillByDefault(Return("BAW123"));

            ON_CALL(this->flightplan, GetAnnotation(3))
                .WillByDefault(Return("55"));

            EXPECT_CALL(this->flightplan, AnnotateFlightStrip(3, _))
                .Times(0);

            this->handler.FlightPlanEvent(this->flightplan, this->radarTarget);
        }

        TEST_F(StandEventHandlerTest, ItDoesntUpdateFlightplanAnnotationsIfNoAssignment)
        {
            ON_CALL(this->flightplan, GetCallsign())
                .WillByDefault(Return("BAW123"));

            ON_CALL(this->flightplan, GetAnnotation(3))
                .WillByDefault(Return("55"));

            EXPECT_CALL(this->flightplan, AnnotateFlightStrip(3, _))
                .Times(0);

            this->handler.FlightPlanEvent(this->flightplan, this->radarTarget);
        }

        TEST_F(StandEventHandlerTest, ItDoesntProcessMessageNotForStands)
        {
            EXPECT_FALSE(this->handler.ProcessMessage("NOTSTANDS:BAW123:EGLL:317"));
        }

        TEST_F(StandEventHandlerTest, ItDoesntProcessMessageWithTooFewParts)
        {
            EXPECT_FALSE(this->handler.ProcessMessage("STANDS:BAW123:317"));
        }

        TEST_F(StandEventHandlerTest, ItDoesntProcessMessageWithTooManyParts)
        {
            EXPECT_FALSE(this->handler.ProcessMessage("STANDS:BAW123:EGLL:317:FOO"));
        }

        TEST_F(StandEventHandlerTest, ItMakesAStandAssignmentFromExternalMessage)
        {
            ON_CALL(this->plugin, GetUserControllerObject())
                .WillByDefault(Return(this->mockController));

            ON_CALL(*this->mockController, IsVatsimRecognisedController())
                .WillByDefault(Return(true));

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

            ON_CALL(this->plugin, GetFlightplanForCallsign("BAW123"))
                .WillByDefault(Return(pluginReturnedFp));

            EXPECT_CALL(*pluginReturnedFp, AnnotateFlightStrip(3, "55"))
                .Times(1);

            EXPECT_CALL(this->api, AssignStandToAircraft("BAW123", 2))
                .Times(1);

            EXPECT_TRUE(this->handler.ProcessMessage("STANDS:BAW123:EGKK:55"));
        }

        TEST_F(StandEventHandlerTest, ItHandlesInvalidFlightplanOnExternalMessage)
        {
            std::shared_ptr<NiceMock<MockEuroScopeCFlightPlanInterface>> pluginReturnedFp = nullptr;

            EXPECT_CALL(this->api, AssignStandToAircraft("BAW123", 2))
                .Times(0);

            EXPECT_TRUE(this->handler.ProcessMessage("STANDS:BAW123:EGKK:55"));
        }

        TEST_F(StandEventHandlerTest, ItDoesntAssignNonExistentStandsFromExternalMessage)
        {
            ON_CALL(this->plugin, GetUserControllerObject())
                .WillByDefault(Return(this->mockController));

            ON_CALL(*this->mockController, IsVatsimRecognisedController())
                .WillByDefault(Return(true));

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

            ON_CALL(this->plugin, GetFlightplanForCallsign("BAW123"))
                .WillByDefault(Return(pluginReturnedFp));

            EXPECT_CALL(*pluginReturnedFp, AnnotateFlightStrip(3, "55"))
                .Times(0);

            EXPECT_CALL(this->api, AssignStandToAircraft("BAW123", 2))
                .Times(0);

            EXPECT_TRUE(this->handler.ProcessMessage("STANDS:BAW123:EGKK:XXX"));
        }
    }  // namespace Stands
}  // namespace UKControllerPluginTest
