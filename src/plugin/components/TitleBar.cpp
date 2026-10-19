#include "TitleBar.h"
#include "ClickableArea.h"
#include "euroscope/EuroscopeRadarLoopbackInterface.h"
#include "graphics/GdiGraphicsInterface.h"
#include "theme/ThemeManager.h"

using UKControllerPlugin::Theme::PaletteKey;
using UKControllerPlugin::Theme::ThemeManager;

namespace UKControllerPlugin::Components {
    TitleBar::~TitleBar() = default;

    std::shared_ptr<TitleBar> TitleBar::Create(std::wstring title, Gdiplus::Rect area)
    {
        return std::shared_ptr<TitleBar>(new TitleBar(title, area));
    }

    std::shared_ptr<TitleBar> TitleBar::WithBackground(PaletteKey key)
    {
        background = key;
        return shared_from_this();
    }

    std::shared_ptr<TitleBar> TitleBar::WithText(PaletteKey key)
    {
        text = key;
        return shared_from_this();
    }

    std::shared_ptr<TitleBar> TitleBar::WithBorder(PaletteKey key)
    {
        border = key;
        return shared_from_this();
    }

    std::shared_ptr<TitleBar> TitleBar::WithDrag(int screenObjectId)
    {
        this->clickableArea = ClickableArea::Create(this->area, screenObjectId, "titleBar", true);
        return shared_from_this();
    }

    std::shared_ptr<TitleBar> TitleBar::WithPosition(Gdiplus::Rect area)
    {
        this->area = std::move(area);
        if (this->clickableArea != nullptr) {
            this->clickableArea->WithPosition(this->area);
        }
        return shared_from_this();
    }

    std::shared_ptr<TitleBar> TitleBar::WithTitle(std::wstring title)
    {
        this->title = title;
        return shared_from_this();
    }

    void TitleBar::Draw(
        Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen) const
    {
        graphics.FillRect(this->area, ThemeManager::Brush(background));
        graphics.DrawString(this->title, this->area, ThemeManager::Brush(text));
        graphics.DrawRect(this->area, ThemeManager::Pen(border));

        if (this->clickableArea != nullptr) {
            this->clickableArea->Apply(graphics, radarScreen);
        }
    }

    TitleBar::TitleBar(std::wstring title, Gdiplus::Rect area) : title(std::move(title)), area(area)
    {
    }
} // namespace UKControllerPlugin::Components
