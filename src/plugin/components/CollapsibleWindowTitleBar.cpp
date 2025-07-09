#include "Button.h"
#include "CollapsibleWindowTitleBar.h"
#include "StandardButtons.h"

namespace UKControllerPlugin::Components {

    CollapsibleWindowTitleBar::CollapsibleWindowTitleBar(
        std::wstring title, Gdiplus::Rect area, std::function<bool()> collapseState, int screenObjectId)
        : TitleBar(title, area)
    {
        this->closeButton = Button::Create(
            {area.GetRight() - 20, area.GetTop() + 5, 10, 10},
            screenObjectId,
            "closeButton",
            Components::CloseButton());

        this->collapseButton = Button::Create(
            {area.GetRight() - 35, area.GetTop() + 5, 10, 10},
            screenObjectId,
            "collapseButton",
            Components::CollapseButton(collapseState));
    }

    std::shared_ptr<CollapsibleWindowTitleBar> CollapsibleWindowTitleBar::Create(
        std::wstring title, Gdiplus::Rect area, std::function<bool()> collapseState, int screenObjectId)
    {
        auto titlebar = std::shared_ptr<CollapsibleWindowTitleBar>(
            new CollapsibleWindowTitleBar(title, area, collapseState, screenObjectId));
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
        Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen, const Windows::GdiplusBrushes& brushes) const
    {
        TitleBar::DrawTheme(graphics, radarScreen, brushes);
        this->closeButton->Draw(graphics, radarScreen);
        this->collapseButton->Draw(graphics, radarScreen);
    }
} // namespace UKControllerPlugin::Components
