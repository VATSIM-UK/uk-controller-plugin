#include "pch/pch.h"
#include "radarscreen/ConfigurableDisplayCollection.h"
#include "radarscreen/ConfigurableDisplayInterface.h"
#include "mock/MockConfigurableDisplay.h"
#include "plugin/PopupMenuItem.h"

using UKControllerPlugin::RadarScreen::ConfigurableDisplayInterface;
using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection;

namespace UKControllerPluginTest {
    namespace RadarScreen {

        TEST(ConfigurableDisplayCollection, EmptyCollectionHasMatchingIterators)
        {
            ConfigurableDisplayCollection collection;
            EXPECT_TRUE(collection.cbegin() == collection.cend());
        }

        TEST(ConfigurableDisplayCollection, CountReturnsNumberOfConfigurableDisplays)
        {
            ConfigurableDisplayCollection collection;
            std::shared_ptr<MockConfigurableDisplay> testDisplay = std::make_shared<MockConfigurableDisplay>();
            collection.RegisterDisplay(testDisplay);
            EXPECT_EQ(1, collection.CountDisplays());
        }

        TEST(ConfigurableDisplayCollection, DoesNotAddDuplicates)
        {
            ConfigurableDisplayCollection collection;
            std::shared_ptr<MockConfigurableDisplay> testDisplay = std::make_shared<MockConfigurableDisplay>();
            collection.RegisterDisplay(testDisplay);
            EXPECT_EQ(1, collection.CountDisplays());
            collection.RegisterDisplay(testDisplay);
            EXPECT_EQ(1, collection.CountDisplays());
        }

        TEST(ConfigurableDisplayCollection, IsIterable)
        {
            ConfigurableDisplayCollection collection;
            std::shared_ptr<MockConfigurableDisplay> testDisplay1 = std::make_shared<MockConfigurableDisplay>();
            std::shared_ptr<MockConfigurableDisplay> testDisplay2 = std::make_shared<MockConfigurableDisplay>();
            collection.RegisterDisplay(testDisplay1);
            collection.RegisterDisplay(testDisplay2);

            for (
                ConfigurableDisplayCollection::const_iterator it = collection.cbegin();
                it != collection.cend();
                ++it
            ) {
                (*it)->Configure("test");
                EXPECT_EQ(2, (*it)->GetConfigurationMenuItem().checked);
            }


            EXPECT_EQ(1, testDisplay1->configured);
            EXPECT_EQ(1, testDisplay1->configured);
        }
    }  // namespace RadarScreen
}  // namespace UKControllerPluginTest
