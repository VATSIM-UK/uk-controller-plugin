#pragma once
#include "dialog/DialogData.h"

namespace UKControllerPlugin {
    namespace Dialog {

        /*
            The model for a class that wants to provide
            access to windows dialogs.
        */
        class DialogProviderInterface
        {
            public:

                /*
                    Open the given dialog
                */
                virtual void OpenDialog(const UKControllerPlugin::Dialog::DialogData & dialog) const = 0;
        };

    }  // namespace Dialog
}  // namespace UKControllerPlugin
