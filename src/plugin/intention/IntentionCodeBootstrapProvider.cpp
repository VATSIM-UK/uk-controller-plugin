#include "CachedAircraftFirExitGenerator.h"
#include "IntentionCodeBootstrapProvider.h"
#include "IntentionCodeEventHandlerCollection.h"
#include "IntentionCodeModuleFactory.h"
#include "IntentionCodeTagItem.h"
#include "SendIntentionCodeUpdatedIntegrationMessage.h"
#include "bootstrap/ModuleFactories.h"
#include "bootstrap/PersistenceContainer.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "integration/IntegrationPersistenceContainer.h"
#include "integration/OutboundIntegrationEventHandler.h"
#include "tag/TagItemCollection.h"

namespace UKControllerPlugin::IntentionCode {

    void IntentionCodeBootstrapProvider::BootstrapPlugin(Bootstrap::PersistenceContainer& container)
    {
        // Register the FIR exit generator for flightplan events
        container.flightplanHandler->RegisterHandler(
            container.moduleFactories->IntentionCode().CachedFirExitGenerator(*container.dependencyLoader));

        // Register integration messages for intention code update events
        container.moduleFactories->IntentionCode().IntentionCodeEventHandlers()->AddHandler(
            std::make_shared<SendIntentionCodeUpdatedIntegrationMessage>(
                container.moduleFactories->IntentionCode().ExitPointCollection(*container.dependencyLoader),
                container.integrationModuleContainer->outboundMessageHandler));

        // Register the tag item for intention codes
        container.tagHandler->RegisterTagItem(
            intentionCodeTagItemId,
            std::make_shared<IntentionCodeTagItem>(container.moduleFactories->IntentionCode().IntentionCodeGenerator(
                *container.dependencyLoader, container.activeCallsigns)));
    }
} // namespace UKControllerPlugin::IntentionCode
