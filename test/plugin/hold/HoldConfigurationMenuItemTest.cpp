#include "aircraft/CallsignSelectionListFactory.h"
#include "hold/HoldConfigurationMenuItem.h"
#include "dialog/DialogManager.h"
#include "hold/HoldDisplayManager.h"
#include "hold/HoldManager.h"
#include "hold/HoldDisplayFactory.h"
#include "hold/PublishedHoldCollection.h"
#include "navaids/NavaidCollection.h"
#include "plugin/FunctionCallEventHandler.h"

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Test;
using UKControllerPlugin::Aircraft::CallsignSelectionListFactory;
using UKControllerPlugin::Dialog::DialogData;
using UKControllerPlugin::Dialog::DialogManager;
using UKControllerPlugin::Hold::HoldConfigurationMenuItem;
using UKControllerPlugin::Hold::HoldDisplayFactory;
using UKControllerPlugin::Hold::HoldDisplayManager;
using UKControllerPlugin::Hold::HoldManager;
using UKControllerPlugin::Hold::PublishedHoldCollection;
using UKControllerPlugin::Navaids::NavaidCollection;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPluginTest::Api::MockApiInterface;
using UKControllerPluginTest::Dialog::MockDialogProvider;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPluginTest::TaskManager::MockTaskRunnerInterface;

namespace UKControllerPluginTest {
    namespace Hold {

        class HoldConfigurationMenuItemTest : public Test
        {
            public:
            HoldConfigurationMenuItemTest()
                : listFactory(functionHandlers, mockPlugin), dialogManager(mockProvider),
                  holdManager(mockApi, mockTaskRunner),
                  displayFactory(mockPlugin, holdManager, navaids, holds, dialogManager, listFactory),
                  displayManager(new HoldDisplayManager(displayFactory)), menuItem(dialogManager, displayManager, 1)
            {
                this->dialogManager.AddDialog(this->dialogData);
            }

            NavaidCollection navaids;
            PublishedHoldCollection holds;
            DialogData dialogData = {IDD_HOLD_SELECTION, "", NULL, NULL, NULL};
            NiceMock<MockTaskRunnerInterface> mockTaskRunner;
            NiceMock<MockApiInterface> mockApi;
            NiceMock<MockEuroscopePluginLoopbackInterface> mockPlugin;
            FunctionCallEventHandler functionHandlers;
            CallsignSelectionListFactory listFactory;
            NiceMock<MockDialogProvider> mockProvider;
            UKControllerPlugin::Dialog::DialogManager dialogManager;
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
            EXPECT_CALL(this->mockProvider, OpenDialog(this->dialogData, _)).Times(1);

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
            EXPECT_CALL(this->mockProvider, OpenDialog(this->dialogData, _)).Times(1);

            this->menuItem.ProcessCommand(".ukcp hold");
        }

        TEST_F(HoldConfigurationMenuItemTest, SelectHoldsSetsHolds)
        {
            this->menuItem.SelectHolds(std::vector<std::string>({"TIMBA", "LAM"}));
            EXPECT_EQ(std::vector<std::string>({"TIMBA", "LAM"}), this->menuItem.GetHolds());
        }
    } // namespace Hold
} // namespace UKControllerPluginTest
