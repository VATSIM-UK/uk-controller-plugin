#include "controller/ControllerPosition.h"
#include "handoff/DepartureHandoffIntegrationDataInitialiser.h"
#include "handoff/HandoffFrequencyUpdatedMessage.h"
#include "handoff/ResolvedHandoff.h"
#include "integration/IntegrationClient.h"
#include "integration/IntegrationConnection.h"

namespace UKControllerPluginTest::Handoff {
    class DepartureHandoffIntegrationDataInitialiserTest : public ::testing::Test
    {
        public:
        DepartureHandoffIntegrationDataInitialiserTest()
            : connection(std::make_shared<testing::NiceMock<Integration::MockConnection>>()),
              client(1, "1", "1", std::make_shared<UKControllerPlugin::Integration::IntegrationConnection>(connection)),
              resolver(std::make_shared<testing::NiceMock<MockDepartureHandoffResolver>>()),
              initialiser(resolver, mockPlugin)
        {
        }

        std::shared_ptr<testing::NiceMock<Integration::MockConnection>> connection;
        UKControllerPlugin::Integration::IntegrationClient client;
        testing::NiceMock<Euroscope::MockEuroscopePluginLoopbackInterface> mockPlugin;
        std::shared_ptr<testing::NiceMock<MockDepartureHandoffResolver>> resolver;
        UKControllerPlugin::Handoff::DepartureHandoffIntegrationDataInitialiser initialiser;
    };

    TEST_F(DepartureHandoffIntegrationDataInitialiserTest, ItInitialisesForTheCorrectMessageType)
    {
        EXPECT_EQ(
            (UKControllerPlugin::Handoff::HandoffFrequencyUpdatedMessage("", "")).GetMessageType(),
            initialiser.InitialisesFor());
    }

    TEST_F(DepartureHandoffIntegrationDataInitialiserTest, ItBroadcastsCachedHandoffs)
    {
        auto controllerPosition1 = std::make_shared<UKControllerPlugin::Controller::ControllerPosition>(
            -1, "UNICOM", 123.456, std::vector<std::string>{}, true, false);
        auto controllerPosition2 = std::make_shared<UKControllerPlugin::Controller::ControllerPosition>(
            -1, "UNICOM", 234.567, std::vector<std::string>{}, true, false);

        auto handoff1 = std::make_shared<UKControllerPlugin::Handoff::ResolvedHandoff>(
            "BAW123", controllerPosition1, nullptr, nullptr);
        auto handoff2 = std::make_shared<UKControllerPlugin::Handoff::ResolvedHandoff>(
            "BAW456", controllerPosition2, nullptr, nullptr);

        auto mockFlightplan1 = std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>>();
        auto mockRadarTarget1 = std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface>>();
        auto mockFlightplan2 = std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>>();
        auto mockRadarTarget2 = std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface>>();

        mockPlugin.AddAllFlightplansItem({mockFlightplan1, mockRadarTarget1});
        mockPlugin.AddAllFlightplansItem({mockFlightplan2, mockRadarTarget2});

        EXPECT_CALL(*resolver, Resolve(testing::Ref(*mockFlightplan1))).WillOnce(testing::Return(handoff1));
        EXPECT_CALL(*resolver, Resolve(testing::Ref(*mockFlightplan2))).WillOnce(testing::Return(handoff2));

        // Expectations on the connection
        UKControllerPlugin::Handoff::HandoffFrequencyUpdatedMessage expected1("BAW123", "123.456");
        UKControllerPlugin::Handoff::HandoffFrequencyUpdatedMessage expected2("BAW456", "234.567");

        EXPECT_CALL(*connection, Send(expected1.ToJson().dump())).Times(1);
        EXPECT_CALL(*connection, Send(expected2.ToJson().dump())).Times(1);

        initialiser.Initialise(client);
    }

} // namespace UKControllerPluginTest::Handoff
