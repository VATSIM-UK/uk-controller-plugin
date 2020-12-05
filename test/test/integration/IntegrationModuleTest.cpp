#include "pch/pch.h"
#include "integration/IntegrationModule.h"

using ::testing::Test;
using UKControllerPlugin::Integration::IntegrationModule;

namespace UKControllerPluginTest {
    namespace Integration {

        class IntegrationModuleTest : public Test
        {

        };

        TEST_F(IntegrationModuleTest, ItDoesSomething)
        {
            EXPECT_TRUE(true);
        }
    }  // namespace Integration
}  // namespace UKControllerPluginTest
