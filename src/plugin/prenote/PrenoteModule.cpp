#include "AcknowledgePrenoteMessage.h"
#include "CancelPrenoteMessageMenu.h"
#include "NewPrenotePushEventHandler.h"
#include "PlayNewPrenoteMessageSound.h"
#include "PrenoteAcknowledgedPushEventHandler.h"
#include "PrenoteDeletedPushEventHandler.h"
#include "PrenoteEventHandler.h"
#include "PrenoteIsRelevantToUser.h"
#include "PrenoteMessageCollection.h"
#include "PrenoteMessageEventHandlerCollection.h"
#include "PrenoteMessageStatusView.h"
#include "PrenoteMessageTimeout.h"
#include "PrenoteModule.h"
#include "PrenoteService.h"
#include "PrenoteStatusIndicatorTagItem.h"
#include "PublishedPrenoteCollection.h"
#include "PublishedPrenoteCollectionFactory.h"
#include "PublishedPrenoteMapper.h"
#include "SendNewPrenoteChatAreaMessage.h"
#include "SendPrenoteCancelledChatAreaMessage.h"
#include "SendPrenoteMenu.h"
#include "TriggerPrenoteMessageStatusView.h"
#include "bootstrap/BootstrapWarningMessage.h"
#include "bootstrap/PersistenceContainer.h"
#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionHierarchy.h"
#include "controller/ControllerPositionHierarchyFactory.h"
#include "dependency/DependencyLoaderInterface.h"
#include "euroscope/CallbackFunction.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "message/UserMessager.h"
#include "plugin/FunctionCallEventHandler.h"
#include "plugin/UKPlugin.h"
#include "push/PushEventProcessorCollection.h"
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

    std::shared_ptr<AcknowledgePrenoteMessage> acknowledge; // NOLINT
    std::unique_ptr<PublishedPrenoteCollection> prenotes;   // NOLINT
    std::unique_ptr<PublishedPrenoteMapper> mapper;         // NOLINT

    void PrenoteModule::BootstrapPlugin(PersistenceContainer& persistence, DependencyLoaderInterface& dependency)
    {
        // Prenote reminders bootstrap
        prenotes = CreatePublishedPrenoteCollection(
            dependency.LoadDependency(GetDependencyKey(), nlohmann::json::array()),
            *persistence.controllerHierarchyFactory);

        mapper = std::make_unique<PublishedPrenoteMapper>(
            *prenotes, *persistence.airfields, *persistence.sidMapper, *persistence.flightRules);

        ControllerPositionHierarchyFactory hierarchyFactory(*persistence.controllerPositions);

        persistence.flightplanHandler->RegisterHandler(std::make_shared<PrenoteEventHandler>(
            std::make_unique<PrenoteService>(
                *mapper, *persistence.airfieldOwnership, *persistence.activeCallsigns, *persistence.userMessager),
            *persistence.pluginUserSettingHandler));

        // Electronic prenote messages
        const auto prenoteUserRelevance = std::make_shared<PrenoteIsRelevantToUser>(*persistence.activeCallsigns);
        persistence.prenotes = std::make_shared<PrenoteMessageCollection>();
        persistence.prenoteMessageHandlers = std::make_unique<PrenoteMessageEventHandlerCollection>();
        persistence.prenoteMessageHandlers->AddHandler(
            std::make_shared<PlayNewPrenoteMessageSound>(prenoteUserRelevance, *persistence.windows));
        persistence.prenoteMessageHandlers->AddHandler(
            std::make_shared<SendNewPrenoteChatAreaMessage>(prenoteUserRelevance, *persistence.plugin));
        persistence.prenoteMessageHandlers->AddHandler(
            std::make_shared<SendPrenoteCancelledChatAreaMessage>(prenoteUserRelevance, *persistence.plugin));

        // Push event processors
        persistence.pushEventProcessors->AddProcessor(std::make_shared<NewPrenotePushEventHandler>(
            persistence.prenotes, *persistence.controllerPositions, *persistence.prenoteMessageHandlers));
        persistence.pushEventProcessors->AddProcessor(
            std::make_shared<PrenoteAcknowledgedPushEventHandler>(persistence.prenotes));
        persistence.pushEventProcessors->AddProcessor(
            std::make_shared<PrenoteDeletedPushEventHandler>(persistence.prenotes));
        persistence.timedHandler->RegisterEvent(
            std::make_shared<PrenoteMessageTimeout>(persistence.prenotes), MESSAGE_TIMEOUT_CHECK_INTERVAL);

        // Status indicator tag item
        persistence.tagHandler->RegisterTagItem(
            MESSAGE_STATUS_INDICATOR_TAG_ITEM_ID,
            std::make_shared<PrenoteStatusIndicatorTagItem>(persistence.prenotes));

        // Cancelling prenote messages menu
        auto cancelCallback = persistence.pluginFunctionHandlers->ReserveNextDynamicFunctionId();
        auto cancelMenu = std::make_shared<CancelPrenoteMessageMenu>(
            persistence.prenotes,
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
            persistence.prenotes,
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
            persistence.prenotes, *persistence.activeCallsigns, *persistence.taskRunner, *persistence.api);
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
        const PersistenceContainer& persistence, RadarRenderableCollection& radarRenderables)
    {
        // Status view renderer
        radarRenderables.RegisterRenderer(
            radarRenderables.ReserveRendererIdentifier(),
            std::make_shared<PrenoteMessageStatusView>(persistence.prenotes, *persistence.controllerPositions),
            RadarRenderableCollection::afterLists);
    }

    auto PrenoteModule::GetDependencyKey() -> std::string
    {
        return "DEPENDENCY_PRENOTES_V2";
    }
} // namespace UKControllerPlugin::Prenote
