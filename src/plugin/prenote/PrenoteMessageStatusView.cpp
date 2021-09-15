#include "ComparePrenoteMessages.h"
#include "PrenoteMessage.h"
#include "PrenoteMessageCollection.h"
#include "PrenoteMessageStatusView.h"
#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionCollection.h"
#include "graphics/GdiGraphicsInterface.h"
#include "helper/HelperFunctions.h"
#include "time/SystemClock.h"

namespace UKControllerPlugin::Prenote {

    PrenoteMessageStatusView::PrenoteMessageStatusView(
        std::shared_ptr<PrenoteMessageCollection> prenotes, const Controller::ControllerPositionCollection& controllers)
        : prenotes(std::move(prenotes)), controllers(controllers),
          backgroundBrush(Gdiplus::Color(BACKGROUND_BRUSH_R_B, BACKGROUND_BRUSH_G, BACKGROUND_BRUSH_R_B)),
          titleBarTextBrush(Gdiplus::Color(TITLE_BAR_TEXT_BRUSH, TITLE_BAR_TEXT_BRUSH, TITLE_BAR_TEXT_BRUSH))
    {
    }

    void PrenoteMessageStatusView::DisplayFor(const std::string& callsign, std::chrono::seconds seconds)
    {
        displayingFor = callsign;
        displayUntil = Time::TimeNow() + seconds;
    }

    void PrenoteMessageStatusView::MoveAllInstances(POINT position)
    {
        displayPosition = position;
    }

    auto PrenoteMessageStatusView::ShouldDisplay() -> bool
    {
        return Time::TimeNow() < displayUntil;
    }

    auto PrenoteMessageStatusView::IsVisible() const -> bool
    {
        return ShouldDisplay();
    }

    void PrenoteMessageStatusView::Render(
        Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen)
    {
        std::set<std::shared_ptr<PrenoteMessage>, ComparePrenoteMessages> messagesToDisplay;
        this->prenotes->Iterate([&messagesToDisplay](const std::shared_ptr<PrenoteMessage>& prenote) {
            if (prenote->GetCallsign() == displayingFor) {
                messagesToDisplay.insert(prenote);
            }
        });

        // Fill the background
        const LONG numberOfPrenotes = static_cast<LONG>(messagesToDisplay.size());
        graphics.FillRect(
            RECT{
                displayPosition.x,
                displayPosition.y,
                displayPosition.x + this->baseWidth,
                displayPosition.y + this->baseHeight + (numberOfPrenotes * this->heighPerPrenote) +
                    (2 * this->textInsets) // Add a bit on the bottom
            },
            this->backgroundBrush);

        // Add the headers / column titles
        graphics.DrawString(L"Controller", this->GetControllerColumnEntryDimensions(0), this->titleBarTextBrush);
        graphics.DrawString(L"Ack", GetAcknowledgedColumnEntryDimensions(0), this->titleBarTextBrush);

        // Add the prenote data
        int rowMultiplier = 1;
        for (const auto& message : messagesToDisplay) {

            const std::wstring controller = HelperFunctions::ConvertToWideString(
                this->controllers.FetchPositionById(message->GetTargetControllerId())->GetCallsign());

            const auto rowColour = GetTextColourForMessage(message);
            auto brushForRow =
                Gdiplus::SolidBrush(Gdiplus::Color(GetRValue(rowColour), GetGValue(rowColour), GetBValue(rowColour)));

            // Controller
            graphics.DrawString(controller, this->GetControllerColumnEntryDimensions(rowMultiplier), brushForRow);

            // Is acknowledged
            graphics.DrawString(
                HelperFunctions::ConvertToWideString(message->IsAcknowledged() ? "Yes" : "No"),
                GetAcknowledgedColumnEntryDimensions(rowMultiplier),
                brushForRow);

            rowMultiplier++;
        }
    }

    /*
     * Set the colour of the message row based on whether the prenote message has been acknowledged
     */
    auto PrenoteMessageStatusView::GetTextColourForMessage(const std::shared_ptr<PrenoteMessage>& message) -> COLORREF
    {
        return message->IsAcknowledged() ? RGB(36, 138, 73) : RGB(227, 227, 227);
    }

    auto PrenoteMessageStatusView::GetControllerColumnEntryDimensions(int rowMultiplier) const -> RECT
    {
        return RECT{
            displayPosition.x + this->textInsets,
            displayPosition.y + this->textInsets + (rowMultiplier * this->heighPerPrenote),
            displayPosition.x + this->textInsets + CONTROLLER_COLUMN_WIDTH,
            displayPosition.y + this->textInsets + ((rowMultiplier + 1) * this->heighPerPrenote),
        };
    }

    auto PrenoteMessageStatusView::GetAcknowledgedColumnEntryDimensions(int rowMultiplier) const -> RECT
    {
        return RECT{
            displayPosition.x + this->textInsets + CONTROLLER_COLUMN_WIDTH + GAP_BETWEEN_COLUMNS,
            displayPosition.y + this->textInsets + (rowMultiplier * this->heighPerPrenote),
            displayPosition.x + this->textInsets + CONTROLLER_COLUMN_WIDTH + GAP_BETWEEN_COLUMNS +
                ACKNOWLEDGED_COLUMN_WIDTH,
            displayPosition.y + this->textInsets + ((rowMultiplier + 1) * this->heighPerPrenote),
        };
    }

    auto PrenoteMessageStatusView::GetDisplayPosition() -> POINT
    {
        return displayPosition;
    }

    auto PrenoteMessageStatusView::GetDisplayUntil() -> std::chrono::system_clock::time_point
    {
        return displayUntil;
    }

    auto PrenoteMessageStatusView::GetDisplayingFor() -> std::string
    {
        return displayingFor;
    }
} // namespace UKControllerPlugin::Prenote
