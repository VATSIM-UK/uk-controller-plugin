#include "PlayNewPrenoteMessageSound.h"
#include "PrenoteMessage.h"
#include "controller/ActiveCallsignCollection.h"
#include "controller/ControllerPosition.h"
#include "windows/WinApiInterface.h"

namespace UKControllerPlugin::Prenote {
    PlayNewPrenoteMessageSound::PlayNewPrenoteMessageSound(
        const Controller::ActiveCallsignCollection& activeCallsigns, Windows::WinApiInterface& winApi)
        : activeCallsigns(activeCallsigns), winApi(winApi)
    {
    }

    void PlayNewPrenoteMessageSound::NewMessage(const PrenoteMessage& message)
    {
        if (!activeCallsigns.UserHasCallsign() ||
            activeCallsigns.GetUserCallsign().GetNormalisedPosition().GetId() != message.GetTargetControllerId()) {
            return;
        }

        // Don't play sound if not for us, or we've somehow sent a prenote to ourselves.
        const auto userPositionId = activeCallsigns.GetUserCallsign().GetNormalisedPosition().GetId();
        if (userPositionId != message.GetTargetControllerId() ||
            message.GetTargetControllerId() == message.GetSendingControllerId()) {
            return;
        }

        winApi.PlayWave(MAKEINTRESOURCE(WAVE_NEW_PRENOTE)); // NOLINT
    }
} // namespace UKControllerPlugin::Prenote
