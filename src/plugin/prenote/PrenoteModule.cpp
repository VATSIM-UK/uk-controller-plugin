#include "AcknowledgePrenoteMessage.h"
#include "CancelPrenoteMessageMenu.h"
#include "DeparturePrenote.h"
#include "NewPrenotePushEventHandler.h"
#include "PendingPrenoteList.h"
#include "PrenoteAcknowledgedPushEventHandler.h"
#include "PrenoteDeletedPushEventHandler.h"
#include "PrenoteEventHandler.h"
#include "PrenoteFactory.h"
#include "PrenoteMessageCollection.h"
#include "PrenoteMessageStatusView.h"
#include "PrenoteMessageTimeout.h"
#include "PrenoteModule.h"
#include "PrenoteService.h"
#include "PrenoteServiceFactory.h"
#include "PrenoteStatusIndicatorTagItem.h"
#include "SendPrenoteMenu.h"
#include "TogglePendingPrenoteList.h"
#include "TriggerPrenoteMessageStatusView.h"
#include "bootstrap/BootstrapWarningMessage.h"
#include "bootstrap/PersistenceContainer.h"
#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionHierarchy.h"
#include "controller/ControllerPositionHierarchyFactory.h"
#include "dependency/DependencyLoaderInterface.h"
#include "euroscope/AsrEventHandlerCollection.h"
#include "euroscope/CallbackFunction.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "message/UserMessager.h"
#include "plugin/FunctionCallEventHandler.h"
#include "plugin/UKPlugin.h"
#include "push/PushEventProcessorCollection.h"
#include "radarscreen/ConfigurableDisplayCollection.h"
#include "tag/TagItemCollection.h"
#include "timedevent/TimedEventCollection.h"

using UKControllerPlugin::Bootstrap::BootstrapWarningMessage;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Controller::ControllerPositionCollection;
using UKControllerPlugin::Controller::ControllerPositionHierarchy;
using UKControllerPlugin::Controller::ControllerPositionHierarchyFactory;
using UKControllerPlugin::Dependency::DependencyLoaderInterface;
using UKControllerPlugin::Euroscope::CallbackFunction;
using UKControllerPlugin::Prenote::DeparturePrenote;
using UKControllerPlugin::Prenote::PrenoteEventHandler;
using UKControllerPlugin::Prenote::PrenoteFactory;
using UKControllerPlugin::Prenote::PrenoteServiceFactory;
using UKControllerPlugin::RadarScreen::RadarRenderableCollection;
using UKControllerPlugin::Tag::TagFunction;
using UKControllerPlugin::Tag::TagItemCollection;

namespace UKControllerPlugin::Prenote {

    const int MESSAGE_STATUS_INDICATOR_TAG_ITEM_ID = 127;

    const int CANCEL_MESSAGE_MENU_TAG_FUNCTION_ID = 9016;
    const int SEND_MESSAGE_MENU_TAG_FUNCTION_ID = 9017;
    const int MESSAGE_STATUS_VIEW_TAG_FUNCTION_ID = 9018;
    const int ACKNOWLEDGE_MESSAGE_TAG_FUNCTION_ID = 9019;

    const int MESSAGE_TIMEOUT_CHECK_INTERVAL = 10;

    std::shared_ptr<PrenoteMessageCollection> messages;     // NOLINT
    std::shared_ptr<AcknowledgePrenoteMessage> acknowledge; // NOLINT

