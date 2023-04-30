#pragma once

namespace UKControllerPlugin::EventHandler {
    class EventBus;
    /**
     * Creates event busses.
     */
    class EventBusFactory
    {
        public:
        virtual ~EventBusFactory() = default;
        [[nodiscard]] virtual auto CreateBus() -> std::unique_ptr<EventBus> = 0;
    };
} // namespace UKControllerPlugin::EventHandler
