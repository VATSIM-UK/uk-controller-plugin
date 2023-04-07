#include "approach/ApproachModuleFactory.h"
#include "bootstrap/ModuleFactories.h"
#include "intention/IntentionCodeModuleFactory.h"

using UKControllerPlugin::Bootstrap::ModuleFactories;

namespace UKControllerPluginTest::Bootstrap {
    class ModuleFactoriesTest : public testing::Test
    {
        public:
        ModuleFactories factories;
    };

    TEST_F(ModuleFactoriesTest, ItReturnsApproachFactoryAsSingleton)
    {
        EXPECT_NE(nullptr, &factories.Approach());
        EXPECT_EQ(&factories.Approach(), &factories.Approach());
    }

    TEST_F(ModuleFactoriesTest, ItReturnsIntentionCodeFactoryAsSingleton)
    {
        EXPECT_NE(nullptr, &factories.IntentionCode());
        EXPECT_EQ(&factories.IntentionCode(), &factories.IntentionCode());
    }
} // namespace UKControllerPluginTest::Bootstrap
