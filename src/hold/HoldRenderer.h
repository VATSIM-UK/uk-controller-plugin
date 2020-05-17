#pragma once
#include "radarscreen/RadarRenderableInterface.h"
#include "radarscreen/ConfigurableDisplayInterface.h"
#include "euroscope/AsrEventHandlerInterface.h"
#include "hold/HoldDisplayManager.h"
#include "hold/HoldDisplay.h"

namespace UKControllerPlugin {
    namespace Hold {

        /*
            Handles the rendering of hold displays on a given ASR
        */
        class HoldRenderer :
            public UKControllerPlugin::RadarScreen::RadarRenderableInterface,
            public UKControllerPlugin::Euroscope::AsrEventHandlerInterface,
            public UKControllerPlugin::RadarScreen::ConfigurableDisplayInterface
        {
            public:

            HoldRenderer(
                    const std::shared_ptr<const UKControllerPlugin::Hold::HoldDisplayManager> displays,
                    const int screenObjectId,
                    const int toggleCallbackFunctionId
                );

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
                void Move(RECT position, std::string objectDescription) override;
                void Render(
                    UKControllerPlugin::Windows::GdiGraphicsInterface & graphics,
                    UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface & radarScreen
                ) override;
                void SetVisible(bool visible);
                void ResetPosition(void) override;

                // Inherited via ConfigurableDisplayInterface
                void Configure(int functionId, std::string subject, RECT screenObjectArea) override;
                UKControllerPlugin::Plugin::PopupMenuItem GetConfigurationMenuItem(void) const override;

                // Inherited via AsrEventHandlerInterface
                void AsrLoadedEvent(UKControllerPlugin::Euroscope::UserSetting & userSetting) override;
                void AsrClosingEvent(UKControllerPlugin::Euroscope::UserSetting & userSetting) override;

                // The id of the callback function to use for the configuration menu item
                const int toggleCallbackFunctionId;

                // The screen object id that relates to this renderer
                const int screenObjectId;

                // The description to show in our menu item
                const std::string menuItemDescription = "Display Holds";

                // The ASR key for whether or not the hold displays should be visible
                const std::string asrVisibleKey = "holdDisplayVisibility";

                // The ASR description for whether or not the hold displays should be visible
                const std::string asrVisibleDescription = "Managed Hold Display Visibility";

            private:

                std::string GetHoldNameFromObjectDescription(std::string objectDescription) const;
                std::string GetButtonNameFromObjectDescription(std::string objectDescription) const;

                // Should the renderer render the holds?
                bool renderHolds = true;

                // The displays to render
                const std::shared_ptr<const UKControllerPlugin::Hold::HoldDisplayManager> displays;
        };
    }  // namespace Hold
}  // namespace UKControllerPlugin
