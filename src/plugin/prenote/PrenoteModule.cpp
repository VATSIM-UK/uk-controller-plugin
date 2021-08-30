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
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "message/UserMessager.h"
#include "tag/TagItemCollection.h"

using UKControllerPlugin::Bootstrap::BootstrapWarningMessage;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Controller::ControllerPositionCollection;
using UKControllerPlugin::Controller::ControllerPositionHierarchy;
using UKControllerPlugin::Controller::ControllerPositionHierarchyFactory;
using UKControllerPlugin::Dependency::DependencyLoaderInterface;
using UKControllerPlugin::Prenote::DeparturePrenote;
using UKControllerPlugin::Prenote::PrenoteEventHandler;
using UKControllerPlugin::Prenote::PrenoteFactory;
using UKControllerPlugin::Prenote::PrenoteServiceFactory;
using UKControllerPlugin::Tag::TagItemCollection;

namespace UKControllerPlugin::Prenote {

    const int MESSAGE_STATUS_INDICATOR_TAG_ITEM_ID = 127;

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
    }

    auto PrenoteModule::GetDependencyKey() -> std::string
    {
        return "DEPENDENCY_PRENOTE";
    }
} // namespace UKControllerPlugin::Prenote
