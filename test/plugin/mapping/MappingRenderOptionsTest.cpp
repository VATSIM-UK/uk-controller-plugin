#include "mapping/MappingRenderOptions.h"

using UKControllerPlugin::Mapping::MappingRenderOptions;

namespace UKControllerPluginTest::Mapping {
    class MappingRenderOptionsTest : public testing::Test
    {
        public:
        MappingRenderOptions renderOptions;
    };

    TEST_F(MappingRenderOptionsTest, ItDoesntShowOptionsByDefault)
    {
        EXPECT_FALSE(renderOptions.ShowOptions());
    }

    TEST_F(MappingRenderOptionsTest, ItCanShowOptions)
    {
        renderOptions.ShowOptions(true);
        EXPECT_TRUE(renderOptions.ShowOptions());
        renderOptions.ShowOptions(false);
        EXPECT_FALSE(renderOptions.ShowOptions());
    }

    TEST_F(MappingRenderOptionsTest, ItDoesntShowVisualReferencePointsByDefault)
    {
        EXPECT_FALSE(renderOptions.ShowVisualReferencePoints());
    }

    TEST_F(MappingRenderOptionsTest, ItCanShowVisualReferencePoints)
    {
        renderOptions.ShowVisualReferencePoints(true);
        EXPECT_TRUE(renderOptions.ShowVisualReferencePoints());
        renderOptions.ShowVisualReferencePoints(false);
        EXPECT_FALSE(renderOptions.ShowVisualReferencePoints());
    }
} // namespace UKControllerPluginTest::Mapping
