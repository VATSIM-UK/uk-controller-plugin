#include "PlayNewPrenoteMessageSound.h"
#include "PrenoteUserRelevanceChecker.h"
#include "windows/WinApiInterface.h"

namespace UKControllerPlugin::Prenote {
    PlayNewPrenoteMessageSound::PlayNewPrenoteMessageSound(
        std::shared_ptr<PrenoteUserRelevanceChecker> prenoteRelevance, Windows::WinApiInterface& winApi)
        : prenoteRelevance(prenoteRelevance), winApi(winApi)
    {
        assert(prenoteRelevance && "Prenote relevance is nullptr");
    }

    void PlayNewPrenoteMessageSound::NewMessage(const PrenoteMessage& message)
    {
        if (!prenoteRelevance->IsRelevant(message)) {
            return;
        }

        winApi.PlayWave(MAKEINTRESOURCE(WAVE_NEW_PRENOTE)); // NOLINT
    }
} // namespace UKControllerPlugin::Prenote
