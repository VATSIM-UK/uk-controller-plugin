#include "DeparturePrenote.h"
#include "NewPrenotePushEventHandler.h"
#include "PrenoteAcknowledgedPushEventHandler.h"
#include "PrenoteEventHandler.h"
#include "PrenoteFactory.h"
#include "PrenoteMessageCollection.h"
#include "PrenoteModule.h"
#include "PrenoteService.h"
#include "PrenoteServiceFactory.h"
#include "bootstrap/BootstrapWarningMessage.h"
#include "bootstrap/PersistenceContainer.h"
#include "controller/ControllerPositionHierarchyFactory.h"
#include "dependency/DependencyLoaderInterface.h"

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
        // Prenote reminders bootstrap
        nlohmann::json prenotes = dependency.LoadDependency(DependencyKey(), nlohmann::json::array());
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

        // Electronic prenote messages bootstrap
        std::shared_ptr<PrenoteMessageCollection> messages = std::make_shared<PrenoteMessageCollection>();
        persistence.pushEventProcessors->AddProcessor(std::make_shared<NewPrenotePushEventHandler>(messages));
        persistence.pushEventProcessors->AddProcessor(std::make_shared<PrenoteAcknowledgedPushEventHandler>(messages));
    }

    auto PrenoteModule::DependencyKey() -> std::string
    {
        return "DEPENDENCY_PRENOTE";
    }
} // namespace UKControllerPlugin::Prenote
