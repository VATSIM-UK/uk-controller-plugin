#include "EventBusBootstrap.h"
#include "bootstrap/PersistenceContainer.h"
#include "eventhandler/StandardEventBusFactory.h"
#include "eventhandler/MutableEventBus.h"
#include "eventhandler/ProcessEventBusEventsOnTimer.h"
#include "timedevent/TimedEventCollection.h"

namespace UKControllerPlugin::EventHandler {

    void BootstrapEventBus(const Bootstrap::PersistenceContainer& persistenceContainer)
    {
        // Create the bus
        auto eventBusFactory = std::make_shared<UKControllerPluginUtils::EventHandler::StandardEventBusFactory>();
        UKControllerPluginUtils::EventHandler::MutableEventBus::SetFactory(eventBusFactory);

        // Set up a timed event to process events that must be processed on the Euroscope thread
        persistenceContainer.timedHandler->RegisterEvent(
            std::make_shared<ProcessEventBusEventsOnTimer>(eventBusFactory->DrainableEventSink()), 1);
    }

    void ShutdownEventBus()
    {
        // Shutdown the event bus
        UKControllerPluginUtils::EventHandler::MutableEventBus::Reset();
    }
} // namespace UKControllerPlugin::EventHandler
