#include "aircraftstate/AircraftStateEventHandler.h"
#include "api/ApiException.h"
#include "time/ParseTimeStrings.h"
#include "time/SystemClock.h"

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;
using ::testing::Throw;
using UKControllerPlugin::AircraftState::AircraftStateEventHandler;
using UKControllerPlugin::Api::ApiException;
using UKControllerPlugin::Push::PushEvent;
using UKControllerPlugin::Time::SetTestNow;
using UKControllerPlugin::Time::TimeNow;
using UKControllerPluginTest::Api::MockApiInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCRadarTargetInterface;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPluginTest::TaskManager::MockTaskRunnerInterface;

namespace UKControllerPluginTest::AircraftState {

    class AircraftStateEventHandlerTest : public Test
    {
        public:
        AircraftStateEventHandlerTest() : handler(api, taskRunner, plugin)
        {
            this->flightPlan = std::make_shared<NiceMock<MockEuroScopeCFlightPlanInterface>>();
            ON_CALL(*this->flightPlan, GetCallsign()).WillByDefault(Return("BAW123"));
            SetTestNow(std::chrono::system_clock::now());
        }

        [[nodiscard]] static auto Message(const std::string& callsign, bool flag, const nlohmann::json& groundState)
            -> nlohmann::json
        {
            return nlohmann::json{{"callsign", callsign}, {"clearance_flag", flag}, {"ground_state", groundState}};
        }

        [[nodiscard]] static auto PushMessage(const nlohmann::json& data) -> PushEvent
        {
            return {"App\\Events\\SomeEvent", "private-some-channel", data, data.dump()};
        }

        void GivenServerState(const std::string& callsign, bool flag, const nlohmann::json& groundState)
        {
            ON_CALL(this->api, GetAircraftStates())
                .WillByDefault(Return(nlohmann::json::array({Message(callsign, flag, groundState)})));
            this->handler.PluginEventsSynced();
        }

        [[nodiscard]] static auto
        Update(const std::string& callsign, std::optional<bool> clearanceFlag, std::optional<std::string> groundState)
            -> nlohmann::json
        {
            nlohmann::json update{{"callsign", callsign}};
            if (clearanceFlag.has_value()) {
                update["clearance_flag"] = clearanceFlag.value();
            }

            if (groundState.has_value()) {
                update["ground_state"] = groundState.value();
            }

            return update;
        }

        // Matches callsigns and values only - timestamps have their own test.
        [[nodiscard]] static auto BatchOf(const std::vector<nlohmann::json>& expected)
        {
            return ::testing::Truly([expected](const nlohmann::json& actual) {
                if (!actual.is_array() || actual.size() != expected.size()) {
                    return false;
                }

                for (size_t index = 0; index < expected.size(); index++) {
                    for (const auto& field : expected[index].items()) {
                        if (!actual[index].contains(field.key()) || actual[index].at(field.key()) != field.value()) {
                            return false;
                        }
                    }
                }

                return true;
            });
        }

        std::shared_ptr<NiceMock<MockEuroScopeCFlightPlanInterface>> flightPlan;
        NiceMock<MockEuroScopeCRadarTargetInterface> radarTarget;
        NiceMock<MockApiInterface> api;
        NiceMock<MockEuroscopePluginLoopbackInterface> plugin;
        MockTaskRunnerInterface taskRunner;
        AircraftStateEventHandler handler;
    };

    TEST_F(AircraftStateEventHandlerTest, ItSubscribesToNoPushChannels)
    {
        EXPECT_TRUE(this->handler.GetPushEventSubscriptions().empty());
    }

    TEST_F(AircraftStateEventHandlerTest, ItDoesNotWriteToEuroscopeOnPushEvents)
    {
        ON_CALL(this->plugin, GetFlightplanForCallsign("BAW123")).WillByDefault(Return(this->flightPlan));

        EXPECT_CALL(*this->flightPlan, SetClearanceFlag(_)).Times(0);
        EXPECT_CALL(*this->flightPlan, SetGroundState(_)).Times(0);

        this->handler.ProcessPushEvent(PushMessage(Message("BAW123", true, "TAXI")));
    }

