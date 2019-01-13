#include "pch/stdafx.h"
#include "prenote/PrenoteModule.h"
#include "prenote/PrenoteEventHandler.h"
#include "dependency/DependencyCache.h"
#include "bootstrap/PersistenceContainer.h"
#include "prenote/DeparturePrenote.h"
#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionHierarchy.h"
#include "prenote/PrenoteFactory.h"
#include "controller/ControllerPositionHierarchyFactory.h"
#include "prenote/PrenoteServiceFactory.h"
#include "bootstrap/BootstrapWarningMessage.h"
#include "prenote/PrenoteService.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Dependency::DependencyCache;
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
        const std::string PrenoteModule::dependencyFile = "prenotes.json";

        void PrenoteModule::BootstrapPlugin(PersistenceContainer & persistence, const DependencyCache & dependency)
        {
            if (!dependency.HasDependency(PrenoteModule::dependencyFile)) {
                BootstrapWarningMessage message("Prenote data file not found, prenotes not loaded");
                persistence.userMessager->SendMessageToUser(message);
                LogError("Prenote data file not found, prenotes not loaded");
                return;
            }

            nlohmann::json prenotes;
            try {
                prenotes = nlohmann::json::parse(dependency.GetDependency(PrenoteModule::dependencyFile));
            }
            catch (...) {
                BootstrapWarningMessage message("JSON exception when parsing prenotes file, prenotes not loaded");
                persistence.userMessager->SendMessageToUser(message);
                LogError("JSON exception when parsing prenotes file, prenotes not loaded");
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
