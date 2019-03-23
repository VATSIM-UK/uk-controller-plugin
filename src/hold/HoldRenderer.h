#pragma once
#include "euroscope/AsrEventHandlerInterface.h"
#include "radarscreen/RadarRenderableInterface.h"

namespace UKControllerPlugin {
    namespace Hold {
        class HoldProfileManager;
        class HoldManager;
    }  // namespace Hold
    namespace Euroscope {
        class EuroscopePluginLoopbackInterface;
    }  // namespace Euroscope
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Hold {

        /*
            Handles the rendering of hold displays on a given ASR
        */
        class HoldRenderer : public UKControllerPlugin::Euroscope::AsrEventHandlerInterface,
            public UKControllerPlugin::RadarScreen::RadarRenderableInterface
        {
            public:

                HoldRenderer(
                    const UKControllerPlugin::Hold::HoldProfileManager & profileManager,
                    const UKControllerPlugin::Hold::HoldManager & holdManager,
                    const UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface & plugin
                );

                // Inherited via AsrEventHandlerInterface
                void AsrLoadedEvent(UKControllerPlugin::Euroscope::UserSetting & userSetting) override;
                void AsrClosingEvent(UKControllerPlugin::Euroscope::UserSetting & userSetting) override;

                // Inherited via RadarRenderableInterface
                bool IsVisible(void) const override;
                void LeftClick(
                    int objectId,
                    std::string objectDescription,
                    UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface & radarScreen
                ) override;
                void RightClick(
                    int objectId,
                    std::string objectDescription,
                    UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface & radarScreen
                ) override;
                void Move(RECT position) override;
                void Render(
                    UKControllerPlugin::Windows::GdiGraphicsInterface & graphics,
                    UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface & radarScreen
                ) override;

            private:
                
                // Manages the holds
                const UKControllerPlugin::Hold::HoldManager & holdManager;

                // Manages the hold profiles
                const UKControllerPlugin::Hold::HoldProfileManager & profileManager;

                // Plugin
                const UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface & plugin;
        };
    }  // namespace Hold
}  // namespace UKControllerPlugin
