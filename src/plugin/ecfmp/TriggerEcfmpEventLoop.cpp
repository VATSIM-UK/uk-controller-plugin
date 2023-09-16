#include "TriggerEcfmpEventLoop.h"

#include <utility>

namespace UKControllerPlugin::ECFMP {

    TriggerECFMPEventLoop::TriggerECFMPEventLoop(std::shared_ptr<::ECFMP::Plugin::Sdk> sdk) : sdk(std::move(sdk))
    {
        assert(this->sdk && "TriggerECFMPEventLoop::TriggerECFMPEventLoop: sdk is null");
    }

    void TriggerECFMPEventLoop::TimedEventTrigger()
    {
        sdk->OnEuroscopeTimerTick();
    }
} // namespace UKControllerPlugin::ECFMP
