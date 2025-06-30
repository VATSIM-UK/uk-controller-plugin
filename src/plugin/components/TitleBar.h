#pragma once
#include <memory>
#include <string>
#include <functional>
#include <windows.h>
#include <gdiplus.h>

namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroscopeRadarLoopbackInterface;
    } // namespace Euroscope
    namespace Windows {
        class GdiGraphicsInterface;
        struct GdiplusBrushes;
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
        std::shared_ptr<TitleBar> WithDefaultBackgroundBrush();
        std::shared_ptr<TitleBar> WithBackgroundBrush(std::shared_ptr<Gdiplus::Brush> brush);
        std::shared_ptr<TitleBar> WithTextBrush(std::shared_ptr<Gdiplus::Brush> brush);
        std::shared_ptr<TitleBar> WithDefaultTextBrush();
        std::shared_ptr<TitleBar> WithBorder(std::shared_ptr<Gdiplus::Pen> pen);
        std::shared_ptr<TitleBar> WithDefaultBorder();
        std::shared_ptr<TitleBar> WithDrag(int screenObjectId);
        std::shared_ptr<TitleBar> WithPosition(Gdiplus::Rect area);
        std::shared_ptr<TitleBar> WithTitle(std::wstring title);
        virtual void
        Draw(Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen) const;
        virtual void DrawTheme(Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen, const Windows::GdiplusBrushes& brushes) const;

        protected:
        TitleBar(std::wstring title, Gdiplus::Rect area);

        private:
        std::shared_ptr<Gdiplus::Brush> backgroundBrush;
        std::shared_ptr<Gdiplus::Brush> textBrush;
        std::shared_ptr<Gdiplus::Pen> borderPen;
        std::shared_ptr<ClickableArea> clickableArea = nullptr;
        std::wstring title;
        Gdiplus::Rect area;
    };
} // namespace UKControllerPlugin::Components
