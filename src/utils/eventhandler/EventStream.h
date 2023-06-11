#pragma once
#include "EventHandlerFlags.h"

namespace UKControllerPluginUtils::EventHandler {
    class EuroscopeThreadEventSink;
    template <class T> class EventHandler;

    template <typename T> class EventStream
    {
        public:
        EventStream(std::shared_ptr<EuroscopeThreadEventSink> euroscopeThreadEventProcessor);

        /**
         * Adds a handler to the event stream.
         */
        void AddHandler(std::shared_ptr<EventHandler<T>> handler, EventHandlerFlags flags);

        /**
         * Processes an event.
         */
        void OnEvent(const T& event);

        // Contains data about the handlers
        using HandlerData = struct
        {
            std::shared_ptr<EventHandler<T>> handler;

            EventHandlerFlags flags;
        };

        [[nodiscard]] auto Handlers() const -> const std::vector<HandlerData>&
        {
            return handlers;
        }

        private:
        // Euroscope event processor
        std::shared_ptr<EuroscopeThreadEventSink> euroscopeThreadEventProcessor;

        // Details about each handler
        std::vector<HandlerData> handlers;
    };
} // namespace UKControllerPluginUtils::EventHandler

// Include the implementation of template methods
#include "EventStream.tpp"
