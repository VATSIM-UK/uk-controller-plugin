#pragma once
#include "radarscreen/RadarRenderableInterface.h"
#include "radarscreen/ConfigurableDisplayCollection.h"

// Forward declarations
namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroscopeRadarLoopbackInterface;
    }  // namespace Euroscope

    namespace Windows {
        class GdiGraphicsInterface;
        struct GdiplusBrushes;
    }  // namespace Windows
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace RadarScreen {
        class ConfigurableDisplayCollection;
    }  // namespace RadarScreen
}  // namespace UKControllerPlugin
// END

namespace UKControllerPlugin {
    namespace RadarScreen {

        /*
            A class to render the controls on screen which quickly display and
            hide things such as history trails.
        */
        class ScreenControls : public UKControllerPlugin::RadarScreen::RadarRenderableInterface
        {
            public:
                ScreenControls(
                    int toggleboxIdEuroscope,
                    const UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection & configurableDisplays,
                    const UKControllerPlugin::Windows::GdiplusBrushes & brushes
                );
                bool IsVisible(void) const;
                void LeftClick(
                    int objectId,
                    std::string objectDescription,
                    UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface & radarScreen
                ) override;
                void Move(RECT position, std::string objectDescription);
                void RightClick(
                    int objectId,
                    std::string objectDescription,
                    UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface & radarScreen
                ) override;
                void Render(
                    UKControllerPlugin::Windows::GdiGraphicsInterface & graphics,
                    UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface & radarScreen
                ) override;
                void ResetPosition(void) override;

                // The id for the togglebox
                const int toggleboxIdEuroscope;

                // Height for each control
                const int controlHeight;

                // Width for each control
                const int controlWidth;

                // Title for the options
                const std::string menuName = "UKCP Options";

            private:

                // Brushes for drawing
                const UKControllerPlugin::Windows::GdiplusBrushes & brushes;

                // Configurable displays
                const UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection configurableDisplays;
        };
    }  // namespace RadarScreen
}  // namespace UKControllerPlugin
