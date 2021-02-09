#include "pch/pch.h"
#include "notifications/NotificationsMenuItem.h"
#include "notifications/NotificationsRepository.h"
#include "mock/MockDialogProvider.h"
#include "dialog/DialogManager.h"
#include "plugin/PopupMenuItem.h"

using ::testing::Test;
using ::testing::NiceMock;
using ::testing::_;
using UKControllerPluginTest::Dialog::MockDialogProvider;
using UKControllerPlugin::Dialog::DialogManager;
using UKControllerPlugin::Dialog::DialogData;
using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPlugin::Notifications::NotificationsMenuItem;
using UKControllerPlugin::Notifications::NotificationsRepository;

namespace UKControllerPluginTest {
    namespace Notifications {

        class NotificationsMenuItemTest : public Test
        {
            public:
                NotificationsMenuItemTest()
                    : dialogManager(mockProvider)
                {
                    std::shared_ptr<NotificationsRepository> repo = std::make_shared<NotificationsRepository>();
                    repo->Add(std::make_unique<UKControllerPlugin::Notifications::Notification>(
                        1,
                        "abc",
                        "def",
                        std::chrono::system_clock::now(),
                        std::chrono::system_clock::now(),
                        std::make_unique<UKControllerPlugin::Controller::ControllerPositionHierarchy>(),
                        "link"
                    ));
                    repo->Add(std::make_unique<UKControllerPlugin::Notifications::Notification>(
                        2,
                        "abc",
                        "def",
                        std::chrono::system_clock::now(),
                        std::chrono::system_clock::now(),
                        std::make_unique<UKControllerPlugin::Controller::ControllerPositionHierarchy>(),
                        "link"
                    ));
                    repo->Add(std::make_unique<UKControllerPlugin::Notifications::Notification>(
                        3,
                        "abc",
                        "def",
                        std::chrono::system_clock::now(),
                        std::chrono::system_clock::now(),
                        std::make_unique<UKControllerPlugin::Controller::ControllerPositionHierarchy>(),
                        "link"
                    ));
                    repo->Get(1)->Read();
                    repo->Get(3)->Read();

                    menuItem = std::make_shared<NotificationsMenuItem>(dialogManager, repo, 5);
                    dialogManager.AddDialog(dialogData);
                }

                DialogData dialogData = { IDD_NOTIFICATIONS, "", NULL, NULL, NULL };
                DialogManager dialogManager;
                NiceMock<MockDialogProvider> mockProvider;
                std::shared_ptr<NotificationsMenuItem> menuItem;
        };

        TEST_F(NotificationsMenuItemTest, GetConfigurationMenuItemReturnsItem)
        {
            PopupMenuItem expected;
            expected.firstValue = "View Notifications (1 unread)";
            expected.secondValue = "";
            expected.callbackFunctionId = 5;
            expected.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
            expected.disabled = false;
            expected.fixedPosition = false;


            EXPECT_TRUE(expected == this->menuItem->GetConfigurationMenuItem());
        }

        TEST_F(NotificationsMenuItemTest, MenuItemConfigurationOpensDialog)
        {
            EXPECT_CALL(this->mockProvider, OpenDialog(this->dialogData, _))
                .Times(1);

            this->menuItem->Configure(0, "", {});
        }
    }  // namespace Notifications
}  // namespace UKControllerPluginTest
