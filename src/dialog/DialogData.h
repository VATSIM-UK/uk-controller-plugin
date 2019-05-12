#pragma once
#include "pch/stdafx.h"

namespace UKControllerPlugin {
    namespace Dialog {

        /*
            A struct of data representing one of UKCPs dialogs
            that can be used to trigger the dialog through the windows
            API.
        */
        typedef struct DialogData {

            // The ID of the dialog
            const unsigned int dialogId;

            // A description of the dialog
            const std::string description;

            // The procedure to run when running the dialog. This should be a function pointer.
            const DLGPROC dialogProcedure;

            // The argument to pass to the dialog on WM_INITDIALOG along with the context argument
            const LPARAM dialogArgument;

            /*
                The class that actually manages the dialog. The manager does not need to care what this is.
                This just keeps the management object in scope.
            */
            std::shared_ptr<void> dialogClass;

            bool operator==(const DialogData & compare) const {
                return this->dialogId == compare.dialogId;
            }

        } DialogData;

    }  // namespace Dialog
}  // namespace UKControllerPlugin
