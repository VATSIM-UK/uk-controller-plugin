#pragma once
#include "MissedApproachRenderMode.h"
#include "euroscope/AsrEventHandlerInterface.h"
#include "radarscreen/RadarRenderableInterface.h"

namespace UKControllerPlugin::Euroscope {
    class EuroscopePluginLoopbackInterface;
} // namespace UKControllerPlugin::Euroscope

namespace UKControllerPlugin::MissedApproach {
    class MissedApproachCollection;

    /**
     * Renders recent missed approaches to the screen
     * so that users can see aircraft conducting missed
     * approaches.
     */
    class MissedApproachRenderer : public RadarScreen::RadarRenderableInterface,
                                   public Euroscope::AsrEventHandlerInterface
    {
        public:
        MissedApproachRenderer(
            std::shared_ptr<MissedApproachCollection> missedApproaches,
            Euroscope::EuroscopePluginLoopbackInterface& plugin);
        void AsrLoadedEvent(Euroscope::UserSetting& userSetting) override;
        void AsrClosingEvent(Euroscope::UserSetting& userSetting) override;
        [[nodiscard]] auto IsVisible() const -> bool override;
        void Render(
            Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen) override;

        private:
        // Contains all the missed approaches currently active
        const std::shared_ptr<MissedApproachCollection> missedApproaches;

        // Lets us access flightplans and radar targets
        const Euroscope::EuroscopePluginLoopbackInterface& plugin;

        // The render mode for missed approaches
        MissedApproachRenderMode renderMode;

        const std::string RENDER_MODE_ASR_KEY = "missedApproachRenderMode";
        const std::string RENDER_MODE_ASR_DESC = "Missed Approach Render Mode";
    };
} // namespace UKControllerPlugin::MissedApproach
