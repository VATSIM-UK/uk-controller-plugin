#include "pch/stdafx.h"
#include "releases/DepartureReleaseRequestView.h"
#include "releases/DepartureReleaseCountdownColours.h"
#include "releases/DepartureReleaseColours.h"
#include "releases/DepartureReleaseRequest.h"
#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionCollection.h"
#include "graphics/GdiGraphicsInterface.h"
#include "releases/DepartureReleaseEventHandler.h"
#include "time/SystemClock.h"
#include "helper/HelperFunctions.h"
#include "timer/TimerDisplay.h"

namespace UKControllerPlugin {
    namespace Releases {

        DepartureReleaseRequestView::DepartureReleaseRequestView(
            const DepartureReleaseEventHandler& handler,
            const Controller::ControllerPositionCollection& controllers,
            const int screenObjectId
        ) : handler(handler), screenObjectId(screenObjectId), backgroundBrush(Gdiplus::Color(58, 57, 58)),
            titleBarTextBrush(Gdiplus::Color(227, 227, 227)), controllers(controllers)
        {}

        void DepartureReleaseRequestView::DisplayFor(std::chrono::seconds seconds)
        {
            displayUntil = Time::TimeNow() + seconds;
        }

        void DepartureReleaseRequestView::MoveAllInstances(POINT position)
        {
            displayPosition = position;
        }

        bool DepartureReleaseRequestView::ShouldDisplay()
        {
            return Time::TimeNow() < displayUntil;
        }

        POINT DepartureReleaseRequestView::GetCoordinates()
        {
            return displayPosition;
        }

        bool DepartureReleaseRequestView::IsVisible() const
        {
            return ShouldDisplay();
        }

        void DepartureReleaseRequestView::Render(
            Windows::GdiGraphicsInterface& graphics,
            Euroscope::EuroscopeRadarLoopbackInterface& radarScreen
        )
        {
            const std::set<std::shared_ptr<DepartureReleaseRequest>>& releasesToDisplay =
                this->handler.GetReleasesToDisplay();

            // Fill the background
            const LONG numberOfReleases = releasesToDisplay.size();
            graphics.FillRect(
                RECT{
                    displayPosition.x,
                    displayPosition.y,
                    displayPosition.x + this->baseWidth,
                    displayPosition.y + this->baseHeight +
                    (numberOfReleases * this->heightPerRelease) + (2 * this->textInsets) // Add a bit on the bottom
                },
                this->backgroundBrush
            );

            // Add the headers / column titles
            graphics.DrawString(
                L"Controller",
                RECT{
                    displayPosition.x + this->textInsets,
                    displayPosition.y + this->textInsets,
                    displayPosition.x + this->textInsets + 100,
                    displayPosition.y + this->textInsets + this->heightPerRelease,
                },
                this->titleBarTextBrush
            );

            graphics.DrawString(
                L"Rlsd",
                RECT{
                    displayPosition.x + this->textInsets + 110,
                    displayPosition.y + this->textInsets,
                    displayPosition.x + this->textInsets + 160,
                    displayPosition.y + this->textInsets + this->heightPerRelease,
                },
                this->titleBarTextBrush
            );

            graphics.DrawString(
                L"Exp",
                RECT{
                    displayPosition.x + this->textInsets + 170,
                    displayPosition.y + this->textInsets,
                    displayPosition.x + this->textInsets + 210,
                    displayPosition.y + this->textInsets + this->heightPerRelease,
                },
                this->titleBarTextBrush
            );

            // Add the release data
            int rowMultiplier = 1;
            for (auto release : releasesToDisplay) {

                std::wstring controller = HelperFunctions::ConvertToWideString(
                    this->controllers.FetchPositionById(release->TargetController())->GetCallsign()
                );

                auto controllerColour = this->GetControllerColour(release);
                auto brushForRow = Gdiplus::SolidBrush(
                    Gdiplus::Color(GetRValue(controllerColour), GetGValue(controllerColour),
                                   GetBValue(controllerColour))
                );

                // Controller
                graphics.DrawString(
                    controller,
                    RECT{
                        displayPosition.x + this->textInsets,
                        displayPosition.y + this->textInsets + (rowMultiplier * this->heightPerRelease),
                        displayPosition.x + this->textInsets + 100,
                        displayPosition.y + this->textInsets + ((rowMultiplier + 1) * this->heightPerRelease),
                    },
                    brushForRow
                );

                if (!release->Approved()) {
                    return;
                }

                auto timerColour = this->GetTimerColour(release);
                auto brushForTimer = Gdiplus::SolidBrush(
                    Gdiplus::Color(GetRValue(timerColour), GetGValue(timerColour), GetBValue(timerColour))
                );

                // Released at or expires at time
                if (release->AwaitingReleasedTime()) {
                    graphics.DrawString(
                        Timer::GetTimerDisplayWide(release->ReleasedAtTime()),
                        RECT{
                            displayPosition.x + this->textInsets + 110,
                            displayPosition.y + this->textInsets + (rowMultiplier * this->heightPerRelease),
                            displayPosition.x + this->textInsets + 160,
                            displayPosition.y + this->textInsets + ((rowMultiplier + 1) * this->heightPerRelease),
                        },
                        brushForTimer
                    );
                } else if (!release->ApprovedWithNoExpiry()) {
                    graphics.DrawString(
                        Timer::GetTimerDisplayWide(release->ReleaseExpiryTime()),
                        RECT{
                            displayPosition.x + this->textInsets + 170,
                            displayPosition.y + this->textInsets + (rowMultiplier * this->heightPerRelease),
                            displayPosition.x + this->textInsets + 210,
                            displayPosition.y + this->textInsets + ((rowMultiplier + 1) * this->heightPerRelease),
                        },
                        brushForTimer
                    );
                }

                rowMultiplier++;
            }
        }

        /*
         * Set the colour of the controller based on the overall status of the release.
         */
        COLORREF DepartureReleaseRequestView::GetControllerColour(std::shared_ptr<DepartureReleaseRequest> request)
        {
            if (request->Rejected()) {
                return statusIndicatorReleaseRejected;
            }

            if (request->Approved()) {
                return statusIndicatorReleased;
            }

            if (request->RequestExpired()) {
                return statusIndicatorReleaseAcknowledged;
            }

            if (request->Acknowledged()) {
                return statusIndicatorReleased;
            }

            return statusIndicatorReleasePending;
        }

        /*
         * Get the timer colour depending on the various times used in releases.
         */
        COLORREF DepartureReleaseRequestView::GetTimerColour(std::shared_ptr<DepartureReleaseRequest> request)
        {
            return request->AwaitingReleasedTime()
                       ? statusIndicatorReleasedAwaitingTime
                       : TimeUntilExpiryColour(request->ReleaseExpiryTime());
        }
    } // namespace Releases
} // namespace UKControllerPlugin
