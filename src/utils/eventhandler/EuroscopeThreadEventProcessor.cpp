#include "EuroscopeThreadEventProcessor.h"
#include <queue>

namespace UKControllerPluginUtils::EventHandler {

    struct EuroscopeThreadEventProcessor::Impl
    {
        // For locking the event queue.
        std::mutex mutex;

        // The event queue.
        std::queue<std::function<void()>> eventQueue;
    };

    EuroscopeThreadEventProcessor::EuroscopeThreadEventProcessor() : impl(std::make_unique<Impl>())
    {
    }

    EuroscopeThreadEventProcessor::~EuroscopeThreadEventProcessor() = default;

    void EuroscopeThreadEventProcessor::OnEvent(const std::function<void()>& event)
    {
        auto guard = std::lock_guard(this->impl->mutex);
        this->impl->eventQueue.push(event);
    }

    void EuroscopeThreadEventProcessor::Drain()
    {
        auto guard = std::lock_guard(this->impl->mutex);
        while (!this->impl->eventQueue.empty()) {
            this->impl->eventQueue.front()();
            this->impl->eventQueue.pop();
        }
    }
} // namespace UKControllerPluginUtils::EventHandler
