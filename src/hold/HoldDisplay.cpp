#include "pch/stdafx.h"
#include "hold/HoldDisplay.h"
#include "hold/HoldDisplayFunctions.h"
#include "hold/HoldManager.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "euroscope/EuroScopeRadarLoopbackInterface.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"
#include "graphics/GdiGraphicsInterface.h"
#include "euroscope/UserSetting.h"

using UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface;
using UKControllerPlugin::Windows::GdiGraphicsInterface;
using UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface;
using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;
using UKControllerPlugin::Hold::HoldManager;
using UKControllerPlugin::Euroscope::UserSetting;

namespace UKControllerPlugin {
    namespace Hold {

        HoldDisplay::HoldDisplay(
            const EuroscopePluginLoopbackInterface & plugin,
            HoldManager & holdManager,
            const UKControllerPlugin::Navaids::Navaid& navaid,
            const std::set<HoldingData>& publishedHolds
        )
            : plugin(plugin),
            holdManager(holdManager),
            publishedHolds(publishedHolds),
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
            blockedLevelBrush(Gdiplus::HatchStyleBackwardDiagonal, Gdiplus::Color(255, 255, 255)),
            dataStartHeight(0),
            informationClickRect({}),
            minimiseClickRect({}),
            navaid(navaid)
        {
            this->stringFormat.SetAlignment(Gdiplus::StringAlignment::StringAlignmentCenter);
            this->stringFormat.SetLineAlignment(Gdiplus::StringAlignment::StringAlignmentCenter);

            this->windowHeight = this->dataStartOffset + ((((this->maximumLevel - this->minLevel) / 1000) + 1) * this->lineHeight);
            this->maxWindowHeight = this->dataStartOffset + ((((this->maximumLevel - this->minLevel) / 1000) + 1) * this->lineHeight);
            this->Move(this->windowPos);
        }

        /*
            A button has been clicked
        */
        void HoldDisplay::ButtonClicked(std::string button)
        {
            if (button == "plus") {
                this->maximumLevel += 1000;
            } else if (button == "minus") {
                if (this->maximumLevel == this->minLevel) {
                    return;
                }

                this->maximumLevel -= 1000;
            } else if (button == "allLevels") {
                this->dataStartOffset + ((((this->maximumLevel - this->minLevel) / 1000) + 1) * this->lineHeight);
                this->maximumLevel = 15000;
            } else if (button == "add") {
                std::shared_ptr<EuroScopeCFlightPlanInterface> fp = this->plugin.GetSelectedFlightplan();
                std::shared_ptr<EuroScopeCRadarTargetInterface> rt = this->plugin.GetSelectedRadarTarget();

                if (!fp || !rt) {
                    LogWarning(
                        "Tried to add aircraft to hold " + this->navaid.identifier +
                        " but none selected"
                    );
                    return;
                }

                this->holdManager.AssignAircraftToHold(
                    *fp,
                    this->navaid.identifier
                );
            } else if (button == "minimise") {
                this->minimised = !this->minimised;
            } else if (button == "information") {
                this->showHoldInformation = !this->showHoldInformation;
            }
        }

        /*
            Get the data start height
        */
        INT HoldDisplay::GetDataStartHeight(void) const
        {
            return this->dataStartHeight;
        }

        /*
            Return the position of the window
        */
        POINT HoldDisplay::GetDisplayPos(void) const
        {
            return this->windowPos;
        }

        /*
            Return the renderable area for the title bar
        */
        Gdiplus::Rect HoldDisplay::GetTitleArea(void) const
        {
            return this->titleArea;
        }

        /*
            Return the click area for the title bar
        */
        RECT HoldDisplay::GetTitleClickArea(void) const
        {
            return this->titleRect;
        }

        /*
            Get renderable area for the minimise button
        */
        Gdiplus::Rect HoldDisplay::GetMinimiseArea(void) const
        {
            return this->minimiseButtonArea;
        }

        /*
            Get clickable area for the minimise button
        */
        RECT HoldDisplay::GetMinimiseClickArea(void) const
        {
            return this->minimiseClickRect;
        }

        /*
            Return the renderable area for the information button
        */
        Gdiplus::Rect HoldDisplay::GetInformationArea(void) const
        {
            return this->informationButtonArea;
        }

