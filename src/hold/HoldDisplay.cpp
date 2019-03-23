#include "pch/stdafx.h"
#include "hold/HoldDisplay.h"
#include "hold/HoldDisplayFunctions.h"
#include "hold/ManagedHold.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "graphics/GdiGraphicsInterface.h"

using UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface;
using UKControllerPlugin::Windows::GdiGraphicsInterface;

namespace UKControllerPlugin {
    namespace Hold {

        HoldDisplay::HoldDisplay(
            const EuroscopePluginLoopbackInterface & plugin,
            const ManagedHold & managedHold
        )
            : plugin(plugin),
            managedHold(managedHold),
            titleBarBrush(Gdiplus::Color(255, 153, 153)),
            backgroundBrush(Gdiplus::Color(0, 0, 0)),
            titleBarTextBrush(Gdiplus::Color(255, 255, 255)),
            fontFamily(L"EuroScope"),
            font(&fontFamily, 12, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel),
            plusFont(&fontFamily, 18, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel),
            stringFormat(Gdiplus::StringFormatFlags::StringFormatFlagsNoClip),
            dataBrush(Gdiplus::Color(0, 176, 0)),
            clearedLevelBrush(Gdiplus::Color(255, 128, 64)),
            borderPen(Gdiplus::Color(255, 255, 255), 1.5f),
            exitButtonBrush(Gdiplus::Color(0, 0, 0)),
            blockedLevelBrush(Gdiplus::HatchStyleBackwardDiagonal, Gdiplus::Color(255, 255, 255))
        {

            this->stringFormat.SetAlignment(Gdiplus::StringAlignment::StringAlignmentCenter);
            this->stringFormat.SetLineAlignment(Gdiplus::StringAlignment::StringAlignmentCenter);
        }

        HoldDisplay::HoldDisplay(const HoldDisplay & copy)
            : plugin(copy.plugin),
            managedHold(copy.managedHold),
            titleBarBrush(Gdiplus::Color(255, 153, 153)),
            backgroundBrush(Gdiplus::Color(0, 0, 0)),
            titleBarTextBrush(Gdiplus::Color(255, 255, 255)), fontFamily(L"EuroScope"),
            font(&fontFamily, 12, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel),
            plusFont(&fontFamily, 16, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel),
            stringFormat(Gdiplus::StringFormatFlags::StringFormatFlagsNoClip),
            dataBrush(Gdiplus::Color(0, 176, 0)),
            clearedLevelBrush(Gdiplus::Color(255, 128, 64)),
            borderPen(Gdiplus::Color(255, 255, 255), 1.5f),
            exitButtonBrush(Gdiplus::Color(0, 0, 0)),
            blockedLevelBrush(Gdiplus::HatchStyleBackwardDiagonal, Gdiplus::Color(255, 255, 255))
        {

        }

        void HoldDisplay::Move(const POINT & pos)
        {
            this->windowPos = pos;

            this->titleArea.X = pos.x;
            this->titleArea.Y = pos.y;

            this->minusButtonRect.X = pos.x + 5;
            this->minusButtonRect.Y = pos.y + this->buttonStartHeight;

            this->plusButtonRect.X = pos.x + 55;
            this->plusButtonRect.Y = pos.y + this->buttonStartHeight;

            this->addButtonRect.X = pos.x + 190;
            this->addButtonRect.Y = pos.y + this->buttonStartHeight;
        }


        void HoldDisplay::DrawRoundRectangle(GdiGraphicsInterface & graphics, Gdiplus::Rect & rect, UINT8 radius)
        {
            Gdiplus::GraphicsPath path;
            path.AddLine(rect.X + radius, rect.Y, rect.X + rect.Width - (radius * 2), rect.Y);
            path.AddArc(rect.X + rect.Width - (radius * 2), rect.Y, radius * 2, radius * 2, 270, 90);
            path.AddLine(rect.X + rect.Width, rect.Y + radius, rect.X + rect.Width, rect.Y + rect.Height - (radius * 2));
            path.AddArc(rect.X + rect.Width - (radius * 2), rect.Y + rect.Height - (radius * 2), radius * 2,
                radius * 2, 0, 90);
            path.AddLine(rect.X + rect.Width - (radius * 2), rect.Y + rect.Height, rect.X + radius, rect.Y + rect.Height);
            path.AddArc(rect.X, rect.Y + rect.Height - (radius * 2), radius * 2, radius * 2, 90, 90);
            path.AddLine(rect.X, rect.Y + rect.Height - (radius * 2), rect.X, rect.Y + radius);
            path.AddArc(rect.X, rect.Y, radius * 2, radius * 2, 180, 90);
            path.CloseFigure();
            graphics.DrawPath(path, this->borderPen);
        }

