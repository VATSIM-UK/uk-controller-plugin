#include "pch/stdafx.h"
#include "releases/ReleaseModule.h"
#include "releases/EnrouteReleaseEventHandler.h"
#include "releases/EnrouteReleaseType.h"
#include "releases/CompareEnrouteReleaseTypes.h"
#include "releases/EnrouteReleaseTypesSerializer.h"
#include "tag/TagFunction.h"
#include "euroscope/CallbackFunction.h"
#include "releases/DepartureReleaseEventHandler.h"
#include "releases/ApproveDepartureReleaseDialog.h"
#include "releases/RequestDepartureReleaseDialog.h"
#include "releases/DepartureReleaseRequestView.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Dependency::DependencyLoaderInterface;
using UKControllerPlugin::Euroscope::CallbackFunction;
using UKControllerPlugin::Tag::TagFunction;
using UKControllerPlugin::Releases::DepartureReleaseEventHandler;
using UKControllerPlugin::Releases::ApproveDepartureReleaseDialog;
using UKControllerPlugin::Releases::RequestDepartureReleaseDialog;
using UKControllerPlugin::Releases::DepartureReleaseRequestView;

namespace UKControllerPlugin {
    namespace Releases {

        const std::string enrouteReleaseTypesDependency = "DEPENDENCY_ENROUTE_RELEASE_TYPES";
        const unsigned int enrouteReleaseTypeTagItemId = 9005;
        const unsigned int enrouteReleasePointTagItemId = 9006;
        const unsigned int departureReleaseRequestDialogTriggerFunctionId = 9012;
        const unsigned int departureReleaseDecisionMenuTriggerFunctionId = 9013;
        const unsigned int departureReleaseStatusViewTriggerFunctionId = 9014;
        const unsigned int departureReleaseCancelMenuFunctionId = 9015;
        const unsigned int departureReleaseStatusIndicatorTagItemId = 124;
        const unsigned int departureReleaseCountdownTimerTagItemId = 125;
        const unsigned int departureReleaseRequestingControllerTagItemId = 126;
        std::shared_ptr<DepartureReleaseEventHandler> departureHandler;

