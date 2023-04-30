#pragma once

namespace UKControllerPlugin::EventHandler {

    template <class T> class EventHandler
    {
        public:
        virtual ~EventHandler() = default;

        /**
         * Process the given event.
         */
        virtual void OnEvent(const T& event) = 0;
    };
} // namespace UKControllerPlugin::EventHandler
