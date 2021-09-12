#include "DepartureReleaseColours.h"
#include "DepartureReleaseCountdownColours.h"
#include "DepartureReleaseEventHandler.h"
#include "DepartureReleaseRequest.h"
#include "DepartureReleaseRequestView.h"
#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionCollection.h"
#include "graphics/GdiGraphicsInterface.h"
#include "helper/HelperFunctions.h"
#include "time/SystemClock.h"
#include "timer/TimerDisplay.h"

namespace UKControllerPlugin::Releases {

    DepartureReleaseRequestView::DepartureReleaseRequestView(
        const DepartureReleaseEventHandler& handler, const Controller::ControllerPositionCollection& controllers)
        : handler(handler), controllers(controllers), backgroundBrush(backgroundColour), titleBarTextBrush(textColour)
    {
    }

    void DepartureReleaseRequestView::DisplayFor(std::chrono::seconds seconds)
    {
        displayUntil = Time::TimeNow() + seconds;
    }

    void DepartureReleaseRequestView::MoveAllInstances(POINT position)
    {
        displayPosition = position;
    }

    auto DepartureReleaseRequestView::ShouldDisplay() -> bool
    {
        return Time::TimeNow() < displayUntil;
    }

    auto DepartureReleaseRequestView::GetCoordinates() -> POINT
    {
        return displayPosition;
    }

    auto DepartureReleaseRequestView::IsVisible() const -> bool
    {
        return ShouldDisplay();
    }

    void DepartureReleaseRequestView::Render(
        Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen)
    {
        const std::set<std::shared_ptr<DepartureReleaseRequest>>& releasesToDisplay =
            this->handler.GetReleasesToDisplay();

        // Fill the background
        const LONG numberOfReleases = static_cast<LONG>(releasesToDisplay.size());
        graphics.FillRect(
            RECT{
                displayPosition.x,
                displayPosition.y,
                displayPosition.x + BASE_WIDTH,
                displayPosition.y + BASE_HEIGHT + (numberOfReleases * HEIGHT_PER_RELEASE) +
                    (2 * TEXT_INSETS) // Add a bit on the bottom
            },
            this->backgroundBrush);

        // Add the headers / column titles
        graphics.DrawString(L"Controller", GetControllerColumnEntryDimensions(0), this->titleBarTextBrush);

        graphics.DrawString(L"Rlsd", GetReleasedColumnEntryDimensions(0), this->titleBarTextBrush);

        graphics.DrawString(L"Exp", GetExpiredColumnEntryDimensions(0), this->titleBarTextBrush);

        // Add the release data
        int rowMultiplier = 1;
        for (const auto& release : releasesToDisplay) {

            const std::wstring controller = HelperFunctions::ConvertToWideString(
                this->controllers.FetchPositionById(release->TargetController())->GetCallsign());

            const auto controllerColour = GetControllerColour(release);
            auto brushForRow = Gdiplus::SolidBrush(
                Gdiplus::Color(GetRValue(controllerColour), GetGValue(controllerColour), GetBValue(controllerColour)));

            // Controller
            graphics.DrawString(controller, GetControllerColumnEntryDimensions(rowMultiplier), brushForRow);

            if (!release->Approved()) {
                rowMultiplier++;
                continue;
            }

            const auto timerColour = GetTimerColour(release);
            auto brushForTimer = Gdiplus::SolidBrush(
                Gdiplus::Color(GetRValue(timerColour), GetGValue(timerColour), GetBValue(timerColour)));

            // Released at or expires at time
            if (release->AwaitingReleasedTime()) {
                graphics.DrawString(
                    Timer::GetTimerDisplayWide(release->ReleasedAtTime()),
                    GetReleasedColumnEntryDimensions(rowMultiplier),
                    brushForTimer);
            } else if (!release->ApprovedWithNoExpiry()) {
                graphics.DrawString(
                    Timer::GetTimerDisplayWide(release->ReleaseExpiryTime()),
                    GetExpiredColumnEntryDimensions(rowMultiplier),
                    brushForTimer);
            }

            rowMultiplier++;
        }
    }

    /*
     * Set the colour of the controller based on the overall status of the release.
     */
    auto DepartureReleaseRequestView::GetControllerColour(const std::shared_ptr<DepartureReleaseRequest>& request)
        -> COLORREF
    {
        if (request->Rejected()) {
            return statusIndicatorReleaseRejected;
        }

        if (request->Approved()) {
            return statusIndicatorReleased;
        }

        if (request->RequestExpired()) {
            return statusIndicatorReleaseExpired;
        }

        if (request->Acknowledged()) {
            return statusIndicatorReleaseAcknowledged;
        }

        return statusIndicatorReleasePending;
    }

    /*
     * Get the timer colour depending on the various times used in releases.
     */
    auto DepartureReleaseRequestView::GetTimerColour(const std::shared_ptr<DepartureReleaseRequest>& request)
        -> COLORREF
    {
        return request->AwaitingReleasedTime() ? statusIndicatorReleasedAwaitingTime
                                               : TimeUntilExpiryColour(request->ReleaseExpiryTime());
    }

    auto DepartureReleaseRequestView::GetControllerColumnEntryDimensions(int rowMultiplier) -> RECT
    {
        return {
            displayPosition.x + TEXT_INSETS,
            displayPosition.y + TEXT_INSETS + (rowMultiplier * HEIGHT_PER_RELEASE),
            displayPosition.x + TEXT_INSETS + CONTROLLER_COLUMN_WIDTH,
            displayPosition.y + TEXT_INSETS + ((rowMultiplier + 1) * HEIGHT_PER_RELEASE),
        };
    }

    auto DepartureReleaseRequestView::GetReleasedColumnEntryDimensions(int rowMultiplier) -> RECT
    {
        return {
            displayPosition.x + TEXT_INSETS + CONTROLLER_COLUMN_WIDTH + GAP_BETWEEN_COLUMNS,
            displayPosition.y + TEXT_INSETS + (rowMultiplier * HEIGHT_PER_RELEASE),
            displayPosition.x + TEXT_INSETS + CONTROLLER_COLUMN_WIDTH + GAP_BETWEEN_COLUMNS + RELEASED_COLUMN_WIDTH,
            displayPosition.y + TEXT_INSETS + ((rowMultiplier + 1) * HEIGHT_PER_RELEASE),
        };
    }

    auto DepartureReleaseRequestView::GetExpiredColumnEntryDimensions(int rowMultiplier) -> RECT
    {
        return {
            displayPosition.x + TEXT_INSETS + CONTROLLER_COLUMN_WIDTH + GAP_BETWEEN_COLUMNS + RELEASED_COLUMN_WIDTH +
                GAP_BETWEEN_COLUMNS,
            displayPosition.y + TEXT_INSETS + (rowMultiplier * HEIGHT_PER_RELEASE),
            displayPosition.x + TEXT_INSETS + CONTROLLER_COLUMN_WIDTH + GAP_BETWEEN_COLUMNS + RELEASED_COLUMN_WIDTH +
                GAP_BETWEEN_COLUMNS + EXPIRED_COLUMN_WIDTH,
            displayPosition.y + TEXT_INSETS + ((rowMultiplier + 1) * HEIGHT_PER_RELEASE),
        };
    }
} // namespace UKControllerPlugin::Releases
