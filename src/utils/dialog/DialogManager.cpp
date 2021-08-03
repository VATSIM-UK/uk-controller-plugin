#include "pch/pch.h"
#include "dialog/DialogData.h"
#include "dialog/DialogManager.h"
#include "dialog/DialogCallArgument.h"

using UKControllerPlugin::Dialog::DialogProviderInterface;

namespace UKControllerPlugin {
    namespace Dialog {

        DialogManager::DialogManager(const DialogProviderInterface & dialogProvider)
            : dialogProvider(dialogProvider)
        {

        }

        /*
            Add a dialog to the manager
        */
        bool DialogManager::AddDialog(UKControllerPlugin::Dialog::DialogData data)
        {
            if (!this->dialogs.insert(data).second) {
                LogWarning("Duplicate dialog added to DialogManager " + std::to_string(data.dialogId));
                return false;
            }

            return true;
        }

        /*
            Returns the number of dialogs
        */
        size_t DialogManager::CountDialogs(void) const
        {
            return this->dialogs.size();
        }

        /*
            Checks if a dialog is registered.
        */
        bool DialogManager::HasDialog(unsigned int dialogId) const
        {
            return this->dialogs.find(dialogId) != this->dialogs.cend();
        }

        /*
            Open the given dialog
        */
        void DialogManager::OpenDialog(unsigned int dialogId, LPARAM contextArgument) const
        {
            auto dialog = this->dialogs.find(dialogId);
            if (dialog == this->dialogs.cend()) {
                LogWarning("Attempted to open non-existant dialog " + std::to_string(dialogId));
                return;
            }

            // Set the dialog arguments internally, so they stay in scope
            LogInfo("Opened dialog " + dialog->description);
            DialogCallArgument arg = { dialog->dialogArgument, contextArgument };
            this->dialogArgs[dialogId] = std::make_unique<DialogCallArgument>(arg);

            this->dialogProvider.OpenDialog(*dialog, this->dialogArgs[dialogId].get());
        }
    }  // namespace Dialog
}  // namespace UKControllerPlugin
