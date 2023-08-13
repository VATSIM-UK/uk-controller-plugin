#include "ecfmp/ECFMPBootstrapProvider.h"
#include "test/BootstrapProviderTestCase.h"
#include "tag/TagItemCollection.h"
#include "timedevent/TimedEventCollection.h"

namespace UKControllerPluginTest::ECFMP {
    class ECFMPBootstrapProviderTest : public UKControllerPluginTest::BootstrapProviderTestCase
    {
        public:
        ECFMPBootstrapProviderTest()
        {
        }

        UKControllerPlugin::ECFMP::ECFMPBootstrapProvider provider;
    };

    TEST_F(ECFMPBootstrapProviderTest, ItRegistersForTimedEventsOnBootstrapPlugin)
    {
        RunBootstrapPlugin(provider);
        EXPECT_EQ(1, container.timedHandler->CountHandlers());
        EXPECT_EQ(1, container.timedHandler->CountHandlersForFrequency(1));
    }

    TEST_F(ECFMPBootstrapProviderTest, ItRegistersTagItemForRelevantFlowMeasures)
    {
        RunBootstrapPlugin(provider);
        EXPECT_EQ(1, container.tagHandler->CountHandlers());
        EXPECT_EQ(1, container.tagHandler->HasHandlerForItemId(131));
    }
} // namespace UKControllerPluginTest::ECFMP
