#pragma once

namespace UKControllerPluginUtils::EventHandler {
    class DrainableEuroscopeThreadEventSink;
    class EventBus;

    /**
     * Creates event busses.
     */
    class EventBusFactory
    {
        public:
        virtual ~EventBusFactory() = default;
        [[nodiscard]] virtual auto CreateBus() -> std::unique_ptr<EventBus> = 0;
        [[nodiscard]] virtual auto DrainableEventSink() -> std::shared_ptr<DrainableEuroscopeThreadEventSink> = 0;
    };
} // namespace UKControllerPluginUtils::EventHandler
