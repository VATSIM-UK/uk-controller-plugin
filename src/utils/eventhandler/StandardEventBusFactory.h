#pragma once
#include <memory>
#include "eventhandler/EventBusFactory.h"

namespace UKControllerPluginUtils::EventHandler {

    /**
     * Creates a "normal" event bus.
     */
    class StandardEventBusFactory : public EventBusFactory
    {
        public:
        auto CreateBus() -> std::unique_ptr<EventBus> override;
    };
} // namespace UKControllerPluginUtils::EventHandler
