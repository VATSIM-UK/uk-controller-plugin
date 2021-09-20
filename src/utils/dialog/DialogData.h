#pragma once

namespace UKControllerPlugin::Dialog {

    /*
        A struct of data representing one of UKCPs dialogs
        that can be used to trigger the dialog through the windows
        API.
    */
    using DialogData = struct DialogData
    {

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

        [[nodiscard]] auto operator==(const DialogData& compare) const -> bool;
    };
} // namespace UKControllerPlugin::Dialog
