#include "stands/StandEventHandler.h"
#include "stands/StandColourConfiguration.h"
#include "stands/StandAssignmentSource.h"
#include <array>
#include <cstddef>
#include "tag/TagData.h"
#include "api/ApiException.h"
#include "controller/ActiveCallsign.h"
#include "controller/ControllerPosition.h"
#include "euroscope/UserSetting.h"
#include "ownership/AirfieldServiceProviderCollection.h"
#include "ownership/ServiceProvision.h"
#include "stands/StandUnassignedMessage.h"
#include "stands/StandAssignedMessage.h"
#include "integration/InboundMessage.h"

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;
using ::testing::Throw;
using UKControllerPlugin::Api::ApiException;
using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::Integration::InboundMessage;
using UKControllerPlugin::Integration::MessageType;
using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPlugin::Push::PushEvent;
using UKControllerPlugin::Push::PushEventSubscription;
using UKControllerPlugin::Stands::CompareStands;
using UKControllerPlugin::Stands::Stand;
using UKControllerPlugin::Stands::StandAssignedMessage;
using UKControllerPlugin::Stands::StandAssignment;
using UKControllerPlugin::Stands::StandColourConfiguration;
using UKControllerPlugin::Stands::StandEventHandler;
using UKControllerPlugin::Stands::StandUnassignedMessage;
using UKControllerPlugin::Tag::TagData;
using UKControllerPluginTest::Api::MockApiInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCControllerInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCRadarTargetInterface;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPluginTest::Euroscope::MockUserSettingProviderInterface;
using UKControllerPluginTest::Integration::MockOutboundIntegrationEventHandler;
using UKControllerPluginTest::TaskManager::MockTaskRunnerInterface;

namespace UKControllerPluginTest {
    namespace Stands {

        class StandEventHandlerTest : public ApiTestCase
        {
            public:
            [[nodiscard]] static auto
            MakeActiveCallsign(const UKControllerPlugin::Controller::ControllerPosition& controller, bool isUser)
                -> std::shared_ptr<UKControllerPlugin::Controller::ActiveCallsign>
            {
                return std::make_shared<UKControllerPlugin::Controller::ActiveCallsign>(
                    "LON_S_CTR", "Test", controller, isUser);
            }

            StandEventHandlerTest()
                : controller(2, "LON_S_CTR", 129.420, std::vector<std::string>{"EGKK"}, true, false, true),
                  userCallsign(MakeActiveCallsign(controller, true)),
                  notUserCallsign(MakeActiveCallsign(controller, false)),
                  airfieldOwnership(
                      std::make_shared<UKControllerPlugin::Ownership::AirfieldServiceProviderCollection>()),
                  userSetting(mockUserSettingProvider),
                  colourConfiguration(std::make_shared<StandColourConfiguration>(userSetting)),
                  tagData(flightplan, radarTarget, 110, 1, itemString, &euroscopeColourCode, &tagColour, &fontSize),
                  handler(api, taskRunner, plugin, &userSetting, mockIntegration, airfieldOwnership, GetStands(), 1)
            {
                ON_CALL(this->flightplan, GetCallsign()).WillByDefault(Return("BAW123"));
                ON_CALL(this->mockUserSettingProvider, KeyExists(_)).WillByDefault(Return(false));

                this->mockController = std::make_shared<NiceMock<MockEuroScopeCControllerInterface>>();
            }

            static std::set<Stand, CompareStands> GetStands()
            {
                std::set<Stand, CompareStands> stands;
                stands.insert({1, "EGKK", "1L"});
                stands.insert({2, "EGKK", "55"});
                stands.insert({3, "EGLL", "317"});
                return stands;
            }

            void SetAssignedStandWithSource(int standId, StandAssignment::Source source)
            {
                this->handler.SetAssignedStand("BAW123", standId, source);
            }

            auto CreateSourceTagData() -> TagData
            {
                return TagData(
                    this->flightplan,
                    this->radarTarget,
                    StandEventHandler::standAssignmentSourceTagItemId,
                    1,
                    this->itemString,
                    &this->euroscopeColourCode,
                    &this->tagColour,
                    &this->fontSize);
            }

            void ExpectSourceTagItemShorthand(StandAssignment::Source source, const std::string& expectedShorthand)
            {
                this->SetAssignedStandWithSource(1, source);
                auto sourceTagData = this->CreateSourceTagData();
                this->handler.SetTagItemData(sourceTagData);
                EXPECT_EQ(expectedShorthand, sourceTagData.GetItemString());
            }

            void ExpectTagItem110ColourForSource(StandAssignment::Source source)
            {
                this->SetAssignedStandWithSource(3, source);
                COLORREF expectedColour = this->colourConfiguration->GetColourForSource(source);
                this->handler.SetTagItemData(this->tagData);
                EXPECT_EQ(expectedColour, this->tagData.GetTagColour());
            }

            void ExpectSourceTagItemColourForSource(StandAssignment::Source source)
            {
                this->SetAssignedStandWithSource(3, source);
                COLORREF expectedColour = this->colourConfiguration->GetColourForSource(source);
                auto sourceTagData = this->CreateSourceTagData();
                this->handler.SetTagItemData(sourceTagData);
                EXPECT_EQ(expectedColour, sourceTagData.GetTagColour());
            }

            // Sets IsTracked, IsTrackedByUser, IsVatsimRecognisedController,
            // GetOrigin("EGKK") and GetDistanceFromOrigin on the fixture flightplan.
            void SetupFlightplan(bool tracked, bool trackedByUser, bool vatsim = true, double distance = 4)
            {
                ON_CALL(this->flightplan, IsTracked()).WillByDefault(Return(tracked));
                ON_CALL(this->flightplan, IsTrackedByUser()).WillByDefault(Return(trackedByUser));
                ON_CALL(this->plugin, GetUserControllerObject()).WillByDefault(Return(this->mockController));
                ON_CALL(*this->mockController, IsVatsimRecognisedController()).WillByDefault(Return(vatsim));
                ON_CALL(this->flightplan, GetOrigin()).WillByDefault(Return("EGKK"));
                ON_CALL(this->flightplan, GetDistanceFromOrigin()).WillByDefault(Return(distance));
            }

            // Creates a NiceMock flightplan wired up as plugin.GetSelectedFlightplan().
            [[nodiscard]] auto MakeSelectedFlightplan(bool tracked = false, bool trackedByUser = false)
                -> std::shared_ptr<NiceMock<MockEuroScopeCFlightPlanInterface>>
            {
                auto fp = std::make_shared<NiceMock<MockEuroScopeCFlightPlanInterface>>();
                ON_CALL(*fp, IsTracked()).WillByDefault(Return(tracked));
                ON_CALL(*fp, IsTrackedByUser()).WillByDefault(Return(trackedByUser));
                ON_CALL(*fp, GetCallsign()).WillByDefault(Return("BAW123"));
                ON_CALL(this->plugin, GetSelectedFlightplan()).WillByDefault(Return(fp));
                return fp;
            }

            // Calls handler.ProcessAction capturing success/failure into the provided refs.
            void CallProcessAction(
                std::shared_ptr<UKControllerPlugin::Integration::MessageInterface> message,
                bool& successCalled,
                std::vector<std::string>& failureMessages)
            {
                this->handler.ProcessAction(
                    message,
                    [&successCalled]() { successCalled = true; },
                    [&failureMessages](std::vector<std::string> msgs) { failureMessages = std::move(msgs); });
            }

            // Creates a mock flightplan registered for "BAW123", sets up the
            // AnnotateFlightStrip(3,"1L") expectation, and expects the integration event.
            [[nodiscard]] auto MakeSyncFlightplanForBaw123Stand1()
                -> std::shared_ptr<NiceMock<MockEuroScopeCFlightPlanInterface>>
            {
                auto fp = std::make_shared<NiceMock<MockEuroScopeCFlightPlanInterface>>();
                ON_CALL(this->plugin, GetFlightplanForCallsign("BAW123")).WillByDefault(Return(fp));
                EXPECT_CALL(*fp, AnnotateFlightStrip(3, "1L")).Times(1);
                auto expectedMsg = std::make_shared<StandAssignedMessage>("BAW123", "EGKK", "1L");
                EXPECT_CALL(this->mockIntegration, SendEvent(MatchMessageInterface(expectedMsg))).Times(1);
                return fp;
            }

            [[nodiscard]] static auto AllSources() -> std::array<StandAssignment::Source, 4>
            {
                using Source = StandAssignment::Source;
                using enum StandAssignment::Source;

                return {User, ReservationAllocator, VaaAllocator, SystemAuto};
            }

            [[nodiscard]] static auto MakeInboundMessage(const std::string& type, const nlohmann::json& data)
            {
                return InboundMessage::FromJson(
                    nlohmann::json{{"type", type}, {"version", 1}, {"id", "foo"}, {"data", data}});
            }

