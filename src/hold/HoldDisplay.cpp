﻿#include "pch/stdafx.h"
#include "hold/HoldDisplay.h"
#include "hold/HoldDisplayFunctions.h"
#include "hold/ManagedHold.h"
#include "hold/HoldManager.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "euroscope/EuroScopeRadarLoopbackInterface.h"
#include "graphics/GdiGraphicsInterface.h"
#include "euroscope/UserSetting.h"

using UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface;
using UKControllerPlugin::Windows::GdiGraphicsInterface;
using UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface;
using UKControllerPlugin::Hold::HoldManager;
using UKControllerPlugin::Euroscope::UserSetting;

namespace UKControllerPlugin {
    namespace Hold {

        HoldDisplay::HoldDisplay(
            const EuroscopePluginLoopbackInterface & plugin,
            const ManagedHold & managedHold,
            HoldManager & holdManager
        )
            : plugin(plugin),
            holdManager(holdManager),
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

            this->windowHeight = this->dataStartOffset + (this->managedHold.GetNumberOfLevels() * this->lineHeight);
            this->Move(this->windowPos);
        }

        /*
            A button has been clicked
        */
        void HoldDisplay::ButtonClicked(std::string button)
        {
            if (button == "plus") {
                if (numLevelsSkipped > 0) {
                    this->numLevelsSkipped--;
                    this->windowHeight = this->windowHeight + this->lineHeight;
                }
            } else if (button == "minus") {
                const unsigned int maxLevelsSkippable = (this->managedHold.GetHoldParameters().maximum -
                            this->managedHold.GetHoldParameters().minimum) / 1000;
                if (maxLevelsSkippable != this->numLevelsSkipped) {
                    numLevelsSkipped++;
                    this->windowHeight = this->windowHeight - this->lineHeight;
                }
            } else if (button == "allLevels") {
                this->windowHeight = this->dataStartOffset + (
                    this->managedHold.GetNumberOfLevels() * this->lineHeight
                );
                this->numLevelsSkipped = 0;
            } else if (button == "add") {
                try {
                    this->holdManager.AddAircraftToHold(
                        *this->plugin.GetSelectedFlightplan(),
                        *this->plugin.GetSelectedRadarTarget(),
                        this->managedHold.GetHoldParameters().identifier
                    );
                } catch (std::invalid_argument) {
                    // Nothing to do
                }
            } else if (button == "minimise") {
                this->minimised = !this->minimised;
            }

        }

        /*
            Return the position of the window
        */
        POINT HoldDisplay::GetDisplayPos(void) const
        {
            return this->windowPos;
        }

        /*
            Return the number of levels skipped on the display
        */
        unsigned int HoldDisplay::GetLevelsSkipped(void) const
        {
            return this->numLevelsSkipped;
        }

