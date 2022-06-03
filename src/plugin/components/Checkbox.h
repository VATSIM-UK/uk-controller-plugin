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

    /**
     * A checkbox
     */
    class Checkbox : public std::enable_shared_from_this<Checkbox>
    {
        public:
        static auto Create(std::wstring label, POINT position, int screenObjectId, std::string name)
            -> std::shared_ptr<Checkbox>;
        auto Checked(bool checked) -> std::shared_ptr<Checkbox>;
        void
        Draw(Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen) const;

        protected:
        Checkbox(std::wstring label, POINT position, int screenObjectId, std::string name);

        private:
        // The label for the checkbox
        std::wstring label;

        // Clickspot area
        Gdiplus::Rect area;
        Gdiplus::Rect labelArea;

        // Pen
        std::shared_ptr<Gdiplus::Pen> pen;
        std::shared_ptr<Gdiplus::Brush> brush;

        // Clickspot
        std::shared_ptr<ClickableArea> clickableArea;

        // Font
        const Gdiplus::Font& font;

        // String format
        const Gdiplus::StringFormat& stringFormat;

        // Checked
        bool checked = false;

        // How big is it
        inline static const int DIMENSIONS = 15;
    };

} // namespace UKControllerPlugin::Components