        /*
            Return the clickable area for the information button
        */
        RECT HoldDisplay::GetInformationClickArea(void) const
        {
            return this->informationClickRect;
        }

        /*
            Return the renderable area for the plus button
        */
        Gdiplus::Rect HoldDisplay::GetPlusArea(void) const
        {
            return this->plusButtonRect;
        }

        /*
            Return the clickable area for the plus button
        */
        RECT HoldDisplay::GetPlusClickArea(void) const
        {
            return this->plusButtonClickRect;
        }

        /*
            Return the renderable area for the minus button
        */
        Gdiplus::Rect HoldDisplay::GetMinusArea(void) const
        {
            return this->minusButtonRect;
        }

        /*
            Return the clickable area for the minus button
        */
        RECT HoldDisplay::GetMinusClickArea(void) const
        {
            return this->minusButtonClickRect;
        }

        /*
            Return the renderable area for the all levels button
        */
        Gdiplus::Rect HoldDisplay::GetAllArea(void) const
        {
            return this->allButtonRect;
        }

        /*
            Return the clickable area for the all levels button
        */
        RECT HoldDisplay::GetAllClickArea(void) const
        {
            return this->allButtonClickRect;
        }

        /*
            Return the renderable area for the add button
        */
        Gdiplus::Rect HoldDisplay::GetAddArea(void) const
        {
            return this->addButtonRect;
        }

        /*
            Return the clickable area for the add button
        */
        RECT HoldDisplay::GetAddClickArea(void) const
        {
            return this->addButtonClickRect;
        }

        /*
            Maps the holding aircraft to their occupied levels
        */
        std::map<int, std::set<std::shared_ptr<HoldingAircraft>, CompareHoldingAircraft>>
            HoldDisplay::MapAircraftToLevels(
                const std::set<std::shared_ptr<HoldingAircraft>, CompareHoldingAircraft>& aircraft
        ) const {

            std::map<int, std::set<std::shared_ptr<HoldingAircraft>, CompareHoldingAircraft>> levelMap;
            std::shared_ptr<EuroScopeCRadarTargetInterface> rt;

            for (
                std::set<std::shared_ptr<HoldingAircraft>, CompareHoldingAircraft>::const_iterator it = aircraft.cbegin();
                it != aircraft.cend();
                ++it
            ) {
                try {
                    rt = this->plugin.GetRadarTargetForCallsign((*it)->GetCallsign());

                    // If the aircraft is above the displaying levels of the hold, dont map
                    int occupied = GetOccupiedLevel(rt->GetFlightLevel(), rt->GetVerticalSpeed());
                    if (occupied > this->maximumLevel || occupied < this->minLevel) {
                        continue;
                    }

                    levelMap[occupied].insert(*it);
                }
                catch (std::invalid_argument) {
                    // Cant display, dont have the data
                }
            }

            return levelMap;
        }

        /*
            Return the height of the window
        */
        int HoldDisplay::GetWindowHeight(void) const
        {
            return this->windowHeight;
        }

        /*
            Return whether or not the display is minimised.
        */
        bool HoldDisplay::IsMinimised(void) const
        {
            return this->minimised;
        }

        /*
            Return whether or not the display is in informtion mode
        */
        bool HoldDisplay::IsInInformationMode(void) const
        {
            return this->showHoldInformation;
        }

        /*
            Load data from an ASR
        */
        void HoldDisplay::LoadDataFromAsr(UserSetting & userSetting)
        {
            this->minimised = userSetting.GetBooleanEntry(
                "hold" + this->navaid.identifier + "Minimised",
                false
            );

            this->Move(
                {
                    userSetting.GetIntegerEntry(
                        "hold" + this->navaid.identifier + "PositionX",
                        100
                    ),
                    userSetting.GetIntegerEntry(
                        "hold" + this->navaid.identifier + "PositionY",
                        100
                    )
                }
            );
        }

