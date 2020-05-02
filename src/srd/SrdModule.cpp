#include "pch/stdafx.h"
#include "srd/SrdModule.h"
#include "srd/SrdSearchDialog.h"
#include "srd/SrdSearchHandler.h"
#include "dialog/DialogData.h"
#include "euroscope/CallbackFunction.h"
#include "tag/TagFunction.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection;
using UKControllerPlugin::Dialog::DialogData;
using UKControllerPlugin::Euroscope::CallbackFunction;
using UKControllerPlugin::Tag::TagFunction;

namespace UKControllerPlugin {
    namespace Srd {

        const int srdDialogTagFunctionId = 9004;

        int handlerCallbackId;

        void BootstrapPlugin(PersistenceContainer& container)
        {
            // Register the dialog
            std::shared_ptr<SrdSearchDialog> dialog = std::make_shared<SrdSearchDialog>(*container.api);
            container.dialogManager->AddDialog(
                {
                    IDD_SRD_SEARCH,
                    "SRD Search",
                    reinterpret_cast<DLGPROC>(dialog->WndProc),
                    reinterpret_cast<LPARAM>(dialog.get()),
                    dialog
                }
            );

            // Create the handler
            handlerCallbackId = container.pluginFunctionHandlers->ReserveNextDynamicFunctionId();
            container.srd = std::make_shared<SrdSearchHandler>(handlerCallbackId,*container.dialogManager);

            // Register a callback function for clicking the the menu item
            CallbackFunction configureCallback(
                handlerCallbackId,
                "SRD Search",
                std::bind(
                    &SrdSearchHandler::Configure,
                    container.srd.get(),
                    std::placeholders::_1,
                    std::placeholders::_2,
                    std::placeholders::_3
                )
            );

            container.pluginFunctionHandlers->RegisterFunctionCall(configureCallback);


            // Register the tag item function
            TagFunction openHoldPopupMenu(
                srdDialogTagFunctionId,
                "Open SRD Search",
                std::bind(
                    &SrdSearchHandler::TagFunction,
                    container.srd.get(),
                    std::placeholders::_1,
                    std::placeholders::_2,
                    std::placeholders::_3,
                    std::placeholders::_4
                )
            );

            container.pluginFunctionHandlers->RegisterFunctionCall(openHoldPopupMenu);
        }

        void BootstrapRadarScreen(const PersistenceContainer& container, ConfigurableDisplayCollection& configurables)
        {
            configurables.RegisterDisplay(container.srd);
        }
    }  // namespace Srd
}  // UKControllerPlugin
