#pragma once
#include "EventBus.h"
#include "EventBusFactory.h"

namespace UKControllerPluginUtils::EventHandler {
    class EventObserver;

    class MutableEventBus : public EventBus
    {
        public:
        static void SetFactory(std::shared_ptr<EventBusFactory> factory);
        void SetObserver(std::shared_ptr<EventObserver> observer);
        static void Reset();
    };
} // namespace UKControllerPluginUtils::EventHandler
