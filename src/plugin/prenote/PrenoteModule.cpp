#include "DeparturePrenote.h"
#include "NewPrenotePushEventHandler.h"
#include "PrenoteAcknowledgedPushEventHandler.h"
#include "PrenoteDeletedPushEventHandler.h"
#include "PrenoteEventHandler.h"
#include "PrenoteFactory.h"
#include "PrenoteMessageCollection.h"
#include "PrenoteMessageTimeout.h"
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
#include "push/PushEventProcessorCollection.h"
#include "timedevent/TimedEventCollection.h"

using UKControllerPlugin::Bootstrap::BootstrapWarningMessage;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Controller::ControllerPositionCollection;
using UKControllerPlugin::Controller::ControllerPositionHierarchy;
using UKControllerPlugin::Controller::ControllerPositionHierarchyFactory;
using UKControllerPlugin::Dependency::DependencyLoaderInterface;

namespace UKControllerPlugin::Prenote {

    const int MESSAGE_TIMEOUT_CHECK_INTERVAL = 10;

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

        // Electronic prenote messages bootstrap
        std::shared_ptr<PrenoteMessageCollection> messages = std::make_shared<PrenoteMessageCollection>();
        persistence.pushEventProcessors->AddProcessor(std::make_shared<NewPrenotePushEventHandler>(messages));
        persistence.pushEventProcessors->AddProcessor(std::make_shared<PrenoteAcknowledgedPushEventHandler>(messages));
        persistence.pushEventProcessors->AddProcessor(std::make_shared<PrenoteDeletedPushEventHandler>(messages));
        persistence.timedHandler->RegisterEvent(
            std::make_shared<PrenoteMessageTimeout>(messages), MESSAGE_TIMEOUT_CHECK_INTERVAL);
    }

    auto PrenoteModule::GetDependencyKey() -> std::string
    {
        return "DEPENDENCY_PRENOTE";
    }
} // namespace UKControllerPlugin::Prenote
