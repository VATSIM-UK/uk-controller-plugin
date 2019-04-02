#include "pch/pch.h"
#include "dialog/DialogManager.h"
#include "mock/MockDialogProvider.h"
#include "dialog/DialogData.h"

using UKControllerPlugin::Dialog::DialogData;
using UKControllerPlugin::Dialog::DialogManager;
using UKControllerPluginTest::Dialog::MockDialogProvider;
using ::testing::Test;
using ::testing::NiceMock;
using ::testing::_;

namespace UKControllerPluginTest {
    namespace Dialog {

        class DialogManagerTest : public Test
        {
            public:
                DialogManagerTest()
                    : manager(mockProvider), mockProvider(testDialog, testDialogArg, testContextArg)
                {

                }


                DialogData testDialog = { 1, "TEST", NULL, NULL, NULL };
                MockDialogProvider<std::string, std::string> mockProvider;
                DialogManager manager;

                std::string testDialogArg = "testd";
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
            this->manager.AddDialog(testDialog);
            this->manager.OpenDialog(555, reinterpret_cast<LPARAM>(&this->testContextArg));
            EXPECT_EQ(0, this->mockProvider.callCount);
        }

        TEST_F(DialogManagerTest, OpensDialogsWithProvider)
        {
            this->manager.OpenDialog(1, reinterpret_cast<LPARAM>(&this->testContextArg));
            EXPECT_EQ(0, this->mockProvider.callCount);
        }
    }  // namespace Dialog
}  // namespace UKControllerPluginTest
