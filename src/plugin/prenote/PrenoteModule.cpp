#include "prenote/PrenoteModule.h"
#include "prenote/PrenoteEventHandler.h"
#include "dependency/DependencyLoaderInterface.h"
#include "bootstrap/PersistenceContainer.h"
#include "prenote/DeparturePrenote.h"
#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionHierarchy.h"
#include "prenote/PrenoteFactory.h"
#include "controller/ControllerPositionHierarchyFactory.h"
#include "prenote/PrenoteServiceFactory.h"
#include "bootstrap/BootstrapWarningMessage.h"
#include "prenote/PrenoteService.h"
#include "message/UserMessager.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Dependency::DependencyLoaderInterface;
using UKControllerPlugin::Prenote::PrenoteEventHandler;
using UKControllerPlugin::Prenote::DeparturePrenote;
using UKControllerPlugin::Controller::ControllerPositionCollection;
using UKControllerPlugin::Controller::ControllerPositionHierarchy;
using UKControllerPlugin::Prenote::PrenoteFactory;
using UKControllerPlugin::Controller::ControllerPositionHierarchyFactory;
using UKControllerPlugin::Prenote::PrenoteServiceFactory;
using UKControllerPlugin::Bootstrap::BootstrapWarningMessage;

namespace UKControllerPlugin {
    namespace Prenote {

        // Set the static properties
        const std::string PrenoteModule::dependencyKey = "DEPENDENCY_PRENOTE";

        void PrenoteModule::BootstrapPlugin(PersistenceContainer & persistence, DependencyLoaderInterface& dependency)
        {
            nlohmann::json prenotes = dependency.LoadDependency(dependencyKey, nlohmann::json::array());
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
                persistence.flightplanHandler->RegisterHandler(
                    std::make_shared<PrenoteEventHandler>(
                        handlerFactory.Create(
                            *persistence.airfieldOwnership,
                            *persistence.activeCallsigns,
                            prenotes
                        ),
                        *persistence.pluginUserSettingHandler
                    )
                );
            } catch (...) {
                // If something goes wrong, someone else will log what.
            }
        }
    }  // namespace Prenote
}  // namespace UKControllerPlugin
