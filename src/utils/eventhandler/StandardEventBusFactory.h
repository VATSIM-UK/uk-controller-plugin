#pragma once
#include "eventhandler/EventBusFactory.h"

namespace UKControllerPluginUtils::EventHandler {

    class EuroscopeThreadEventProcessor;
    class DrainableEuroscopeThreadEventSink;

    /**
     * Creates a "normal" event bus.
     */
    class StandardEventBusFactory : public EventBusFactory
    {
        public:
        auto CreateBus() -> std::unique_ptr<EventBus> override;
        auto DrainableEventSink() -> std::shared_ptr<DrainableEuroscopeThreadEventSink> override;

        private:
        auto GetSink() -> std::shared_ptr<EuroscopeThreadEventProcessor>;

        // The event sink that we will drain events from.
        std::shared_ptr<EuroscopeThreadEventProcessor> sink;
    };
} // namespace UKControllerPluginUtils::EventHandler
