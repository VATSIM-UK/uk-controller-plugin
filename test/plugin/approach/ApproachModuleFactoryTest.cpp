#include "approach/ApproachModuleFactory.h"
#include "approach/ApproachSequencer.h"

using UKControllerPlugin::Approach::ApproachModuleFactory;

namespace UKControllerPluginTest::Approach {
    class ApproachModuleFactoryTest : public testing::Test
    {
        public:
        ApproachModuleFactory factory;
    };

    TEST_F(ApproachModuleFactoryTest, ItMakesASequencer)
    {
        EXPECT_NE(nullptr, &factory.Sequencer());
    }

    TEST_F(ApproachModuleFactoryTest, ItOnlyHasOneSequencer)
    {
        EXPECT_EQ(&factory.Sequencer(), &factory.Sequencer());
    }
} // namespace UKControllerPluginTest::Approach
