#pragma once

namespace UKControllerPluginUtils::EventHandler {

    /**
     * An interface for processing events on the Euroscope thread.
     */
    class DrainableEuroscopeThreadEventSink
    {
        public:
        virtual ~DrainableEuroscopeThreadEventSink() = default;
        virtual void Drain() = 0;
    };
} // namespace UKControllerPluginUtils::EventHandler