        void HoldDisplay::PaintWindow(GdiGraphicsInterface & graphics)
        {
            // Paint a black background
            Gdiplus::Rect borderRect = {
                this->windowPos.x,
                this->windowPos.y,
                windowWidth,
                windowHeight
            };
            graphics.FillRect(borderRect, this->backgroundBrush);

            // Title bar
            graphics.FillRect(this->titleArea, this->titleBarBrush);
            std::wstring holdName = ConvertToTchar(this->managedHold.GetHoldParameters().description);
            graphics.DrawString(
                ConvertToTchar(this->managedHold.GetHoldParameters().description),
                this->titleArea,
                this->titleBarTextBrush
            );
            graphics.DrawLine(
                this->borderPen,
                { this->titleArea.X, this->titleArea.Y + this->titleArea.Height },
                { this->titleArea.X + this->titleArea.Width, this->titleArea.Y + this->titleArea.Height }
            );

            // Exit Button
            graphics.FillRect(this->exitButtonArea, this->exitButtonBrush);
            graphics.DrawString(L"X", this->exitButtonArea, this->titleBarTextBrush);


            // Action buttons
            this->DrawRoundRectangle(graphics, minusButtonRect, 5);
            graphics.DrawString(L"-", minusButtonRect, this->titleBarTextBrush);

            this->DrawRoundRectangle(graphics, plusButtonRect, 5);
            graphics.DrawString(L"+", plusButtonRect, this->titleBarTextBrush);

            this->DrawRoundRectangle(graphics, addButtonRect, 5);
            graphics.DrawString(L"ADD", addButtonRect, this->titleBarTextBrush);

            // Hold display
            Gdiplus::Rect numbersDisplay = {
                windowPos.x,
                windowPos.y + this->dataStartHeight,
                30,
                15
            };

            // Render all the possible levels in the hold
            unsigned int levelNumber = 0;
            for (
                unsigned int i = this->managedHold.GetHoldParameters().maximum;
                i >= this->managedHold.GetHoldParameters().minimum;
                i -= 1000
            ) {
                // Don't display the level if we're hiding it
                if (levelNumber < this->numLevelsSkipped) {
                    levelNumber++;
                    continue;
                }

                // Display the restriction hash if a level is restricted
                const HoldingData & holdParameters = this->managedHold.GetHoldParameters();
                for (
                    std::set<std::unique_ptr<AbstractHoldLevelRestriction>>::const_iterator it
                    = holdParameters.restrictions.cbegin();
                    it != holdParameters.restrictions.cend();
                    ++it
                    ) {
                    if ((*it)->LevelRestricted(i)) {
                        Gdiplus::Rect restrictedRect = {
                            0,
                            numbersDisplay.Y,
                            windowWidth,
                            this->lineHeight
                        };
                        graphics.FillRect (
                            restrictedRect,
                            this->blockedLevelBrush
                        );
                    }
                }

                graphics.DrawString(
                    GetLevelDisplayString(i),
                    numbersDisplay,
                    this->titleBarTextBrush
                );

                numbersDisplay.Y = numbersDisplay.Y + this->lineHeight;
                levelNumber++;
            }


            // Rects for rendering the actual data
            Gdiplus::Rect callsignDisplay = {
                this->windowPos.x + 35,
                this->dataStartHeight,
                this->windowPos.y + 90,
                this->lineHeight
            };

            Gdiplus::Rect actualLevelDisplay = {
                this->windowPos.x + 130,
                this->dataStartHeight,
                this->windowPos.y + 30,
                this->lineHeight
            };

            Gdiplus::Rect clearedLevelDisplay = {
                this->windowPos.x + 165,
                this->dataStartHeight,
                this->windowPos.y + 30,
                this->lineHeight
            };

            Gdiplus::Rect timeInHoldDisplay = {
                this->windowPos.x + 200,
                this->dataStartHeight,
                this->windowPos.y + 30,
                this->lineHeight
            };

            // Loop the aircraft in the hold and render them
            for (
                ManagedHold::ManagedHoldAircraft::const_iterator it = this->managedHold.cbegin();
                it != this->managedHold.cend();
                ++it
            ) {
                unsigned int occupied = GetOccupiedLevel(it->reportedLevel, it->verticalSpeed);
                unsigned int displayRow = GetDisplayRow(this->managedHold.GetHoldParameters().maximum, occupied);

                // Dont render any aircraft where a level is skipped
                if (displayRow < this->numLevelsSkipped) {
                    continue;
                }

                callsignDisplay.Y = this->dataStartHeight + (this->lineHeight * displayRow);
                actualLevelDisplay.Y = this->dataStartHeight + (this->lineHeight * displayRow);
                clearedLevelDisplay.Y = this->dataStartHeight + (this->lineHeight * displayRow);
                timeInHoldDisplay.Y = this->dataStartHeight + (this->lineHeight * displayRow);

                // The callsign display
                std::wstring callsign = ConvertToTchar(it->callsign);
                graphics.DrawString(
                    callsign,
                    callsignDisplay,
                    this->dataBrush
                );
                
                // Reported level
                graphics.DrawString(
                    GetLevelDisplayString(it->reportedLevel),
                    actualLevelDisplay,
                    this->dataBrush
                );


                // Cleared level
                graphics.DrawString(
                    GetLevelDisplayString(it->clearedLevel),
                    clearedLevelDisplay,
                    this->clearedLevelBrush
                );

                // Time in hold
                std::wstring timeString = GetTimeInHoldDisplayString(it->entryTime);
                graphics.DrawString(
                    timeString,
                    timeInHoldDisplay,
                    this->dataBrush
                );
            }

            // Border around whole thing, draw this last
            graphics.DrawRect(
                borderRect,
                this->borderPen
            );
        }

