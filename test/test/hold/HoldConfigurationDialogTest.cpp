#include "pch/pch.h"
#include "hold/HoldConfigurationDialog.h"
#include "hold/HoldingData.h"
#include "hold/HoldWindowManager.h"
#include "hold/HoldManager.h"
#include "mock/MockEuroscopePluginLoopbackInterface.h"
#include "hold/HoldSelectionMenu.h"
#include "hold/HoldProfileManager.h"
#include "mock/MockApiInterface.h"

using UKControllerPlugin::Hold::HoldConfigurationDialog;
using UKControllerPlugin::Hold::HoldingData;
using UKControllerPlugin::Hold::HoldWindowManager;
using UKControllerPlugin::Hold::HoldManager;
using UKControllerPlugin::Hold::HoldProfileManager;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPlugin::Hold::HoldSelectionMenu;
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
                    : windowManager(NULL, NULL, holdManager, mockPlugin), selectionMenu(holdManager, mockPlugin, 1),
                    dialog(windowManager, selectionMenu, holdProfileManager), holdProfileManager(mockApi)
                {

                }

                NiceMock<MockApiInterface> mockApi;
                NiceMock<MockEuroscopePluginLoopbackInterface> mockPlugin;
                HoldProfileManager holdProfileManager;
                HoldManager holdManager;
                HoldWindowManager windowManager;
                HoldSelectionMenu selectionMenu;
                HoldConfigurationDialog dialog;
        };

        TEST_F(HoldConfigurationDialogTest, ItStartsEmpty)
        {
            EXPECT_EQ(0, this->dialog.CountHolds());
        }

        TEST_F(HoldConfigurationDialogTest, ItAddsAHold)
        {
            HoldingData hold = { 1, "TIMBA", "TIMBA", 8000, 15000, 209, "left" };
            this->dialog.AddHold(hold);
            EXPECT_EQ(1, this->dialog.CountHolds());
        }

        TEST_F(HoldConfigurationDialogTest, ItReturnsTrueOnSuccessfulHoldAdd)
        {
            HoldingData hold = { 1, "TIMBA", "TIMBA", 8000, 15000, 209, "left" };
            EXPECT_TRUE(this->dialog.AddHold(hold));
        }

        TEST_F(HoldConfigurationDialogTest, ItDoesntAddDuplicateHolds)
        {
            HoldingData hold = { 1, "TIMBA", "TIMBA", 8000, 15000, 209, "left" };
            this->dialog.AddHold(hold);
            this->dialog.AddHold(hold);
            EXPECT_EQ(1, this->dialog.CountHolds());
        }

        TEST_F(HoldConfigurationDialogTest, ItReturnsFalseOnUnsuccessfulHoldAdd)
        {
            HoldingData hold = { 1, "TIMBA", "TIMBA", 8000, 15000, 209, "left" };
            this->dialog.AddHold(hold);
            EXPECT_FALSE(this->dialog.AddHold(hold));
        }
    }  // namespace Hold
}  // namespace UKControllerPluginTest
