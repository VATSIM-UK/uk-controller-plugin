#pragma once

#include "theme/PaletteKey.h"

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
     * A class that builds a titlebar and then renders to screen.
     */
    class TitleBar : public std::enable_shared_from_this<TitleBar>
    {
        public:
        virtual ~TitleBar();
        static std::shared_ptr<TitleBar> Create(std::wstring title, Gdiplus::Rect area);
        std::shared_ptr<TitleBar> WithBackground(Theme::PaletteKey key);
        std::shared_ptr<TitleBar> WithText(Theme::PaletteKey key);
        std::shared_ptr<TitleBar> WithBorder(Theme::PaletteKey key);
        std::shared_ptr<TitleBar> WithDrag(int screenObjectId);
        std::shared_ptr<TitleBar> WithPosition(Gdiplus::Rect area);
        std::shared_ptr<TitleBar> WithTitle(std::wstring title);
        virtual void
        Draw(Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen) const;

        protected:
        TitleBar(std::wstring title, Gdiplus::Rect area);

        private:
        Theme::PaletteKey background = Theme::PaletteKey::Header;
        Theme::PaletteKey text = Theme::PaletteKey::Text;
        Theme::PaletteKey border = Theme::PaletteKey::Border;
        std::shared_ptr<ClickableArea> clickableArea = nullptr;
        std::wstring title;
        Gdiplus::Rect area;
    };
} // namespace UKControllerPlugin::Components
