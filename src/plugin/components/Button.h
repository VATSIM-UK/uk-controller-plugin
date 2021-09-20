#pragma once

namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroscopeRadarLoopbackInterface;
    } // namespace Euroscope
    namespace Windows {
        class GdiGraphicsInterface;
    } // namespace Windows
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Components {

    class ClickableArea;

    /*
     * A class that builds a button and then renders to screen.
     */
    class Button
    {
        public:
        static auto Create(
            Gdiplus::Rect area,
            int screenObjectId,
            std::string screenObjectDescription,
            std::function<void(Windows::GdiGraphicsInterface&, const Gdiplus::Rect&)> customDraw)
            -> std::shared_ptr<Button>;
        void WithPosition(Gdiplus::Rect area);
        void
        Draw(Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen) const;

        protected:
        Button(
            Gdiplus::Rect area,
            int screenObjectId,
            std::string screenObjectDescription,
            std::function<void(Windows::GdiGraphicsInterface&, const Gdiplus::Rect&)> customDraw);

        private:
        const std::function<void(Windows::GdiGraphicsInterface&, const Gdiplus::Rect&)> customDraw;
        Gdiplus::Rect area;
        std::shared_ptr<ClickableArea> clickableArea;
    };
} // namespace UKControllerPlugin::Components
