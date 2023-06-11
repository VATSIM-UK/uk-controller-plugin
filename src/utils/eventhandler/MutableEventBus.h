#pragma once
#include "EventBus.h"
#include "EventBusFactory.h"

namespace UKControllerPluginUtils::EventHandler {
    class EventObserver;
    class EuroscopeThreadEventSink;

    class MutableEventBus : public EventBus
    {
        public:
        MutableEventBus(std::shared_ptr<EuroscopeThreadEventSink> euroscopeThreadEventProcessor);
        static void SetFactory(std::shared_ptr<EventBusFactory> factory);
        void SetObserver(std::shared_ptr<EventObserver> observer);
        [[nodiscard]] static auto IsInitialised() -> bool;
        static void Reset();
    };
} // namespace UKControllerPluginUtils::EventHandler