    TEST_F(AircraftStateEventHandlerTest, ItLoadsAircraftStatesOnLogin)
    {
        ON_CALL(this->api, GetAircraftStates())
            .WillByDefault(
                Return(nlohmann::json::array({Message("BAW123", true, "TAXI"), Message("BAW456", false, "PUSH")})));

        this->handler.PluginEventsSynced();

        EXPECT_EQ(2, this->handler.Count());
        EXPECT_TRUE(this->handler.Get("BAW123").clearanceFlag);
        EXPECT_EQ("TAXI", this->handler.Get("BAW123").groundState);
        EXPECT_FALSE(this->handler.Get("BAW456").clearanceFlag);
        EXPECT_EQ("PUSH", this->handler.Get("BAW456").groundState);
    }

    TEST_F(AircraftStateEventHandlerTest, ItHandlesNullGroundStatesOnLogin)
    {
        this->GivenServerState("BAW123", true, nlohmann::json::value_t::null);

        EXPECT_EQ(1, this->handler.Count());
        EXPECT_TRUE(this->handler.Get("BAW123").clearanceFlag);
        EXPECT_EQ("", this->handler.Get("BAW123").groundState);
    }

    TEST_F(AircraftStateEventHandlerTest, ItHandlesApiExceptionsOnLogin)
    {
        ON_CALL(this->api, GetAircraftStates()).WillByDefault(Throw(ApiException("Test")));

        EXPECT_NO_THROW(this->handler.PluginEventsSynced());
        EXPECT_EQ(0, this->handler.Count());
    }

    TEST_F(AircraftStateEventHandlerTest, ItIgnoresNonArrayDataOnLogin)
    {
        ON_CALL(this->api, GetAircraftStates()).WillByDefault(Return(nlohmann::json::object()));

        this->handler.PluginEventsSynced();

        EXPECT_EQ(0, this->handler.Count());
    }

    TEST_F(AircraftStateEventHandlerTest, ItIgnoresInvalidStatesOnLogin)
    {
        ON_CALL(this->api, GetAircraftStates())
            .WillByDefault(
                Return(nlohmann::json::array({Message("BAW123", true, "WIBBLE"), Message("BAW456", false, "PUSH")})));

        this->handler.PluginEventsSynced();

        EXPECT_EQ(1, this->handler.Count());
        EXPECT_EQ("PUSH", this->handler.Get("BAW456").groundState);
    }

    TEST_F(AircraftStateEventHandlerTest, ItWritesServerStateToEuroscopeOnLogin)
    {
        ON_CALL(this->plugin, GetFlightplanForCallsign("BAW123")).WillByDefault(Return(this->flightPlan));
        ON_CALL(*this->flightPlan, GetClearanceFlag()).WillByDefault(Return(false));
        ON_CALL(*this->flightPlan, GetGroundState()).WillByDefault(Return(""));

        EXPECT_CALL(*this->flightPlan, SetClearanceFlag(true)).Times(1);
        EXPECT_CALL(*this->flightPlan, SetGroundState("TAXI")).Times(1);

        this->GivenServerState("BAW123", true, "TAXI");
    }

    TEST_F(AircraftStateEventHandlerTest, ItDoesNotWriteWhenEuroscopeAlreadyMatches)
    {
        ON_CALL(this->plugin, GetFlightplanForCallsign("BAW123")).WillByDefault(Return(this->flightPlan));
        ON_CALL(*this->flightPlan, GetClearanceFlag()).WillByDefault(Return(true));
        ON_CALL(*this->flightPlan, GetGroundState()).WillByDefault(Return("TAXI"));

        EXPECT_CALL(*this->flightPlan, SetClearanceFlag(_)).Times(0);
        EXPECT_CALL(*this->flightPlan, SetGroundState(_)).Times(0);

        this->GivenServerState("BAW123", true, "TAXI");
    }

    TEST_F(AircraftStateEventHandlerTest, ItOnlyWritesAnAircraftOnce)
    {
        ON_CALL(this->plugin, GetFlightplanForCallsign("BAW123")).WillByDefault(Return(this->flightPlan));
        ON_CALL(*this->flightPlan, GetClearanceFlag()).WillByDefault(Return(false));

        EXPECT_CALL(*this->flightPlan, SetClearanceFlag(true)).Times(1);

        this->GivenServerState("BAW123", true, nlohmann::json::value_t::null);

        for (auto event = 0; event < 10; event++) {
            this->handler.FlightPlanEvent(*this->flightPlan, this->radarTarget);
        }
    }

