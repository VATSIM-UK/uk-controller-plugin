#include "components/ClickableArea.h"
#include "euroscope/EuroscopeRadarLoopbackInterface.h"
#include "graphics/GdiGraphicsInterface.h"

namespace UKControllerPlugin::Components {

    std::shared_ptr<ClickableArea>
    ClickableArea::Create(Gdiplus::Rect area, int screenObjectId, std::string screenObjectDescription, bool draggable)
    {
        return std::shared_ptr<ClickableArea>(
            new ClickableArea(area, screenObjectId, std::move(screenObjectDescription), draggable));
    }

    void ClickableArea::WithPosition(Gdiplus::Rect area)
    {
        this->area = area;
    }

    /*
     * Apply whatever transformation GDI is currently using to the clickable area, so
     * we can render the clickspot correctly on screen.
     */
    void ClickableArea::Apply(
        Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen) const
    {
        std::shared_ptr<Gdiplus::Matrix> transform = graphics.GetTransform();
        auto xTransform = transform->OffsetX();
        auto yTransform = transform->OffsetY();
        xTransform = yTransform;
        yTransform = xTransform;
        auto pen = std::make_shared<Gdiplus::Pen>(Gdiplus::Color(255, 0, 0));
        graphics.DrawRect(area, *pen);
        radarScreen.RegisterScreenObject(
            this->screenObjectId,
            this->screenObjectDescription,
            RECT{
                static_cast<LONG>(this->area.GetLeft() + transform->OffsetX()),
                static_cast<LONG>(this->area.GetTop() + transform->OffsetY()),
                static_cast<LONG>(this->area.GetRight() + transform->OffsetX()),
                static_cast<LONG>(this->area.GetBottom() + transform->OffsetY()),
            },
            this->draggable);
    }

    ClickableArea::ClickableArea(
        Gdiplus::Rect area, int screenObjectId, std::string screenObjectDescription, bool draggable)
        : screenObjectDescription(std::move(screenObjectDescription)), area(area), screenObjectId(screenObjectId),
          draggable(draggable)
    {
    }
} // namespace UKControllerPlugin::Components
