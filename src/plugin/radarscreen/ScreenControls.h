#pragma once
#include "ConfigurableDisplayCollection.h"
#include "RadarRenderableInterface.h"

// Forward declarations
namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroscopeRadarLoopbackInterface;
    } // namespace Euroscope

    namespace Windows {
        class GdiGraphicsInterface;
        struct GdiplusBrushes;
    } // namespace Windows
} // namespace UKControllerPlugin

namespace UKControllerPlugin::RadarScreen {

    /*
        A class to render the controls on screen which quickly display and
        hide things such as history trails.
    */
    class ScreenControls : public UKControllerPlugin::RadarScreen::RadarRenderableInterface
    {
        public:
        ScreenControls(
            int toggleboxIdEuroscope,
            UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection configurableDisplays,
            const UKControllerPlugin::Windows::GdiplusBrushes& brushes);
        [[nodiscard]] auto IsVisible() const -> bool override;
        void LeftClick(
            UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface& radarScreen,
            int objectId,
            const std::string& objectDescription,
            POINT mousePos,
            RECT itemArea) override;
        void Move(RECT position, std::string objectDescription) override;
        void RightClick(
            int objectId,
            const std::string& objectDescription,
            UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface& radarScreen) override;
        void Render(
            UKControllerPlugin::Windows::GdiGraphicsInterface& graphics,
            UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface& radarScreen) override;
        void ResetPosition() override;

        private:
        // Brushes for drawing
        const UKControllerPlugin::Windows::GdiplusBrushes& brushes;

        // Configurable displays
        const UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection configurableDisplays;

        // The id for the togglebox
        const int toggleboxIdEuroscope;

        // Height for each control
        const int controlHeight = 20;

        // Width for each control
        const int controlWidth = 20;

        // Title for the options
        const std::string menuName = "UKCP Options";
    };
} // namespace UKControllerPlugin::RadarScreen