            void SetProviderForAirfield(
                const std::string& airfield,
                UKControllerPlugin::Ownership::ServiceType serviceType =
                    UKControllerPlugin::Ownership::ServiceType::Delivery)
            {
                std::vector<std::shared_ptr<UKControllerPlugin::Ownership::ServiceProvision>> provisions;
                provisions.push_back(
                    std::make_shared<UKControllerPlugin::Ownership::ServiceProvision>(serviceType, userCallsign));
                airfieldOwnership->SetProvidersForAirfield(airfield, provisions);
            }

            void SetupAutoStandSelectionMenu(const std::string& destination = "EGGW", double distance = 1.0)
            {
                SetupFlightplan(false, false, true, distance);
                ON_CALL(this->flightplan, GetDestination()).WillByDefault(Return(destination));
                this->handler.DisplayStandSelectionMenu(this->flightplan, this->radarTarget, "", {0, 0});
            }

            [[nodiscard]] auto MakeSelectedFlightplanForAutoRequest(
                const std::string& destination, double distanceFromOrigin, const std::string& aircraftType = "")
                -> std::shared_ptr<NiceMock<MockEuroScopeCFlightPlanInterface>>
            {
                auto pluginReturnedFp = std::make_shared<NiceMock<MockEuroScopeCFlightPlanInterface>>();

                ON_CALL(*pluginReturnedFp, IsTracked()).WillByDefault(Return(true));
                ON_CALL(*pluginReturnedFp, IsTrackedByUser()).WillByDefault(Return(true));
                ON_CALL(*pluginReturnedFp, GetCallsign()).WillByDefault(Return("BAW123"));
                ON_CALL(*pluginReturnedFp, GetOrigin()).WillByDefault(Return("EGKK"));
                ON_CALL(*pluginReturnedFp, GetDestination()).WillByDefault(Return(destination));
                ON_CALL(*pluginReturnedFp, GetDistanceFromOrigin()).WillByDefault(Return(distanceFromOrigin));

                if (!aircraftType.empty()) {
                    ON_CALL(*pluginReturnedFp, GetAircraftType()).WillByDefault(Return(aircraftType));
                }

                ON_CALL(this->plugin, GetSelectedFlightplan()).WillByDefault(Return(pluginReturnedFp));
                return pluginReturnedFp;
            }

            void SetupRadarTargetForCallsign(const std::string& callsign, double latitude = 123, double longitude = 456)
            {
                auto pluginReturnedRt = std::make_shared<NiceMock<MockEuroScopeCRadarTargetInterface>>();

                EuroScopePlugIn::CPosition position;
                position.m_Latitude = latitude;
                position.m_Longitude = longitude;
                ON_CALL(*pluginReturnedRt, GetPosition()).WillByDefault(Return(position));

                ON_CALL(this->plugin, GetRadarTargetForCallsign(callsign)).WillByDefault(Return(pluginReturnedRt));
            }

            void SelectAutoAndExpectNoStandAssigned()
            {
                this->handler.StandSelected(1, "AUTO", {});
                this->AwaitApiCallCompletion();
                EXPECT_EQ(this->handler.noStandAssigned, this->handler.GetAssignedStandForCallsign("BAW123"));
            }

            void ExpectProcessActionFailureNoAssignment(
                const std::shared_ptr<UKControllerPlugin::Integration::MessageInterface>& message)
            {
                bool successCalled = false;
                std::vector<std::string> failureMessages;
                CallProcessAction(message, successCalled, failureMessages);

                EXPECT_FALSE(successCalled);
                EXPECT_FALSE(failureMessages.empty());
                EXPECT_EQ(this->handler.noStandAssigned, this->handler.GetAssignedStandForCallsign("BAW123"));
            }

            [[nodiscard]] static auto DepartureStandRequestBody() -> nlohmann::json
            {
                return nlohmann::json{
                    {"callsign", "BAW123"},
                    {"departure_airfield", "EGKK"},
                    {"assignment_type", "departure"},
                    {"latitude", 123},
                    {"longitude", 456},
                };
            }

            [[nodiscard]] static auto ArrivalStandRequestBody() -> nlohmann::json
            {
                return nlohmann::json{
                    {"callsign", "BAW123"},
                    {"departure_airfield", "EGKK"},
                    {"arrival_airfield", "EGGW"},
                    {"assignment_type", "arrival"},
                    {"aircraft_type", "B738"},
                };
            }

            double fontSize = 24.1;
            COLORREF tagColour = RGB(255, 255, 255);
            int euroscopeColourCode = EuroScopePlugIn::TAG_COLOR_ASSUMED;
            char itemString[16] = "Foooooo";
            NiceMock<MockApiInterface> api;
            NiceMock<MockEuroscopePluginLoopbackInterface> plugin;
            NiceMock<MockTaskRunnerInterface> taskRunner;
            NiceMock<MockOutboundIntegrationEventHandler> mockIntegration;
            NiceMock<MockEuroScopeCFlightPlanInterface> flightplan;
            NiceMock<MockEuroScopeCRadarTargetInterface> radarTarget;
            std::shared_ptr<NiceMock<MockEuroScopeCControllerInterface>> mockController;
            NiceMock<MockUserSettingProviderInterface> mockUserSettingProvider;
            UKControllerPlugin::Controller::ControllerPosition controller;
            std::shared_ptr<UKControllerPlugin::Controller::ActiveCallsign> userCallsign;
            std::shared_ptr<UKControllerPlugin::Controller::ActiveCallsign> notUserCallsign;
            std::shared_ptr<UKControllerPlugin::Ownership::AirfieldServiceProviderCollection> airfieldOwnership;
            UserSetting userSetting;
            std::shared_ptr<StandColourConfiguration> colourConfiguration;
            TagData tagData;
            StandEventHandler handler;
        };

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

        TEST_F(StandEventHandlerTest, ItReturnsDescriptionForSourceTagItem)
        {
            EXPECT_EQ(
                "Stand Assignment Source",
                this->handler.GetTagItemDescription(StandEventHandler::standAssignmentSourceTagItemId));
        }

        TEST_F(StandEventHandlerTest, ItReturnsSourceShorthandForSourceTagItem)
        {
            const auto sources = AllSources();
            const std::array<std::string, 4> expectedShorthand = {"USER", "RES ", "VAA ", "AUTO"};

            for (size_t i = 0; i < std::size(sources); ++i) {
                this->ExpectSourceTagItemShorthand(sources[i], expectedShorthand[i]);
            }
        }

        TEST_F(StandEventHandlerTest, ItReturnsNothingForSourceTagItemIfStandNotAssigned)
        {
            auto sourceTagData = this->CreateSourceTagData();
            this->handler.SetTagItemData(sourceTagData);
            EXPECT_EQ("Foooooo", sourceTagData.GetItemString());
        }

        TEST_F(StandEventHandlerTest, TagItem110UsesSourceColours)
        {
            const auto sources = AllSources();

            for (const auto& source : sources) {
                this->ExpectTagItem110ColourForSource(source);
            }
        }

        TEST_F(StandEventHandlerTest, SourceTagItemUsesSourceColours)
        {
            const auto sources = AllSources();

            for (const auto& source : sources) {
                this->ExpectSourceTagItemColourForSource(source);
            }
        }

        TEST_F(StandEventHandlerTest, ItSubscribesToChannels)
        {
            std::set<PushEventSubscription> expectedSubscriptions;
            expectedSubscriptions.insert({PushEventSubscription::SUB_TYPE_CHANNEL, "private-stand-assignments"});
            EXPECT_EQ(expectedSubscriptions, this->handler.GetPushEventSubscriptions());
        }

        TEST_F(StandEventHandlerTest, ItAssignsStandsFromWebsocketMessage)
        {
            PushEvent message{
                "App\\Events\\StandAssignedEvent",
                "private-stand-assignments",
                nlohmann::json{
                    {"callsign", "BAW123"},
                    {"stand_id", 1},
                }};

            auto pluginReturnedFp = MakeSyncFlightplanForBaw123Stand1();

            this->handler.ProcessPushEvent(message);
            ASSERT_EQ(1, this->handler.GetAssignedStandForCallsign("BAW123"));
        }

        TEST_F(StandEventHandlerTest, ItHandlesNoFlightplanForAnnotations)
        {
            PushEvent message{
                "App\\Events\\StandAssignedEvent",
                "private-stand-assignments",
                nlohmann::json{
                    {"callsign", "BAW123"},
                    {"stand_id", 1},
                }};

            ON_CALL(this->plugin, GetFlightplanForCallsign("BAW123")).WillByDefault(Return(nullptr));

            this->handler.ProcessPushEvent(message);
            ASSERT_EQ(1, this->handler.GetAssignedStandForCallsign("BAW123"));
        }

        TEST_F(StandEventHandlerTest, ItDoesntAssignStandFromWebsocketMessageIfNoCallsign)
        {
            PushEvent message{
                "App\\Events\\StandAssignedEvent",
                "private-stand-assignments",
                nlohmann::json{
                    {"stand_id", 1},
                }};

            this->handler.ProcessPushEvent(message);
            ASSERT_EQ(this->handler.noStandAssigned, this->handler.GetAssignedStandForCallsign("BAW123"));
        }

