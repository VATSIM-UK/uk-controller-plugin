#pragma once

// Forward declarations
namespace UKControllerPlugin {
    namespace Windows {
        class GdiGraphicsInterface;
    }  // namespace Windows
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroscopeRadarLoopbackInterface;
    }  // namespace Euroscope
}  // namespace UKControllerPlugin
// END

namespace UKControllerPlugin {
    namespace RadarScreen {

        /*
            An interface inherited by objects that are able to render some information
            to the EuroScope radar screen and/or have clickspots for which they wish to
            trigger some form of action.
        */
        class RadarRenderableInterface
        {
            public:
                virtual ~RadarRenderableInterface() = default;

                /*
                    Returns true if the object should be rendered.
                */
                virtual bool IsVisible(void) const = 0;

                /*
                    Called when a screen object is left clicked on.
                */
                virtual void LeftClick(
                    UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface& radarScreen,
                    int objectId,
                    std::string objectDescription,
                    POINT mousePos,
                    RECT itemArea
                ) = 0;

                /*
                    Called when a screen object is right clicked on.
                */
                virtual void RightClick(
                    int objectId,
                    std::string objectDescription,
                    UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface & radarScreen
                ) = 0;

                /*
                    Called when an object is moved.
                */
                virtual void Move(RECT position, std::string objectDescription) = 0;

                /*
                    Render to the screen and possibly add clickable spots.
                */
                virtual void Render(
                    UKControllerPlugin::Windows::GdiGraphicsInterface & graphics,
                    UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface & radarScreen
                ) = 0;

                /*
                    Reset the position of the object to default
                */
                virtual void ResetPosition() = 0;
        };
    }  // namespace RadarScreen
}  // namespace UKControllerPlugin
