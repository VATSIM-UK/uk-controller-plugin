#include "mapping/MappingBootstrapProvider.h"

using UKControllerPlugin::Mapping::MappingBootstrapProvider;

namespace UKControllerPluginTest::Mapping {
    class MappingBootstrapProviderTest : public BootstrapProviderTestCase
    {
        public:\
        MappingBootstrapProvider provider;
    };

    TEST_F(MappingBootstrapProviderTest, ItBootstrapsRendererToRadarScreen)
    {
        this->RunBootstrapRadarScreen(provider);
        EXPECT_EQ(1, renderers.CountRenderers());
        EXPECT_EQ(1, renderers.CountRenderersInPhase(renderers.beforeTags));
    }
} // namespace UKControllerPluginTest::Mapping
