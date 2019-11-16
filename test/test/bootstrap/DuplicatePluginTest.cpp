#include "pch/pch.h"
#include "bootstrap/DuplicatePlugin.h"

using UKControllerPlugin::Bootstrap::DuplicatePlugin;

namespace UKControllerPluginTest {
    namespace Bootstrap {

        TEST(DuplicatePluginTest, FirstPluginIsNotADuplicate)
        {
            DuplicatePlugin plugin;
            EXPECT_FALSE(plugin.Duplicate());
        }

        TEST(DuplicatePluginTest, SecondPluginIsDuplicate)
        {
            DuplicatePlugin plugin1;
            EXPECT_FALSE(plugin1.Duplicate());

            DuplicatePlugin plugin2;
            EXPECT_TRUE(plugin2.Duplicate());
        }

        TEST(DuplicatePluginTest, NextPluginWillNotBeDuplicateIfPrimaryGoesAway)
        {
            DuplicatePlugin * plugin1 = new DuplicatePlugin;
            EXPECT_FALSE(plugin1->Duplicate());

            DuplicatePlugin plugin2;
            EXPECT_TRUE(plugin2.Duplicate());

            delete plugin1;

            DuplicatePlugin plugin3;
            EXPECT_FALSE(plugin3.Duplicate());
        }
    }  // namespace Bootstrap
}  // namespace UKControllerPluginTest
