#include "controller/ActiveCallsignCollection.h"
#include "intention/CachedAircraftFirExitGenerator.h"
#include "intention/CachedAircraftIntentionCodeGenerator.h"
#include "intention/IntentionCodeEventHandlerCollection.h"
#include "intention/IntentionCodeModuleFactory.h"

using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::IntentionCode::CachedAircraftFirExitGenerator;
using UKControllerPlugin::IntentionCode::CachedAircraftIntentionCodeGenerator;
using UKControllerPlugin::IntentionCode::IntentionCodeModuleFactory;

namespace UKControllerPluginTest::IntentionCode {
    class IntentionCodeModuleFactoryTest : public testing::Test
    {
        public:
        IntentionCodeModuleFactoryTest()
        {
            ON_CALL(dependencyLoader, LoadDependency("DEPENDENCY_FIR_EXIT_POINTS", nlohmann::json::array()))
                .WillByDefault(testing::Return(nlohmann::json::array()));

            ON_CALL(dependencyLoader, LoadDependency("DEPENDENCY_INTENTION_CODES", nlohmann::json::array()))
                .WillByDefault(testing::Return(nlohmann::json::array()));
        }

        ActiveCallsignCollection activeCallsigns;
        testing::NiceMock<Dependency::MockDependencyLoader> dependencyLoader;
        IntentionCodeModuleFactory factory;
    };

    TEST_F(IntentionCodeModuleFactoryTest, TestItLoadsExitPointsAsSingleton)
    {
        EXPECT_EQ(&factory.ExitPointCollection(dependencyLoader), &factory.ExitPointCollection(dependencyLoader));
    }

    TEST_F(IntentionCodeModuleFactoryTest, TestItLoadsIntentionCodesAsSingleton)
    {
        EXPECT_EQ(
            &factory.IntentionCodes(dependencyLoader, activeCallsigns),
            &factory.IntentionCodes(dependencyLoader, activeCallsigns));
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

    TEST_F(IntentionCodeModuleFactoryTest, TestItLoadsEventHandlersAsSingleton)
    {
        const auto& handlers = factory.IntentionCodeEventHandlers();
        EXPECT_EQ(0, handlers.CountHandlers());
        EXPECT_EQ(&handlers, &factory.IntentionCodeEventHandlers());
    }

    TEST_F(IntentionCodeModuleFactoryTest, TestItLoadsIntentionCodeGeneratorAsSingleton)
    {
        EXPECT_EQ(
            &factory.IntentionCodeGenerator(dependencyLoader, activeCallsigns),
            &factory.IntentionCodeGenerator(dependencyLoader, activeCallsigns));
    }

    TEST_F(IntentionCodeModuleFactoryTest, TestItLoadsCachedIntentionCodeGeneratorAsSingleton)
    {
        EXPECT_EQ(
            &factory.CachedIntentionCodeGenerator(dependencyLoader, activeCallsigns),
            &factory.CachedIntentionCodeGenerator(dependencyLoader, activeCallsigns));
    }

    TEST_F(IntentionCodeModuleFactoryTest, IntentionCodeGeneratorIsCachedInstance)
    {
        try {
            static_cast<void>(dynamic_cast<CachedAircraftIntentionCodeGenerator&>(
                factory.IntentionCodeGenerator(dependencyLoader, activeCallsigns)));
            SUCCEED();
        } catch (std::bad_cast&) {
            FAIL();
        }
    }
} // namespace UKControllerPluginTest::IntentionCode
