#include "EventBus.h"
#include "EventBusFactory.h"

namespace UKControllerPluginUtils::EventHandler {

    std::unique_ptr<EventBus> EventBus::singleton;
    std::shared_ptr<EventBusFactory> EventBus::factory;

    EventBus::EventBus(std::shared_ptr<EuroscopeThreadEventSink> euroscopeThreadEventProcessor)
        : euroscopeThreadEventProcessor(std::move(euroscopeThreadEventProcessor))
    {
        assert(this->euroscopeThreadEventProcessor && "Euroscope thread event processor cannot be null");
    }

    EventBus::~EventBus() = default;

    auto EventBus::Bus() -> EventBus&
    {
        if (EventBus::singleton) {
            return *EventBus::singleton;
        }

        if (!EventBus::factory) {
            throw std::exception("No event bus factory specified");
        }

        LogInfo("Created EventBus");
        EventBus::singleton = EventBus::factory->CreateBus();

        return *EventBus::singleton;
    }

    auto EventBus::GetAnyStream(const std::type_index& type) const -> const std::any&
    {
        return streams.at(type);
    }
} // namespace UKControllerPluginUtils::EventHandler
