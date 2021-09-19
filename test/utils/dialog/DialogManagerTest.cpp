#include "dialog/DialogManager.h"
#include "dialog/DialogData.h"

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Test;
using UKControllerPlugin::Dialog::DialogData;
using UKControllerPlugin::Dialog::DialogManager;
using UKControllerPluginTest::Dialog::MockDialogProvider;

namespace UKControllerPluginUtilsTest {
    namespace Dialog {

        class DialogManagerTest : public Test
        {
            public:
            DialogManagerTest() : manager(mockProvider)
            {
            }

            DialogData testDialog = {1, "TEST", NULL, NULL, NULL};
            NiceMock<MockDialogProvider> mockProvider;
            DialogManager manager;

            std::string testContextArg = "testc";
        };

        TEST_F(DialogManagerTest, ItStartsEmpty)
        {
            EXPECT_EQ(0, this->manager.CountDialogs());
        }

        TEST_F(DialogManagerTest, ItAddsADialog)
        {
            this->manager.AddDialog(testDialog);
            EXPECT_EQ(1, this->manager.CountDialogs());
            EXPECT_TRUE(this->manager.HasDialog(1));
        }

        TEST_F(DialogManagerTest, AddDialogReturnsTrueSuccess)
        {
            EXPECT_TRUE(this->manager.AddDialog(testDialog));
        }

        TEST_F(DialogManagerTest, AddDialogReturnsFalseOnFailure)
        {
            this->manager.AddDialog(testDialog);
            EXPECT_FALSE(this->manager.AddDialog(testDialog));
        }

        TEST_F(DialogManagerTest, AddDialogDoesntDuplicate)
        {
            this->manager.AddDialog(testDialog);
            this->manager.AddDialog(testDialog);
            EXPECT_EQ(1, this->manager.CountDialogs());
            EXPECT_TRUE(this->manager.HasDialog(1));
        }

        TEST_F(DialogManagerTest, DoesntTryToOpenNonExistantDialogs)
        {
            EXPECT_CALL(this->mockProvider, OpenDialog(_, _)).Times(0);

            this->manager.AddDialog(testDialog);
            this->manager.OpenDialog(555, reinterpret_cast<LPARAM>(&this->testContextArg));
        }

        TEST_F(DialogManagerTest, OpensDialogsWithProvider)
        {
            EXPECT_CALL(this->mockProvider, OpenDialog(this->testDialog, _)).Times(1);

            this->manager.AddDialog(testDialog);
            this->manager.OpenDialog(1, reinterpret_cast<LPARAM>(&this->testContextArg));
        }
    } // namespace Dialog
} // namespace UKControllerPluginUtilsTest
