#include "ApproveDepartureReleaseDialog.h"
#include "CompareEnrouteReleaseTypes.h"
#include "DepartureReleaseEventHandler.h"
#include "DepartureReleaseRequestView.h"
#include "DepartureReleaseRequestedEvent.h"
#include "EnrouteReleaseEventHandler.h"
#include "EnrouteReleaseTypesSerializer.h"
#include "RejectDepartureReleaseDialog.h"
#include "ReleaseIsTargetedAtUser.h"
#include "ReleaseModule.h"
#include "RequestDepartureReleaseDialog.h"
#include "SendReleaseRequestedChatAreaMessage.h"
#include "bootstrap/PersistenceContainer.h"
#include "collection/Collection.h"
#include "controller/HandoffEventHandlerCollection.h"
#include "dependency/DependencyLoaderInterface.h"
#include "dialog/DialogManager.h"
#include "euroscope/CallbackFunction.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "eventhandler/EventBus.h"
#include "eventhandler/EventHandlerFlags.h"
#include "plugin/FunctionCallEventHandler.h"
#include "plugin/UKPlugin.h"
#include "push/PushEventProcessorCollection.h"
#include "tag/TagFunction.h"
#include "tag/TagItemCollection.h"
#include "timedevent/TimedEventCollection.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Dependency::DependencyLoaderInterface;
using UKControllerPlugin::Euroscope::CallbackFunction;
using UKControllerPlugin::Euroscope::EuroscopeFlightplanListInterface;
using UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface;
using UKControllerPlugin::Releases::ApproveDepartureReleaseDialog;
using UKControllerPlugin::Releases::DepartureReleaseEventHandler;
using UKControllerPlugin::Releases::DepartureReleaseRequestView;
using UKControllerPlugin::Releases::RequestDepartureReleaseDialog;
using UKControllerPlugin::Tag::TagFunction;

namespace UKControllerPlugin::Releases {

    const unsigned int enrouteReleaseEventFrequency = 10;
    const unsigned int departureReleaseEventFrequency = 15;
    const unsigned int enrouteReleaseTypeTagItemId = 9005;
    const unsigned int enrouteReleasePointTagItemId = 9006;
    const unsigned int departureReleaseRequestDialogTriggerFunctionId = 9012;
    const unsigned int departureReleaseDecisionMenuTriggerFunctionId = 9013;
    const unsigned int departureReleaseStatusViewTriggerFunctionId = 9014;
    const unsigned int departureReleaseCancelMenuFunctionId = 9015;
    const unsigned int departureReleaseStatusIndicatorTagItemId = 124;
    const unsigned int departureReleaseCountdownTimerTagItemId = 125;
    const unsigned int departureReleaseRequestingControllerTagItemId = 126;

