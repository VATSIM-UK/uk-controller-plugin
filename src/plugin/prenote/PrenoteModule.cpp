#include "CancelPrenoteMessageMenu.h"
#include "DeparturePrenote.h"
#include "PrenoteEventHandler.h"
#include "PrenoteFactory.h"
#include "PrenoteMessageCollection.h"
#include "PrenoteModule.h"
#include "PrenoteService.h"
#include "PrenoteServiceFactory.h"
#include "PrenoteStatusIndicatorTagItem.h"
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
#include "tag/TagItemCollection.h"

using UKControllerPlugin::Bootstrap::BootstrapWarningMessage;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Controller::ControllerPositionCollection;
using UKControllerPlugin::Controller::ControllerPositionHierarchy;
using UKControllerPlugin::Controller::ControllerPositionHierarchyFactory;
using UKControllerPlugin::Dependency::DependencyLoaderInterface;
using UKControllerPlugin::Euroscope::CallbackFunction;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::Prenote::DeparturePrenote;
using UKControllerPlugin::Prenote::PrenoteEventHandler;
using UKControllerPlugin::Prenote::PrenoteFactory;
using UKControllerPlugin::Prenote::PrenoteServiceFactory;
using UKControllerPlugin::Tag::TagFunction;
using UKControllerPlugin::Tag::TagItemCollection;

namespace UKControllerPlugin::Prenote {

    const int MESSAGE_STATUS_INDICATOR_TAG_ITEM_ID = 127;

    const int CANCEL_MESSAGE_MENU_TAG_FUNCTION_ID = 9016;

    void PrenoteModule::BootstrapPlugin(PersistenceContainer& persistence, DependencyLoaderInterface& dependency)
    {
        nlohmann::json prenotes = dependency.LoadDependency(GetDependencyKey(), nlohmann::json::array());
        if (prenotes.empty()) {
            BootstrapWarningMessage message("Prenote data not found, prenotes not loaded");
            persistence.userMessager->SendMessageToUser(message);
            LogError("Prenote data not found, prenotes not loaded");
            return;
        }

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

        auto messages = std::make_shared<PrenoteMessageCollection>();
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
    }

    auto PrenoteModule::GetDependencyKey() -> std::string
    {
        return "DEPENDENCY_PRENOTE";
    }
} // namespace UKControllerPlugin::Prenote
