#include "MissedApproachRenderOptions.h"
#include "euroscope/UserSetting.h"

using UKControllerPlugin::Ownership::ServiceType;

namespace UKControllerPlugin::MissedApproach {

    MissedApproachRenderOptions::MissedApproachRenderOptions() : mode(MissedApproachRenderMode::None), duration(0)
    {
    }

    void MissedApproachRenderOptions::AsrLoadedEvent(Euroscope::UserSetting& userSetting)
    {
        // What to render
        auto asrRenderMode = userSetting.GetUnsignedIntegerEntry(
            this->RENDER_MODE_ASR_KEY, static_cast<unsigned int>(MissedApproachRenderMode::None));

        this->mode = asrRenderMode >= static_cast<unsigned int>(MissedApproachRenderMode::Invalid)
                         ? MissedApproachRenderMode::None
                         : static_cast<MissedApproachRenderMode>(asrRenderMode);

        // How long to render
        duration = std::chrono::seconds(
            userSetting.GetUnsignedIntegerEntry(this->RENDER_DURATION_ASR_KEY, DEFAULT_RENDER_DURATION));
    }

    void MissedApproachRenderOptions::AsrClosingEvent(Euroscope::UserSetting& userSetting)
    {
        userSetting.Save(this->RENDER_MODE_ASR_KEY, this->RENDER_MODE_ASR_DESC, static_cast<unsigned int>(this->mode));

        userSetting.Save(
            this->RENDER_DURATION_ASR_KEY, this->RENDER_DURATION_ASR_DESC, static_cast<unsigned int>(duration.count()));
    }

    auto MissedApproachRenderOptions::Mode() const -> MissedApproachRenderMode
    {
        return mode;
    }

    void MissedApproachRenderOptions::SetMode(MissedApproachRenderMode mode)
    {
        this->mode = mode;
    }

    auto MissedApproachRenderOptions::Duration() const -> const std::chrono::seconds&
    {
        return duration;
    }

    void MissedApproachRenderOptions::SetDuration(std::chrono::seconds duration)
    {
        this->duration = duration;
    }
} // namespace UKControllerPlugin::MissedApproach
