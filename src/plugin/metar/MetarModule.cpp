#include "MetarComponentsFactory.h"
#include "MetarComponentsFactoryFactory.h"
#include "MetarEventHandlerCollection.h"
#include "MetarModule.h"
#include "MetarsUpdatedPushEventProcessor.h"
#include "ParsedMetarCollection.h"
#include "ParsedMetarFactory.h"
#include "bootstrap/PersistenceContainer.h"
#include "push/PushEventProcessorCollection.h"

namespace UKControllerPlugin::Metar {

    std::unique_ptr<ParsedMetarCollection> parsedMetars;
    std::shared_ptr<MetarComponentsFactory> componentsFactory;
    std::unique_ptr<ParsedMetarFactory> parsedMetarFactory;

    void BootstrapPlugin(Bootstrap::PersistenceContainer& container)
    {
        // Make the bits
        parsedMetars = std::make_unique<ParsedMetarCollection>();
        componentsFactory = BuildComponentsFactory();
        parsedMetarFactory = std::make_unique<ParsedMetarFactory>(*componentsFactory, *container.airfields);

        // Collection for handling METAR events
        container.metarEventHandler = std::make_unique<MetarEventHandlerCollection>();

        // Handle all the push events for METARs
        container.pushEventProcessors->AddProcessor(
            std::make_shared<MetarsUpdatedPushEventProcessor>(*parsedMetars, *parsedMetarFactory, *container.api));
    }
} // namespace UKControllerPlugin::Metar
