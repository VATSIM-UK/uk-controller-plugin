#include "dialog/DialogManager.h"
#include "missedapproach/ConfigureMissedApproaches.h"

using testing::NiceMock;
using UKControllerPlugin::Dialog::DialogData;
using UKControllerPlugin::Dialog::DialogManager;
using UKControllerPlugin::MissedApproach::ConfigureMissedApproaches;
using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPluginTest::Dialog::MockDialogProvider;

namespace UKControllerPluginTest::MissedApproach {
    class ConfigureMissedApproachesTest : public testing::Test
    {
        public:
        ConfigureMissedApproachesTest() : dialogManager(mockProvider), menuItem(nullptr, dialogManager, 5)
        {
            this->dialogManager.AddDialog(this->dialogData);
        }

        DialogData dialogData = {IDD_MISSED_APPROACH, "", NULL, NULL, NULL};
        NiceMock<MockDialogProvider> mockProvider;
        DialogManager dialogManager;
        ConfigureMissedApproaches menuItem;
    };

    TEST_F(ConfigureMissedApproachesTest, ItReturnsMenuItem)
    {
        PopupMenuItem expected;
        expected.firstValue = "Configure Missed Approaches";
        expected.secondValue = "";
        expected.callbackFunctionId = 5;
        expected.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
        expected.disabled = false;
        expected.fixedPosition = false;

        EXPECT_EQ(expected, menuItem.GetConfigurationMenuItem());
    }

    TEST_F(ConfigureMissedApproachesTest, MenuItemConfigurationOpensDialog)
    {
        EXPECT_CALL(this->mockProvider, OpenDialog(this->dialogData, testing::_)).Times(1);

        this->menuItem.Configure(0, "", {});
    }
} // namespace UKControllerPluginTest::MissedApproach