    TEST_F(AircraftStateEventHandlerTest, ItDoesNotWriteToAircraftTrackedByAnotherController)
    {
        ON_CALL(this->plugin, GetFlightplanForCallsign("BAW123")).WillByDefault(Return(this->flightPlan));
        ON_CALL(*this->flightPlan, IsTracked()).WillByDefault(Return(true));
        ON_CALL(*this->flightPlan, IsTrackedByUser()).WillByDefault(Return(false));

        EXPECT_CALL(*this->flightPlan, SetClearanceFlag(_)).Times(0);
        EXPECT_CALL(*this->flightPlan, SetGroundState(_)).Times(0);

        this->GivenServerState("BAW123", true, "TAXI");
        this->handler.FlightPlanEvent(*this->flightPlan, this->radarTarget);
    }

    TEST_F(AircraftStateEventHandlerTest, ItRetriesOnceTheTrackIsReleased)
    {
        ON_CALL(*this->flightPlan, IsTracked()).WillByDefault(Return(true));
        ON_CALL(*this->flightPlan, IsTrackedByUser()).WillByDefault(Return(false));
        ON_CALL(*this->flightPlan, GetClearanceFlag()).WillByDefault(Return(false));

        this->GivenServerState("BAW123", true, nlohmann::json::value_t::null);
        this->handler.FlightPlanEvent(*this->flightPlan, this->radarTarget);

        EXPECT_CALL(*this->flightPlan, SetClearanceFlag(true)).Times(1);

        ON_CALL(*this->flightPlan, IsTracked()).WillByDefault(Return(false));
        this->handler.FlightPlanEvent(*this->flightPlan, this->radarTarget);
    }

    TEST_F(AircraftStateEventHandlerTest, ItWritesToAircraftTrackedByTheUser)
    {
        ON_CALL(this->plugin, GetFlightplanForCallsign("BAW123")).WillByDefault(Return(this->flightPlan));
        ON_CALL(*this->flightPlan, IsTracked()).WillByDefault(Return(true));
        ON_CALL(*this->flightPlan, IsTrackedByUser()).WillByDefault(Return(true));
        ON_CALL(*this->flightPlan, GetClearanceFlag()).WillByDefault(Return(false));

        EXPECT_CALL(*this->flightPlan, SetClearanceFlag(true)).Times(1);

        this->GivenServerState("BAW123", true, nlohmann::json::value_t::null);
    }

    TEST_F(AircraftStateEventHandlerTest, ItBackfillsAircraftThatAppearAfterLogin)
    {
        this->GivenServerState("BAW123", true, nlohmann::json::value_t::null);

        ON_CALL(*this->flightPlan, GetClearanceFlag()).WillByDefault(Return(false));
        EXPECT_CALL(*this->flightPlan, SetClearanceFlag(true)).Times(1);

        this->handler.FlightPlanEvent(*this->flightPlan, this->radarTarget);
    }

    TEST_F(AircraftStateEventHandlerTest, ItDoesNothingOnFlightPlanEventsForUnknownAircraft)
    {
        EXPECT_CALL(*this->flightPlan, SetClearanceFlag(_)).Times(0);
        EXPECT_CALL(*this->flightPlan, SetGroundState(_)).Times(0);

        this->handler.FlightPlanEvent(*this->flightPlan, this->radarTarget);
    }

    TEST_F(AircraftStateEventHandlerTest, ItSendsControllerChangesToTheApi)
    {
        ON_CALL(*this->flightPlan, GetClearanceFlag()).WillByDefault(Return(true));
        ON_CALL(*this->flightPlan, GetGroundState()).WillByDefault(Return(""));

        EXPECT_CALL(this->api, UpdateAircraftStates(BatchOf({Update("BAW123", true, std::nullopt)}))).Times(1);

        this->handler.ControllerFlightPlanDataEvent(*this->flightPlan, EuroScopePlugIn::CTR_DATA_TYPE_CLEARENCE_FLAG);
        this->handler.TimedEventTrigger();
    }

