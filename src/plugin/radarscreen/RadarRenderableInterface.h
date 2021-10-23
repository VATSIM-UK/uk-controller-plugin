#pragma once

namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroscopeRadarLoopbackInterface;
    } // namespace Euroscope
    namespace Windows {
        class GdiGraphicsInterface;
    } // namespace Windows
} // namespace UKControllerPlugin

namespace UKControllerPlugin::RadarScreen {

    /*
        An interface inherited by objects that are able to render some information
        to the EuroScope radar screen and/or have clickspots for which they wish to
        trigger some form of action.
    */
    class RadarRenderableInterface
    {
        public:
        RadarRenderableInterface();
        virtual ~RadarRenderableInterface() = default;
        RadarRenderableInterface(const RadarRenderableInterface&);
        RadarRenderableInterface(RadarRenderableInterface&&) noexcept;
        auto operator=(const RadarRenderableInterface&) -> RadarRenderableInterface&;
        auto operator=(RadarRenderableInterface&&) noexcept -> RadarRenderableInterface&;

        /*
            Returns true if the object should be rendered.
        */
        [[nodiscard]] virtual auto IsVisible() const -> bool = 0;

        /*
            Called when a screen object is left clicked on.
        */
        virtual void LeftClick(
            UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface& radarScreen,
            int objectId,
            const std::string& objectDescription,
            POINT mousePos,
            RECT itemArea);

        /*
            Called when a screen object is right clicked on.
        */
        virtual void RightClick(
            int objectId,
            const std::string& objectDescription,
            UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface& radarScreen);

        /*
            Called when an object is moved.
        */
        virtual void Move(RECT position, std::string objectDescription);

        /*
            Render to the screen and possibly add clickable spots.
        */
        virtual void Render(
            UKControllerPlugin::Windows::GdiGraphicsInterface& graphics,
            UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface& radarScreen) = 0;

        /*
            Reset the position of the object to default
        */
        virtual void ResetPosition();
    };
} // namespace UKControllerPlugin::RadarScreen
