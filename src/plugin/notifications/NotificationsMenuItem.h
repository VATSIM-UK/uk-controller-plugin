#pragma once
#include "NotificationsRepository.h"
#include "dialog/DialogManager.h"
#include "radarscreen/ConfigurableDisplayInterface.h"

namespace UKControllerPlugin::Notifications {
    /*
        The configuration menu item for viewing notifications
    */
    class NotificationsMenuItem : public RadarScreen::ConfigurableDisplayInterface
    {
        public:
        NotificationsMenuItem(
            const UKControllerPlugin::Dialog::DialogManager& dialogManager,
            std::shared_ptr<NotificationsRepository> notifications,
            int openDialogCallbackId);

        // Inherited via ConfigurableDisplayInterface
        void Configure(int functionId, std::string subject, RECT screenObjectArea) override;
        [[nodiscard]] auto GetConfigurationMenuItem() const -> Plugin::PopupMenuItem override;

        private:
        [[nodiscard]] auto GetMenuItemDescription() const -> std::string;

        // The callback id
        const int openDialogCallbackId;

        // Manages dialogs
        const UKControllerPlugin::Dialog::DialogManager& dialogManager;

        // All the notifications
        std::shared_ptr<NotificationsRepository> notifications;
    };
} // namespace UKControllerPlugin::Notifications