        void BootstrapPlugin(PersistenceContainer& container, DependencyLoaderInterface& dependencies)
        {
            // Load everything to do with ENROUTE releases
            std::set<EnrouteReleaseType, CompareEnrouteReleaseTypes> releaseTypes;
            from_json(
                dependencies.LoadDependency(enrouteReleaseTypesDependency, nlohmann::json::array()),
                releaseTypes
            );

            // Create the handler
            std::shared_ptr<EnrouteReleaseEventHandler> handler = std::make_shared<EnrouteReleaseEventHandler>(
                *container.api,
                *container.plugin,
                *container.taskRunner,
                releaseTypes,
                container.pluginFunctionHandlers->ReserveNextDynamicFunctionId(),
                container.pluginFunctionHandlers->ReserveNextDynamicFunctionId()
            );

            // TAG function to trigger the release type menu and receive the changes
            TagFunction openReleaseTypePopupMenu(
                enrouteReleaseTypeTagItemId,
                "Open Enroute Release Type Menu",
                std::bind(
                    &EnrouteReleaseEventHandler::DisplayReleaseTypeMenu,
                    handler,
                    std::placeholders::_1,
                    std::placeholders::_2,
                    std::placeholders::_3,
                    std::placeholders::_4
                )
            );
            container.pluginFunctionHandlers->RegisterFunctionCall(openReleaseTypePopupMenu);

            CallbackFunction releaseTypeSelectedCallback(
                handler->releaseTypeSelectedCallbackId,
                "Release Type Selected",
                std::bind(
                    &EnrouteReleaseEventHandler::ReleaseTypeSelected,
                    handler,
                    std::placeholders::_1,
                    std::placeholders::_2,
                    std::placeholders::_3
                )
            );
            container.pluginFunctionHandlers->RegisterFunctionCall(releaseTypeSelectedCallback);

            // TAG function to trigger the release edit popup and receive the changes
            TagFunction openReleasePointEditBox(
                enrouteReleasePointTagItemId,
                "Edit Enroute Release Point",
                std::bind(
                &EnrouteReleaseEventHandler::DisplayReleasePointEditBox,
                    handler,
                    std::placeholders::_1,
                    std::placeholders::_2,
                    std::placeholders::_3,
                    std::placeholders::_4
                )
            );
            container.pluginFunctionHandlers->RegisterFunctionCall(openReleasePointEditBox);

            CallbackFunction editReleaseTypeCallback(
                handler->editReleasePointCallbackId,
                "Release Point Edited",
                std::bind(
                &EnrouteReleaseEventHandler::EditReleasePoint,
                    handler,
                    std::placeholders::_1,
                    std::placeholders::_2,
                    std::placeholders::_3
                )
            );
            container.pluginFunctionHandlers->RegisterFunctionCall(editReleaseTypeCallback);

            // Add to events
            container.websocketProcessors->AddProcessor(handler);
            container.tagHandler->RegisterTagItem(handler->enrouteReleaseTypeTagItemId, handler);
            container.tagHandler->RegisterTagItem(handler->enrouteReleasePointTagItemId, handler);
            container.tagHandler->RegisterTagItem(handler->enrouteReleasePointOrBlankTagItemId, handler);
            container.timedHandler->RegisterEvent(handler, 10);
            container.controllerHandoffHandlers->RegisterHandler(handler);


            // Everything to do with DEPARTURE releases
            const int releaseDecisionCallbackId = container.pluginFunctionHandlers->ReserveNextDynamicFunctionId();
            const int releaseCancellationCallbackId = container.pluginFunctionHandlers->ReserveNextDynamicFunctionId();
            departureHandler =
                std::make_shared<DepartureReleaseEventHandler>(
                    *container.api,
                    *container.taskRunner,
                    *container.plugin,
                    *container.controllerPositions,
                    *container.activeCallsigns,
                    *container.dialogManager,
                    departureReleaseRequestDialogTriggerFunctionId,
                    departureReleaseDecisionMenuTriggerFunctionId,
                    releaseDecisionCallbackId,
                    releaseCancellationCallbackId
                );
            container.websocketProcessors->AddProcessor(departureHandler);
            container.tagHandler->RegisterTagItem(departureReleaseStatusIndicatorTagItemId, departureHandler);
            container.tagHandler->RegisterTagItem(departureReleaseCountdownTimerTagItemId, departureHandler);
            container.tagHandler->RegisterTagItem(departureReleaseRequestingControllerTagItemId, departureHandler);

            // Callback for when a release decision is made
            CallbackFunction releaseDecisionCallback(
                releaseDecisionCallbackId,
                "Departure Release Decision Made",
                std::bind(
                    &DepartureReleaseEventHandler::ReleaseDecisionMade,
                    departureHandler,
                    std::placeholders::_1,
                    std::placeholders::_2,
                    std::placeholders::_3
                )
            );
            container.pluginFunctionHandlers->RegisterFunctionCall(releaseDecisionCallback);

            // TAG function to trigger the request dialog
            TagFunction openDepartureReleaseRequestDialog(
                departureReleaseRequestDialogTriggerFunctionId,
                "Open Departure Release Request Dialog",
                std::bind(
                    &DepartureReleaseEventHandler::OpenRequestDialog,
                    departureHandler.get(),
                    std::placeholders::_1,
                    std::placeholders::_2,
                    std::placeholders::_3,
                    std::placeholders::_4
                )
            );
            container.pluginFunctionHandlers->RegisterFunctionCall(openDepartureReleaseRequestDialog);


            // TAG function to trigger the decision menu
            TagFunction openDepartureReleaseDecisionMenu(
                departureReleaseDecisionMenuTriggerFunctionId,
                "Open Departure Release Decision Menu",
                std::bind(
                    &DepartureReleaseEventHandler::OpenDecisionMenu,
                    departureHandler.get(),
                    std::placeholders::_1,
                    std::placeholders::_2,
                    std::placeholders::_3,
                    std::placeholders::_4
                )
            );
            container.pluginFunctionHandlers->RegisterFunctionCall(openDepartureReleaseDecisionMenu);

            // TAG function to trigger the status view
            TagFunction openDepartureReleaseStatusView(
                departureReleaseStatusViewTriggerFunctionId,
                "Show Departure Release Request Status View",
                std::bind(
                    &DepartureReleaseEventHandler::ShowStatusDisplay,
                    departureHandler.get(),
                    std::placeholders::_1,
                    std::placeholders::_2,
                    std::placeholders::_3,
                    std::placeholders::_4
                )
            );
            container.pluginFunctionHandlers->RegisterFunctionCall(openDepartureReleaseStatusView);

            // TAG function to trigger the cancellation menu
            TagFunction openDepartureReleaseCancellationMenu(
                departureReleaseCancelMenuFunctionId,
                "Open Departure Release Request Cancellation Menu",
                std::bind(
                    &DepartureReleaseEventHandler::SelectReleaseRequestToCancel,
                    departureHandler.get(),
                    std::placeholders::_1,
                    std::placeholders::_2,
                    std::placeholders::_3,
                    std::placeholders::_4
                )
            );
            container.pluginFunctionHandlers->RegisterFunctionCall(openDepartureReleaseCancellationMenu);

            // Callback for when a release decision is made
            CallbackFunction releaseCancelledCallback(
                releaseCancellationCallbackId,
                "Departure Release Request Cancelled",
                std::bind(
                    &DepartureReleaseEventHandler::RequestCancelled,
                    departureHandler,
                    std::placeholders::_1,
                    std::placeholders::_2,
                    std::placeholders::_3
                )
            );
            container.pluginFunctionHandlers->RegisterFunctionCall(releaseCancelledCallback);

            // Dialog for requesting departure releases
            std::shared_ptr<RequestDepartureReleaseDialog> requestDialog =
                std::make_shared<RequestDepartureReleaseDialog>(
                    departureHandler,
                    *container.activeCallsigns,
                    *container.plugin
                );

            container.dialogManager->AddDialog(
                {
                    IDD_DEPARTURE_RELEASE_REQUEST,
                    "Request Departure Releases",
                    reinterpret_cast<DLGPROC>(requestDialog->WndProc),
                    reinterpret_cast<LPARAM>(requestDialog.get()),
                    requestDialog
                }
            );

            // Dialog for approving departure releases
            std::shared_ptr<ApproveDepartureReleaseDialog> approveDialog = std::make_shared<
                ApproveDepartureReleaseDialog
            >(departureHandler);

            container.dialogManager->AddDialog(
                {
                    IDD_DEPARTURE_RELEASE_APPROVE,
                    "Approve Departure Releases",
                    reinterpret_cast<DLGPROC>(approveDialog->WndProc),
                    reinterpret_cast<LPARAM>(approveDialog.get()),
                    approveDialog
                }
            );

            // Add to handlers
            container.timedHandler->RegisterEvent(departureHandler, 15);
        }

        void BootstrapRadarScreen(
            const PersistenceContainer& container,
            RadarScreen::RadarRenderableCollection& renderables
        )
        {
            const int rendererId = renderables.ReserveRendererIdentifier();
            auto releaseRequestView = std::make_shared<DepartureReleaseRequestView>(
                *departureHandler,
                *container.controllerPositions,
                rendererId
            );
            renderables.RegisterRenderer(rendererId, releaseRequestView, renderables.afterLists);
        }
    }  // namespace Releases
}  // namespace UKControllerPlugin
