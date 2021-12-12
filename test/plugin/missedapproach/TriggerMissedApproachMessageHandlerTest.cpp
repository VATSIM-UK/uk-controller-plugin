#include "integration/InboundMessage.h"
#include "missedapproach/MissedApproach.h"
#include "missedapproach/MissedApproachAudioAlert.h"
#include "missedapproach/MissedApproachCollection.h"
#include "missedapproach/MissedApproachOptions.h"
#include "missedapproach/MissedApproachTriggeredMessage.h"
#include "missedapproach/TriggerMissedApproach.h"
#include "missedapproach/TriggerMissedApproachMessageHandler.h"
#include "ownership/AirfieldServiceProviderCollection.h"

using ::testing::NiceMock;
using UKControllerPlugin::Integration::InboundMessage;
using UKControllerPlugin::MissedApproach::MissedApproach;
using UKControllerPlugin::MissedApproach::MissedApproachAudioAlert;
using UKControllerPlugin::MissedApproach::MissedApproachCollection;
using UKControllerPlugin::MissedApproach::MissedApproachOptions;
using UKControllerPlugin::MissedApproach::MissedApproachTriggeredMessage;
using UKControllerPlugin::MissedApproach::TriggerMissedApproach;
using UKControllerPlugin::MissedApproach::TriggerMissedApproachMessageHandler;
using UKControllerPlugin::Ownership::AirfieldServiceProviderCollection;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPluginTest::Windows::MockWinApi;

namespace UKControllerPluginTest::MissedApproach {
    class TriggerMissedApproachMessageHandlerTest : public testing::Test
    {
        public:
        TriggerMissedApproachMessageHandlerTest()
            : successFunction([this]() { successCalled = true; }),
              failFunction([this](std::vector<std::string> messages) { this->failureMessages = messages; }),
              collection(std::make_shared<MissedApproachCollection>()),
              options(std::make_shared<MissedApproachOptions>()),
              audioAlert(std::make_shared<MissedApproachAudioAlert>(options, plugin, serviceProviders, windows)),
              trigger(collection, windows, api, serviceProviders, audioAlert, mockIntegration), handler(trigger, plugin)
        {
            pluginReturnedFlightplan = std::make_shared<Euroscope::MockEuroScopeCFlightPlanInterface>();
            pluginReturnedRadarTarget = std::make_shared<Euroscope::MockEuroScopeCRadarTargetInterface>();
            ON_CALL(*pluginReturnedFlightplan, GetCallsign).WillByDefault(testing::Return("BAW123"));
            ON_CALL(*pluginReturnedFlightplan, GetDistanceToDestination).WillByDefault(testing::Return(999999));
        }

        bool successCalled = false;
        std::vector<std::string> failureMessages;
        std::function<void(void)> successFunction;
        std::function<void(std::vector<std::string>)> failFunction;
        AirfieldServiceProviderCollection serviceProviders;
        std::shared_ptr<Euroscope::MockEuroScopeCRadarTargetInterface> pluginReturnedRadarTarget;
        std::shared_ptr<Euroscope::MockEuroScopeCFlightPlanInterface> pluginReturnedFlightplan;
        NiceMock<Integration::MockOutboundIntegrationEventHandler> mockIntegration;
        NiceMock<MockWinApi> windows;
        NiceMock<Api::MockApiInterface> api;
        NiceMock<MockEuroscopePluginLoopbackInterface> plugin;
        std::shared_ptr<class MissedApproach> missed1;
        std::shared_ptr<class MissedApproach> missed2;
        std::shared_ptr<class MissedApproach> missed3;
        std::shared_ptr<MissedApproachCollection> collection;
        std::shared_ptr<MissedApproachOptions> options;
        std::shared_ptr<MissedApproachAudioAlert> audioAlert;
        TriggerMissedApproach trigger;
        TriggerMissedApproachMessageHandler handler;
    };

    TEST_F(TriggerMissedApproachMessageHandlerTest, ItFailsIfCallsignNotString)
    {
        auto message = InboundMessage::FromJson(nlohmann::json{
            {"type", "trigger_missed_approach"}, {"version", 1}, {"id", "foo"}, {"data", {{"callsign", 123}}}});

        this->handler.ProcessAction(message, successFunction, failFunction);
        EXPECT_FALSE(successCalled);
        EXPECT_EQ(1, failureMessages.size());
        EXPECT_EQ("Missed approach message data is invalid", failureMessages[0]);
    }

    TEST_F(TriggerMissedApproachMessageHandlerTest, ItFailsIfCallsignNotPresent)
    {
        auto message = InboundMessage::FromJson(nlohmann::json{
            {"type", "trigger_missed_approach"}, {"version", 1}, {"id", "foo"}, {"data", {{"notcallsign", "BAW123"}}}});

        this->handler.ProcessAction(message, successFunction, failFunction);
        EXPECT_FALSE(successCalled);
        EXPECT_EQ(1, failureMessages.size());
        EXPECT_EQ("Missed approach message data is invalid", failureMessages[0]);
    }

    TEST_F(TriggerMissedApproachMessageHandlerTest, ItFailsIfCallsignNoFlightplan)
    {
        auto message = InboundMessage::FromJson(nlohmann::json{
            {"type", "trigger_missed_approach"}, {"version", 1}, {"id", "foo"}, {"data", {{"callsign", "BAW123"}}}});

        EXPECT_CALL(this->plugin, GetFlightplanForCallsign("BAW123")).Times(1).WillOnce(testing::Return(nullptr));

        EXPECT_CALL(this->plugin, GetRadarTargetForCallsign("BAW123"))
            .Times(1)
            .WillOnce(testing::Return(pluginReturnedRadarTarget));

        this->handler.ProcessAction(message, successFunction, failFunction);
        EXPECT_FALSE(successCalled);
        EXPECT_EQ(1, failureMessages.size());
        EXPECT_EQ("Flightplan or radar target not found for missed approach", failureMessages[0]);
    }

    TEST_F(TriggerMissedApproachMessageHandlerTest, ItStartsTheTrigger)
    {
        auto message = InboundMessage::FromJson(nlohmann::json{
            {"type", "trigger_missed_approach"}, {"version", 1}, {"id", "foo"}, {"data", {{"callsign", "BAW123"}}}});

        EXPECT_CALL(this->plugin, GetFlightplanForCallsign("BAW123"))
            .Times(1)
            .WillOnce(testing::Return(pluginReturnedFlightplan));

        EXPECT_CALL(this->plugin, GetRadarTargetForCallsign("BAW123"))
            .Times(1)
            .WillOnce(testing::Return(pluginReturnedRadarTarget));

        this->handler.ProcessAction(message, successFunction, failFunction);
        EXPECT_FALSE(successCalled);
        EXPECT_EQ(1, failureMessages.size());
        EXPECT_EQ("Aircraft not eligible for missed approach", failureMessages[0]);
    }
} // namespace UKControllerPluginTest::MissedApproach
