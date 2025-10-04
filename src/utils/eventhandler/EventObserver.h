#pragma once
#include <any>

namespace UKControllerPluginUtils::EventHandler {
    /**
     * Interface for observing the events coming out of the event bus (for testing purposes)
     */
    class EventObserver
    {
        public:
        virtual ~EventObserver() = default;
        virtual void OnEvent(std::any event) = 0;
    };
} // namespace UKControllerPluginUtils::EventHandler
