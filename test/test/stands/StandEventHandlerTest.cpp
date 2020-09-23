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
#include "api/ApiException.h"

using ::testing::Test;
using UKControllerPlugin::Api::ApiException;
using UKControllerPlugin::Stands::StandEventHandler;
using UKControllerPlugin::Stands::CompareStands;
using UKControllerPlugin::Stands::Stand;
using UKControllerPlugin::Tag::TagData;
using UKControllerPlugin::Websocket::WebsocketSubscription;
using UKControllerPluginTest::Api::MockApiInterface;
using UKControllerPluginTest::TaskManager::MockTaskRunnerInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCRadarTargetInterface;
using UKControllerPlugin::Websocket::WebsocketMessage;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Throw;

namespace UKControllerPluginTest {
    namespace Stands {

        class StandEventHandlerTest : public Test
        {
            public:
                StandEventHandlerTest()
                    : handler(api, taskRunner, this->GetStands(), 1),
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
    }  // namespace Stands
}  // namespace UKControllerPluginTest
