#pragma once
#include "push/PushEventProcessorInterface.h"

namespace UKControllerPlugin {
    namespace Controller {
        class ControllerPositionCollection;
    } // namespace Controller
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Prenote {
    class PrenoteMessageCollection;
    class PrenoteMessageEventHandlerCollection;

    class NewPrenotePushEventHandler : public UKControllerPlugin::Push::PushEventProcessorInterface
    {
        public:
        NewPrenotePushEventHandler(
            std::shared_ptr<PrenoteMessageCollection> prenotes,
            const Controller::ControllerPositionCollection& controllers,
            const PrenoteMessageEventHandlerCollection& eventHandlers);
        void ProcessPushEvent(const Push::PushEvent& message) override;
        [[nodiscard]] auto GetPushEventSubscriptions() const
            -> std::set<UKControllerPlugin::Push::PushEventSubscription> override;

        private:
        [[nodiscard]] auto MessageValid(const nlohmann::json& message) const -> bool;

        // All the prenotes
        const std::shared_ptr<PrenoteMessageCollection> prenotes;

        // All the controllers
        const Controller::ControllerPositionCollection& controllers;

        // Handles events related to prenote messages
        const PrenoteMessageEventHandlerCollection& eventHandlers;
    };
} // namespace UKControllerPlugin::Prenote
