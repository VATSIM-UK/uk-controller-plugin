#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "intention/IntentionCodeBootstrapProvider.h"

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
            ON_CALL(
                dynamic_cast<testing::NiceMock<Dependency::MockDependencyLoader>&>(*container.dependencyLoader),
                LoadDependency("DEPENDENCY_FIR_EXIT_POINTS", nlohmann::json::array()))
                .WillByDefault(testing::Return(nlohmann::json::array()));
        }

        IntentionCodeBootstrapProvider provider;
    };

    TEST_F(IntentionCodeModuleBootstrapProviderTest, TestItLoadsExitPointsAsSingleton)
    {
        this->RunBootstrapPlugin(provider);
        EXPECT_EQ(1, container.flightplanHandler->CountHandlers());
    }
} // namespace UKControllerPluginTest::IntentionCode
