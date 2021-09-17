#pragma once
#include "push/PushEventProcessorInterface.h"

namespace UKControllerPlugin {
    namespace Controller {
        class ActiveCallsignCollection;
        class ControllerPositionCollection;
    } // namespace Controller
    namespace Windows {
        class WinApiInterface;
    } // namespace Windows
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Prenote {
    class PrenoteMessageCollection;

    class NewPrenotePushEventHandler : public UKControllerPlugin::Push::PushEventProcessorInterface
    {
        public:
        NewPrenotePushEventHandler(
            std::shared_ptr<PrenoteMessageCollection> prenotes,
            const Controller::ControllerPositionCollection& controllers,
            const Controller::ActiveCallsignCollection& activeCallsigns,
            Windows::WinApiInterface& winApi);
        void ProcessPushEvent(const Push::PushEvent& message) override;
        [[nodiscard]] auto GetPushEventSubscriptions() const
            -> std::set<UKControllerPlugin::Push::PushEventSubscription> override;

        private:
        [[nodiscard]] auto MessageValid(const nlohmann::json& message) const -> bool;

        // All the prenotes
        const std::shared_ptr<PrenoteMessageCollection> prenotes;

        // All the controllers
        const Controller::ControllerPositionCollection& controllers;

        // Active controllers
        const Controller::ActiveCallsignCollection& activeCallsigns;

        // For playing sounds
        Windows::WinApiInterface& winApi;
    };
} // namespace UKControllerPlugin::Prenote
