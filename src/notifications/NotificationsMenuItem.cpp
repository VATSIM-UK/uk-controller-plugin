#include "pch/stdafx.h"
#include "notifications/NotificationsMenuItem.h"

namespace UKControllerPlugin {
    namespace Notifications {
        NotificationsMenuItem::NotificationsMenuItem(
            const UKControllerPlugin::Dialog::DialogManager& dialogManager,
            std::shared_ptr<NotificationsRepository> notifications,
            unsigned openDialogCallbackId
        ) :dialogManager(dialogManager), openDialogCallbackId(std::move(openDialogCallbackId)),
            notifications(notifications)
        {
        }

        /*
            The user has selected to open the hold configuration dialog,
            open it.
        */
        void NotificationsMenuItem::Configure(int functionId, std::string subject, RECT screenObjectArea)
        {
            this->dialogManager.OpenDialog(IDD_NOTIFICATIONS, reinterpret_cast<LPARAM>(this));
        }

        /*
            Get the configuration menu item
        */
        Plugin::PopupMenuItem NotificationsMenuItem::GetConfigurationMenuItem(void) const
        {
            Plugin::PopupMenuItem returnVal;
            returnVal.firstValue = this->GetMenuItemDescription();
            returnVal.secondValue = "";
            returnVal.callbackFunctionId = this->openDialogCallbackId;
            returnVal.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
            returnVal.disabled = false;
            returnVal.fixedPosition = false;
            return returnVal;
        }

        std::string NotificationsMenuItem::GetMenuItemDescription() const
        {
            return "View Notifications (" + std::to_string(this->notifications->CountUnread()) + " unread)";
        }
    }  // namespace Notifications
}  // namespace UKControllerPlugin
