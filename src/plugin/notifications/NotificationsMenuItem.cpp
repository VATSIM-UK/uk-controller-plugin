#include "NotificationsMenuItem.h"

namespace UKControllerPlugin::Notifications {
    NotificationsMenuItem::NotificationsMenuItem(
        const UKControllerPlugin::Dialog::DialogManager& dialogManager,
        std::shared_ptr<NotificationsRepository> notifications,
        int openDialogCallbackId)
        : openDialogCallbackId(openDialogCallbackId), dialogManager(dialogManager),
          notifications(std::move(notifications))
    {
    }

    /*
        The user has selected to open the hold configuration dialog,
        open it.
    */
    void NotificationsMenuItem::Configure(int functionId, std::string subject, RECT screenObjectArea)
    {
        this->dialogManager.OpenDialog(IDD_NOTIFICATIONS, reinterpret_cast<LPARAM>(this)); // NOLINT
    }

    /*
        Get the configuration menu item
    */
    auto NotificationsMenuItem::GetConfigurationMenuItem() const -> Plugin::PopupMenuItem
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

    auto NotificationsMenuItem::GetMenuItemDescription() const -> std::string
    {
        return "View Notifications (" + std::to_string(this->notifications->CountUnread()) + " unread)";
    }
} // namespace UKControllerPlugin::Notifications