        /*
            Load data from an ASR
        */
        void HoldDisplay::LoadDataFromAsr(UserSetting & userSetting, unsigned int holdProfileId)
        {
            this->numLevelsSkipped = userSetting.GetUnsignedIntegerEntry(
                "holdProfile" + std::to_string(holdProfileId) + "Hold" +
                    std::to_string(this->managedHold.GetHoldParameters().identifier) + "LevelsSkipped",
                0
            );

            this->minimised = userSetting.GetBooleanEntry(
                "holdProfile" + std::to_string(holdProfileId) + "Hold" +
                    std::to_string(this->managedHold.GetHoldParameters().identifier) + "Minimised",
                false
            );

            this->Move(
                {
                    userSetting.GetIntegerEntry(
                        "holdProfile" + std::to_string(holdProfileId) + "Hold" +
                            std::to_string(this->managedHold.GetHoldParameters().identifier) + "PositionX",
                        100
                    ),
                    userSetting.GetIntegerEntry(
                        "holdProfile" + std::to_string(holdProfileId) + "Hold" +
                            std::to_string(this->managedHold.GetHoldParameters().identifier) + "PositionY",
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

        void HoldDisplay::PaintWindow(
            GdiGraphicsInterface & graphics,
            EuroscopeRadarLoopbackInterface & radarScreen,
            const int screenObjectId
        ) const {

            Gdiplus::Rect borderRect = {
                this->windowPos.x,
                this->windowPos.y,
                windowWidth,
                windowHeight
            };

            // Paint a black background
            if (!this->minimised) {
                graphics.FillRect(borderRect, this->backgroundBrush);
            }

            // Title bar
            radarScreen.RegisterScreenObject(
                screenObjectId,
                std::to_string(this->managedHold.GetHoldParameters().identifier),
                this->titleRect,
                true
            );
            graphics.FillRect(this->titleArea, this->titleBarBrush);
            graphics.DrawRect(this->titleArea, this->borderPen);

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

            // Minimise Button
            graphics.DrawRect(this->minimiseButtonArea, this->borderPen);
            graphics.DrawString(this->minimised ? L"\u25B2" : L"\u25BC", this->minimiseButtonArea, this->titleBarTextBrush);
            radarScreen.RegisterScreenObject(
                screenObjectId,
                std::to_string(this->managedHold.GetHoldParameters().identifier) + "/minimise",
                this->minimiseClickRect,
                false
            );

            // If minimised, don't carry on drawing
            if (this->minimised) {
                return;
            }

            // Action buttons
            this->DrawRoundRectangle(graphics, minusButtonRect, 5);
            graphics.DrawString(L"-", minusButtonRect, this->titleBarTextBrush);
            radarScreen.RegisterScreenObject(
                screenObjectId,
                std::to_string(this->managedHold.GetHoldParameters().identifier) + "/minus",
                this->minusButtonClickRect,
                false
            );

            this->DrawRoundRectangle(graphics, plusButtonRect, 5);
            graphics.DrawString(L"+", plusButtonRect, this->titleBarTextBrush);
            radarScreen.RegisterScreenObject(
                screenObjectId,
                std::to_string(this->managedHold.GetHoldParameters().identifier) + "/plus",
                this->plusButtonClickRect,
                false
            );

            this->DrawRoundRectangle(graphics, addButtonRect, 5);
            graphics.DrawString(L"ADD", addButtonRect, this->titleBarTextBrush);
            radarScreen.RegisterScreenObject(
                screenObjectId,
                std::to_string(this->managedHold.GetHoldParameters().identifier) + "/add",
                this->addButtonClickRect,
                false
            );

            this->DrawRoundRectangle(graphics, allButtonRect, 5);
            graphics.DrawString(L"ALL", allButtonRect, this->titleBarTextBrush);
            radarScreen.RegisterScreenObject(
                screenObjectId,
                std::to_string(this->managedHold.GetHoldParameters().identifier) + "/allLevels",
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

                callsignDisplay.Y = this->dataStartHeight + (this->lineHeight * (displayRow - this->numLevelsSkipped));
                actualLevelDisplay.Y = this->dataStartHeight + (this->lineHeight * (displayRow - this->numLevelsSkipped));
                clearedLevelDisplay.Y = this->dataStartHeight + (this->lineHeight * (displayRow - this->numLevelsSkipped));
                timeInHoldDisplay.Y = this->dataStartHeight + (this->lineHeight * (displayRow - this->numLevelsSkipped));

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

            // Line sectioning off the bottom level.
            unsigned int bottomLevelLineHeight = this->dataStartHeight + 
                (this->lineHeight * (this->managedHold.GetNumberOfLevels() - this->numLevelsSkipped - 1)) - 2;

            Gdiplus::GraphicsPath path;
            path.AddLine(
                this->windowPos.x,
                bottomLevelLineHeight,
                this->windowPos.x + this->windowWidth,
                bottomLevelLineHeight
            );
            graphics.DrawPath(path, this->borderPen);

            // Border around whole thing, draw this last
            graphics.DrawRect(
                borderRect,
                this->borderPen
            );
        }

        /*
            Save display data to the ASR
        */
        void HoldDisplay::SaveDataToAsr(UserSetting & userSetting, unsigned int holdProfileId, std::string profileName) const
        {
            userSetting.Save(
                "holdProfile" + std::to_string(holdProfileId) + "Hold" +
                    std::to_string(this->managedHold.GetHoldParameters().identifier) + "LevelsSkipped",
                "Hold Profile (" + profileName + " - " + this->managedHold.GetHoldParameters().description + ") Levels Skipped",
                this->numLevelsSkipped
            );

            userSetting.Save(
                "holdProfile" + std::to_string(holdProfileId) + "Hold" +
                std::to_string(this->managedHold.GetHoldParameters().identifier) + "Minimised",
                "Hold Profile (" + profileName + " - " + this->managedHold.GetHoldParameters().description + ") Minimised",
                this->minimised
            );

            userSetting.Save(
                "holdProfile" + std::to_string(holdProfileId) + "Hold" +
                    std::to_string(this->managedHold.GetHoldParameters().identifier) + "PositionX",
                "Hold Profile (" + profileName + " - " + this->managedHold.GetHoldParameters().description + ") X Pos",
                this->windowPos.x
            );

            userSetting.Save(
                "holdProfile" + std::to_string(holdProfileId) + "Hold" +
                std::to_string(this->managedHold.GetHoldParameters().identifier) + "PositionY",
                "Hold Profile (" + profileName + " - " + this->managedHold.GetHoldParameters().description + ") Y Pos",
                this->windowPos.y
            );
        }
    }  // namespace Hold
}  // namespace UKControllerPlugin
