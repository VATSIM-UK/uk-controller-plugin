#pragma once
#include "dialog/DialogProviderInterface.h"
#include "dialog/DialogCallArgument.h"
#include "dialog/CompareDialogs.h"

namespace UKControllerPlugin {
    namespace Dialog {

        /*
            Provides access to dialogs
        */
        class DialogManager
        {
            public:
                explicit DialogManager(const UKControllerPlugin::Dialog::DialogProviderInterface & dialogProvider);
                bool AddDialog(UKControllerPlugin::Dialog::DialogData data);
                size_t CountDialogs(void) const;
                bool HasDialog(unsigned int dialogId) const;
                void OpenDialog(unsigned int dialogId, LPARAM contextArgument) const;

            private:

                // A class that provides the actual function calls to create the dialog
                const UKControllerPlugin::Dialog::DialogProviderInterface & dialogProvider;

                // All the dialogs
                std::set<UKControllerPlugin::Dialog::DialogData, UKControllerPlugin::Dialog::CompareDialogs> dialogs;

                // Map to store pointers to dialog args, so that they remain in scope whilst dialogs are open
                mutable std::map<
                    unsigned int,
                    std::unique_ptr<UKControllerPlugin::Dialog::DialogCallArgument>
                > dialogArgs;
        };
    }  // namespace Dialog
}  // namespace UKControllerPlugin
