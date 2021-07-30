#include "pch/pch.h"
#include "hold/HoldConfigurationMenuItem.h"
#include "dialog/DialogManager.h"
#include "mock/MockDialogProvider.h"
#include "dialog/DialogData.h"
#include "hold/HoldDisplayManager.h"
#include "hold/HoldManager.h"
#include "hold/HoldDisplayFactory.h"
#include "mock/MockApiInterface.h"
#include "mock/MockEuroscopePluginLoopbackInterface.h"
#include "plugin/PopupMenuItem.h"
#include "navaids/NavaidCollection.h"
#include "hold/PublishedHoldCollection.h"
#include "mock/MockTaskRunnerInterface.h"

using UKControllerPlugin::Hold::HoldConfigurationMenuItem;
using UKControllerPlugin::Dialog::DialogManager;
using UKControllerPluginTest::Dialog::MockDialogProvider;
using UKControllerPlugin::Dialog::DialogData;
using UKControllerPlugin::Hold::HoldDisplayManager;
using UKControllerPlugin::Hold::HoldManager;
using UKControllerPlugin::Hold::HoldDisplayFactory;
using UKControllerPluginTest::Api::MockApiInterface;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPluginTest::TaskManager::MockTaskRunnerInterface;
using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPlugin::Hold::PublishedHoldCollection;
using UKControllerPlugin::Navaids::NavaidCollection;
using ::testing::NiceMock;
using ::testing::Test;
using ::testing::_;

namespace UKControllerPluginTest {
    namespace Hold {

        class HoldConfigurationMenuItemTest : public Test
        {
            public:
                HoldConfigurationMenuItemTest()
                : dialogManager(mockProvider),
                    displayFactory(mockPlugin, holdManager, navaids, holds, dialogManager),
                    menuItem(dialogManager, displayManager, 1),
                    displayManager(new HoldDisplayManager(holdManager, displayFactory)),
                    holdManager(mockApi, mockTaskRunner)
                {
                    this->dialogManager.AddDialog(this->dialogData);
                }

                NavaidCollection navaids;
                PublishedHoldCollection holds;
                DialogData dialogData = { IDD_HOLD_SELECTION, "", NULL, NULL, NULL };
                NiceMock<MockTaskRunnerInterface> mockTaskRunner;
                NiceMock<MockApiInterface> mockApi;
                NiceMock<MockEuroscopePluginLoopbackInterface> mockPlugin;
                UKControllerPlugin::Dialog::DialogManager dialogManager;
                NiceMock<MockDialogProvider> mockProvider;
                HoldManager holdManager;
                HoldDisplayFactory displayFactory;
                std::shared_ptr<HoldDisplayManager> displayManager;
                HoldConfigurationMenuItem menuItem;
        };

        TEST_F(HoldConfigurationMenuItemTest, GetConfigurationMenuItemReturnsItem)
        {
            PopupMenuItem expected;
            expected.firstValue = this->menuItem.menuItemDescription;
            expected.secondValue = "";
            expected.callbackFunctionId = 1;
            expected.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
            expected.disabled = false;
            expected.fixedPosition = false;


            EXPECT_TRUE(expected == this->menuItem.GetConfigurationMenuItem());
        }

        TEST_F(HoldConfigurationMenuItemTest, MenuItemConfigurationOpensDialog)
        {
            EXPECT_CALL(this->mockProvider, OpenDialog(this->dialogData, _))
                .Times(1);

            this->menuItem.Configure(0, "", {});
        }

        TEST_F(HoldConfigurationMenuItemTest, ProcessCommandReturnsFalseIfUnknownCommand)
        {
            EXPECT_FALSE(this->menuItem.ProcessCommand("bla"));
        }

        TEST_F(HoldConfigurationMenuItemTest, ProcessCommandReturnsTrueIfValidCommand)
        {
            EXPECT_TRUE(this->menuItem.ProcessCommand(".ukcp hold"));
        }

        TEST_F(HoldConfigurationMenuItemTest, ProcessCommandOpensDialog)
        {
            EXPECT_CALL(this->mockProvider, OpenDialog(this->dialogData, _))
                .Times(1);

            this->menuItem.ProcessCommand(".ukcp hold");
        }

        TEST_F(HoldConfigurationMenuItemTest, SelectHoldsSetsHolds)
        {
            this->menuItem.SelectHolds(std::vector<std::string>({ "TIMBA", "LAM" }));
            EXPECT_EQ(std::vector<std::string>({ "TIMBA", "LAM" }), this->menuItem.GetHolds());
        }
    }  // namespace Hold
}  // namespace UKControllerPluginTest
