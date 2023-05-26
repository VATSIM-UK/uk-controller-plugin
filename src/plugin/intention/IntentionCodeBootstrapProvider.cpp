#include "CachedAircraftFirExitGenerator.h"
#include "CachedAircraftIntentionCodeGenerator.h"
#include "IntentionCodeBootstrapProvider.h"
#include "IntentionCodeIntegrationDataInitialiser.h"
#include "IntentionCodeEventHandlerCollection.h"
#include "IntentionCodeModuleFactory.h"
#include "IntentionCodeTagItem.h"
#include "SendIntentionCodeUpdatedIntegrationMessage.h"
#include "bootstrap/ModuleFactories.h"
#include "bootstrap/PersistenceContainer.h"
#include "controller/ActiveCallsignCollection.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "integration/IntegrationDataInitialisers.h"
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
        const auto intentionCodeGenerator = container.moduleFactories->IntentionCode().CachedIntentionCodeGenerator(
            *container.dependencyLoader, container.activeCallsigns);
        container.tagHandler->RegisterTagItem(
            intentionCodeTagItemId, std::make_shared<IntentionCodeTagItem>(intentionCodeGenerator));

        // Register the intention code generator for flightplan events and controller events
        container.flightplanHandler->RegisterHandler(intentionCodeGenerator);
        container.activeCallsigns->AddHandler(intentionCodeGenerator);

        // Register the integration initialiser
        container.integrationModuleContainer->dataInitialisers->Add(
            std::make_shared<IntentionCodeIntegrationDataInitialiser>(
                intentionCodeGenerator,
                container.moduleFactories->IntentionCode().ExitPointCollection(*container.dependencyLoader),
                *container.plugin));
    }
} // namespace UKControllerPlugin::IntentionCode