    TEST_F(AircraftStateEventHandlerTest, ItSendsGroundStateChangesToTheApi)
    {
        ON_CALL(*this->flightPlan, GetClearanceFlag()).WillByDefault(Return(false));
        ON_CALL(*this->flightPlan, GetGroundState()).WillByDefault(Return("PUSH"));

        EXPECT_CALL(this->api, UpdateAircraftStates(BatchOf({Update("BAW123", std::nullopt, "PUSH")}))).Times(1);

        this->handler.ControllerFlightPlanDataEvent(*this->flightPlan, EuroScopePlugIn::CTR_DATA_TYPE_GROUND_STATE);
        this->handler.TimedEventTrigger();
    }

    TEST_F(AircraftStateEventHandlerTest, ItSendsEveryChangedAircraftInOneRequest)
    {
        EXPECT_CALL(this->api, UpdateAircraftStates(_)).Times(1);

        for (const auto& callsign : {"BAW123", "BAW456", "BAW789"}) {
            auto flightPlan = std::make_shared<NiceMock<MockEuroScopeCFlightPlanInterface>>();
            ON_CALL(*flightPlan, GetCallsign()).WillByDefault(Return(callsign));
            ON_CALL(*flightPlan, GetClearanceFlag()).WillByDefault(Return(true));

            this->handler.ControllerFlightPlanDataEvent(*flightPlan, EuroScopePlugIn::CTR_DATA_TYPE_CLEARENCE_FLAG);
        }

        EXPECT_EQ(3, this->handler.PendingCount());
        this->handler.TimedEventTrigger();
        EXPECT_EQ(0, this->handler.PendingCount());
    }

    TEST_F(AircraftStateEventHandlerTest, ItSendsNothingWhenThereIsNothingPending)
    {
        EXPECT_CALL(this->api, UpdateAircraftStates(_)).Times(0);

        this->handler.TimedEventTrigger();
    }

    TEST_F(AircraftStateEventHandlerTest, ItDoesNotSendChangesThatMatchTheServer)
    {
        this->GivenServerState("BAW123", true, "TAXI");

        ON_CALL(*this->flightPlan, GetClearanceFlag()).WillByDefault(Return(true));
        ON_CALL(*this->flightPlan, GetGroundState()).WillByDefault(Return("TAXI"));

        EXPECT_CALL(this->api, UpdateAircraftStates(_)).Times(0);

        this->handler.ControllerFlightPlanDataEvent(*this->flightPlan, EuroScopePlugIn::CTR_DATA_TYPE_CLEARENCE_FLAG);
        this->handler.TimedEventTrigger();

        EXPECT_EQ(0, this->handler.PendingCount());
    }

    TEST_F(AircraftStateEventHandlerTest, ItNeverSendsAnEmptyGroundState)
    {
        this->GivenServerState("BAW123", false, "TAXI");

        ON_CALL(*this->flightPlan, GetClearanceFlag()).WillByDefault(Return(true));
        ON_CALL(*this->flightPlan, GetGroundState()).WillByDefault(Return(""));

        EXPECT_CALL(this->api, UpdateAircraftStates(BatchOf({Update("BAW123", true, std::nullopt)}))).Times(1);

        this->handler.ControllerFlightPlanDataEvent(*this->flightPlan, EuroScopePlugIn::CTR_DATA_TYPE_CLEARENCE_FLAG);
        this->handler.TimedEventTrigger();

        EXPECT_EQ("TAXI", this->handler.Get("BAW123").groundState);
    }

    TEST_F(AircraftStateEventHandlerTest, ItIgnoresOtherControllerDataTypes)
    {
        EXPECT_CALL(this->api, UpdateAircraftStates(_)).Times(0);

        this->handler.ControllerFlightPlanDataEvent(*this->flightPlan, EuroScopePlugIn::CTR_DATA_TYPE_SQUAWK);
        this->handler.TimedEventTrigger();
    }

    TEST_F(AircraftStateEventHandlerTest, ItHandlesApiExceptionsOnUpdate)
    {
        ON_CALL(*this->flightPlan, GetClearanceFlag()).WillByDefault(Return(true));
        ON_CALL(this->api, UpdateAircraftStates(_)).WillByDefault(Throw(ApiException("Test")));

        this->handler.ControllerFlightPlanDataEvent(*this->flightPlan, EuroScopePlugIn::CTR_DATA_TYPE_CLEARENCE_FLAG);

        EXPECT_NO_THROW(this->handler.TimedEventTrigger());
    }

