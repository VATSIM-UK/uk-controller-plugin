#include "pch/stdafx.h"
#include "components/Button.h"
#include "components/ClickableArea.h"
#include "euroscope/EuroscopeRadarLoopbackInterface.h"
#include "graphics/GdiGraphicsInterface.h"

namespace UKControllerPlugin::Components {

    std::shared_ptr<Button> Button::Create(
        Gdiplus::Rect area,
        int screenObjectId,
        std::string screenObjectDescription,
        std::function<void(Windows::GdiGraphicsInterface&, const Gdiplus::Rect&)>
        customDraw
    )
    {
        return std::shared_ptr<
            Button>(new Button(area, screenObjectId, std::move(screenObjectDescription), std::move(customDraw))
        );
    }

    void Button::WithPosition(Gdiplus::Rect area)
    {
        this->area = area;
        this->clickableArea->WithPosition(area);
    }

    void Button::Draw(
        Windows::GdiGraphicsInterface& graphics,
        Euroscope::EuroscopeRadarLoopbackInterface& radarScreen
    ) const
    {
        graphics.Translated(
            static_cast<Gdiplus::REAL>(this->area.X),
            static_cast<Gdiplus::REAL>(this->area.Y),
            [&graphics, this]
            {
                this->customDraw(graphics, this->area);
            }
        );
        this->clickableArea->Apply(graphics, radarScreen);
    }

    Button::Button(
        Gdiplus::Rect area,
        int screenObjectId,
        std::string screenObjectDescription,
        std::function<void(Windows::GdiGraphicsInterface&, const Gdiplus::Rect&)> customDraw):
        screenObjectDescription(screenObjectDescription), customDraw(std::move(customDraw)), area(area),
        screenObjectId(screenObjectId)
    {
        this->clickableArea = ClickableArea::Create(area, screenObjectId, screenObjectDescription, false);
    }

} // namespace UKControllerPlugin::Components
