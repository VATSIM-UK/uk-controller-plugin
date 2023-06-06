#include "EuroscopeThreadEventProcessor.h"
#include "MutableEventBus.h"
#include "StandardEventBusFactory.h"

namespace UKControllerPluginUtils::EventHandler {

    auto StandardEventBusFactory::CreateBus() -> std::unique_ptr<EventBus>
    {
        return std::make_unique<MutableEventBus>(GetSink());
    }

    auto StandardEventBusFactory::GetSink() -> std::shared_ptr<EuroscopeThreadEventProcessor>
    {
        if (!this->sink) {
            this->sink = std::make_shared<EuroscopeThreadEventProcessor>();
        }

        return sink;
    }

    auto StandardEventBusFactory::DrainableEventSink() -> std::shared_ptr<DrainableEuroscopeThreadEventSink>
    {
        return GetSink();
    }
} // namespace UKControllerPluginUtils::EventHandler
