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
        static std::shared_ptr<CollapsibleWindowTitleBar>
        Create(std::wstring title, Gdiplus::Rect area, std::function<bool()> collapseState, int screenObjectId);
        void Draw(Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen)
            const override;
        void DrawTheme(Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen, const Windows::GdiplusBrushes& brushes) const override;

        protected:
        CollapsibleWindowTitleBar(
            std::wstring title, Gdiplus::Rect area, std::function<bool()> collapseState, int screenObjectId);

        private:
        // The close button
        std::shared_ptr<Button> closeButton;

        // The collapse button
        std::shared_ptr<Button> collapseButton;
    };
} // namespace UKControllerPlugin::Components
