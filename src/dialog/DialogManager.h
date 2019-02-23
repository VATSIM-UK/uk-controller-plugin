#pragma once
#include "dialog/DialogProviderInterface.h"
#include "dialog/CompareDialogs.h"

namespace UKControllerPlugin {
    namespace Dialog {

        /*
            Provides access to dialogs
        */
        class DialogManager
        {
            public:
                DialogManager(const UKControllerPlugin::Dialog::DialogProviderInterface & dialogProvider);
                bool AddDialog(UKControllerPlugin::Dialog::DialogData data);
                size_t CountDialogs(void) const;
                bool HasDialog(unsigned int dialogId) const;
                void OpenDialog(unsigned int dialogId) const;

            private:
                
                // A class that provides the actual function calls to create the dialog
                const UKControllerPlugin::Dialog::DialogProviderInterface & dialogProvider;

                // All the dialogs
                std::set<UKControllerPlugin::Dialog::DialogData, UKControllerPlugin::Dialog::CompareDialogs> dialogs;
        };
    }  // namespace Dialog
}  // namespace UKControllerPlugin
