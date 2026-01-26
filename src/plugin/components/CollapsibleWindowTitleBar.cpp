#include "Button.h"
#include "CollapsibleWindowTitleBar.h"
#include "StandardButtons.h"

namespace UKControllerPlugin::Components {

    CollapsibleWindowTitleBar::CollapsibleWindowTitleBar(
        std::wstring title,
        Gdiplus::Rect area,
        std::function<bool()> collapseState,
        int screenObjectId,
        const Windows::GdiplusBrushes& brushes)
        : TitleBar(title, area), brushes(brushes)
    {
        this->closeButton = Button::Create(
            {area.GetRight() - 20, area.GetTop() + 5, 10, 10},
            screenObjectId,
            "closeButton",
            Components::CloseButton(this->brushes));

        this->collapseButton = Button::Create(
            {area.GetRight() - 35, area.GetTop() + 5, 10, 10},
            screenObjectId,
            "collapseButton",
            Components::CollapseButton(this->brushes, collapseState));
    }

    std::shared_ptr<CollapsibleWindowTitleBar> CollapsibleWindowTitleBar::Create(
        std::wstring title,
        Gdiplus::Rect area,
        std::function<bool()> collapseState,
        int screenObjectId,
        const Windows::GdiplusBrushes& brushes)
    {
        auto titlebar = std::shared_ptr<CollapsibleWindowTitleBar>(
            new CollapsibleWindowTitleBar(title, area, collapseState, screenObjectId, brushes));
        titlebar->WithDefaultBorder()->WithDefaultTextBrush()->WithDefaultBackgroundBrush()->WithDrag(screenObjectId);

        return titlebar;
    }

    void CollapsibleWindowTitleBar::Draw(
        Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen) const
    {
        TitleBar::Draw(graphics, radarScreen);
        this->closeButton->Draw(graphics, radarScreen);
        this->collapseButton->Draw(graphics, radarScreen);
    }

    void CollapsibleWindowTitleBar::DrawTheme(
        Windows::GdiGraphicsInterface& graphics,
        Euroscope::EuroscopeRadarLoopbackInterface& radarScreen,
        const Windows::GdiplusBrushes& brushes) const
    {
        TitleBar::DrawTheme(graphics, radarScreen, brushes);
        this->closeButton->Draw(graphics, radarScreen);
        this->collapseButton->Draw(graphics, radarScreen);
    }
} // namespace UKControllerPlugin::Components
