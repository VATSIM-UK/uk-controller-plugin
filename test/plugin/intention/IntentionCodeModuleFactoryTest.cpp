#include "intention/CachedAircraftFirExitGenerator.h"
#include "intention/IntentionCodeModuleFactory.h"

using UKControllerPlugin::IntentionCode::CachedAircraftFirExitGenerator;
using UKControllerPlugin::IntentionCode::IntentionCodeModuleFactory;

namespace UKControllerPluginTest::IntentionCode {
    class IntentionCodeModuleFactoryTest : public testing::Test
    {
        public:
        IntentionCodeModuleFactoryTest()
        {
            ON_CALL(dependencyLoader, LoadDependency("DEPENDENCY_FIR_EXIT_POINTS", nlohmann::json::array()))
                .WillByDefault(testing::Return(nlohmann::json::array()));
        }
        testing::NiceMock<Dependency::MockDependencyLoader> dependencyLoader;
        IntentionCodeModuleFactory factory;
    };

    TEST_F(IntentionCodeModuleFactoryTest, TestItLoadsExitPointsAsSingleton)
    {
        EXPECT_EQ(&factory.ExitPointCollection(dependencyLoader), &factory.ExitPointCollection(dependencyLoader));
    }

    TEST_F(IntentionCodeModuleFactoryTest, TestItLoadsCachedExitGeneratorAsSingleton)
    {
        EXPECT_EQ(factory.CachedFirExitGenerator(dependencyLoader), factory.CachedFirExitGenerator(dependencyLoader));
    }

    TEST_F(IntentionCodeModuleFactoryTest, TestItLoadsExitGeneratorAsSingleton)
    {
        EXPECT_EQ(&factory.FirExitGenerator(dependencyLoader), &factory.FirExitGenerator(dependencyLoader));
    }

    TEST_F(IntentionCodeModuleFactoryTest, ExitGeneratorIsCachedInstance)
    {
        try {
            static_cast<void>(
                dynamic_cast<CachedAircraftFirExitGenerator&>(factory.FirExitGenerator(dependencyLoader)));
            SUCCEED();
        } catch (std::bad_cast&) {
            FAIL();
        }
    }
} // namespace UKControllerPluginTest::IntentionCode
