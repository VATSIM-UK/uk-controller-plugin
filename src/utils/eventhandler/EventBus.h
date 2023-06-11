#pragma once
#include "EventHandlerFlags.h"
#include "EuroscopeThreadEventSink.h"
#include <typeindex>

namespace UKControllerPluginUtils::EventHandler {
    template <class T> class EventHandler;
    template <class T> class EventStream;
    class EventObserver;
    class EventBusFactory;

    /**
     * Handles events around the plugin
     */
    class EventBus
    {
        public:
        EventBus(std::shared_ptr<EuroscopeThreadEventSink> euroscopeThreadEventProcessor);
        ~EventBus();

        [[nodiscard]] static auto Bus() -> EventBus&;

        /**
         * Add a handler for an event.
         */
        template <typename T> void AddHandler(std::shared_ptr<EventHandler<T>> handler, EventHandlerFlags flags);

        /**
         * Fire an event.
         */
        template <typename T> void OnEvent(const T& event);

        /**
         * Get the given stream as a std::any. For test purposes only and should not be called
         * anywhere else.
         */
        [[nodiscard]] auto GetAnyStream(const std::type_index& type) const -> const std::any&;

        protected:
        template <typename T> [[nodiscard]] auto GetStream() -> EventStream<T>&;

        // This is a singleton
        static std::unique_ptr<EventBus> singleton;

        // A factory for event busses
        static std::shared_ptr<EventBusFactory> factory;

        // Euroscope event processor
        std::shared_ptr<EuroscopeThreadEventSink> euroscopeThreadEventProcessor;

        // The event streams
        std::map<std::type_index, std::any> streams;

        // An observer
        std::shared_ptr<EventObserver> observer = nullptr;
    };
} // namespace UKControllerPluginUtils::EventHandler

// Include the implementation of template methods
#include "EventBus.tpp"