    void PrenoteModule::BootstrapPlugin(PersistenceContainer& persistence, DependencyLoaderInterface& dependency)
    {
        // Prenote reminders bootstrap
        nlohmann::json prenotes = dependency.LoadDependency(GetDependencyKey(), nlohmann::json::array());

        ControllerPositionHierarchyFactory hierarchyFactory(*persistence.controllerPositions);
        PrenoteFactory prenoteFactory(hierarchyFactory);
        PrenoteServiceFactory handlerFactory(prenoteFactory, *persistence.userMessager);

        try {
            // Register handler
            persistence.flightplanHandler->RegisterHandler(std::make_shared<PrenoteEventHandler>(
                handlerFactory.Create(*persistence.airfieldOwnership, *persistence.activeCallsigns, prenotes),
                *persistence.pluginUserSettingHandler));
        } catch (...) {
            // If something goes wrong, someone else will log what.
        }

        // Electronic prenote messages
        messages = std::make_shared<PrenoteMessageCollection>();

        // Push event processors
        persistence.pushEventProcessors->AddProcessor(std::make_shared<NewPrenotePushEventHandler>(
            messages, *persistence.controllerPositions, *persistence.activeCallsigns, *persistence.windows));
        persistence.pushEventProcessors->AddProcessor(std::make_shared<PrenoteAcknowledgedPushEventHandler>(messages));
        persistence.pushEventProcessors->AddProcessor(std::make_shared<PrenoteDeletedPushEventHandler>(messages));
        persistence.timedHandler->RegisterEvent(
            std::make_shared<PrenoteMessageTimeout>(messages), MESSAGE_TIMEOUT_CHECK_INTERVAL);

        // Status indicator tag item
        persistence.tagHandler->RegisterTagItem(
            MESSAGE_STATUS_INDICATOR_TAG_ITEM_ID, std::make_shared<PrenoteStatusIndicatorTagItem>(messages));

        // Cancelling prenote messages menu
        auto cancelCallback = persistence.pluginFunctionHandlers->ReserveNextDynamicFunctionId();
        auto cancelMenu = std::make_shared<CancelPrenoteMessageMenu>(
            messages,
            *persistence.controllerPositions,
            *persistence.activeCallsigns,
            *persistence.plugin,
            *persistence.taskRunner,
            *persistence.api,
            cancelCallback);

        TagFunction openCancelPrenoteMenu(
            CANCEL_MESSAGE_MENU_TAG_FUNCTION_ID,
            "Open Cancel Prenote Message Menu",
            [cancelMenu](
                UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& fp,
                UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& rt,
                const std::string& context,
                const POINT& mousePos) { cancelMenu->DisplayPrenoteToDeleteMenu(fp, mousePos); });
        persistence.pluginFunctionHandlers->RegisterFunctionCall(openCancelPrenoteMenu);

        CallbackFunction cancelPrenoteCallback(
            cancelCallback, "Prenote Cancel", [cancelMenu](int functionId, std::string subject, RECT screenObjectArea) {
                cancelMenu->ControllerForPrenoteDeletionSelected(std::move(subject));
            });
        persistence.pluginFunctionHandlers->RegisterFunctionCall(cancelPrenoteCallback);

        // Send prenote messages menu
        auto sendCallback = persistence.pluginFunctionHandlers->ReserveNextDynamicFunctionId();
        auto sendMenu = std::make_shared<SendPrenoteMenu>(
            messages,
            *persistence.controllerPositions,
            *persistence.activeCallsigns,
            *persistence.plugin,
            *persistence.taskRunner,
            *persistence.api,
            sendCallback);

        TagFunction openSendPrenoteMenu(
            SEND_MESSAGE_MENU_TAG_FUNCTION_ID,
            "Open Prenote Controller Selection Menu",
            [sendMenu](
                UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& fp,
                UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& rt,
                const std::string& context,
                const POINT& mousePos) { sendMenu->DisplayControllerSelectionMenu(fp, mousePos); });
        persistence.pluginFunctionHandlers->RegisterFunctionCall(openSendPrenoteMenu);

        CallbackFunction sendPrenoteCallback(
            sendCallback, "Prenote Send", [sendMenu](int functionId, std::string subject, RECT screenObjectArea) {
                sendMenu->ControllerForPrenoteSelected(std::move(subject));
            });
        persistence.pluginFunctionHandlers->RegisterFunctionCall(sendPrenoteCallback);

        // Acknowledge message
        acknowledge = std::make_shared<AcknowledgePrenoteMessage>(
            messages, *persistence.activeCallsigns, *persistence.taskRunner, *persistence.api);
        auto acknowledgeForTagFunction = acknowledge;

        TagFunction acknowledgePrenoteMessage(
            ACKNOWLEDGE_MESSAGE_TAG_FUNCTION_ID,
            "Acknowledge Prenote",
            [acknowledgeForTagFunction](
                UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& fp,
                UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& rt,
                const std::string& context,
                const POINT& mousePos) { acknowledgeForTagFunction->Acknowledge(fp); });
        persistence.pluginFunctionHandlers->RegisterFunctionCall(acknowledgePrenoteMessage);

        // Prenote message status view
        auto statusViewTagFunction = std::make_shared<TriggerPrenoteMessageStatusView>();

        TagFunction triggerStatusView(
            MESSAGE_STATUS_VIEW_TAG_FUNCTION_ID,
            "Open Prenote Message Status View",
            [statusViewTagFunction](
                UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& fp,
                UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& rt,
                const std::string& context,
                const POINT& mousePos) { statusViewTagFunction->Trigger(fp, mousePos); });
        persistence.pluginFunctionHandlers->RegisterFunctionCall(triggerStatusView);
    }

    void PrenoteModule::BootstrapRadarScreen(
        const PersistenceContainer& persistence,
        RadarRenderableCollection& radarRenderables,
        RadarScreen::ConfigurableDisplayCollection& configurables,
        Euroscope::AsrEventHandlerCollection& asrHandlers)
    {
        // Status view renderer
        radarRenderables.RegisterRenderer(
            radarRenderables.ReserveRendererIdentifier(),
            std::make_shared<PrenoteMessageStatusView>(messages, *persistence.controllerPositions),
            RadarRenderableCollection::afterLists);

        // Pending list renderer
        auto listRendererId = radarRenderables.ReserveRendererIdentifier();
        auto listRenderer = std::make_shared<PendingPrenoteList>(
            messages,
            acknowledge,
            *persistence.plugin,
            *persistence.controllerPositions,
            *persistence.activeCallsigns,
            radarRenderables.ReserveScreenObjectIdentifier(listRendererId));

        radarRenderables.RegisterRenderer(listRendererId, listRenderer, RadarRenderableCollection::afterLists);
        asrHandlers.RegisterHandler(listRenderer);

        // Configuration menu toggle for pending list
        auto toggleListCallbackId = persistence.pluginFunctionHandlers->ReserveNextDynamicFunctionId();
        auto listToggle = std::make_shared<TogglePendingPrenoteList>(listRenderer, toggleListCallbackId);
        configurables.RegisterDisplay(listToggle);

        CallbackFunction showReleaseRequestListCallback(
            toggleListCallbackId,
            "Toggle Pending Prenote List",
            [listToggle](int functionId, std::string subject, RECT screenObjectArea) {
                listToggle->Configure(functionId, std::move(subject), screenObjectArea);
            });
        persistence.pluginFunctionHandlers->RegisterFunctionCall(showReleaseRequestListCallback);
    }

    auto PrenoteModule::GetDependencyKey() -> std::string
    {
        return "DEPENDENCY_PRENOTE";
    }
} // namespace UKControllerPlugin::Prenote