        /*
            Move the display
        */
        void HoldDisplay::Move(const POINT & pos)
        {
            // General window pos
            this->windowPos = pos;
            this->dataStartHeight = this->dataStartOffset + pos.y;

            // Title bar
            this->titleArea.X = pos.x;
            this->titleArea.Y = pos.y;

            this->titleRect = {
                this->titleArea.X,
                this->titleArea.Y,
                this->titleArea.X + this->titleArea.Width,
                this->titleArea.Y + this->titleArea.Height
            };

            // Minimise Button
            this->minimiseButtonArea.X = pos.x + 5;
            this->minimiseButtonArea.Y = this->titleArea.Y + 2;

            this->minimiseClickRect = {
                this->minimiseButtonArea.X,
                this->minimiseButtonArea.Y,
                this->minimiseButtonArea.X + this->minimiseButtonArea.Width,
                this->minimiseButtonArea.Y + this->minimiseButtonArea.Height
            };

            // Information Button
            this->informationButtonArea.X = minimiseButtonArea.X + minimiseButtonArea.Width + 5;
            this->informationButtonArea.Y = this->titleArea.Y + 2;

            this->informationClickRect = {
                this->informationButtonArea.X,
                this->informationButtonArea.Y,
                this->informationButtonArea.X + this->informationButtonArea.Width,
                this->informationButtonArea.Y + this->informationButtonArea.Height
            };

            // Minus button
            this->minusButtonRect.X = pos.x + 5;
            this->minusButtonRect.Y = this->buttonStartOffset + pos.y;

            this->minusButtonClickRect = {
                this->minusButtonRect.X,
                this->minusButtonRect.Y,
                this->minusButtonRect.X + this->minusButtonRect.Width,
                this->minusButtonRect.Y + this->minusButtonRect.Height
            };

            // Plus button
            this->plusButtonRect.X = pos.x + 55;
            this->plusButtonRect.Y = this->buttonStartOffset + pos.y;

            this->plusButtonClickRect = {
                this->plusButtonRect.X,
                this->plusButtonRect.Y,
                this->plusButtonRect.X + this->plusButtonRect.Width,
                this->plusButtonRect.Y + this->plusButtonRect.Height
            };

            // Show all levels button
            this->allButtonRect.X = pos.x + 105;
            this->allButtonRect.Y = this->buttonStartOffset + pos.y;

            this->allButtonClickRect = {
                this->allButtonRect.X,
                this->allButtonRect.Y,
                this->allButtonRect.X + this->allButtonRect.Width,
                this->allButtonRect.Y + this->allButtonRect.Height
            };

            // Add button
            this->addButtonRect.X = pos.x + 155;
            this->addButtonRect.Y = this->buttonStartOffset + pos.y;

            this->addButtonClickRect = {
                this->addButtonRect.X,
                this->addButtonRect.Y,
                this->addButtonRect.X + this->addButtonRect.Width,
                this->addButtonRect.Y + this->addButtonRect.Height
            };
        }

        /*
            Draw the rounded rectangles that form the action buttons
        */
        void HoldDisplay::DrawRoundRectangle(
            GdiGraphicsInterface & graphics,
            const Gdiplus::Rect & rect,
            UINT8 radius
        ) const {
            Gdiplus::GraphicsPath path;
            path.AddLine(rect.X + radius, rect.Y, rect.X + rect.Width - (radius * 2), rect.Y);
            path.AddArc(rect.X + rect.Width - (radius * 2), rect.Y, radius * 2, radius * 2, 270, 90);
            path.AddLine(
                rect.X + rect.Width,
                rect.Y + radius,
                rect.X + rect.Width,
                rect.Y + rect.Height - (radius * 2)
            );
            path.AddArc(rect.X + rect.Width - (radius * 2), rect.Y + rect.Height - (radius * 2), radius * 2,
                radius * 2, 0, 90);
            path.AddLine(
                rect.X + rect.Width - (radius * 2),
                rect.Y + rect.Height,
                rect.X + radius,
                rect.Y + rect.Height
            );
            path.AddArc(rect.X, rect.Y + rect.Height - (radius * 2), radius * 2, radius * 2, 90, 90);
            path.AddLine(rect.X, rect.Y + rect.Height - (radius * 2), rect.X, rect.Y + radius);
            path.AddArc(rect.X, rect.Y, radius * 2, radius * 2, 180, 90);
            path.CloseFigure();
            graphics.DrawPath(path, this->borderPen);
        }

