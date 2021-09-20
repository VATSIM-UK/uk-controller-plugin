#pragma once
#include "HoldDisplay.h"
#include "HoldDisplayManager.h"
#include "euroscope/AsrEventHandlerInterface.h"
#include "radarscreen/ConfigurableDisplayInterface.h"
#include "radarscreen/RadarRenderableInterface.h"

namespace UKControllerPlugin::Hold {

    /*
        Handles the rendering of hold displays on a given ASR
    */
    class HoldRenderer : public UKControllerPlugin::RadarScreen::RadarRenderableInterface,
                         public UKControllerPlugin::Euroscope::AsrEventHandlerInterface,
                         public UKControllerPlugin::RadarScreen::ConfigurableDisplayInterface
    {
        public:
        HoldRenderer(
            std::shared_ptr<const UKControllerPlugin::Hold::HoldDisplayManager> displays,
            int screenObjectId,
            int toggleCallbackFunctionId);

        // Inherited via RadarRenderableInterface
        [[nodiscard]] auto IsVisible() const -> bool override;
        void LeftClick(
            UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface& radarScreen,
            int objectId,
            const std::string& objectDescription,
            POINT mousePos,
            RECT itemArea) override;
        void Move(RECT position, std::string objectDescription) override;
        void Render(
            UKControllerPlugin::Windows::GdiGraphicsInterface& graphics,
            UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface& radarScreen) override;
        void SetVisible(bool visible);
        void ResetPosition() override;

        // Inherited via ConfigurableDisplayInterface
        void Configure(int functionId, std::string subject, RECT screenObjectArea) override;
        [[nodiscard]] auto GetConfigurationMenuItem() const -> UKControllerPlugin::Plugin::PopupMenuItem override;

        // Inherited via AsrEventHandlerInterface
        void AsrLoadedEvent(UKControllerPlugin::Euroscope::UserSetting& userSetting) override;
        void AsrClosingEvent(UKControllerPlugin::Euroscope::UserSetting& userSetting) override;
        [[nodiscard]] auto GetToggleCallbackId() const -> int;

        private:
        [[nodiscard]] static auto GetHoldNameFromObjectDescription(const std::string& objectDescription) -> std::string;
        [[nodiscard]] static auto GetCallsignFromObjectDescription(const std::string& objectDescription) -> std::string;
        [[nodiscard]] static auto GetButtonNameFromObjectDescription(const std::string& objectDescription)
            -> std::string;

        // Should the renderer render the holds?
        bool renderHolds = true;

        // The displays to render
        const std::shared_ptr<const UKControllerPlugin::Hold::HoldDisplayManager> displays;

        // The screen object id that relates to this renderer
        const int screenObjectId;

        // The id of the callback function to use for the configuration menu item
        const int toggleCallbackFunctionId;

        // The description to show in our menu item
        const std::string menuItemDescription = "Display Holds";

        // The ASR key for whether or not the hold displays should be visible
        const std::string asrVisibleKey = "holdDisplayVisibility";

        // The ASR description for whether or not the hold displays should be visible
        const std::string asrVisibleDescription = "Managed Hold Display Visibility";

        inline static const POINT DEFAULT_POSITION = {100, 100};
    };
} // namespace UKControllerPlugin::Hold
