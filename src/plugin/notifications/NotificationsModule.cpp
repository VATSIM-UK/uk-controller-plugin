#include "NotificationsDialog.h"
#include "NotificationsEventHandler.h"
#include "NotificationsMenuItem.h"
#include "NotificationsModule.h"
#include "NotificationsRepositoryFactory.h"
#include "bootstrap/PersistenceContainer.h"
#include "bootstrap/PersistenceContainer.h"
#include "euroscope/CallbackFunction.h"
#include "plugin/FunctionCallEventHandler.h"
#include "radarscreen/ConfigurableDisplayCollection.h"

namespace UKControllerPlugin::Notifications {

    void BootstrapPlugin(Bootstrap::PersistenceContainer& container)
    {
        // Create the dialog
        std::shared_ptr<NotificationsRepository> repository = Make(container);

        // Create the hold selection dialog
        std::shared_ptr<NotificationsDialog> dialog =
            std::make_shared<NotificationsDialog>(repository, *container.api, *container.activeCallsigns);

        container.dialogManager->AddDialog(
            {IDD_NOTIFICATIONS,
             "Notifications",
             reinterpret_cast<DLGPROC>(dialog->WndProc), // NOLINT
             reinterpret_cast<LPARAM>(dialog.get()),     // NOLINT
             dialog});

        // Create the menu item and register with handler
        const int callbackId = container.pluginFunctionHandlers->ReserveNextDynamicFunctionId();
        auto menuItem = std::make_shared<NotificationsMenuItem>(*container.dialogManager, repository, callbackId);
        container.notificationsMenuItem = menuItem;

        const Euroscope::CallbackFunction menuItemCallback(
            callbackId,
            "Notifications menu callback",
            [menuItem](int functionId, std::string subject, RECT screenObjectArea) {
                menuItem->Configure(functionId, std::move(subject), screenObjectArea);
            });
        container.pluginFunctionHandlers->RegisterFunctionCall(menuItemCallback);

        // Create the event handler
        std::shared_ptr<NotificationsEventHandler> eventHandler =
            std::make_shared<NotificationsEventHandler>(repository, *container.userMessager);
        container.activeCallsigns->AddHandler(eventHandler);
    }

    void BootstrapRadarScreen(
        const Bootstrap::PersistenceContainer& container, RadarScreen::ConfigurableDisplayCollection& displays)
    {
        displays.RegisterDisplay(container.notificationsMenuItem);
    }
} // namespace UKControllerPlugin::Notifications