        /*
            The real callback used for the window messages
        */
        //LRESULT HoldDisplay::_WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
        //{
        //    switch (msg)
        //    {
        //        case WM_NCHITTEST: {
        //            LRESULT hit = DefWindowProc(hwnd, msg, wParam, lParam);
        //            POINT mousePoint = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
        //            ScreenToClient(this->selfHandle, &mousePoint);
        //            Gdiplus::REAL xpos = (Gdiplus::REAL) GET_X_LPARAM(lParam);
        //            Gdiplus::REAL ypos = (Gdiplus::REAL) GET_Y_LPARAM(lParam);

        //            return this->titleArea.Contains((Gdiplus::REAL) mousePoint.x, (Gdiplus::REAL) mousePoint.y) 
        //                ? HTCAPTION 
        //                : HTCLIENT;
        //        }
        //        case WM_LBUTTONUP: {
        //            POINT p;
        //            GetCursorPos(&p);
        //            ScreenToClient(hwnd, &p);

        //            if (this->plusButtonRect.Contains(p.x, p.y)) {
        //                if (numLevelsSkipped > 0) this->numLevelsSkipped--;
        //                InvalidateRgn(this->selfHandle, NULL, TRUE);
        //            } else if (this->minusButtonRect.Contains(p.x, p.y)) {
        //                const unsigned int maxLevelsSkippable = (this->managedHold.GetHoldParameters().maximum -
        //                    this->managedHold.GetHoldParameters().minimum) / 1000;
        //                if (maxLevelsSkippable != this->numLevelsSkipped) numLevelsSkipped++;
        //                InvalidateRgn(this->selfHandle, NULL, TRUE);
        //            }
        //            return TRUE;
        //        }
        //        case WM_PAINT: {
        //            PAINTSTRUCT ps;
        //            HDC hdc = BeginPaint(hwnd, &ps);
        //            this->PaintWindow(hdc);
        //            EndPaint(hwnd, &ps);
        //            return TRUE;
        //        }
        //        case WM_CLOSE:
        //            DestroyWindow(hwnd);
        //            break;
        //        case WM_DESTROY:
        //            break;
        //        default:
        //            return DefWindowProc(hwnd, msg, wParam, lParam);
        //    }
        //    return FALSE;
        //}
    }  // namespace Hold
}  // namespace UKControllerPlugin
