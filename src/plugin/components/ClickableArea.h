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

    /*
     * A class that represents a clickable area in ES.
     * ES can't do GDI and can't handle translations.
     */
    class ClickableArea
    {
        public:
        static std::shared_ptr<ClickableArea>
        Create(Gdiplus::Rect area, int screenObjectId, std::string screenObjectDescription, bool draggable);
        void WithPosition(Gdiplus::Rect area);
        void
        Apply(Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen) const;

        protected:
        ClickableArea(Gdiplus::Rect area, int screenObjectId, std::string screenObjectDescription, bool draggable);

        private:
        std::string screenObjectDescription;
        Gdiplus::Rect area;
        int screenObjectId;
        bool draggable;
    };
} // namespace UKControllerPlugin::Components
