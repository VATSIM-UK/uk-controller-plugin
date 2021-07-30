#pragma once
#include "dialog/DialogManager.h"
#include "radarscreen/ConfigurableDisplayInterface.h"
#include "notifications/NotificationsRepository.h"

namespace UKControllerPlugin {
    namespace Notifications {
        /*
            The configuration menu item for viewing notifications
        */
        class NotificationsMenuItem: public RadarScreen::ConfigurableDisplayInterface
        {
            public:
                NotificationsMenuItem(
                    const UKControllerPlugin::Dialog::DialogManager & dialogManager,
                    std::shared_ptr<NotificationsRepository> notifications,
                    unsigned int openDialogCallbackId
                );

                // Inherited via ConfigurableDisplayInterface
                void Configure(int functionId, std::string subject, RECT screenObjectArea) override;
                UKControllerPlugin::Plugin::PopupMenuItem GetConfigurationMenuItem(void) const override;

            private:

                std::string GetMenuItemDescription() const;

                // The callback id
                const unsigned int openDialogCallbackId;

                // Manages dialogs
                const UKControllerPlugin::Dialog::DialogManager & dialogManager;

                // All the notifications
                std::shared_ptr<NotificationsRepository> notifications;
        };
    }  // namespace Notifications
}  // namespace UKControllerPlugin