        /*
            Render textual data about the hold itself.
        */
        void HoldDisplay::RenderHoldInformation(
            GdiGraphicsInterface & graphics,
            EuroscopeRadarLoopbackInterface & radarScreen,
            const int screenObjectId
        ) const {
            // Black background and white border
            Gdiplus::Rect borderRect = {
               this->windowPos.x,
               this->windowPos.y,
               this->windowWidth,
               this->informationDisplayWindowHeight
            };

            graphics.FillRect(borderRect, this->backgroundBrush);
            graphics.DrawRect(
                borderRect,
                this->borderPen
            );

            // Render the title bar
            this->RenderTitleBar(graphics, radarScreen, screenObjectId);

            if (!this->publishedHolds.size()) {
                return;
            }

            // Render the data
            Gdiplus::Rect dataRect = {
                this->windowPos.x,
                this->dataStartHeight,
                this->windowWidth,
                this->lineHeight
            };

            graphics.DrawString(
                std::wstring(L"Fix: ") + ConvertToTchar(this->navaid.identifier),
                dataRect,
                this->dataBrush
            );

            dataRect.Y = dataRect.Y + this->lineHeight + 5;
            graphics.DrawString(
                std::wstring(L"Inbound: ") + ConvertToTchar(this->publishedHolds.find(0)->inbound),
                dataRect,
                this->dataBrush
            );

            dataRect.Y = dataRect.Y + this->lineHeight + 5;
            graphics.DrawString(
                std::wstring(L"Turn: ") + ConvertToTchar(this->publishedHolds.find(0)->turnDirection),
                dataRect,
                this->dataBrush
            );

            dataRect.Y = dataRect.Y + this->lineHeight + 5;
            graphics.DrawString(
                std::wstring(L"Maximum: ") + ConvertToTchar(this->publishedHolds.find(0)->maximum),
                dataRect,
                this->dataBrush
            );

            dataRect.Y = dataRect.Y + this->lineHeight + 5;
            graphics.DrawString(
                std::wstring(L"Minimum: ") + ConvertToTchar(this->publishedHolds.find(0)->minimum),
                dataRect,
                this->dataBrush
            );
        }

        /*
            Draw the title bar for the hold display
        */
        void HoldDisplay::RenderTitleBar(
            GdiGraphicsInterface & graphics,
            EuroscopeRadarLoopbackInterface & radarScreen,
            const int screenObjectId
        ) const {

            // Title bar
            radarScreen.RegisterScreenObject(
                screenObjectId,
                this->navaid.identifier,
                this->titleRect,
                true
            );
            graphics.FillRect(this->titleArea, this->titleBarBrush);
            graphics.DrawRect(this->titleArea, this->borderPen);

            std::wstring holdName = ConvertToTchar(this->navaid.identifier);
            graphics.DrawString(
                ConvertToTchar(this->navaid.identifier),
                this->titleArea,
                this->titleBarTextBrush
            );
            graphics.DrawLine(
                this->borderPen,
                { this->titleArea.X, this->titleArea.Y + this->titleArea.Height },
                { this->titleArea.X + this->titleArea.Width, this->titleArea.Y + this->titleArea.Height }
            );

            // Minimise Button
            graphics.DrawRect(this->minimiseButtonArea, this->borderPen);
            graphics.DrawString(
                this->minimised ? L"\u25B2" : L"\u25BC",
                this->minimiseButtonArea,
                this->titleBarTextBrush
            );
            radarScreen.RegisterScreenObject(
                screenObjectId,
                this->navaid.identifier + "/minimise",
                this->minimiseClickRect,
                false
            );

            // Information button
            graphics.DrawRect(this->informationButtonArea, this->borderPen);
            graphics.DrawString(L"i", this->informationButtonArea, this->titleBarTextBrush);
            radarScreen.RegisterScreenObject(
                screenObjectId,
                this->navaid.identifier + "/information",
                this->informationClickRect,
                false
            );
        }

