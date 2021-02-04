#include "pch/stdafx.h"
#include "notifications/NotificationsModule.h"
#include "notifications/NotificationsRepositoryFactory.h"
#include "notifications/NotificationsDialog.h"
#include "notifications/NotificationsMenuItem.h"
#include "euroscope/CallbackFunction.h"

namespace UKControllerPlugin {
    namespace Notifications {

        std::shared_ptr<NotificationsMenuItem> menuItem;

        void BootstrapPlugin(Bootstrap::PersistenceContainer& container)
        {
            // Create the dialog
            std::shared_ptr<NotificationsRepository> repository = Make(container);

            // Create the hold selection dialog
            std::shared_ptr<NotificationsDialog> dialog = std::make_shared<NotificationsDialog>(
                repository,
                *container.api,
                *container.dialogManager
            );

            container.dialogManager->AddDialog(
                {
                    IDD_NOTIFICATIONS,
                    "Notifications",
                    reinterpret_cast<DLGPROC>(dialog->WndProc),
                    reinterpret_cast<LPARAM>(dialog.get()),
                    dialog
                }
            );

            // Create the menu item and register with handler
            const int callbackId = container.pluginFunctionHandlers->ReserveNextDynamicFunctionId();
            menuItem = std::make_shared<NotificationsMenuItem>(
                *container.dialogManager,
                repository,
                callbackId
            );

            const Euroscope::CallbackFunction menuItemCallback(
                callbackId,
                "Notifications menu callback",
                std::bind(
                    &NotificationsMenuItem::Configure,
                    menuItem,
                    std::placeholders::_1,
                    std::placeholders::_2,
                    std::placeholders::_3
                )
            );
            container.pluginFunctionHandlers->RegisterFunctionCall(menuItemCallback);
        }

        void BootstrapRadarScreen(RadarScreen::ConfigurableDisplayCollection& displays)
        {
            displays.RegisterDisplay(menuItem);
        }
    }  // namespace Notifications
}  // namespace UKControllerPlugin
