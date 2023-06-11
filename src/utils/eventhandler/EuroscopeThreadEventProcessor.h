#pragma once
#include "DrainableEuroscopeThreadEventSink.h"
#include "EuroscopeThreadEventSink.h"

namespace UKControllerPluginUtils::EventHandler {

    /**
     * Processes events on the Euroscope thread.
     */
    class EuroscopeThreadEventProcessor : public EuroscopeThreadEventSink, public DrainableEuroscopeThreadEventSink
    {
        public:
        EuroscopeThreadEventProcessor();
        ~EuroscopeThreadEventProcessor() override;
        void OnEvent(const std::function<void()>& event) override;
        void Drain() override;

        private:
        struct Impl;
        std::unique_ptr<Impl> impl;
    };

} // namespace UKControllerPluginUtils::EventHandler
