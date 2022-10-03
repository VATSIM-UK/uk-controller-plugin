#pragma once
#include "PrenoteMessageEventHandlerInterface.h"

namespace UKControllerPlugin::Euroscope {
    class EuroscopePluginLoopbackInterface;
} // namespace UKControllerPlugin::Euroscope

namespace UKControllerPlugin::Prenote {
    class PrenoteUserRelevanceChecker;

    class SendPrenoteCancelledChatAreaMessage : public PrenoteMessageEventHandlerInterface
    {
        public:
        SendPrenoteCancelledChatAreaMessage(
            std::shared_ptr<PrenoteUserRelevanceChecker> prenoteRelevance,
            Euroscope::EuroscopePluginLoopbackInterface& plugin);
        void MessageCancelled(const PrenoteMessage& message) override;

        private:
        // Checks relevance of prenotes
        const std::shared_ptr<PrenoteUserRelevanceChecker> prenoteRelevance;

        // For sending messages
        Euroscope::EuroscopePluginLoopbackInterface& plugin;
    };
} // namespace UKControllerPlugin::Prenote
