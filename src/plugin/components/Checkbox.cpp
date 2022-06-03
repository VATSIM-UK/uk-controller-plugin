#include "Checkbox.h"
#include "ClickableArea.h"
#include "graphics/FontManager.h"
#include "graphics/GdiGraphicsInterface.h"
#include "graphics/StringFormatManager.h"

namespace UKControllerPlugin::Components {

    Checkbox::Checkbox(std::wstring label, POINT position, int screenObjectId, std::string name)
        : label(std::move(label)), area(position.x, position.y, DIMENSIONS, DIMENSIONS),
          labelArea(area.GetRight() + 10, position.y, 250, DIMENSIONS),
          pen(std::make_shared<Gdiplus::Pen>(Gdiplus::Color(255, 255, 255))),
          brush(std::make_shared<Gdiplus::SolidBrush>(Gdiplus::Color(255, 255, 255))),
          clickableArea(ClickableArea::Create(
              Gdiplus::Rect(area.GetLeft(), area.GetTop(), labelArea.GetRight() - area.GetLeft(), labelArea.Height),
              screenObjectId,
              name,
              false)),
          font(Graphics::FontManager::Instance().GetDefault()),
          stringFormat(Graphics::StringFormatManager::Instance().GetLeftAlign())
    {
    }

    auto Checkbox::Create(std::wstring label, POINT position, int screenObjectId, std::string name)
        -> std::shared_ptr<Checkbox>
    {
        return std::shared_ptr<Checkbox>(new Checkbox(label, position, screenObjectId, name));
    }

    auto Checkbox::Checked(bool checked) -> std::shared_ptr<Checkbox>
    {
        this->checked = checked;
        return shared_from_this();
    }

    void Checkbox::Draw(
        Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen) const
    {
        // Draw the border
        graphics.DrawRect(area, *pen);

        // Draw the checkmark if checked
        if (checked) {
            graphics.DrawLine(
                *pen, Gdiplus::Point{area.GetLeft(), area.GetTop()}, Gdiplus::Point{area.GetRight(), area.GetBottom()});
            graphics.DrawLine(
                *pen, Gdiplus::Point{area.GetLeft(), area.GetBottom()}, Gdiplus::Point{area.GetRight(), area.GetTop()});
        }

        // Clickspot
        clickableArea->Apply(graphics, radarScreen);

        // Label
        graphics.DrawString(label, labelArea, *brush, stringFormat, font);
    }
} // namespace UKControllerPlugin::Components
