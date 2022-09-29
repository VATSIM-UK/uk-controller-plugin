#pragma once
#include "PrenoteMessageEventHandlerInterface.h"

namespace UKControllerPlugin {
    namespace Controller {
        class ActiveCallsignCollection;
    } // namespace Controller
    namespace Windows {
        class WinApiInterface;
    } // namespace Windows
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Prenote {
    class PlayNewPrenoteMessageSound : public PrenoteMessageEventHandlerInterface
    {
        public:
        PlayNewPrenoteMessageSound(
            const Controller::ActiveCallsignCollection& activeCallsigns, Windows::WinApiInterface& winApi);
        void NewMessage(const PrenoteMessage& message) override;

        private:
        // All the active controllers
        const Controller::ActiveCallsignCollection& activeCallsigns;

        // Windows API
        Windows::WinApiInterface& winApi;
    };
} // namespace UKControllerPlugin::Prenote
