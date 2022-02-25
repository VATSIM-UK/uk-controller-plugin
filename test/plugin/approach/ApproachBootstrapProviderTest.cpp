#include "approach/ApproachBootstrapProvider.h"

using UKControllerPlugin::Approach::ApproachBootstrapProvider;

namespace UKControllerPluginTest::Approach {
    class ApproachBootstrapProviderTest : public BootstrapProviderTestCase
    {
        public:
        ApproachBootstrapProvider provider;
    };

    TEST_F(ApproachBootstrapProviderTest, ItRegistersTheRingRenderer)
    {
        this->RunBootstrapRadarScreen(provider);
        EXPECT_EQ(1, renderers.CountRenderers());
    }
} // namespace UKControllerPluginTest::Approach
