#include "SrdModule.h"
#include "SrdSearchDialog.h"
#include "SrdSearchHandler.h"
#include "bootstrap/PersistenceContainer.h"
#include "dialog/DialogManager.h"
#include "radarscreen/ConfigurableDisplayCollection.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Dialog::DialogData;
using UKControllerPlugin::Euroscope::CallbackFunction;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection;
using UKControllerPlugin::Tag::TagFunction;

namespace UKControllerPlugin::Srd {

    const int srdDialogTagFunctionId = 9004;

    int handlerCallbackId; // NOLINT

    std::shared_ptr<SrdSearchHandler> srdSearchHandler; // NOLINT

    void BootstrapPlugin(PersistenceContainer& container)
    {
        // Register the dialog
        std::shared_ptr<SrdSearchDialog> dialog = std::make_shared<SrdSearchDialog>(*container.api);
        container.dialogManager->AddDialog(
            {IDD_SRD_SEARCH,
             "SRD Search",
             reinterpret_cast<DLGPROC>(dialog->WndProc), // NOLINT
             reinterpret_cast<LPARAM>(dialog.get()),     // NOLINT
             dialog});

        // Create the handler
        handlerCallbackId = container.pluginFunctionHandlers->ReserveNextDynamicFunctionId();
        srdSearchHandler = std::make_shared<SrdSearchHandler>(handlerCallbackId, *container.dialogManager);
        auto searchHandlerCopy = srdSearchHandler;

        // Register a callback function for clicking the the menu item
        CallbackFunction configureCallback(
            handlerCallbackId,
            "SRD Search",
            [searchHandlerCopy](int functionId, std::string subject, RECT screenObjectArea) {
                srdSearchHandler->Configure(functionId, std::move(subject), screenObjectArea);
            });

        container.pluginFunctionHandlers->RegisterFunctionCall(configureCallback);

        // Register the tag item function
        TagFunction openHoldPopupMenu(
            srdDialogTagFunctionId,
            "Open SRD Search",
            [searchHandlerCopy](
                UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& fp,
                UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& rt,
                const std::string& context,
                const POINT& mousePos) { searchHandlerCopy->TagFunction(fp, rt, context, mousePos); });

        container.pluginFunctionHandlers->RegisterFunctionCall(openHoldPopupMenu);
    }

    void BootstrapRadarScreen(ConfigurableDisplayCollection& configurables)
    {
        configurables.RegisterDisplay(srdSearchHandler);
    }
} // namespace UKControllerPlugin::Srd
