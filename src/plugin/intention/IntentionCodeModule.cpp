#include "IntentionCodeCache.h"
#include "IntentionCodeEventHandler.h"
#include "IntentionCodeFactory.h"
#include "IntentionCodeModule.h"
#include "SectorExitRepositoryFactory.h"
#include "bootstrap/PersistenceContainer.h"
#include "controller/ControllerStatusEventHandlerCollection.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "tag/TagItemCollection.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Flightplan::FlightPlanEventHandlerCollection;
using UKControllerPlugin::IntentionCode::IntentionCodeCache;
using UKControllerPlugin::IntentionCode::IntentionCodeEventHandler;
using UKControllerPlugin::IntentionCode::IntentionCodeFactory;
using UKControllerPlugin::IntentionCode::SectorExitRepositoryFactory;

namespace UKControllerPlugin::IntentionCode {

    // Bootstrap the intention code module
    void IntentionCodeModule::BootstrapPlugin(PersistenceContainer& container)
    {
        container.sectorExitPoints = std::move(SectorExitRepositoryFactory::Create());

        // Create the handler and its dependencies
        std::shared_ptr<IntentionCodeEventHandler> handler = std::make_shared<IntentionCodeEventHandler>(
            std::move(*IntentionCodeFactory::Create(*container.sectorExitPoints)),
            IntentionCodeCache(),
            *container.integrationModuleContainer->outboundMessageHandler);

        // Register with required event handlers.
        container.flightplanHandler->RegisterHandler(handler);
        container.tagHandler->RegisterTagItem(IntentionCodeModule::tagItemId, handler);
        container.controllerHandler->RegisterHandler(handler);
    }
} // namespace UKControllerPlugin::IntentionCode
