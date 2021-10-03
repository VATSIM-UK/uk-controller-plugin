#include "MissedApproachCollection.h"
#include "MissedApproachRenderer.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "euroscope/UserSetting.h"

namespace UKControllerPlugin::MissedApproach {

    MissedApproachRenderer::MissedApproachRenderer(
        std::shared_ptr<MissedApproachCollection> missedApproaches, Euroscope::EuroscopePluginLoopbackInterface& plugin)
        : missedApproaches(std::move(missedApproaches)), plugin(plugin)
    {
        this->plugin.GetSelectedFlightplan();
    }

    void MissedApproachRenderer::AsrLoadedEvent(Euroscope::UserSetting& userSetting)
    {
        auto asrRenderMode = userSetting.GetUnsignedIntegerEntry(
            this->RENDER_MODE_ASR_KEY, static_cast<unsigned int>(MissedApproachRenderMode::None));

        this->renderMode = asrRenderMode >= static_cast<unsigned int>(MissedApproachRenderMode::Invalid)
                               ? MissedApproachRenderMode::None
                               : static_cast<MissedApproachRenderMode>(asrRenderMode);
    }

    void MissedApproachRenderer::AsrClosingEvent(Euroscope::UserSetting& userSetting)
    {
        userSetting.Save(
            this->RENDER_MODE_ASR_KEY, this->RENDER_MODE_ASR_DESC, static_cast<unsigned int>(this->renderMode));
    }

    auto MissedApproachRenderer::IsVisible() const -> bool
    {
        return this->renderMode != MissedApproachRenderMode::None;
    }

    void MissedApproachRenderer::Render(
        Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen)
    {
    }
} // namespace UKControllerPlugin::MissedApproach
