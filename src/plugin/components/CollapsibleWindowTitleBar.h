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
            int screenObjectId);
        void Draw(Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen)
            const override;

        CollapsibleWindowTitleBar(
            const std::wstring& title,
            Gdiplus::Rect area,
            const std::function<bool()>& collapseState,
            int screenObjectId);

        private:
        // The close button
        std::shared_ptr<Button> closeButton;

        // The collapse button
        std::shared_ptr<Button> collapseButton;
    };
} // namespace UKControllerPlugin::Components