        TEST_F(StandEventHandlerTest, ItDoesntAssignStandFromWebsocketMessageIfCallsignInvalid)
        {
            PushEvent message{
                "App\\Events\\StandAssignedEvent",
                "private-stand-assignments",
                nlohmann::json{
                    {"callsign", 123},
                    {"stand_id", 1},
                }};

            this->handler.ProcessPushEvent(message);
            ASSERT_EQ(this->handler.noStandAssigned, this->handler.GetAssignedStandForCallsign("BAW123"));
        }

        TEST_F(StandEventHandlerTest, ItDoesntAssignStandFromWebsocketMessageIfNoStandId)
        {
            PushEvent message{
                "App\\Events\\StandAssignedEvent",
                "private-stand-assignments",
                nlohmann::json{
                    {"callsign", "BAW123"},
                }};

            this->handler.ProcessPushEvent(message);
            ASSERT_EQ(this->handler.noStandAssigned, this->handler.GetAssignedStandForCallsign("BAW123"));
        }

        TEST_F(StandEventHandlerTest, ItDoesntAssignStandFromWebsocketMessageIfStandIdInvalid)
        {
            PushEvent message{
                "App\\Events\\StandAssignedEvent",
                "private-stand-assignments",
                nlohmann::json{
                    {"callsign", "BAW123"},
                    {"stand_id", "1"},
                }};

            this->handler.ProcessPushEvent(message);
            ASSERT_EQ(this->handler.noStandAssigned, this->handler.GetAssignedStandForCallsign("BAW123"));
        }

        TEST_F(StandEventHandlerTest, ItDoesntAssignStandFromWebsocketMessageIfStandIdNotRealStand)
        {
            PushEvent message{
                "App\\Events\\StandAssignedEvent",
                "private-stand-assignments",
                nlohmann::json{
                    {"callsign", "BAW123"},
                    {"stand_id", -999},
                }};

            this->handler.ProcessPushEvent(message);
            ASSERT_EQ(this->handler.noStandAssigned, this->handler.GetAssignedStandForCallsign("BAW123"));
        }

        TEST_F(StandEventHandlerTest, ItHandlesNoFlightplanForUnassignmentRemovalOfAnnotations)
        {
            this->handler.SetAssignedStand("BAW123", 3);
            PushEvent message{
                "App\\Events\\StandUnassignedEvent",
                "private-stand-assignments",
                nlohmann::json{{"callsign", "BAW123"}}};

            ON_CALL(this->plugin, GetFlightplanForCallsign("BAW123")).WillByDefault(Return(nullptr));

            this->handler.ProcessPushEvent(message);
            ASSERT_EQ(this->handler.noStandAssigned, this->handler.GetAssignedStandForCallsign("BAW123"));
        }

        TEST_F(StandEventHandlerTest, ItUnassignsStandsFromWebsocketMessage)
        {
            this->handler.SetAssignedStand("BAW123", 3);
            PushEvent message{
                "App\\Events\\StandUnassignedEvent",
                "private-stand-assignments",
                nlohmann::json{{"callsign", "BAW123"}}};

            std::shared_ptr<NiceMock<MockEuroScopeCFlightPlanInterface>> pluginReturnedFp =
                std::make_shared<NiceMock<MockEuroScopeCFlightPlanInterface>>();

            ON_CALL(this->plugin, GetFlightplanForCallsign("BAW123")).WillByDefault(Return(pluginReturnedFp));

            EXPECT_CALL(*pluginReturnedFp, AnnotateFlightStrip(3, "")).Times(1);

            auto expectedMessage = std::make_shared<StandUnassignedMessage>("BAW123");
            EXPECT_CALL(this->mockIntegration, SendEvent(MatchMessageInterface(expectedMessage))).Times(1);

            this->handler.ProcessPushEvent(message);
            ASSERT_EQ(this->handler.noStandAssigned, this->handler.GetAssignedStandForCallsign("BAW123"));
        }

        TEST_F(StandEventHandlerTest, ItDoesntUnassignStandFromWebsocketMessageIfCallsignMissing)
        {
            this->handler.SetAssignedStand("BAW123", 3);
            PushEvent message{
                "App\\Events\\StandUnassignedEvent",
                "private-stand-assignments",
                nlohmann::json{{"notcallsign", "BAW123"}}};

            this->handler.ProcessPushEvent(message);
            ASSERT_EQ(3, this->handler.GetAssignedStandForCallsign("BAW123"));
        }

        TEST_F(StandEventHandlerTest, ItDoesntUnassignStandFromWebsocketMessageIfCallsignInvalid)
        {
            this->handler.SetAssignedStand("BAW123", 3);
            PushEvent message{
                "App\\Events\\StandUnassignedEvent", "private-stand-assignments", nlohmann::json{{"callsign", 123}}};

            this->handler.ProcessPushEvent(message);
            ASSERT_EQ(3, this->handler.GetAssignedStandForCallsign("BAW123"));
        }

        TEST_F(StandEventHandlerTest, ItHandlesUnassignStandFromWebsocketMessageIfStandNotAssigned)
        {
            PushEvent message{
                "App\\Events\\StandUnassignedEvent",
                "private-stand-assignments",
                nlohmann::json{{"callsign", "BAW123"}}};

            this->handler.ProcessPushEvent(message);
            ASSERT_EQ(this->handler.noStandAssigned, this->handler.GetAssignedStandForCallsign("BAW123"));
        }

        TEST_F(StandEventHandlerTest, ItLoadsAssignmentsOnPluginEventsSync)
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

            std::shared_ptr<NiceMock<MockEuroScopeCFlightPlanInterface>> pluginReturnedFp1 =
                std::make_shared<NiceMock<MockEuroScopeCFlightPlanInterface>>();

            ON_CALL(this->plugin, GetFlightplanForCallsign("BAW123")).WillByDefault(Return(pluginReturnedFp1));

            EXPECT_CALL(*pluginReturnedFp1, AnnotateFlightStrip(3, "1L")).Times(1);

            auto expectedMessage = std::make_shared<StandAssignedMessage>("BAW123", "EGKK", "1L");
            EXPECT_CALL(this->mockIntegration, SendEvent(MatchMessageInterface(expectedMessage))).Times(1);

            std::shared_ptr<NiceMock<MockEuroScopeCFlightPlanInterface>> pluginReturnedFp2 =
                std::make_shared<NiceMock<MockEuroScopeCFlightPlanInterface>>();

            ON_CALL(this->plugin, GetFlightplanForCallsign("VIR245")).WillByDefault(Return(pluginReturnedFp2));

            EXPECT_CALL(*pluginReturnedFp2, AnnotateFlightStrip(3, "55")).Times(1);

            auto expectedMessage2 = std::make_shared<StandAssignedMessage>("VIR245", "EGKK", "55");
            EXPECT_CALL(this->mockIntegration, SendEvent(MatchMessageInterface(expectedMessage2))).Times(1);

            EXPECT_CALL(this->api, GetAssignedStands()).Times(1).WillOnce(Return(assignments));

            this->handler.PluginEventsSynced();
            ASSERT_EQ(1, this->handler.GetAssignedStandForCallsign("BAW123"));
            ASSERT_EQ(2, this->handler.GetAssignedStandForCallsign("VIR245"));
        }

        TEST_F(StandEventHandlerTest, ItClearsPreviousAssignmentsOnPluginEventsSync)
        {
            nlohmann::json assignments = nlohmann::json::array();
            assignments.push_back({
                {"callsign", "BAW123"},
                {"stand_id", 1},
            });

            EXPECT_CALL(this->api, GetAssignedStands()).Times(1).WillOnce(Return(assignments));

            this->handler.SetAssignedStand("RYR234", 3);
            this->handler.PluginEventsSynced();
            ASSERT_EQ(this->handler.noStandAssigned, this->handler.GetAssignedStandForCallsign("RYR234"));
        }

        TEST_F(StandEventHandlerTest, ItHandlesNonArrayStandAssignments)
        {
            nlohmann::json assignments = nlohmann::json::object();

            EXPECT_CALL(this->api, GetAssignedStands()).Times(1).WillOnce(Return(assignments));

            this->handler.PluginEventsSynced();
            ASSERT_EQ(this->handler.noStandAssigned, this->handler.GetAssignedStandForCallsign("BAW123"));
            ASSERT_EQ(this->handler.noStandAssigned, this->handler.GetAssignedStandForCallsign("VIR245"));
        }

        TEST_F(StandEventHandlerTest, ItHandlesNonArrayAssignmentsOnPluginEventSync)
        {
            nlohmann::json assignments = nlohmann::json::array();
            assignments.push_back({
                {"callsign", "BAW123"},
                {"stand_id", 1},
            });
            assignments.push_back(nlohmann::json::object());

            auto pluginReturnedFp = MakeSyncFlightplanForBaw123Stand1();

            EXPECT_CALL(this->api, GetAssignedStands()).Times(1).WillOnce(Return(assignments));

            this->handler.PluginEventsSynced();
            ASSERT_EQ(1, this->handler.GetAssignedStandForCallsign("BAW123"));
            ASSERT_EQ(this->handler.noStandAssigned, this->handler.GetAssignedStandForCallsign("VIR245"));
        }

