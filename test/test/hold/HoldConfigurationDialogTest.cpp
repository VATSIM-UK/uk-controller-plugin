#include "pch/pch.h"
#include "hold/HoldConfigurationDialog.h"
#include "hold/HoldingData.h"
#include "hold/HoldProfileManager.h"
#include "mock/MockApiInterface.h"

using UKControllerPlugin::Hold::HoldConfigurationDialog;
using UKControllerPlugin::Hold::HoldingData;
using UKControllerPlugin::Hold::HoldProfileManager;
using UKControllerPluginTest::Api::MockApiInterface;
using ::testing::Return;
using ::testing::NiceMock;
using ::testing::Test;

namespace UKControllerPluginTest {
    namespace Hold {

        class HoldConfigurationDialogTest : public Test
        {
            public:
                HoldConfigurationDialogTest()
                    : dialog(holdProfileManager), holdProfileManager(mockApi)
                {

                }

                NiceMock<MockApiInterface> mockApi;
                HoldProfileManager holdProfileManager;
                HoldConfigurationDialog dialog;
        };

        TEST_F(HoldConfigurationDialogTest, ItStartsEmpty)
        {
            EXPECT_EQ(0, this->dialog.CountHolds());
        }

        TEST_F(HoldConfigurationDialogTest, ItAddsAHold)
        {
            HoldingData hold = { 1, "TIMBA", "TIMBA", 8000, 15000, 209, "left" };
            this->dialog.AddHold(std::move(hold));
            EXPECT_EQ(1, this->dialog.CountHolds());
        }

        TEST_F(HoldConfigurationDialogTest, ItReturnsTrueOnSuccessfulHoldAdd)
        {
            HoldingData hold = { 1, "TIMBA", "TIMBA", 8000, 15000, 209, "left" };
            EXPECT_TRUE(this->dialog.AddHold(std::move(hold)));
        }

        TEST_F(HoldConfigurationDialogTest, ItDoesntAddDuplicateHolds)
        {
            HoldingData hold = { 1, "TIMBA", "TIMBA", 8000, 15000, 209, "left" };
            this->dialog.AddHold(std::move(hold));
            this->dialog.AddHold(std::move(hold));
            EXPECT_EQ(1, this->dialog.CountHolds());
        }

        TEST_F(HoldConfigurationDialogTest, ItReturnsFalseOnUnsuccessfulHoldAdd)
        {
            HoldingData hold = { 1, "TIMBA", "TIMBA", 8000, 15000, 209, "left" };
            this->dialog.AddHold(std::move(hold));
            EXPECT_FALSE(this->dialog.AddHold(std::move(hold)));
        }
    }  // namespace Hold
}  // namespace UKControllerPluginTest
