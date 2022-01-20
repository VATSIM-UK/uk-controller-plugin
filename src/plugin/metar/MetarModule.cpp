#include "MetarComponentsFactory.h"
#include "MetarComponentsFactoryFactory.h"
#include "MetarEventHandlerCollection.h"
#include "MetarModule.h"
#include "MetarsUpdatedPushEventProcessor.h"
#include "ParsedMetarCollection.h"
#include "ParsedMetarFactory.h"
#include "PressureQueryCommandHandler.h"
#include "bootstrap/PersistenceContainer.h"
#include "command/CommandHandlerCollection.h"
#include "push/PushEventProcessorCollection.h"

namespace UKControllerPlugin::Metar {

    std::unique_ptr<ParsedMetarCollection> parsedMetars;       // NOLINT
    std::shared_ptr<MetarComponentsFactory> componentsFactory; // NOLINT
    std::unique_ptr<ParsedMetarFactory> parsedMetarFactory;    // NOLINT

    void BootstrapPlugin(Bootstrap::PersistenceContainer& container)
    {
        // Make the bits
        parsedMetars = std::make_unique<ParsedMetarCollection>();
        componentsFactory = BuildComponentsFactory();
        parsedMetarFactory = std::make_unique<ParsedMetarFactory>(*componentsFactory, *container.airfields);

        // Collection for handling METAR events
        container.metarEventHandler = std::make_unique<MetarEventHandlerCollection>();

        // Handle all the push events for METARs
        container.pushEventProcessors->AddProcessor(std::make_shared<MetarsUpdatedPushEventProcessor>(
            *parsedMetars, *parsedMetarFactory, *container.api, *container.metarEventHandler));

        // Handler for the pressure query command
        container.commandHandlers->RegisterHandler(
            std::make_shared<PressureQueryCommandHandler>(*parsedMetars, *container.userMessager));
    }
} // namespace UKControllerPlugin::Metar
