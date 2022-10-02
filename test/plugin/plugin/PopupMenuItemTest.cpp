#include "plugin/PopupMenuItem.h"

using UKControllerPlugin::Plugin::PopupMenuItem;

namespace UKControllerPluginTest {
    namespace Plugin {

        TEST(PopupMenuItem, EqualityReturnsTrueForEqualObjects)
        {
            PopupMenuItem item1;
            PopupMenuItem item2;
            EXPECT_TRUE(item1 == item2);
        }

        TEST(PopupMenuItem, EqualityReturnsFalseIfFirstValueDifferent)
        {
            PopupMenuItem item1;
            item1.firstValue = "notthesame";
            PopupMenuItem item2;
            EXPECT_FALSE(item1 == item2);
        }

        TEST(PopupMenuItem, EqualityReturnsFalseIfSecondValueDifferent)
        {
            PopupMenuItem item1;
            item1.secondValue = "notthesame";
            PopupMenuItem item2;
            EXPECT_FALSE(item1 == item2);
        }

        TEST(PopupMenuItem, EqualityReturnsFalseIfCallbackIdDifferent)
        {
            PopupMenuItem item1;
            item1.callbackFunctionId = 10;
            PopupMenuItem item2;
            EXPECT_FALSE(item1 == item2);
        }

        TEST(PopupMenuItem, EqualityReturnsFalseIfCheckedDifferent)
        {
            PopupMenuItem item1;
            item1.checked = 66;
            PopupMenuItem item2;
            EXPECT_FALSE(item1 == item2);
        }

        TEST(PopupMenuItem, EqualityReturnsFalseIfDisabledDifferent)
        {
            PopupMenuItem item1;
            item1.disabled = false;
            PopupMenuItem item2;
            EXPECT_FALSE(item1 == item2);
        }

        TEST(PopupMenuItem, EqualityReturnsFalseIfFixedPositionDifferent)
        {
            PopupMenuItem item1;
            item1.fixedPosition = true;
            PopupMenuItem item2;
            EXPECT_FALSE(item1 == item2);
        }
    } // namespace Plugin
} // namespace UKControllerPluginTest
