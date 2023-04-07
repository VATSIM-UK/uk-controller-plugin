#pragma once
#include "PrenoteMessageEventHandlerInterface.h"

namespace UKControllerPlugin::Euroscope {
    class EuroscopePluginLoopbackInterface;
    class UserSetting;
} // namespace UKControllerPlugin::Euroscope

namespace UKControllerPlugin::Prenote {
    class PrenoteUserRelevanceChecker;

    class SendPrenoteCancelledChatAreaMessage : public PrenoteMessageEventHandlerInterface
    {
        public:
        SendPrenoteCancelledChatAreaMessage(
            std::shared_ptr<PrenoteUserRelevanceChecker> prenoteRelevance,
            Euroscope::EuroscopePluginLoopbackInterface& plugin,
            Euroscope::UserSetting& userSettings);
        void MessageCancelled(const PrenoteMessage& message) override;

        private:
        [[nodiscard]] auto UserWantsChatAreaMessages() const -> bool;
        // Checks relevance of prenotes
        const std::shared_ptr<PrenoteUserRelevanceChecker> prenoteRelevance;

        // For sending messages
        Euroscope::EuroscopePluginLoopbackInterface& plugin;

        // User settings
        Euroscope::UserSetting& userSettings;
    };
} // namespace UKControllerPlugin::Prenote
