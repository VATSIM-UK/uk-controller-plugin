#pragma once
#include "EventHandlerFlags.h"

namespace UKControllerPlugin::EventHandler {
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

        protected:
        template <typename T> [[nodiscard]] auto GetStream() -> EventStream<T>&;

        // This is a singleton
        static std::unique_ptr<EventBus> singleton;

        // A factory for event busses
        static std::shared_ptr<EventBusFactory> factory;

        // The event streams
        std::map<std::type_index, std::any> streams;

        // An observer
        std::shared_ptr<EventObserver> observer = nullptr;
    };
} // namespace UKControllerPlugin::EventHandler

// Include the implementation of template methods
#include "EventBus.tpp"
