#pragma once
#include "push/PushEventProcessorInterface.h"

namespace UKControllerPlugin::Controller {
    class ControllerPositionCollection;
} // namespace UKControllerPlugin::Controller

namespace UKControllerPlugin::Prenote {
    class PrenoteMessageCollection;

    class NewPrenotePushEventHandler : public UKControllerPlugin::Push::PushEventProcessorInterface
    {
        public:
        NewPrenotePushEventHandler(
            std::shared_ptr<PrenoteMessageCollection> prenotes,
            const Controller::ControllerPositionCollection& controllers);
        void ProcessPushEvent(const Push::PushEvent& message) override;
        [[nodiscard]] auto GetPushEventSubscriptions() const
            -> std::set<UKControllerPlugin::Push::PushEventSubscription> override;

        private:
        [[nodiscard]] auto MessageValid(const nlohmann::json& message) const -> bool;

        // All the prenotes
        const std::shared_ptr<PrenoteMessageCollection> prenotes;

        // All the controllers
        const Controller::ControllerPositionCollection& controllers;
    };
} // namespace UKControllerPlugin::Prenote
