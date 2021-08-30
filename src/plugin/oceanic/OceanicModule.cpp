#include "OceanicClearanceDialog.h"
#include "OceanicEventHandler.h"
#include "OceanicModule.h"
#include "bootstrap/PersistenceContainer.h"
#include "dialog/DialogManager.h"
#include "plugin/FunctionCallEventHandler.h"
#include "tag/TagFunction.h"
#include "tag/TagItemCollection.h"
#include "timedevent/TimedEventCollection.h"

namespace UKControllerPlugin::Oceanic {

    const int NATTRAK_UPDATE_FREQUENCY = 60;
    const int CLEARANCE_INDICATOR_TAG_FUNCTION_ID = 118;
    const int CLEARANCE_LEVEL_TAG_FUNCTION_ID = 119;
    const int CLEARANCE_MACH_TAG_FUNCTION_ID = 120;
    const int CLEARANCE_ENTRY_POINT_TAG_FUNCTION_ID = 121;
    const int CLEARANCE_TRACK_IDENTIFIER_TAG_FUNCTION_ID = 122;
    const int CLEARANCE_ENTRY_TIME_TAG_FUNCTION_ID = 123;
    const int oceanicClearanceDialogTagFunctionId = 9010;

    void BootstrapPlugin(const Bootstrap::PersistenceContainer& container)
    {
        // Create the clearance dialog
        std::shared_ptr<OceanicClearanceDialog> clearanceDialog = std::make_shared<OceanicClearanceDialog>();
        container.dialogManager->AddDialog(
            {IDD_OCEANIC_CLEARANCE,
             "Oceanic Clearance",
             reinterpret_cast<DLGPROC>(clearanceDialog->WndProc), // NOLINT
             reinterpret_cast<LPARAM>(clearanceDialog.get()),     // NOLINT
             clearanceDialog});

        // Create the event handler
        std::shared_ptr<OceanicEventHandler> handler =
            std::make_shared<OceanicEventHandler>(*container.curl, *container.taskRunner, *container.dialogManager);

        // Register the tag item function
        Tag::TagFunction openOceanicClearanceDialog(
            oceanicClearanceDialogTagFunctionId,
            "Open Oceanic Clearance Dialog",
            [handler](
                Euroscope::EuroScopeCFlightPlanInterface& flightplan,
                Euroscope::EuroScopeCRadarTargetInterface& radarTarget,
                std::string context,
                const POINT& mousePos) {
                handler->TagFunction(flightplan, radarTarget, std::move(context), mousePos);
            });

        // Register handlers
        container.pluginFunctionHandlers->RegisterFunctionCall(openOceanicClearanceDialog);
        container.timedHandler->RegisterEvent(handler, NATTRAK_UPDATE_FREQUENCY);
        container.tagHandler->RegisterTagItem(CLEARANCE_INDICATOR_TAG_FUNCTION_ID, handler);
        container.tagHandler->RegisterTagItem(CLEARANCE_LEVEL_TAG_FUNCTION_ID, handler);
        container.tagHandler->RegisterTagItem(CLEARANCE_MACH_TAG_FUNCTION_ID, handler);
        container.tagHandler->RegisterTagItem(CLEARANCE_ENTRY_POINT_TAG_FUNCTION_ID, handler);
        container.tagHandler->RegisterTagItem(CLEARANCE_TRACK_IDENTIFIER_TAG_FUNCTION_ID, handler);
        container.tagHandler->RegisterTagItem(CLEARANCE_ENTRY_TIME_TAG_FUNCTION_ID, handler);
    }
} // namespace UKControllerPlugin::Oceanic
