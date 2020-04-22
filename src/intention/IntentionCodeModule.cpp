#include "pch/stdafx.h"
#include "intention/IntentionCodeModule.h"
#include "intention/IntentionCodeEventHandler.h"
#include "intention/IntentionCodeFactory.h"
#include "intention/IntentionCodeCache.h"
#include "bootstrap/PersistenceContainer.h"
#include "intention/SectorExitRepositoryFactory.h"

using UKControllerPlugin::Flightplan::FlightPlanEventHandlerCollection;
using UKControllerPlugin::IntentionCode::IntentionCodeEventHandler;
using UKControllerPlugin::IntentionCode::IntentionCodeFactory;
using UKControllerPlugin::IntentionCode::IntentionCodeCache;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::IntentionCode::SectorExitRepositoryFactory;

namespace UKControllerPlugin {
    namespace IntentionCode {

        // Bootstrap the intention code module
        void IntentionCodeModule::BootstrapPlugin(
            PersistenceContainer & container
        ) {
            container.sectorExitPoints = std::move(SectorExitRepositoryFactory::Create());

            // Create the handler and its dependencies
            std::shared_ptr<IntentionCodeEventHandler> handler = std::make_shared<IntentionCodeEventHandler>(
                    std::move(*IntentionCodeFactory::Create(*container.sectorExitPoints)),
                    IntentionCodeCache()
            );

            // Register with required event handlers.
            container.flightplanHandler->RegisterHandler(handler);
            container.tagHandler->RegisterTagItem(IntentionCodeModule::tagItemId, handler);
            container.controllerHandler->RegisterHandler(handler);
        }
    }  // namespace IntentionCode
}  // namespace UKControllerPlugin
