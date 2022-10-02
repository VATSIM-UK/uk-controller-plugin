#include "dialog/DialogData.h"

using UKControllerPlugin::Dialog::DialogData;

namespace UKControllerPluginUtilsTest {
    namespace Dialog {

        TEST(DialogDataTest, EqualityReturnsTrueBothEqual)
        {
            DialogData dialog1 = { 1, "Test", NULL, NULL, NULL };
            DialogData dialog2 = { 1, "Test", NULL, NULL, NULL };
            EXPECT_TRUE(dialog1 == dialog2);
        }

        TEST(DialogDataTest, EqualityReturnsFalseDifferentId)
        {
            DialogData dialog1 = { 1, "Test", NULL, NULL, NULL };
            DialogData dialog2 = { 2, "Test", NULL, NULL, NULL };
            EXPECT_FALSE(dialog1 == dialog2);
        }
    }  // namespace Dialog
}  // namespace UKControllerPluginUtilsTest
