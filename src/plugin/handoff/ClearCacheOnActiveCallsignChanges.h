#pragma once
#include "controller/ActiveCallsignEventHandlerInterface.h"

namespace UKControllerPlugin::Handoff {
    class HandoffCache;

    class ClearCacheOnActiveCallsignChanges : public Controller::ActiveCallsignEventHandlerInterface
    {
        public:
        ClearCacheOnActiveCallsignChanges(HandoffCache& cache);
        void ActiveCallsignAdded(const Controller::ActiveCallsign& callsign) override;
        void ActiveCallsignRemoved(const Controller::ActiveCallsign& callsign) override;
        void CallsignsFlushed() override;

        private:
        // The cache to clear
        HandoffCache& cache;
    };
} // namespace UKControllerPlugin::Handoff