        TEST_F(StandEventHandlerTest, ItHandlesAssignmentsWithMissingCallsignOnPluginEventSync)
        {
            nlohmann::json assignments = nlohmann::json::array();
            assignments.push_back({
                {"callsign", "BAW123"},
                {"stand_id", 1},
            });
            assignments.push_back({
                {"stand_id", 2},
            });

            auto pluginReturnedFp = MakeSyncFlightplanForBaw123Stand1();

            EXPECT_CALL(this->api, GetAssignedStands()).Times(1).WillOnce(Return(assignments));

            this->handler.PluginEventsSynced();
            ASSERT_EQ(1, this->handler.GetAssignedStandForCallsign("BAW123"));
            ASSERT_EQ(this->handler.noStandAssigned, this->handler.GetAssignedStandForCallsign("VIR245"));
        }

        TEST_F(StandEventHandlerTest, ItHandlesAssignmentsWithInvalidCallsignOnPluginEventSync)
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

            auto pluginReturnedFp = MakeSyncFlightplanForBaw123Stand1();

            EXPECT_CALL(this->api, GetAssignedStands()).Times(1).WillOnce(Return(assignments));

            this->handler.PluginEventsSynced();
            ASSERT_EQ(1, this->handler.GetAssignedStandForCallsign("BAW123"));
            ASSERT_EQ(this->handler.noStandAssigned, this->handler.GetAssignedStandForCallsign("VIR245"));
        }

        TEST_F(StandEventHandlerTest, ItHandlesAssignmentsWithMissingStandOnPluginEventSync)
        {
            nlohmann::json assignments = nlohmann::json::array();
            assignments.push_back({
                {"callsign", "BAW123"},
                {"stand_id", 1},
            });
            assignments.push_back({
                {"callsign", "VIR245"},
            });

            auto pluginReturnedFp = MakeSyncFlightplanForBaw123Stand1();

            EXPECT_CALL(this->api, GetAssignedStands()).Times(1).WillOnce(Return(assignments));

            this->handler.PluginEventsSynced();
            ASSERT_EQ(1, this->handler.GetAssignedStandForCallsign("BAW123"));
            ASSERT_EQ(this->handler.noStandAssigned, this->handler.GetAssignedStandForCallsign("VIR245"));
        }

        TEST_F(StandEventHandlerTest, ItHandlesAssignmentsWithInvalidStandOnPluginEventSync)
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

            auto pluginReturnedFp = MakeSyncFlightplanForBaw123Stand1();

            EXPECT_CALL(this->api, GetAssignedStands()).Times(1).WillOnce(Return(assignments));

            this->handler.PluginEventsSynced();
            ASSERT_EQ(1, this->handler.GetAssignedStandForCallsign("BAW123"));
            ASSERT_EQ(this->handler.noStandAssigned, this->handler.GetAssignedStandForCallsign("VIR245"));
        }

        TEST_F(StandEventHandlerTest, ItHandlesAssignmentsWithNonExistentStandOnPluginEventSync)
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

            auto pluginReturnedFp = MakeSyncFlightplanForBaw123Stand1();

            EXPECT_CALL(this->api, GetAssignedStands()).Times(1).WillOnce(Return(assignments));

            this->handler.PluginEventsSynced();
            ASSERT_EQ(1, this->handler.GetAssignedStandForCallsign("BAW123"));
            ASSERT_EQ(this->handler.noStandAssigned, this->handler.GetAssignedStandForCallsign("VIR245"));
        }

        TEST_F(StandEventHandlerTest, ItHandlesApiExceptionsWhenFetchingStandsOnPluginEventSync)
        {
            EXPECT_CALL(this->api, GetAssignedStands()).Times(1).WillOnce(Throw(ApiException("Foo")));

            this->handler.PluginEventsSynced();
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

            PopupMenuItem menuItemStandAuto;
            menuItemStandAuto.firstValue = "AUTO";
            menuItemStandAuto.secondValue = "Automatic";
            menuItemStandAuto.callbackFunctionId = 1;
            menuItemStandAuto.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
            menuItemStandAuto.disabled = false;
            menuItemStandAuto.fixedPosition = true;

            PopupMenuItem menuItemStandNone;
            menuItemStandNone.firstValue = "--";
            menuItemStandNone.secondValue = "None";
            menuItemStandNone.callbackFunctionId = 1;
            menuItemStandNone.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
            menuItemStandNone.disabled = false;
            menuItemStandNone.fixedPosition = true;

            // Tracked by user
            SetupFlightplan(true, true);

            RECT expectedArea = {0, 0, 400, 600};
            EXPECT_CALL(this->plugin, TriggerPopupList(RectEq(expectedArea), "Assign Stand", 2)).Times(1);

            EXPECT_CALL(this->plugin, AddItemToPopupList(menuItemStand1)).Times(1);

            EXPECT_CALL(this->plugin, AddItemToPopupList(menuItemStand2)).Times(1);

            EXPECT_CALL(this->plugin, AddItemToPopupList(menuItemStandAuto)).Times(1);

            EXPECT_CALL(this->plugin, AddItemToPopupList(menuItemStandNone)).Times(1);

            this->handler.DisplayStandSelectionMenu(this->flightplan, this->radarTarget, "", {0, 0});
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

            PopupMenuItem menuItemStandAuto;
            menuItemStandAuto.firstValue = "AUTO";
            menuItemStandAuto.secondValue = "Automatic";
            menuItemStandAuto.callbackFunctionId = 1;
            menuItemStandAuto.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
            menuItemStandAuto.disabled = false;
            menuItemStandAuto.fixedPosition = true;

            PopupMenuItem menuItemStandNone;
            menuItemStandNone.firstValue = "--";
            menuItemStandNone.secondValue = "None";
            menuItemStandNone.callbackFunctionId = 1;
            menuItemStandNone.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
            menuItemStandNone.disabled = false;
            menuItemStandNone.fixedPosition = true;

            // Not tracked by anyone
            SetupFlightplan(false, false, true, 8);
            ON_CALL(this->flightplan, GetDestination()).WillByDefault(Return("EGLL"));

            RECT expectedArea = {0, 0, 400, 600};
            EXPECT_CALL(this->plugin, TriggerPopupList(RectEq(expectedArea), "Assign Stand", 2)).Times(1);

            EXPECT_CALL(this->plugin, AddItemToPopupList(menuItemStand)).Times(1);

            EXPECT_CALL(this->plugin, AddItemToPopupList(menuItemStandAuto)).Times(1);

            EXPECT_CALL(this->plugin, AddItemToPopupList(menuItemStandNone)).Times(1);

            this->handler.DisplayStandSelectionMenu(this->flightplan, this->radarTarget, "", {0, 0});
        }

        TEST_F(StandEventHandlerTest, ItDoesntTriggerTheSelectionMenuIfFlightplanTrackedBySomeoneElse)
        {
            // Not tracked by user
            SetupFlightplan(true, false);

            RECT expectedArea = {0, 0, 400, 600};
            EXPECT_CALL(this->plugin, TriggerPopupList(RectEq(expectedArea), "Assign Stand", 2)).Times(0);

            EXPECT_CALL(this->plugin, AddItemToPopupList(_)).Times(0);

            this->handler.DisplayStandSelectionMenu(this->flightplan, this->radarTarget, "", {0, 0});
        }

        TEST_F(StandEventHandlerTest, ItDoesntTriggerTheSelectionMenuIfNotVatsimRecognisedController)
        {
            // Not tracked by user
            SetupFlightplan(false, false, false);

            RECT expectedArea = {0, 0, 400, 600};
            EXPECT_CALL(this->plugin, TriggerPopupList(RectEq(expectedArea), "Assign Stand", 2)).Times(0);

            EXPECT_CALL(this->plugin, AddItemToPopupList(_)).Times(0);

            this->handler.DisplayStandSelectionMenu(this->flightplan, this->radarTarget, "", {0, 0});
        }

        TEST_F(StandEventHandlerTest, ItTriggersTheStandEditBoxWithDepartureAirportIfWithinLimit)
        {
            // Tracked by user
            SetupFlightplan(true, true);

            RECT expectedArea = {0, 0, 80, 25};
            EXPECT_CALL(this->plugin, ShowTextEditPopup(RectEq(expectedArea), 1, "")).Times(1);

            this->handler.DisplayStandAssignmentEditBox(this->flightplan, this->radarTarget, "", {0, 0});
            EXPECT_EQ("EGKK", this->handler.GetLastAirfield());
        }

        TEST_F(StandEventHandlerTest, ItTriggersTheStandEditBoxWithArrivalAirportIfNotCloseToDeparture)
        {
            // Not tracked by anyone
            SetupFlightplan(false, false, true, 8);
            ON_CALL(this->flightplan, GetDestination()).WillByDefault(Return("EGLL"));

            RECT expectedArea = {0, 0, 80, 25};
            EXPECT_CALL(this->plugin, ShowTextEditPopup(RectEq(expectedArea), 1, "")).Times(1);

            this->handler.DisplayStandAssignmentEditBox(this->flightplan, this->radarTarget, "", {0, 0});
            EXPECT_EQ("EGLL", this->handler.GetLastAirfield());
        }

        TEST_F(StandEventHandlerTest, ItDoesntTriggerTheEditBoxIfFlightplanTrackedBySomeoneElse)
        {
            // Not tracked by user
            SetupFlightplan(true, false);

            EXPECT_CALL(this->plugin, ShowTextEditPopup(_, _, _)).Times(0);

            this->handler.DisplayStandAssignmentEditBox(this->flightplan, this->radarTarget, "", {0, 0});
            EXPECT_EQ("", this->handler.GetLastAirfield());
        }

        TEST_F(StandEventHandlerTest, ItDoesntTriggerTheEditBoxIfNotVatsimRecognisedController)
        {
            // Not tracked by user
            SetupFlightplan(true, false, false);

            EXPECT_CALL(this->plugin, ShowTextEditPopup(_, _, _)).Times(0);

            this->handler.DisplayStandAssignmentEditBox(this->flightplan, this->radarTarget, "", {0, 0});
            EXPECT_EQ("", this->handler.GetLastAirfield());
        }

        TEST_F(StandEventHandlerTest, ItTriggersTheStandEditBoxWithCurrentStandIfOneAssigned)
        {
            this->handler.SetAssignedStand("BAW123", 1);

            // Not tracked by anyone
            SetupFlightplan(false, false, true, 8);
            ON_CALL(this->flightplan, GetDestination()).WillByDefault(Return("EGLL"));

            RECT expectedArea = {0, 0, 80, 25};
            EXPECT_CALL(this->plugin, ShowTextEditPopup(RectEq(expectedArea), 1, "1L")).Times(1);

            this->handler.DisplayStandAssignmentEditBox(this->flightplan, this->radarTarget, "", {0, 0});
        }

        TEST_F(StandEventHandlerTest, ItDoesntMakeAStandAssignmentRequestIfFlightplanTrackedBySomeoneElse)
        {
            // Trigger the menu first to set the last airport
            SetupFlightplan(true, false);

            this->handler.DisplayStandSelectionMenu(this->flightplan, this->radarTarget, "", {0, 0});

            auto pluginReturnedFp = MakeSelectedFlightplan(true, false);

            EXPECT_CALL(this->api, AssignStandToAircraft("BAW123", _)).Times(0);

            this->handler.StandSelected(1, "55", {});
            EXPECT_EQ(this->handler.noStandAssigned, this->handler.GetAssignedStandForCallsign("BAW123"));
        }

        TEST_F(StandEventHandlerTest, ItDoesntMakeAStandAssignmentRequestIfNotVatsimRecognisedController)
        {
            // Trigger the menu first to set the last airport
            SetupFlightplan(false, false, false);

            this->handler.DisplayStandSelectionMenu(this->flightplan, this->radarTarget, "", {0, 0});

            auto pluginReturnedFp = MakeSelectedFlightplan(true, false);

            EXPECT_CALL(this->api, AssignStandToAircraft("BAW123", _)).Times(0);

            this->handler.StandSelected(1, "55", {});
            EXPECT_EQ(this->handler.noStandAssigned, this->handler.GetAssignedStandForCallsign("BAW123"));
        }

        TEST_F(StandEventHandlerTest, ItDoesntMakeAStandAssignmentRequestIfMenuOrEditBoxWasntPreviouslyTriggered)
        {
            ON_CALL(this->plugin, GetUserControllerObject()).WillByDefault(Return(this->mockController));

            ON_CALL(*this->mockController, IsVatsimRecognisedController()).WillByDefault(Return(true));

            auto pluginReturnedFp = MakeSelectedFlightplan();

            EXPECT_CALL(this->api, AssignStandToAircraft("BAW123", 2)).Times(0);

            this->handler.StandSelected(1, "55", {});
            EXPECT_EQ(this->handler.noStandAssigned, this->handler.GetAssignedStandForCallsign("BAW123"));
        }

        TEST_F(StandEventHandlerTest, ItMakesAStandAssignmentRequestWhenStandSelected)
        {
            // Trigger the menu first to set the last airport
            SetupFlightplan(false, false);

            this->handler.DisplayStandSelectionMenu(this->flightplan, this->radarTarget, "", {0, 0});

            auto pluginReturnedFp = MakeSelectedFlightplan();

            ON_CALL(this->plugin, GetFlightplanForCallsign("BAW123")).WillByDefault(Return(pluginReturnedFp));

            EXPECT_CALL(*pluginReturnedFp, AnnotateFlightStrip(3, "55")).Times(1);

            auto expectedMessage = std::make_shared<StandAssignedMessage>("BAW123", "EGKK", "55");
            EXPECT_CALL(this->mockIntegration, SendEvent(MatchMessageInterface(expectedMessage))).Times(1);

            EXPECT_CALL(this->api, AssignStandToAircraft("BAW123", 2)).Times(1);

            this->handler.StandSelected(1, "55", {});
            EXPECT_EQ(2, this->handler.GetAssignedStandForCallsign("BAW123"));
        }

        TEST_F(StandEventHandlerTest, StandAssignmentsAreCaseInsensitive)
        {
            // Trigger the menu first to set the last airport
            SetupFlightplan(false, false);

            this->handler.DisplayStandSelectionMenu(this->flightplan, this->radarTarget, "", {0, 0});

            auto pluginReturnedFp = MakeSelectedFlightplan();

            ON_CALL(this->plugin, GetFlightplanForCallsign("BAW123")).WillByDefault(Return(pluginReturnedFp));

            EXPECT_CALL(*pluginReturnedFp, AnnotateFlightStrip(3, "1L")).Times(1);

            auto expectedMessage = std::make_shared<StandAssignedMessage>("BAW123", "EGKK", "1L");
            EXPECT_CALL(this->mockIntegration, SendEvent(MatchMessageInterface(expectedMessage))).Times(1);

            EXPECT_CALL(this->api, AssignStandToAircraft("BAW123", 1)).Times(1);

            this->handler.StandSelected(1, "1l", {});
            EXPECT_EQ(1, this->handler.GetAssignedStandForCallsign("BAW123"));
        }

        TEST_F(StandEventHandlerTest, ItHandlesApiExceptionOnStandAssignment)
        {
            // Trigger the menu first to set the last airport
            SetupFlightplan(false, false);

            this->handler.DisplayStandSelectionMenu(this->flightplan, this->radarTarget, "", {0, 0});

            auto pluginReturnedFp = MakeSelectedFlightplan();

            ON_CALL(this->plugin, GetFlightplanForCallsign("BAW123")).WillByDefault(Return(pluginReturnedFp));

            EXPECT_CALL(*pluginReturnedFp, AnnotateFlightStrip(3, "55")).Times(1);

            auto expectedMessage = std::make_shared<StandAssignedMessage>("BAW123", "EGKK", "55");
            EXPECT_CALL(this->mockIntegration, SendEvent(MatchMessageInterface(expectedMessage))).Times(1);

            EXPECT_CALL(this->api, AssignStandToAircraft("BAW123", 2)).Times(1).WillOnce(Throw(ApiException("Foo")));

            EXPECT_NO_THROW(this->handler.StandSelected(1, "55", {}));
            EXPECT_EQ(2, this->handler.GetAssignedStandForCallsign("BAW123"));
        }

        TEST_F(StandEventHandlerTest, ItDoesntAssignStandFromTheWrongAirfield)
        {
            // Trigger the menu first to set the last airport
            SetupFlightplan(false, false);

            this->handler.DisplayStandSelectionMenu(this->flightplan, this->radarTarget, "", {0, 0});

            auto pluginReturnedFp = MakeSelectedFlightplan();

            EXPECT_CALL(this->api, AssignStandToAircraft("BAW123", _)).Times(0);

            this->handler.StandSelected(1, "317", {});
            EXPECT_EQ(this->handler.noStandAssigned, this->handler.GetAssignedStandForCallsign("BAW123"));
        }

        TEST_F(StandEventHandlerTest, ItDeletesStandAssignmentsIfSet)
        {
            this->handler.SetAssignedStand("BAW123", 2);

            // Trigger the menu first to set the last airport
            SetupFlightplan(false, false);

            this->handler.DisplayStandSelectionMenu(this->flightplan, this->radarTarget, "", {0, 0});

            auto pluginReturnedFp = MakeSelectedFlightplan();

            ON_CALL(this->plugin, GetFlightplanForCallsign("BAW123")).WillByDefault(Return(pluginReturnedFp));

            EXPECT_CALL(this->api, DeleteStandAssignmentForAircraft("BAW123")).Times(1);

            EXPECT_CALL(*pluginReturnedFp, AnnotateFlightStrip(3, "")).Times(1);

            auto expectedMessage = std::make_shared<StandUnassignedMessage>("BAW123");
            EXPECT_CALL(this->mockIntegration, SendEvent(MatchMessageInterface(expectedMessage))).Times(1);

            this->handler.StandSelected(1, "--", {});
            EXPECT_EQ(this->handler.noStandAssigned, this->handler.GetAssignedStandForCallsign("BAW123"));
        }

        TEST_F(StandEventHandlerTest, ItDeletesStandAssignmentsViaTheEditBoxIfSet)
        {
            this->handler.SetAssignedStand("BAW123", 2);

            // Trigger the menu first to set the last airport
            SetupFlightplan(false, false);

            this->handler.DisplayStandSelectionMenu(this->flightplan, this->radarTarget, "", {0, 0});

            auto pluginReturnedFp = MakeSelectedFlightplan();

            ON_CALL(this->plugin, GetFlightplanForCallsign("BAW123")).WillByDefault(Return(pluginReturnedFp));

            EXPECT_CALL(this->api, DeleteStandAssignmentForAircraft("BAW123")).Times(1);

            EXPECT_CALL(*pluginReturnedFp, AnnotateFlightStrip(3, "")).Times(1);

            auto expectedMessage = std::make_shared<StandUnassignedMessage>("BAW123");
            EXPECT_CALL(this->mockIntegration, SendEvent(MatchMessageInterface(expectedMessage))).Times(1);

            this->handler.StandSelected(1, "", {});
            EXPECT_EQ(this->handler.noStandAssigned, this->handler.GetAssignedStandForCallsign("BAW123"));
        }

        TEST_F(StandEventHandlerTest, ItHandlesApiExceptionsInAssignmentDeletion)
        {
            this->handler.SetAssignedStand("BAW123", 2);

            // Trigger the menu first to set the last airport
            SetupFlightplan(false, false);

            this->handler.DisplayStandSelectionMenu(this->flightplan, this->radarTarget, "", {0, 0});

            auto pluginReturnedFp = MakeSelectedFlightplan();

            EXPECT_CALL(this->api, DeleteStandAssignmentForAircraft("BAW123"))
                .Times(1)
                .WillOnce(Throw(ApiException("Foo")));

            EXPECT_NO_THROW(this->handler.StandSelected(1, "--", {}));
            EXPECT_EQ(this->handler.noStandAssigned, this->handler.GetAssignedStandForCallsign("BAW123"));
        }

        TEST_F(StandEventHandlerTest, ItDoesntDeleteStandAssignmentIfNotDoneViaMenuOrEditBox)
        {
            this->handler.SetAssignedStand("BAW123", 2);

            ON_CALL(this->plugin, GetUserControllerObject()).WillByDefault(Return(this->mockController));

            ON_CALL(*this->mockController, IsVatsimRecognisedController()).WillByDefault(Return(true));

            auto pluginReturnedFp = MakeSelectedFlightplan();

            EXPECT_CALL(this->api, DeleteStandAssignmentForAircraft("BAW123")).Times(0);

            this->handler.StandSelected(1, "", {});
            EXPECT_EQ(2, this->handler.GetAssignedStandForCallsign("BAW123"));
        }

        TEST_F(StandEventHandlerTest, ItDoesntMakeDeletionRequestIfNoStandAssigned)
        {
            // Trigger the menu first to set the last airport
            SetupFlightplan(false, false);

            this->handler.DisplayStandSelectionMenu(this->flightplan, this->radarTarget, "", {0, 0});

            auto pluginReturnedFp = MakeSelectedFlightplan();

            EXPECT_CALL(this->api, DeleteStandAssignmentForAircraft(_)).Times(0);

            this->handler.StandSelected(1, "--", {});
            EXPECT_EQ(this->handler.noStandAssigned, this->handler.GetAssignedStandForCallsign("BAW123"));
        }

        TEST_F(StandEventHandlerTest, ItDoesntMakeDeletionRequestIfFlightplanTrackedBySomeoneElse)
        {
            this->handler.SetAssignedStand("BAW123", 2);

            // Trigger the menu first to set the last airport
            SetupFlightplan(false, false);

            this->handler.DisplayStandSelectionMenu(this->flightplan, this->radarTarget, "", {0, 0});

            auto pluginReturnedFp = MakeSelectedFlightplan(true, false);

            EXPECT_CALL(this->api, DeleteStandAssignmentForAircraft(_)).Times(0);

            this->handler.StandSelected(1, "--", {});
            EXPECT_EQ(2, this->handler.GetAssignedStandForCallsign("BAW123"));
        }

        TEST_F(StandEventHandlerTest, ItDoesntMakeDeletionRequestIfNotVatsimRecognisedController)
        {
            this->handler.SetAssignedStand("BAW123", 2);

            // Trigger the menu first to set the last airport
            SetupFlightplan(false, false, false);

            this->handler.DisplayStandSelectionMenu(this->flightplan, this->radarTarget, "", {0, 0});

            auto pluginReturnedFp = MakeSelectedFlightplan();

            EXPECT_CALL(this->api, DeleteStandAssignmentForAircraft(_)).Times(0);

            this->handler.StandSelected(1, "--", {});
            EXPECT_EQ(2, this->handler.GetAssignedStandForCallsign("BAW123"));
        }

        TEST_F(StandEventHandlerTest, ItUpdatesFlightplanAnnotationsOnEvent)
        {
            this->handler.SetAssignedStand("BAW123", 2);

            ON_CALL(this->flightplan, GetCallsign()).WillByDefault(Return("BAW123"));

            ON_CALL(this->flightplan, GetAnnotation(3)).WillByDefault(Return("LOL"));

            EXPECT_CALL(this->flightplan, AnnotateFlightStrip(3, "55")).Times(1);

            this->handler.FlightPlanEvent(this->flightplan, this->radarTarget);
        }

        TEST_F(StandEventHandlerTest, ItDoesntUpdateFlightplanAnnotationsIfAlreadySet)
        {
            this->handler.SetAssignedStand("BAW123", 2);

            ON_CALL(this->flightplan, GetCallsign()).WillByDefault(Return("BAW123"));

            ON_CALL(this->flightplan, GetAnnotation(3)).WillByDefault(Return("55"));

            EXPECT_CALL(this->flightplan, AnnotateFlightStrip(3, _)).Times(0);

            this->handler.FlightPlanEvent(this->flightplan, this->radarTarget);
        }

        TEST_F(StandEventHandlerTest, ItDoesntUpdateFlightplanAnnotationsIfNoAssignment)
        {
            ON_CALL(this->flightplan, GetCallsign()).WillByDefault(Return("BAW123"));

            ON_CALL(this->flightplan, GetAnnotation(3)).WillByDefault(Return("55"));

            EXPECT_CALL(this->flightplan, AnnotateFlightStrip(3, _)).Times(0);

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
            ON_CALL(this->plugin, GetUserControllerObject()).WillByDefault(Return(this->mockController));

            ON_CALL(*this->mockController, IsVatsimRecognisedController()).WillByDefault(Return(true));

            auto pluginReturnedFp = MakeSelectedFlightplan();

            ON_CALL(this->plugin, GetFlightplanForCallsign("BAW123")).WillByDefault(Return(pluginReturnedFp));

            EXPECT_CALL(*pluginReturnedFp, AnnotateFlightStrip(3, "55")).Times(1);

            auto expectedMessage = std::make_shared<StandAssignedMessage>("BAW123", "EGKK", "55");
            EXPECT_CALL(this->mockIntegration, SendEvent(MatchMessageInterface(expectedMessage))).Times(1);

            EXPECT_CALL(this->api, AssignStandToAircraft("BAW123", 2)).Times(1);

            EXPECT_TRUE(this->handler.ProcessMessage("STANDS:BAW123:EGKK:55"));
        }

        TEST_F(StandEventHandlerTest, StandAssignmentsFromExternalMessagesAreCaseInsensitive)
        {
            ON_CALL(this->plugin, GetUserControllerObject()).WillByDefault(Return(this->mockController));

            ON_CALL(*this->mockController, IsVatsimRecognisedController()).WillByDefault(Return(true));

            auto pluginReturnedFp = MakeSelectedFlightplan();

            ON_CALL(this->plugin, GetFlightplanForCallsign("BAW123")).WillByDefault(Return(pluginReturnedFp));

            EXPECT_CALL(*pluginReturnedFp, AnnotateFlightStrip(3, "1L")).Times(1);

            auto expectedMessage = std::make_shared<StandAssignedMessage>("BAW123", "EGKK", "1L");
            EXPECT_CALL(this->mockIntegration, SendEvent(MatchMessageInterface(expectedMessage))).Times(1);

            EXPECT_CALL(this->api, AssignStandToAircraft("BAW123", 1)).Times(1);

            EXPECT_TRUE(this->handler.ProcessMessage("STANDS:BAW123:EGKK:1l"));
        }

        TEST_F(StandEventHandlerTest, ItHandlesInvalidFlightplanOnExternalMessage)
        {
            std::shared_ptr<NiceMock<MockEuroScopeCFlightPlanInterface>> pluginReturnedFp = nullptr;

            EXPECT_CALL(this->api, AssignStandToAircraft("BAW123", 2)).Times(0);

            EXPECT_TRUE(this->handler.ProcessMessage("STANDS:BAW123:EGKK:55"));
        }

        TEST_F(StandEventHandlerTest, ItDoesntAssignNonExistentStandsFromExternalMessage)
        {
            ON_CALL(this->plugin, GetUserControllerObject()).WillByDefault(Return(this->mockController));

            ON_CALL(*this->mockController, IsVatsimRecognisedController()).WillByDefault(Return(true));

            auto pluginReturnedFp = MakeSelectedFlightplan();

            ON_CALL(this->plugin, GetFlightplanForCallsign("BAW123")).WillByDefault(Return(pluginReturnedFp));

            EXPECT_CALL(*pluginReturnedFp, AnnotateFlightStrip(3, "55")).Times(0);

            EXPECT_CALL(this->api, AssignStandToAircraft("BAW123", 2)).Times(0);

            EXPECT_TRUE(this->handler.ProcessMessage("STANDS:BAW123:EGKK:XXX"));
        }

        TEST_F(StandEventHandlerTest, ItSubscribesToIntegrationMessages)
        {
            std::vector<MessageType> messages{{"assign_stand", 1}, {"unassign_stand", 1}};
            EXPECT_EQ(messages, this->handler.ActionsToProcess());
        }

        TEST_F(StandEventHandlerTest, ItUnassignsStandFromIntegrationMessage)
        {
            this->handler.SetAssignedStand("BAW123", 3);
            auto message = MakeInboundMessage("unassign_stand", {{"callsign", "BAW123"}});

            EXPECT_CALL(this->api, DeleteStandAssignmentForAircraft("BAW123")).Times(1);

            bool successCalled = false;
            std::vector<std::string> failureMessages;
            CallProcessAction(message, successCalled, failureMessages);

            EXPECT_TRUE(successCalled);
            EXPECT_TRUE(failureMessages.empty());
            EXPECT_EQ(this->handler.noStandAssigned, this->handler.GetAssignedStandForCallsign("BAW123"));
        }

        TEST_F(StandEventHandlerTest, ItDoesntUnassignStandFromAircraftFromMessageIfNothingToDo)
        {
            auto message = MakeInboundMessage("unassign_stand", {{"callsign", "BAW123"}});

            EXPECT_CALL(this->api, DeleteStandAssignmentForAircraft("BAW123")).Times(0);

            bool successCalled = false;
            std::vector<std::string> failureMessages;
            CallProcessAction(message, successCalled, failureMessages);

            EXPECT_TRUE(successCalled);
            EXPECT_TRUE(failureMessages.empty());
            EXPECT_EQ(this->handler.noStandAssigned, this->handler.GetAssignedStandForCallsign("BAW123"));
        }

        TEST_F(StandEventHandlerTest, ItDoesntUnassignStandFromAircraftIfBadCallsign)
        {
            this->handler.SetAssignedStand("BAW123", 3);
            auto message = MakeInboundMessage("unassign_stand", {{"callsign", 123}});

            bool successCalled = false;
            std::vector<std::string> failureMessages;
            CallProcessAction(message, successCalled, failureMessages);

            EXPECT_FALSE(successCalled);
            EXPECT_FALSE(failureMessages.empty());
            EXPECT_EQ(3, this->handler.GetAssignedStandForCallsign("BAW123"));
        }

        TEST_F(StandEventHandlerTest, ItDoesntUnassignStandFromAircraftIfNoCallsign)
        {
            this->handler.SetAssignedStand("BAW123", 3);
            auto message = MakeInboundMessage("unassign_stand", nlohmann::json::object());

            bool successCalled = false;
            std::vector<std::string> failureMessages;
            CallProcessAction(message, successCalled, failureMessages);

            EXPECT_FALSE(successCalled);
            EXPECT_FALSE(failureMessages.empty());
            EXPECT_EQ(3, this->handler.GetAssignedStandForCallsign("BAW123"));
        }

        TEST_F(StandEventHandlerTest, ItMakesStandAssignmentFromIntegrationMessage)
        {
            ON_CALL(this->plugin, GetUserControllerObject()).WillByDefault(Return(this->mockController));

            ON_CALL(*this->mockController, IsVatsimRecognisedController()).WillByDefault(Return(true));

            auto pluginReturnedFp = MakeSelectedFlightplan();

            ON_CALL(this->plugin, GetFlightplanForCallsign("BAW123")).WillByDefault(Return(pluginReturnedFp));

            EXPECT_CALL(*pluginReturnedFp, AnnotateFlightStrip(3, "55")).Times(1);

            auto expectedMessage = std::make_shared<StandAssignedMessage>("BAW123", "EGKK", "55");
            EXPECT_CALL(this->mockIntegration, SendEvent(MatchMessageInterface(expectedMessage))).Times(1);

            EXPECT_CALL(this->api, AssignStandToAircraft("BAW123", 2)).Times(1);

            auto message =
                MakeInboundMessage("assign_stand", {{"callsign", "BAW123"}, {"airfield", "EGKK"}, {"stand", "55"}});

            bool successCalled = false;
            std::vector<std::string> failureMessages;
            CallProcessAction(message, successCalled, failureMessages);

            EXPECT_TRUE(successCalled);
            EXPECT_TRUE(failureMessages.empty());
            EXPECT_EQ(2, this->handler.GetAssignedStandForCallsign("BAW123"));
        }

        TEST_F(StandEventHandlerTest, ItFailsAssignmentFromMessageIfAssignmentFails)
        {
            ON_CALL(this->plugin, GetFlightplanForCallsign("BAW123")).WillByDefault(Return(nullptr));

            auto message =
                MakeInboundMessage("assign_stand", {{"callsign", "BAW123"}, {"airfield", "EGKK"}, {"stand", "55"}});
            ExpectProcessActionFailureNoAssignment(message);
        }

        TEST_F(StandEventHandlerTest, ItFailsAssignmentFromMessageIfCallsignMissing)
        {
            auto message = MakeInboundMessage("assign_stand", {{"airfield", "EGKK"}, {"stand", "55"}});
            ExpectProcessActionFailureNoAssignment(message);
        }

        TEST_F(StandEventHandlerTest, ItFailsAssignmentFromMessageIfCallsignNotString)
        {
            auto message =
                MakeInboundMessage("assign_stand", {{"callsign", 123}, {"airfield", "EGKK"}, {"stand", "55"}});
            ExpectProcessActionFailureNoAssignment(message);
        }

        TEST_F(StandEventHandlerTest, ItFailsAssignmentFromMessageIfAirfieldMissing)
        {
            auto message = MakeInboundMessage("assign_stand", {{"callsign", "BAW123"}, {"stand", "55"}});
            ExpectProcessActionFailureNoAssignment(message);
        }

        TEST_F(StandEventHandlerTest, ItFailsAssignmentFromMessageIfAirfieldNotString)
        {
            auto message =
                MakeInboundMessage("assign_stand", {{"callsign", "BAW123"}, {"airfield", 123}, {"stand", "55"}});
            ExpectProcessActionFailureNoAssignment(message);
        }

        TEST_F(StandEventHandlerTest, ItFailsAssignmentFromMessageIfStandMissing)
        {
            auto message = MakeInboundMessage("assign_stand", {{"callsign", "BAW123"}, {"airfield", "EGKK"}});
            ExpectProcessActionFailureNoAssignment(message);
        }

        TEST_F(StandEventHandlerTest, ItFailsAssignmentFromMessageIfStandNotString)
        {
            auto message =
                MakeInboundMessage("assign_stand", {{"callsign", "BAW123"}, {"airfield", "EGKK"}, {"stand", 55}});
            ExpectProcessActionFailureNoAssignment(message);
        }

        TEST_F(StandEventHandlerTest, ItRequestsADepartureStandFromApi)
        {
            // Make "us" the delivery controller
            SetProviderForAirfield("EGKK");

            SetupAutoStandSelectionMenu("EGGW", 1);
            [[maybe_unused]] const auto selectedFlightplan = MakeSelectedFlightplanForAutoRequest("EGGW", 1);
            SetupRadarTargetForCallsign("BAW123");

            this->ExpectApiRequest()
                ->Post()
                .To("stand/assignment/requestauto")
                .WithBody(DepartureStandRequestBody())
                .WillReturnCreated()
                .WithResponseBody(nlohmann::json{{"stand_id", 3}});

            this->handler.StandSelected(1, "AUTO", {});

            this->AwaitApiCallCompletion();
            EXPECT_EQ(3, this->handler.GetAssignedStandForCallsign("BAW123"));
        }

        TEST_F(StandEventHandlerTest, DepartureStandRequestsHandlesNonCreatedCode)
        {
            // Make "us" the delivery controller
            SetProviderForAirfield("EGKK");

            SetupAutoStandSelectionMenu("EGGW", 1);
            [[maybe_unused]] const auto selectedFlightplan = MakeSelectedFlightplanForAutoRequest("EGGW", 1);
            SetupRadarTargetForCallsign("BAW123");

            this->ExpectApiRequest()
                ->Post()
                .To("stand/assignment/requestauto")
                .WithBody(DepartureStandRequestBody())
                .WillReturnForbidden();

            SelectAutoAndExpectNoStandAssigned();
        }

        TEST_F(StandEventHandlerTest, DepartureStandRequestsHandlesUnknownStandId)
        {
            // Make "us" the delivery controller
            SetProviderForAirfield("EGKK");

            SetupAutoStandSelectionMenu("EGGW", 1);
            [[maybe_unused]] const auto selectedFlightplan = MakeSelectedFlightplanForAutoRequest("EGGW", 1);
            SetupRadarTargetForCallsign("BAW123");

            this->ExpectApiRequest()
                ->Post()
                .To("stand/assignment/requestauto")
                .WithBody(DepartureStandRequestBody())
                .WillReturnCreated()
                .WithResponseBody(nlohmann::json{{"stand_id", 999}});

            SelectAutoAndExpectNoStandAssigned();
        }

        TEST_F(StandEventHandlerTest, DepartureStandRequestsHandlesStandIdNotInteger)
        {
            // Make "us" the delivery controller
            SetProviderForAirfield("EGKK");

            SetupAutoStandSelectionMenu("EGGW", 1);
            [[maybe_unused]] const auto selectedFlightplan = MakeSelectedFlightplanForAutoRequest("EGGW", 1);
            SetupRadarTargetForCallsign("BAW123");

            this->ExpectApiRequest()
                ->Post()
                .To("stand/assignment/requestauto")
                .WithBody(DepartureStandRequestBody())
                .WillReturnCreated()
                .WithResponseBody(nlohmann::json{{"stand_id", "abc"}});

            SelectAutoAndExpectNoStandAssigned();
        }

        TEST_F(StandEventHandlerTest, DepartureStandRequestsHandlesNoStandIdInJson)
        {
            // Make "us" the delivery controller
            SetProviderForAirfield("EGKK");

            SetupAutoStandSelectionMenu("EGGW", 1);
            [[maybe_unused]] const auto selectedFlightplan = MakeSelectedFlightplanForAutoRequest("EGGW", 1);
            SetupRadarTargetForCallsign("BAW123");

            this->ExpectApiRequest()
                ->Post()
                .To("stand/assignment/requestauto")
                .WithBody(DepartureStandRequestBody())
                .WillReturnCreated()
                .WithResponseBody(nlohmann::json{{"not_stand_id", 3}});

            SelectAutoAndExpectNoStandAssigned();
        }

        TEST_F(StandEventHandlerTest, DepartureStandRequestsHandlesNotFoundRadarTarget)
        {
            // Make "us" the delivery controller
            SetProviderForAirfield("EGKK");

            SetupAutoStandSelectionMenu("EGGW", 1);
            [[maybe_unused]] const auto selectedFlightplan = MakeSelectedFlightplanForAutoRequest("EGGW", 1);

            ON_CALL(this->plugin, GetRadarTargetForCallsign("BAW123")).WillByDefault(Return(nullptr));

            this->ExpectNoApiRequests();

            SelectAutoAndExpectNoStandAssigned();
        }

        TEST_F(StandEventHandlerTest, DepartureStandRequestsHandlesUnknownDistanceFromOrigin)
        {
            // Make "us" the delivery controller
            SetProviderForAirfield("EGKK");

            SetupAutoStandSelectionMenu("EGGW", 0.0);
            [[maybe_unused]] const auto selectedFlightplan = MakeSelectedFlightplanForAutoRequest("EGGW", 0.0);
            SetupRadarTargetForCallsign("BAW123");

            this->ExpectNoApiRequests();

            SelectAutoAndExpectNoStandAssigned();
        }

        TEST_F(StandEventHandlerTest, DepartureStandRequestsHandlesUserNotProvidingDelivery)
        {
            // Make "us" the delivery controller
            SetProviderForAirfield("EGKK", UKControllerPlugin::Ownership::ServiceType::Ground);

            SetupAutoStandSelectionMenu("EGGW", 1);
            [[maybe_unused]] const auto selectedFlightplan = MakeSelectedFlightplanForAutoRequest("EGGW", 1);
            SetupRadarTargetForCallsign("BAW123");

            this->ExpectNoApiRequests();

            SelectAutoAndExpectNoStandAssigned();
        }

        TEST_F(StandEventHandlerTest, ItRequestsAnArrivalStandFromApi)
        {
            // Make "us" the delivery controller
            SetProviderForAirfield("EGGW");

            SetupAutoStandSelectionMenu("EGGW", 1);
            [[maybe_unused]] const auto selectedFlightplan = MakeSelectedFlightplanForAutoRequest("EGGW", 55, "B738");

            this->ExpectApiRequest()
                ->Post()
                .To("stand/assignment/requestauto")
                .WithBody(ArrivalStandRequestBody())
                .WillReturnCreated()
                .WithResponseBody(nlohmann::json{{"stand_id", 3}});

            this->handler.StandSelected(1, "AUTO", {});

            this->AwaitApiCallCompletion();
            EXPECT_EQ(3, this->handler.GetAssignedStandForCallsign("BAW123"));
        }

        TEST_F(StandEventHandlerTest, ArrivalStandRequestHandlesInvalidStatusCodeInResponse)
        {
            // Make "us" the delivery controller
            SetProviderForAirfield("EGGW");

            SetupAutoStandSelectionMenu("EGGW", 1);
            [[maybe_unused]] const auto selectedFlightplan = MakeSelectedFlightplanForAutoRequest("EGGW", 55, "B738");

            this->ExpectApiRequest()
                ->Post()
                .To("stand/assignment/requestauto")
                .WithBody(ArrivalStandRequestBody())
                .WillReturnServerError();

            SelectAutoAndExpectNoStandAssigned();
        }

        TEST_F(StandEventHandlerTest, ArrivalStandRequestHandlesInvalidStandIdInJson)
        {
            // Make "us" the delivery controller
            SetProviderForAirfield("EGGW");

            SetupAutoStandSelectionMenu("EGGW", 1);
            [[maybe_unused]] const auto selectedFlightplan = MakeSelectedFlightplanForAutoRequest("EGGW", 55, "B738");

            this->ExpectApiRequest()
                ->Post()
                .To("stand/assignment/requestauto")
                .WithBody(ArrivalStandRequestBody())
                .WillReturnCreated()
                .WithResponseBody(nlohmann::json{{"stand_id", 999}});

            SelectAutoAndExpectNoStandAssigned();
        }

        TEST_F(StandEventHandlerTest, ArrivalStandRequestHandlesNonIntegerStandIdInJson)
        {
            // Make "us" the delivery controller
            SetProviderForAirfield("EGGW");

            SetupAutoStandSelectionMenu("EGGW", 1);
            [[maybe_unused]] const auto selectedFlightplan = MakeSelectedFlightplanForAutoRequest("EGGW", 55, "B738");

            this->ExpectApiRequest()
                ->Post()
                .To("stand/assignment/requestauto")
                .WithBody(ArrivalStandRequestBody())
                .WillReturnCreated()
                .WithResponseBody(nlohmann::json{{"stand_id", "abc"}});

            SelectAutoAndExpectNoStandAssigned();
        }

        TEST_F(StandEventHandlerTest, ArrivalStandRequestHandlesNoStandIdInJson)
        {
            // Make "us" the delivery controller
            SetProviderForAirfield("EGGW");

            SetupAutoStandSelectionMenu("EGGW", 1);
            [[maybe_unused]] const auto selectedFlightplan = MakeSelectedFlightplanForAutoRequest("EGGW", 55, "B738");

            this->ExpectApiRequest()
                ->Post()
                .To("stand/assignment/requestauto")
                .WithBody(ArrivalStandRequestBody())
                .WillReturnCreated()
                .WithResponseBody(nlohmann::json{{"not_stand_id", 3}});

            SelectAutoAndExpectNoStandAssigned();
        }

        TEST_F(StandEventHandlerTest, ArrivalStandRequestHandlesZeroDistanceToOrigin)
        {
            // Make "us" the delivery controller
            SetProviderForAirfield("EGGW");

            SetupAutoStandSelectionMenu("EGGW", 1);
            [[maybe_unused]] const auto selectedFlightplan = MakeSelectedFlightplanForAutoRequest("EGGW", 0.0, "B738");

            this->ExpectNoApiRequests();

            SelectAutoAndExpectNoStandAssigned();
        }

        TEST_F(StandEventHandlerTest, ArrivalStandRequestHandlesUserNotProvidingDelivery)
        {
            // Make "us" the delivery controller
            SetProviderForAirfield("EGGW", UKControllerPlugin::Ownership::ServiceType::Ground);

            SetupAutoStandSelectionMenu("EGGW", 1);
            [[maybe_unused]] const auto selectedFlightplan = MakeSelectedFlightplanForAutoRequest("EGGW", 55, "B738");

            this->ExpectNoApiRequests();

            SelectAutoAndExpectNoStandAssigned();
        }
    } // namespace Stands
} // namespace UKControllerPluginTest
