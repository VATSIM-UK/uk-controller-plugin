#include "EventBus.h"
#include "EventBusFactory.h"

namespace UKControllerPlugin::EventHandler {

    std::unique_ptr<EventBus> EventBus::singleton;
    std::shared_ptr<EventBusFactory> EventBus::factory;

    EventBus::~EventBus() = default;

    auto EventBus::Bus() -> EventBus&
    {
        if (EventBus::singleton) {
            return *EventBus::singleton;
        }

        if (!EventBus::factory) {
            throw std::exception("No event bus factory specified");
        }

        EventBus::singleton = EventBus::factory->CreateBus();

        return *EventBus::singleton;
    }
} // namespace UKControllerPlugin::EventHandler
