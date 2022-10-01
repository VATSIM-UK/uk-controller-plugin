#pragma once
#include "PrenoteMessageEventHandlerInterface.h"

namespace UKControllerPlugin::Windows {
    class WinApiInterface;
} // namespace UKControllerPlugin::Windows

namespace UKControllerPlugin::Prenote {
    class PrenoteUserRelevanceChecker;

    class PlayNewPrenoteMessageSound : public PrenoteMessageEventHandlerInterface
    {
        public:
        PlayNewPrenoteMessageSound(
            std::shared_ptr<PrenoteUserRelevanceChecker> prenoteRelevance, Windows::WinApiInterface& winApi);
        void NewMessage(const PrenoteMessage& message) override;

        private:
        // All the active controllers
        const std::shared_ptr<PrenoteUserRelevanceChecker> prenoteRelevance;

        // Windows API
        Windows::WinApiInterface& winApi;
    };
} // namespace UKControllerPlugin::Prenote