    TEST_F(AircraftStateEventHandlerTest, ItCoalescesRepeatedChangesToOneAircraft)
    {
        EXPECT_CALL(this->api, UpdateAircraftStates(BatchOf({Update("BAW123", std::nullopt, "TAXI")}))).Times(1);

        for (const auto& groundState : {"STUP", "PUSH", "TAXI"}) {
            ON_CALL(*this->flightPlan, GetGroundState()).WillByDefault(Return(groundState));
            this->handler.ControllerFlightPlanDataEvent(*this->flightPlan, EuroScopePlugIn::CTR_DATA_TYPE_GROUND_STATE);
        }

        EXPECT_EQ(1, this->handler.PendingCount());
        this->handler.TimedEventTrigger();
    }

    TEST_F(AircraftStateEventHandlerTest, ItMergesBothFieldsIntoOneEntry)
    {
        EXPECT_CALL(this->api, UpdateAircraftStates(BatchOf({Update("BAW123", true, "TAXI")}))).Times(1);

        ON_CALL(*this->flightPlan, GetClearanceFlag()).WillByDefault(Return(true));
        this->handler.ControllerFlightPlanDataEvent(*this->flightPlan, EuroScopePlugIn::CTR_DATA_TYPE_CLEARENCE_FLAG);

        ON_CALL(*this->flightPlan, GetGroundState()).WillByDefault(Return("TAXI"));
        this->handler.ControllerFlightPlanDataEvent(*this->flightPlan, EuroScopePlugIn::CTR_DATA_TYPE_GROUND_STATE);

        this->handler.TimedEventTrigger();
    }

    TEST_F(AircraftStateEventHandlerTest, ItStampsEachFieldWithWhenItChanged)
    {
        const auto start = TimeNow();
        nlohmann::json sent;
        ON_CALL(this->api, UpdateAircraftStates(_)).WillByDefault(::testing::SaveArg<0>(&sent));

        ON_CALL(*this->flightPlan, GetClearanceFlag()).WillByDefault(Return(true));
        this->handler.ControllerFlightPlanDataEvent(*this->flightPlan, EuroScopePlugIn::CTR_DATA_TYPE_CLEARENCE_FLAG);

        SetTestNow(start + std::chrono::seconds(30));
        ON_CALL(*this->flightPlan, GetGroundState()).WillByDefault(Return("TAXI"));
        this->handler.ControllerFlightPlanDataEvent(*this->flightPlan, EuroScopePlugIn::CTR_DATA_TYPE_GROUND_STATE);

        SetTestNow(start + std::chrono::seconds(60));
        this->handler.TimedEventTrigger();

        ASSERT_EQ(1, sent.size());
        EXPECT_EQ(
            UKControllerPlugin::Time::ToDateTimeString(start), sent[0].at("clearance_flag_at").get<std::string>());
        EXPECT_EQ(
            UKControllerPlugin::Time::ToDateTimeString(start + std::chrono::seconds(30)),
            sent[0].at("ground_state_at").get<std::string>());
    }

    TEST_F(AircraftStateEventHandlerTest, ItDropsPendingUpdatesOnDisconnect)
    {
        ON_CALL(*this->flightPlan, GetClearanceFlag()).WillByDefault(Return(true));

        EXPECT_CALL(this->api, UpdateAircraftStates(_)).Times(0);

        this->handler.ControllerFlightPlanDataEvent(*this->flightPlan, EuroScopePlugIn::CTR_DATA_TYPE_CLEARENCE_FLAG);
        this->handler.FlightPlanDisconnectEvent(*this->flightPlan);
        this->handler.TimedEventTrigger();

        EXPECT_EQ(0, this->handler.PendingCount());
    }

    TEST_F(AircraftStateEventHandlerTest, ItRemovesStatesOnDisconnect)
    {
        this->GivenServerState("BAW123", true, "TAXI");
        EXPECT_EQ(1, this->handler.Count());

        this->handler.FlightPlanDisconnectEvent(*this->flightPlan);

        EXPECT_EQ(0, this->handler.Count());
    }
} // namespace UKControllerPluginTest::AircraftState
