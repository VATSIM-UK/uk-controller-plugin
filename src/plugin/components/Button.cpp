#include "Button.h"
#include "ClickableArea.h"
#include "euroscope/EuroscopeRadarLoopbackInterface.h"
#include "graphics/GdiGraphicsInterface.h"

namespace UKControllerPlugin::Components {

    auto Button::Create(
        Gdiplus::Rect area,
        int screenObjectId,
        std::string screenObjectDescription,
        std::function<void(Windows::GdiGraphicsInterface&, const Gdiplus::Rect&)> customDraw) -> std::shared_ptr<Button>
    {
        return std::shared_ptr<Button>(
            new Button(area, screenObjectId, std::move(screenObjectDescription), std::move(customDraw)));
    }

    void Button::WithPosition(Gdiplus::Rect area)
    {
        this->area = area;
        this->clickableArea->WithPosition(area);
    }

    void
    Button::Draw(Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen) const
    {
        graphics.Translated(
            static_cast<Gdiplus::REAL>(this->area.X), static_cast<Gdiplus::REAL>(this->area.Y), [&graphics, this] {
                this->customDraw(graphics, this->area);
            });
        this->clickableArea->Apply(graphics, radarScreen);
    }

    Button::Button(
        Gdiplus::Rect area,
        int screenObjectId,
        std::string screenObjectDescription,
        std::function<void(Windows::GdiGraphicsInterface&, const Gdiplus::Rect&)> customDraw)
        : customDraw(std::move(customDraw)), area(area)
    {
        this->clickableArea = ClickableArea::Create(area, screenObjectId, std::move(screenObjectDescription), false);
    }
} // namespace UKControllerPlugin::Components
