#include "pch/stdafx.h"
#include "srd/SrdModule.h"
#include "srd/SrdSearchDialog.h"
#include "srd/SrdSearchHandler.h"
#include "dialog/DialogData.h"
#include "euroscope/CallbackFunction.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection;
using UKControllerPlugin::Dialog::DialogData;
using UKControllerPlugin::Euroscope::CallbackFunction;

namespace UKControllerPlugin {
    namespace Srd {

        void BootstrapPlugin(PersistenceContainer& container)
        {
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
        }

        void BootstrapRadarScreen(const PersistenceContainer& container, ConfigurableDisplayCollection& configurables)
        {
            int handlerCallbackId = container.pluginFunctionHandlers->ReserveNextDynamicFunctionId();
            std::shared_ptr<SrdSearchHandler> handler = std::make_shared<SrdSearchHandler>(
                handlerCallbackId,
                *container.dialogManager
            );

            CallbackFunction configureCallback(
                handlerCallbackId,
                "SRD Search",
                std::bind(
                    &SrdSearchHandler::Configure,
                    handler,
                    std::placeholders::_1,
                    std::placeholders::_2,
                    std::placeholders::_3
                )
            );

            configurables.RegisterDisplay(handler);
            container.pluginFunctionHandlers->RegisterFunctionCall(configureCallback);
        }
    }  // namespace Srd
}  // UKControllerPlugin
