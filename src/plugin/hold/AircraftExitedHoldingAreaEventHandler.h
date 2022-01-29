#pragma once
#include "push/PushEventProcessorInterface.h"

namespace UKControllerPlugin::Navaids {
    class NavaidCollection;
} // namespace UKControllerPlugin::Navaids

namespace UKControllerPlugin::Hold {
    class HoldManager;

    class AircraftExitedHoldingAreaEventHandler : public Push::PushEventProcessorInterface
    {
        public:
        AircraftExitedHoldingAreaEventHandler(HoldManager& holdManager, const Navaids::NavaidCollection& navaids);
        void ProcessPushEvent(const Push::PushEvent& message) override;
        [[nodiscard]] auto GetPushEventSubscriptions() const -> std::set<Push::PushEventSubscription> override;

        private:
        [[nodiscard]] auto DataValid(const nlohmann::json& data) const -> bool;

        // For managing proximity holds
        HoldManager& holdManager;

        // All the navaids
        const Navaids::NavaidCollection& navaids;
    };
} // namespace UKControllerPlugin::Hold
