#pragma once
#include "TitleBar.h"

namespace UKControllerPlugin::Components {
    class Button;

    /**
     * The standard title bar with the standard close and
     * collapse buttons.
     */
    class CollapsibleWindowTitleBar : public TitleBar
    {
        public:
        static std::shared_ptr<CollapsibleWindowTitleBar> Create(
            const std::wstring& title,
            Gdiplus::Rect area,
            const std::function<bool()>& collapseState,
            int screenObjectId,
            const Windows::GdiplusBrushes& brushes);
        void Draw(Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen)
            const override;
        void DrawTheme(
            Windows::GdiGraphicsInterface& graphics,
            Euroscope::EuroscopeRadarLoopbackInterface& radarScreen,
            const Windows::GdiplusBrushes& themeBrushes) const override;

        CollapsibleWindowTitleBar(
            const std::wstring& title,
            Gdiplus::Rect area,
            const std::function<bool()>& collapseState,
            int screenObjectId,
            const Windows::GdiplusBrushes& brushes);

        private:
        // The brushes for theming
        const Windows::GdiplusBrushes& brushes;

        // The close button
        std::shared_ptr<Button> closeButton;

        // The collapse button
        std::shared_ptr<Button> collapseButton;
    };
} // namespace UKControllerPlugin::Components
