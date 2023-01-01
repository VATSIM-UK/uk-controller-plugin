#include "intention/AircraftIntentionCode.h"
#include "intention/FirExitPoint.h"
#include "intention/FirExitPointCollection.h"
#include "intention/IntentionCodeMetadata.h"
#include "intention/IntentionCodeModel.h"
#include "intention/IntentionCodeUpdatedMessage.h"
#include "intention/SendIntentionCodeUpdatedIntegrationMessage.h"
#include "intention/SingleCode.h"

using UKControllerPlugin::IntentionCode::AircraftIntentionCode;
using UKControllerPlugin::IntentionCode::FirExitPoint;
using UKControllerPlugin::IntentionCode::FirExitPointCollection;
using UKControllerPlugin::IntentionCode::IntentionCodeMetadata;
using UKControllerPlugin::IntentionCode::IntentionCodeModel;
using UKControllerPlugin::IntentionCode::IntentionCodeUpdatedMessage;
using UKControllerPlugin::IntentionCode::SendIntentionCodeUpdatedIntegrationMessage;
using UKControllerPlugin::IntentionCode::SingleCode;

namespace UKControllerPluginTest::IntentionCode {

    class SendIntentionCodeUpdatedIntegrationMessageTest : public testing::Test
    {
        public:
        SendIntentionCodeUpdatedIntegrationMessageTest()
            : exitDetermination(std::make_shared<MockExitDetermination>()),
              exitPoint(std::make_shared<FirExitPoint>(1, "TEST", false, exitDetermination)),
              firExitCollection(std::make_shared<FirExitPointCollection>()),
              outboundHandler(std::make_shared<testing::NiceMock<Integration::MockOutboundIntegrationEventHandler>>()),
              sendMessage(firExitCollection, outboundHandler)
        {
            firExitCollection->Add(exitPoint);
        }

        std::shared_ptr<MockExitDetermination> exitDetermination;
        std::shared_ptr<FirExitPoint> exitPoint;
        std::shared_ptr<FirExitPointCollection> firExitCollection;
        std::shared_ptr<testing::NiceMock<Integration::MockOutboundIntegrationEventHandler>> outboundHandler;
        SendIntentionCodeUpdatedIntegrationMessage sendMessage;
    };

    TEST_F(SendIntentionCodeUpdatedIntegrationMessageTest, ItSendsIntentionCodeUpdateMessageNoMatchedIntentionCode)
    {
        AircraftIntentionCode aircraftIntentionCode;
        aircraftIntentionCode.callsign = "BAW123";
        aircraftIntentionCode.intentionCode = "A1";

        std::shared_ptr<UKControllerPlugin::Integration::MessageInterface> expectedMessage =
            std::make_shared<IntentionCodeUpdatedMessage>("BAW123", "", "A1");

        EXPECT_CALL(*this->outboundHandler, SendEvent(MatchMessageInterface(expectedMessage))).Times(1);

        sendMessage.IntentionCodeUpdated(aircraftIntentionCode);
    }

    TEST_F(SendIntentionCodeUpdatedIntegrationMessageTest, ItSendsIntentionCodeUpdateMessageNoFirExitPoint)
    {
        auto condition = std::make_unique<testing::NiceMock<MockCondition>>();
        auto code = std::make_unique<SingleCode>("AF");
        auto metadata = std::make_unique<IntentionCodeMetadata>();
        auto model =
            std::make_shared<IntentionCodeModel>(1, std::move(code), std::move(condition), std::move(metadata));

        AircraftIntentionCode aircraftIntentionCode;
        aircraftIntentionCode.callsign = "BAW123";
        aircraftIntentionCode.intentionCode = "A1";
        aircraftIntentionCode.matchedIntentionCode = model;

        std::shared_ptr<UKControllerPlugin::Integration::MessageInterface> expectedMessage =
            std::make_shared<IntentionCodeUpdatedMessage>("BAW123", "", "A1");

        EXPECT_CALL(*this->outboundHandler, SendEvent(MatchMessageInterface(expectedMessage))).Times(1);

        sendMessage.IntentionCodeUpdated(aircraftIntentionCode);
    }

    TEST_F(SendIntentionCodeUpdatedIntegrationMessageTest, ItSendsIntentionCodeUpdateMessageFirExitPointNotFoundById)
    {
        auto condition = std::make_unique<testing::NiceMock<MockCondition>>();
        auto code = std::make_unique<SingleCode>("AF");
        auto metadata = std::make_unique<IntentionCodeMetadata>();
        metadata->exitPoint = 5;
        auto model =
            std::make_shared<IntentionCodeModel>(1, std::move(code), std::move(condition), std::move(metadata));

        AircraftIntentionCode aircraftIntentionCode;
        aircraftIntentionCode.callsign = "BAW123";
        aircraftIntentionCode.intentionCode = "A1";
        aircraftIntentionCode.matchedIntentionCode = model;

        std::shared_ptr<UKControllerPlugin::Integration::MessageInterface> expectedMessage =
            std::make_shared<IntentionCodeUpdatedMessage>("BAW123", "", "A1");

        EXPECT_CALL(*this->outboundHandler, SendEvent(MatchMessageInterface(expectedMessage))).Times(1);

        sendMessage.IntentionCodeUpdated(aircraftIntentionCode);
    }

    TEST_F(SendIntentionCodeUpdatedIntegrationMessageTest, ItSendsIntentionCodeUpdateMessageWithFirExitPoint)
    {
        auto condition = std::make_unique<testing::NiceMock<MockCondition>>();
        auto code = std::make_unique<SingleCode>("AF");
        auto metadata = std::make_unique<IntentionCodeMetadata>();
        metadata->exitPoint = 1;
        auto model =
            std::make_shared<IntentionCodeModel>(1, std::move(code), std::move(condition), std::move(metadata));

        AircraftIntentionCode aircraftIntentionCode;
        aircraftIntentionCode.callsign = "BAW123";
        aircraftIntentionCode.intentionCode = "A1";
        aircraftIntentionCode.matchedIntentionCode = model;

        std::shared_ptr<UKControllerPlugin::Integration::MessageInterface> expectedMessage =
            std::make_shared<IntentionCodeUpdatedMessage>("BAW123", "TEST", "A1");

        EXPECT_CALL(*this->outboundHandler, SendEvent(MatchMessageInterface(expectedMessage))).Times(1);

        sendMessage.IntentionCodeUpdated(aircraftIntentionCode);
    }
} // namespace UKControllerPluginTest::IntentionCode
