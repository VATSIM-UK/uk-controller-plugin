#include "DeparturePrenote.h"
#include "PrenoteEventHandler.h"
#include "PrenoteFactory.h"
#include "PrenoteModule.h"
#include "PrenoteService.h"
#include "PrenoteServiceFactory.h"
#include "bootstrap/BootstrapWarningMessage.h"
#include "bootstrap/PersistenceContainer.h"
#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionHierarchy.h"
#include "controller/ControllerPositionHierarchyFactory.h"
#include "dependency/DependencyLoaderInterface.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "message/UserMessager.h"

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

namespace UKControllerPlugin::Prenote {

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
    }
    auto PrenoteModule::GetDependencyKey() -> std::string
    {
        return "DEPENDENCY_PRENOTE";
    }
} // namespace UKControllerPlugin::Prenote