        /*
            Render the managed hold data - positions of aircraft in the hold etc
        */
        void HoldDisplay::RenderManagedHoldDisplay(
            GdiGraphicsInterface & graphics,
            EuroscopeRadarLoopbackInterface & radarScreen,
            const int screenObjectId
        ) const {

            Gdiplus::Rect borderRect = {
               this->windowPos.x,
               this->windowPos.y,
               this->windowWidth,
               this->windowHeight
            };

            // Black background
            graphics.FillRect(borderRect, this->backgroundBrush);

            // Render the title bar
            this->RenderTitleBar(graphics, radarScreen, screenObjectId);

            // Action buttons
            this->DrawRoundRectangle(graphics, minusButtonRect, 5);
            graphics.DrawString(L"-", minusButtonRect, this->titleBarTextBrush);
            radarScreen.RegisterScreenObject(
                screenObjectId,
                this->navaid.identifier + "/minus",
                this->minusButtonClickRect,
                false
            );

            this->DrawRoundRectangle(graphics, plusButtonRect, 5);
            graphics.DrawString(L"+", plusButtonRect, this->titleBarTextBrush);
            radarScreen.RegisterScreenObject(
                screenObjectId,
                this->navaid.identifier + "/plus",
                this->plusButtonClickRect,
                false
            );

            this->DrawRoundRectangle(graphics, addButtonRect, 5);
            graphics.DrawString(L"ADD", addButtonRect, this->titleBarTextBrush);
            radarScreen.RegisterScreenObject(
                screenObjectId,
                this->navaid.identifier + "/add",
                this->addButtonClickRect,
                false
            );

            this->DrawRoundRectangle(graphics, allButtonRect, 5);
            graphics.DrawString(L"ALL", allButtonRect, this->titleBarTextBrush);
            radarScreen.RegisterScreenObject(
                screenObjectId,
                this->navaid.identifier + "/allLevels",
                this->allButtonClickRect,
                false
            );

            // Hold display
            Gdiplus::Rect numbersDisplay = {
                this->windowPos.x,
                this->dataStartHeight,
                30,
                15
            };

            // The row in question
            Gdiplus::Rect holdRow = {
                this->windowPos.x,
                this->dataStartHeight,
                this->windowWidth,
                15
            };

            // Rects for rendering the actual data
            Gdiplus::Rect callsignDisplay = {
                this->windowPos.x + 35,
                this->dataStartHeight,
                60,
                this->lineHeight
            };

            Gdiplus::Rect actualLevelDisplay = {
                this->windowPos.x + 100,
                this->dataStartHeight,
                30,
                this->lineHeight
            };

            Gdiplus::Rect clearedLevelDisplay = {
                this->windowPos.x + 135,
                this->dataStartHeight,
                30,
                this->lineHeight
            };

            Gdiplus::Rect timeInHoldDisplay = {
                this->windowPos.x + 170,
                this->dataStartHeight,
                30,
                this->lineHeight
            };

            // Loop over all the possiible levels in the hold and render
            const std::map<int, std::set<std::shared_ptr<HoldingAircraft>, CompareHoldingAircraft>> holdingAircraft =
                this->MapAircraftToLevels(this->holdManager.GetAircraftForHold(this->navaid.identifier));
            for (
                unsigned int i = this->maximumLevel;
                i >= this->minLevel;
                i -= 1000
            ) {
                bool levelRestricted = false;

                // Loop the published holds and check if we need to render any restrictions
                for (
                    std::set<HoldingData>::const_iterator publishedIt = this->publishedHolds.cbegin();
                    publishedIt != this->publishedHolds.cend();
                    publishedIt++
                ) {
                    // Print the restrictions
                    for (
                        std::set<std::unique_ptr<AbstractHoldLevelRestriction>>::const_iterator it
                        = publishedIt->restrictions.cbegin();
                        it != publishedIt->restrictions.cend();
                        ++it
                    ) {
                        if ((*it)->LevelRestricted(i)) {
                            levelRestricted = true;
                            break;
                        }
                    }
                }
                
                // No holding aircraft at this level, so just render the blank display
                if (holdingAircraft.size() == 0) {
                    // Render the row
                    graphics.FillRect(holdRow, this->backgroundBrush);

                    // Render the restrictions
                    if (levelRestricted) {
                        graphics.FillRect(holdRow, this->blockedLevelBrush);
                    }


                    // Render the numbers
                    graphics.DrawString(GetLevelDisplayString(i), numbersDisplay, this->titleBarTextBrush);

                } else {
                    std::set<std::shared_ptr<HoldingAircraft>, CompareHoldingAircraft> aircraftAtLevel =
                        holdingAircraft.at(i);
                    int i = 0;

                    std::shared_ptr<EuroScopeCRadarTargetInterface> rt;
                    std::shared_ptr<EuroScopeCFlightPlanInterface> fp;

                    // We have holding aircraft to deal with, render them in
                    for (
                        std::set<std::shared_ptr<HoldingAircraft>, CompareHoldingAircraft>::const_iterator
                            it = aircraftAtLevel.cbegin();
                        it != aircraftAtLevel.cend();
                        ++it
                    ) {
                        // Render the row
                        graphics.FillRect(holdRow, this->backgroundBrush);

                        // Render the restrictions
                        if (levelRestricted) {
                            graphics.FillRect(holdRow, this->blockedLevelBrush);
                        }

                        // Render the numbers
                        if (i == 0) {
                            graphics.DrawString(GetLevelDisplayString(i), numbersDisplay, this->titleBarTextBrush);
                            i++;
                        }

                        // Render the aircraft data
                        try {
                            rt = this->plugin.GetRadarTargetForCallsign((*it)->GetCallsign());
                            fp = this->plugin.GetFlightplanForCallsign((*it)->GetCallsign());

                            // Callsign
                            std::wstring callsign = ConvertToTchar((*it)->GetCallsign());
                            graphics.DrawString(
                                callsign,
                                callsignDisplay,
                                this->dataBrush
                            );

                            // Reported level
                            graphics.DrawString(
                                GetLevelDisplayString(rt->GetFlightLevel()),
                                actualLevelDisplay,
                                this->dataBrush
                            );


                            // Cleared level
                            graphics.DrawString(
                                GetLevelDisplayString(fp->GetClearedAltitude()),
                                clearedLevelDisplay,
                                this->clearedLevelBrush
                            );

                            // Time in hold, if it's assigned
                            if ((*it)->GetAssignedHold() != (*it)->noHoldAssigned) {
                                std::wstring timeString = GetTimeInHoldDisplayString((*it)->GetAssignedHoldEntryTime());
                                graphics.DrawString(
                                    timeString,
                                    timeInHoldDisplay,
                                    this->dataBrush
                                );
                            }
                        }
                        catch (std::invalid_argument) {
                            // Skip the render
                        }

                        // Increase the lines
                        numbersDisplay.Y = numbersDisplay.Y + this->lineHeight;
                        callsignDisplay.Y = callsignDisplay.Y + this->lineHeight;
                        actualLevelDisplay.Y = actualLevelDisplay.Y + this->lineHeight;
                        clearedLevelDisplay.Y = clearedLevelDisplay.Y + this->lineHeight;
                        timeInHoldDisplay.Y = timeInHoldDisplay.Y + this->lineHeight;
                    }
                }

            }

            // Border around whole thing, draw this last
            graphics.DrawRect(
                RECT{
                    this->windowPos.x,
                    this->windowPos.y,
                    this->windowPos.x + this->windowWidth,
                    timeInHoldDisplay.Y + this->lineHeight
                },
                this->borderPen
            );
        }

