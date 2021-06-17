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
            static std::shared_ptr<Button> Create(
                Gdiplus::Rect area,
                int screenObjectId,
                std::string screenObjectDescription,
                std::function<void(Windows::GdiGraphicsInterface& graphics)> customDraw
            );
            void WithPosition(Gdiplus::Rect area);
            void Draw(
                Windows::GdiGraphicsInterface& graphics,
                Euroscope::EuroscopeRadarLoopbackInterface& radarScreen
            ) const;

        protected:
            Button(
                Gdiplus::Rect area,
                int screenObjectId,
                std::string screenObjectDescription,
                std::function<void(Windows::GdiGraphicsInterface& graphics)> customDraw
            );

        private:
            std::string screenObjectDescription;
            std::function<void(Windows::GdiGraphicsInterface& graphics)> customDraw;
            Gdiplus::Rect area;
            int screenObjectId;
            std::shared_ptr<ClickableArea> clickableArea;
    };
} // namespace UKControllerPlugin::Components
