#include "approach/ApproachBootstrapProvider.h"

using UKControllerPlugin::Approach::ApproachBootstrapProvider;

namespace UKControllerPluginTest::Approach {
    class ApproachBootstrapProviderTest : public BootstrapProviderTestCase
    {
        public:
        ApproachBootstrapProvider provider;
    };

    TEST_F(ApproachBootstrapProviderTest, ItRegistersTheRenderers)
    {
        this->RunBootstrapRadarScreen(provider);
        EXPECT_EQ(2, renderers.CountRenderers());
        EXPECT_EQ(2, renderers.CountRenderersInPhase(renderers.beforeTags));
    }
} // namespace UKControllerPluginTest::Approach
