#pragma once
#include <memory>
#include "eventhandler/EventBusFactory.h"

namespace UKControllerPlugin::EventHandler {

    /**
     * Creates a "normal" event bus.
     */
    class StandardEventBusFactory : public EventBusFactory
    {
        public:
        auto CreateBus() -> std::unique_ptr<EventBus> override;
    };
} // namespace UKControllerPlugin::EventHandler
