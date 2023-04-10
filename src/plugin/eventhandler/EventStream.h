#pragma once
#include "EventHandlerFlags.h"

namespace UKControllerPlugin::EventHandler {
    template <class T> class EventHandler;

    template <typename T> class EventStream
    {
        public:
        /**
         * Adds a handler to the event stream.
         */
        void AddHandler(std::shared_ptr<EventHandler<T>> handler, EventHandlerFlags flags);

        /**
         * Processes an event.
         */
        void OnEvent(const T& event);

        private:
        // Contains data about the handlers
        using HandlerData = struct
        {
            std::shared_ptr<EventHandler<T>> handler;

            EventHandlerFlags flags;
        };
        std::vector<HandlerData> handlers;
    };
} // namespace UKControllerPlugin::EventHandler

// Include the implementation of template methods
#include "EventStream.tpp"
