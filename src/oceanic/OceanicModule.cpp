#include "pch/stdafx.h"
#include "oceanic/OceanicModule.h"
#include "bootstrap/PersistenceContainer.h"
#include "oceanic/OceanicEventHandler.h"
#include "oceanic/OceanicClearanceDialog.h"
#include "tag/TagFunction.h"


namespace UKControllerPlugin {
    namespace Oceanic {

        const int oceanicClearanceDialogTagFunctionId = 9010;

        void BootstrapPlugin(const Bootstrap::PersistenceContainer& container)
        {
            // Create the clearance dialog
            std::shared_ptr<OceanicClearanceDialog> clearanceDialog = std::make_shared<OceanicClearanceDialog>();
            container.dialogManager->AddDialog(
                {
                    IDD_OCEANIC_CLEARANCE,
                    "Oceanic Clearance",
                    reinterpret_cast<DLGPROC>(clearanceDialog->WndProc),
                    reinterpret_cast<LPARAM>(clearanceDialog.get()),
                    clearanceDialog
                }
            );

            // Create the event handler
            std::shared_ptr<OceanicEventHandler> handler = std::make_shared<OceanicEventHandler>(
                *container.curl,
                *container.taskRunner,
                *container.dialogManager
            );

            // Register the tag item function
            Tag::TagFunction openOceanicClearanceDialog(
                oceanicClearanceDialogTagFunctionId,
                "Open Oceanic Clearance Dialog",
                std::bind(
                    &OceanicEventHandler::TagFunction,
                    handler,
                    std::placeholders::_1,
                    std::placeholders::_2,
                    std::placeholders::_3,
                    std::placeholders::_4
                )
            );

            // Register handlers
            container.pluginFunctionHandlers->RegisterFunctionCall(openOceanicClearanceDialog);
            container.timedHandler->RegisterEvent(handler, 60);
            container.tagHandler->RegisterTagItem(118, handler);
            container.tagHandler->RegisterTagItem(119, handler);
            container.tagHandler->RegisterTagItem(120, handler);
            container.tagHandler->RegisterTagItem(121, handler);
        }
    } // namespace Oceanic
} // namespace UKControllerPlugin
