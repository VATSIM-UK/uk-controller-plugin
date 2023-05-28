#pragma once

namespace UKControllerPluginUtils::EventHandler {

    /**
     * An interface for processing events on the Euroscope thread.
     */
    class EuroscopeThreadEventSink
    {
        public:
        virtual ~EuroscopeThreadEventSink() = default;
        virtual void OnEvent(const std::function<void()>& event) = 0;
    };
} // namespace UKControllerPluginUtils::EventHandler
