#include "approach/ApproachModuleFactory.h"
#include "bootstrap/ModuleFactories.h"

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
} // namespace UKControllerPluginTest::Bootstrap
