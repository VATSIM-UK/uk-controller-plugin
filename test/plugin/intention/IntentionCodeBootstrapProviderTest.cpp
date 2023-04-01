#include "bootstrap/ModuleFactories.h"
#include "controller/ActiveCallsignCollection.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "integration/IntegrationPersistenceContainer.h"
#include "integration/IntegrationServer.h"
#include "intention/IntentionCodeBootstrapProvider.h"
#include "intention/IntentionCodeEventHandlerCollection.h"
#include "intention/IntentionCodeModuleFactory.h"

using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Flightplan::FlightPlanEventHandlerCollection;
using UKControllerPlugin::IntentionCode::IntentionCodeBootstrapProvider;

namespace UKControllerPluginTest::IntentionCode {
    class IntentionCodeModuleBootstrapProviderTest : public BootstrapProviderTestCase
    {
        public:
        IntentionCodeModuleBootstrapProviderTest()
        {
            container.flightplanHandler = std::make_unique<FlightPlanEventHandlerCollection>();
            container.dependencyLoader = std::make_unique<testing::NiceMock<Dependency::MockDependencyLoader>>();
            container.integrationModuleContainer =
                std::unique_ptr<UKControllerPlugin::Integration::IntegrationPersistenceContainer>(
                    new UKControllerPlugin::Integration::IntegrationPersistenceContainer(
                        nullptr, nullptr, nullptr, nullptr));
            container.integrationModuleContainer->outboundMessageHandler =
                std::make_unique<testing::NiceMock<Integration::MockOutboundIntegrationEventHandler>>();
            container.activeCallsigns = std::make_shared<ActiveCallsignCollection>();
            ON_CALL(
                dynamic_cast<testing::NiceMock<Dependency::MockDependencyLoader>&>(*container.dependencyLoader),
                LoadDependency("DEPENDENCY_FIR_EXIT_POINTS", nlohmann::json::array()))
                .WillByDefault(testing::Return(nlohmann::json::array()));
        }

        IntentionCodeBootstrapProvider provider;
    };

    TEST_F(IntentionCodeModuleBootstrapProviderTest, TestItRegistersFirExitAndIntentionGeneratorsForFlightplanEvents)
    {
        this->RunBootstrapPlugin(provider);
        EXPECT_EQ(2, container.flightplanHandler->CountHandlers());
    }

    TEST_F(IntentionCodeModuleBootstrapProviderTest, TestItRegistersIntentionGeneratorForActiveCallsignEvents)
    {
        this->RunBootstrapPlugin(provider);
        EXPECT_EQ(1, container.activeCallsigns->CountHandlers());
    }

    TEST_F(IntentionCodeModuleBootstrapProviderTest, TestItRegistersIntegrationCodeUpdatedMessageForEvents)
    {
        this->RunBootstrapPlugin(provider);
        EXPECT_EQ(1, container.moduleFactories->IntentionCode().IntentionCodeEventHandlers()->CountHandlers());
    }
} // namespace UKControllerPluginTest::IntentionCode
