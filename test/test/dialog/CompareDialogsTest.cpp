#pragma once
#include "pch/pch.h"
#include "dialog/DialogData.h"
#include "dialog/CompareDialogs.h"

using UKControllerPlugin::Dialog::DialogData;
using UKControllerPlugin::Dialog::CompareDialogs;
using ::testing::Test;

namespace UKControllerPluginTest {
    namespace Dialog {

        class CompareDialogsTest : public Test
        {
            public:
                CompareDialogs compare;
        };

        TEST_F(CompareDialogsTest, LessThanUnsignedIntReturnsTrueIfLessThan)
        {
            DialogData dialog = { 1, "Test", NULL, NULL, NULL };
            EXPECT_TRUE(compare(dialog, 2));
        }

        TEST_F(CompareDialogsTest, LessThanStructReturnsTrueIfLessThan)
        {
            DialogData dialog = { 1, "Test", NULL, NULL, NULL };
            EXPECT_TRUE(compare(0, dialog));
        }

        TEST_F(CompareDialogsTest, CompareReturnsTrueIfFirstLessThanLast)
        {
            DialogData dialog1 = { 1, "Test", NULL, NULL, NULL };
            DialogData dialog2 = { 12, "Test", NULL, NULL, NULL };
            EXPECT_TRUE(compare(dialog1, dialog2));
        }
    }  // namespace Dialog
}  // namespace UKControllerPluginTest