        void HoldDisplay::PaintWindow(
            GdiGraphicsInterface & graphics,
            EuroscopeRadarLoopbackInterface & radarScreen,
            const int screenObjectId
        ) const {

            // Minimised, just render the title bar.
            if (this->minimised) {
                this->RenderTitleBar(graphics, radarScreen, screenObjectId);
                return;
            }

            // Render the correct view.
            this->showHoldInformation
                ? this->RenderHoldInformation(graphics, radarScreen, screenObjectId)
                : this->RenderManagedHoldDisplay(graphics, radarScreen, screenObjectId);
        }

        /*
            Save display data to the ASR
        */
        void HoldDisplay::SaveDataToAsr(
            UserSetting & userSetting
        ) const {

            // Minimised window
            userSetting.Save(
                "hold" + this->navaid.identifier + "Minimised",
                "Hold " + this->navaid.identifier + " Minimised",
                this->minimised
            );

            // X position
            userSetting.Save(
                "hold" + this->navaid.identifier + "PositionX",
                "Hold " + this->navaid.identifier + " X Position",
                this->windowPos.x
            );

            // Y position
            userSetting.Save(
                "hold" + this->navaid.identifier + "PositionY",
                "Hold " + this->navaid.identifier + " Y Position",
                this->windowPos.y
            );
        }
    }  // namespace Hold
}  // namespace UKControllerPlugin