    void BootstrapPlugin(
        PersistenceContainer& container,
        EuroscopePluginLoopbackInterface& plugin,
        DependencyLoaderInterface& dependencies)
    {
        // Load everything to do with ENROUTE releases
        std::set<EnrouteReleaseType, CompareEnrouteReleaseTypes> releaseTypes;
        from_json(dependencies.LoadDependency(GetReleaseTypesDependencyKey(), nlohmann::json::array()), releaseTypes);

        // Create the handler
        std::shared_ptr<EnrouteReleaseEventHandler> handler = std::make_shared<EnrouteReleaseEventHandler>(
            *container.api,
            *container.plugin,
            *container.taskRunner,
            releaseTypes,
            container.pluginFunctionHandlers->ReserveNextDynamicFunctionId(),
            container.pluginFunctionHandlers->ReserveNextDynamicFunctionId());

        // TAG function to trigger the release type menu and receive the changes
        TagFunction openReleaseTypePopupMenu(
            enrouteReleaseTypeTagItemId,
            "Open Enroute Release Type Menu",
            [handler](
                UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& fp,
                UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& rt,
                const std::string& context,
                const POINT& mousePos) { handler->DisplayReleaseTypeMenu(fp, rt, context, mousePos); });
        container.pluginFunctionHandlers->RegisterFunctionCall(openReleaseTypePopupMenu);

        CallbackFunction releaseTypeSelectedCallback(
            handler->GetReleaseTypeSelectedCallbackId(),
            "Release Type Selected",
            [handler](int functionId, const std::string& subject, RECT screenObjectArea) {
                handler->ReleaseTypeSelected(functionId, subject, screenObjectArea);
            });
        container.pluginFunctionHandlers->RegisterFunctionCall(releaseTypeSelectedCallback);

        // TAG function to trigger the release edit popup and receive the changes
        TagFunction openReleasePointEditBox(
            enrouteReleasePointTagItemId,
            "Edit Enroute Release Point",
            [handler](
                UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& fp,
                UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& rt,
                const std::string& context,
                const POINT& mousePos) { handler->DisplayReleasePointEditBox(fp, rt, context, mousePos); });
        container.pluginFunctionHandlers->RegisterFunctionCall(openReleasePointEditBox);

        CallbackFunction editReleaseTypeCallback(
            handler->GetEditReleasePointCallbackId(),
            "Release Point Edited",
            [handler](int functionId, const std::string& subject, RECT screenObjectArea) {
                handler->EditReleasePoint(functionId, subject, screenObjectArea);
            });
        container.pluginFunctionHandlers->RegisterFunctionCall(editReleaseTypeCallback);

        // Add to events
        container.pushEventProcessors->AddProcessor(handler);
        container.tagHandler->RegisterTagItem(handler->GetReleaseTypeTagItemId(), handler);
        container.tagHandler->RegisterTagItem(handler->GetReleasePointTagItemId(), handler);
        container.tagHandler->RegisterTagItem(handler->GetReleasePointOrBlankTagItemId(), handler);
        container.timedHandler->RegisterEvent(handler, enrouteReleaseEventFrequency);
        container.controllerHandoffHandlers->RegisterHandler(handler);

        // Everything to do with DEPARTURE releases

        // Create the event handler
        const int releaseDecisionCallbackId = container.pluginFunctionHandlers->ReserveNextDynamicFunctionId();
        const int releaseCancellationCallbackId = container.pluginFunctionHandlers->ReserveNextDynamicFunctionId();
        auto releaseCollection = std::make_shared<UKControllerPlugin::Releases::DepartureReleaseRequestCollection>();
        auto departureHandler = std::make_shared<DepartureReleaseEventHandler>(
            releaseCollection,
            *container.api,
            *container.taskRunner,
            *container.plugin,
            *container.controllerPositions,
            *container.activeCallsigns,
            *container.dialogManager,
            *container.windows,
            *container.userMessager,
            releaseDecisionCallbackId,
            releaseCancellationCallbackId);
        container.departureReleaseHandler = departureHandler;
        container.pushEventProcessors->AddProcessor(departureHandler);
        container.tagHandler->RegisterTagItem(departureReleaseStatusIndicatorTagItemId, departureHandler);
        container.tagHandler->RegisterTagItem(departureReleaseCountdownTimerTagItemId, departureHandler);
        container.tagHandler->RegisterTagItem(departureReleaseRequestingControllerTagItemId, departureHandler);

        // Callback for when a release decision is made
        CallbackFunction releaseDecisionCallback(
            releaseDecisionCallbackId,
            "Departure Release Decision Made",
            [departureHandler](int functionId, const std::string& subject, RECT screenObjectArea) {
                departureHandler->ReleaseDecisionMade(functionId, subject, screenObjectArea);
            });
        container.pluginFunctionHandlers->RegisterFunctionCall(releaseDecisionCallback);

        // TAG function to trigger the request dialog
        TagFunction openDepartureReleaseRequestDialog(
            departureReleaseRequestDialogTriggerFunctionId,
            "Open Departure Release Request Dialog",
            [departureHandler](
                UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& fp,
                UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& rt,
                const std::string& context,
                const POINT& mousePos) { departureHandler->OpenRequestDialog(fp, rt, context, mousePos); });
        container.pluginFunctionHandlers->RegisterFunctionCall(openDepartureReleaseRequestDialog);

        // TAG function to trigger the decision menu
        TagFunction openDepartureReleaseDecisionMenu(
            departureReleaseDecisionMenuTriggerFunctionId,
            "Open Departure Release Decision Menu",
            [departureHandler](
                UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& fp,
                UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& rt,
                const std::string& context,
                const POINT& mousePos) { departureHandler->OpenDecisionMenu(fp, rt, context, mousePos); });
        container.pluginFunctionHandlers->RegisterFunctionCall(openDepartureReleaseDecisionMenu);

        // TAG function to trigger the status view
        TagFunction openDepartureReleaseStatusView(
            departureReleaseStatusViewTriggerFunctionId,
            "Show Departure Release Request Status View",
            [departureHandler](
                UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& fp,
                UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& rt,
                const std::string& context,
                const POINT& mousePos) { departureHandler->ShowStatusDisplay(fp, rt, context, mousePos); });
        container.pluginFunctionHandlers->RegisterFunctionCall(openDepartureReleaseStatusView);

        // TAG function to trigger the cancellation menu
        TagFunction openDepartureReleaseCancellationMenu(
            departureReleaseCancelMenuFunctionId,
            "Open Departure Release Request Cancellation Menu",
            [departureHandler](
                UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& fp,
                UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& rt,
                const std::string& context,
                const POINT& mousePos) { departureHandler->SelectReleaseRequestToCancel(fp, rt, context, mousePos); });
        container.pluginFunctionHandlers->RegisterFunctionCall(openDepartureReleaseCancellationMenu);

        // Callback for when a release decision is made
        CallbackFunction releaseCancelledCallback(
            releaseCancellationCallbackId,
            "Departure Release Request Cancelled",
            [departureHandler](int functionId, const std::string& subject, RECT screenObjectArea) {
                departureHandler->RequestCancelled(functionId, subject, screenObjectArea);
            });
        container.pluginFunctionHandlers->RegisterFunctionCall(releaseCancelledCallback);

        // Dialog for requesting departure releases
        std::shared_ptr<RequestDepartureReleaseDialog> requestDialog = std::make_shared<RequestDepartureReleaseDialog>(
            departureHandler, *container.activeCallsigns, *container.plugin);

        container.dialogManager->AddDialog(
            {IDD_DEPARTURE_RELEASE_REQUEST,
             "Request Departure Releases",
             reinterpret_cast<DLGPROC>(requestDialog->WndProc), // NOLINT
             reinterpret_cast<LPARAM>(requestDialog.get()),     // NOLINT
             requestDialog});

        // Dialog for approving departure releases
        std::shared_ptr<ApproveDepartureReleaseDialog> approveDialog =
            std::make_shared<ApproveDepartureReleaseDialog>(departureHandler);

        container.dialogManager->AddDialog(
            {IDD_DEPARTURE_RELEASE_APPROVE,
             "Approve Departure Releases",
             reinterpret_cast<DLGPROC>(approveDialog->WndProc), // NOLINT
             reinterpret_cast<LPARAM>(approveDialog.get()),     // NOLINT
             approveDialog});

        // Dialog for rejecting departure releases
        auto rejectDialog = std::make_shared<RejectDepartureReleaseDialog>(departureHandler);

        container.dialogManager->AddDialog(
            {IDD_DEPARTURE_RELEASE_REJECT,
             "Reject Departure Releases",
             reinterpret_cast<DLGPROC>(rejectDialog->WndProc), // NOLINT
             reinterpret_cast<LPARAM>(rejectDialog.get()),     // NOLINT
             rejectDialog});

        // Add to handlers
        container.timedHandler->RegisterEvent(departureHandler, departureReleaseEventFrequency);

        // Event handlers for displaying chat area messages
        auto releaseIsTargetedAtUser = std::make_shared<ReleaseIsTargetedAtUser>(container.activeCallsigns);
        UKControllerPluginUtils::EventHandler::EventBus::Bus().AddHandler<DepartureReleaseRequestedEvent>(
            std::make_shared<SendReleaseRequestedChatAreaMessage>(
                releaseIsTargetedAtUser, *container.plugin, *container.pluginUserSettingHandler),
            UKControllerPluginUtils::EventHandler::EventHandlerFlags::Sync);
    }

    void
    BootstrapRadarScreen(const PersistenceContainer& container, RadarScreen::RadarRenderableCollection& renderables)
    {
        // Create the request view renderer
        auto releaseRequestView = std::make_shared<DepartureReleaseRequestView>(
            *container.departureReleaseHandler, *container.controllerPositions);
        renderables.RegisterRenderer(
            renderables.ReserveRendererIdentifier(), releaseRequestView, renderables.afterLists);
    }

    auto GetReleaseTypesDependencyKey() -> std::string
    {
        return "DEPENDENCY_ENROUTE_RELEASE_TYPES";
    }
} // namespace UKControllerPlugin::Releases
