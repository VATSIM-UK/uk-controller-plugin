#include "integration/IntegrationClient.h"
#include "integration/IntegrationConnection.h"
#include "integration/MessageType.h"
#include "intention/AircraftIntentionCode.h"
#include "intention/FirExitPoint.h"
#include "intention/FirExitPointCollection.h"
#include "intention/IntentionCodeIntegrationDataInitialiser.h"
#include "intention/IntentionCodeMetadata.h"
#include "intention/IntentionCodeModel.h"
#include "intention/IntentionCodeUpdatedMessage.h"
#include "intention/SingleCode.h"
#include "mock/MockEuroScopeCFlightplanInterface.h"

using UKControllerPlugin::IntentionCode::AircraftIntentionCode;
using UKControllerPlugin::IntentionCode::IntentionCodeMetadata;
using UKControllerPlugin::IntentionCode::IntentionCodeModel;
using UKControllerPlugin::IntentionCode::IntentionCodeUpdatedMessage;
using UKControllerPlugin::IntentionCode::SingleCode;

namespace UKControllerPluginTest::IntentionCode {

    class IntentionCodeIntegrationDataInitialiserTest : public testing::Test
    {
        public:
        IntentionCodeIntegrationDataInitialiserTest()
            : exitDetermination(std::make_shared<MockExitDetermination>()),
              exitPoint(std::make_shared<UKControllerPlugin::IntentionCode::FirExitPoint>(
                  1, "TEST", false, exitDetermination)),
              firExitCollection(std::make_shared<UKControllerPlugin::IntentionCode::FirExitPointCollection>()),
              outboundHandler(std::make_shared<testing::NiceMock<Integration::MockOutboundIntegrationEventHandler>>()),
              mockIntentionCodeGenerator(
                  std::make_shared<testing::NiceMock<IntentionCode::MockAircraftIntentionCodeGenerator>>()),
              flightplan(std::shared_ptr<testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>>()),
              radarTarget(std::shared_ptr<testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface>>()),
              connection(std::make_shared<testing::NiceMock<Integration::MockConnection>>()),
              client(1, "1", "1", std::make_shared<UKControllerPlugin::Integration::IntegrationConnection>(connection)),
              initialiser(mockIntentionCodeGenerator, firExitCollection, mockPlugin)
        {
            firExitCollection->Add(exitPoint);
            mockPlugin.AddAllFlightplansItem({flightplan, radarTarget});
        }

        std::shared_ptr<MockExitDetermination> exitDetermination;
        std::shared_ptr<UKControllerPlugin::IntentionCode::FirExitPoint> exitPoint;
        std::shared_ptr<UKControllerPlugin::IntentionCode::FirExitPointCollection> firExitCollection;
        std::shared_ptr<testing::NiceMock<Integration::MockOutboundIntegrationEventHandler>> outboundHandler;
        std::shared_ptr<testing::NiceMock<IntentionCode::MockAircraftIntentionCodeGenerator>>
            mockIntentionCodeGenerator;
        std::shared_ptr<testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>> flightplan;
        std::shared_ptr<testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface>> radarTarget;
        std::shared_ptr<testing::NiceMock<Integration::MockConnection>> connection;
        UKControllerPlugin::Integration::IntegrationClient client;
        testing::NiceMock<Euroscope::MockEuroscopePluginLoopbackInterface> mockPlugin;
        UKControllerPlugin::IntentionCode::IntentionCodeIntegrationDataInitialiser initialiser;
    };

    TEST_F(IntentionCodeIntegrationDataInitialiserTest, ItInitialisesForIntentionCodeUpdatedMessage)
    {
        EXPECT_EQ(
            UKControllerPlugin::Integration::MessageType({"intention_code_updated", 1}), initialiser.InitialisesFor());
    }

    TEST_F(IntentionCodeIntegrationDataInitialiserTest, ItDoesntSendUpdatedMessageIfNoIntentionCode)
    {
        EXPECT_CALL(*mockIntentionCodeGenerator, Generate(testing::Ref(*flightplan), testing::Ref(*radarTarget)))
            .WillOnce(testing::Return(nullptr));

        EXPECT_CALL(*connection, Send).Times(0);

        initialiser.Initialise(client);
    }

    TEST_F(IntentionCodeIntegrationDataInitialiserTest, ItSendsIntentionCodeUpdateMessageNoMatchedIntentionCode)
    {
        AircraftIntentionCode aircraftIntentionCode;
        aircraftIntentionCode.callsign = "BAW123";
        aircraftIntentionCode.intentionCode = "A1";

        EXPECT_CALL(*mockIntentionCodeGenerator, Generate(testing::Ref(*flightplan), testing::Ref(*radarTarget)))
            .WillOnce(testing::Return(std::make_shared<AircraftIntentionCode>(aircraftIntentionCode)));

        std::shared_ptr<UKControllerPlugin::Integration::MessageInterface> expectedMessage =
            std::make_shared<IntentionCodeUpdatedMessage>("BAW123", "", "A1");

        EXPECT_CALL(*connection, Send(expectedMessage->ToJson().dump())).Times(1);

        initialiser.Initialise(client);
    }

    TEST_F(IntentionCodeIntegrationDataInitialiserTest, ItSendsIntentionCodeUpdateMessageNoFirExitPoint)
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

        EXPECT_CALL(*mockIntentionCodeGenerator, Generate(testing::Ref(*flightplan), testing::Ref(*radarTarget)))
            .WillOnce(testing::Return(std::make_shared<AircraftIntentionCode>(aircraftIntentionCode)));

        std::shared_ptr<UKControllerPlugin::Integration::MessageInterface> expectedMessage =
            std::make_shared<IntentionCodeUpdatedMessage>("BAW123", "", "A1");

        EXPECT_CALL(*connection, Send(expectedMessage->ToJson().dump())).Times(1);

        initialiser.Initialise(client);
    }

    TEST_F(IntentionCodeIntegrationDataInitialiserTest, ItSendsIntentionCodeUpdateMessageFirExitPointNotFoundById)
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

        EXPECT_CALL(*mockIntentionCodeGenerator, Generate(testing::Ref(*flightplan), testing::Ref(*radarTarget)))
            .WillOnce(testing::Return(std::make_shared<AircraftIntentionCode>(aircraftIntentionCode)));

        std::shared_ptr<UKControllerPlugin::Integration::MessageInterface> expectedMessage =
            std::make_shared<IntentionCodeUpdatedMessage>("BAW123", "", "A1");

        EXPECT_CALL(*connection, Send(expectedMessage->ToJson().dump())).Times(1);

        initialiser.Initialise(client);
    }

    TEST_F(IntentionCodeIntegrationDataInitialiserTest, ItSendsIntentionCodeUpdateMessageWithFirExitPoint)
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

        EXPECT_CALL(*mockIntentionCodeGenerator, Generate(testing::Ref(*flightplan), testing::Ref(*radarTarget)))
            .WillOnce(testing::Return(std::make_shared<AircraftIntentionCode>(aircraftIntentionCode)));

        std::shared_ptr<UKControllerPlugin::Integration::MessageInterface> expectedMessage =
            std::make_shared<IntentionCodeUpdatedMessage>("BAW123", "TEST", "A1");

        EXPECT_CALL(*connection, Send(expectedMessage->ToJson().dump())).Times(1);

        initialiser.Initialise(client);
    }
} // namespace UKControllerPluginTest::IntentionCode
