#include "pch/stdafx.h"
#include "countdown/CountdownRenderer.h"
#include "euroscope/UserSetting.h"
#include "graphics/GdiGraphicsInterface.h"
#include "graphics/GdiplusBrushes.h"
#include "euroscope/EuroscopeRadarLoopbackInterface.h"
#include "countdown/CountdownTimer.h"

using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface;
using UKControllerPlugin::Windows::GdiGraphicsInterface;
using UKControllerPlugin::Windows::GdiplusBrushes;
namespace UKControllerPlugin {
    namespace Countdown {

        CountdownRenderer::CountdownRenderer(
            CountdownTimer & countdownModule,
            int functionsClickspotId,
            int timeDisplayClickspotId,
            int closeClickspotId,
            int toogleCallbackFunctionId,
            const GdiplusBrushes & brushes
        )
            : countdownModule(countdownModule), functionsClickspotId(functionsClickspotId),
            timeDisplayClickspotId(timeDisplayClickspotId), closeClickspotId(closeClickspotId),
            toggleCallbackFunctionId(toogleCallbackFunctionId), brushes(brushes)
        {
        }

        /*
            Called when the ASR is first loaded, configures the renderer with the per-ASR settings.
        */
        void CountdownRenderer::AsrLoadedEvent(UserSetting & userSetting)
        {
            // Visibility
            this->visible = userSetting.GetBooleanEntry(this->userSettingVisible, true);

            // Position
            this->ShiftAllElements(
                userSetting.GetIntegerEntry(this->userSettingPosX, 100),
                userSetting.GetIntegerEntry(this->userSettingPosY, 100)
            );
        }

        /*
            Called when the ASR is closed, saves rendering options to the ASR for next time.
        */
        void CountdownRenderer::AsrClosingEvent(UserSetting & userSetting)
        {
            userSetting.Save(this->userSettingVisible, this->visibleUserSettingDescription, this->visible);
            userSetting.Save(this->userSettingPosX, this->xPositionUserSettingDescription, this->timeDisplayArea.left);
            userSetting.Save(this->userSettingPosY, this->yPositionUserSettingDescription, this->timeDisplayArea.top);
        }

        /*
            Called when the configuration option is selected in the configuration menu. Toggle
            the visibility.
        */
        void CountdownRenderer::Configure(std::string subject)
        {
            this->visible = !this->visible;
        }

        /*
            Handle the case where one of the function buttons are clicked.
        */
        void CountdownRenderer::FunctionButtonClick(std::string function, EuroscopeRadarLoopbackInterface & radarScreen)
        {
            if (function == "0.5") {
                this->countdownModule.StartTimer(30);
            } else if (function == "1") {
                this->countdownModule.StartTimer(60);
            } else if (function == "1.5") {
                this->countdownModule.StartTimer(90);
            } else if (function == "2") {
                this->countdownModule.StartTimer(120);
            } else if (function == "3") {
                this->countdownModule.StartTimer(180);
            } else if (function == "R") {
                this->countdownModule.ResetTimer(false);
            }
        }

        /*
            Returns the configuration for configuration menu item.
        */
        PopupMenuItem CountdownRenderer::GetConfigurationMenuItem(void) const
        {
            PopupMenuItem returnVal;
            returnVal.firstValue = this->menuItemDescription;
            returnVal.secondValue = "";
            returnVal.callbackFunctionId = this->toggleCallbackFunctionId;
            returnVal.checked = this->visible;
            returnVal.disabled = false;
            returnVal.fixedPosition = false;
            return returnVal;
        }

        /*
            Returns the display area of the close clickspot
        */
        RECT CountdownRenderer::GetCloseClickspotDisplayArea(void) const
        {
            return this->closeClickspotDisplayArea;
        }

        /*
            Returns the display area of the ninety second clickspot
        */
        RECT CountdownRenderer::GetNinetySecondDisplayArea(void) const
        {
            return this->ninetysecondDisplayArea;
        }

        /*
            Returns the display area of one minute clickspot
        */
        RECT CountdownRenderer::GetOneMinuteDisplayArea(void) const
        {
            return this->oneMinuteDisplayArea;
        }

        /*
            Returns the display area of the reset clickspot
        */
        RECT CountdownRenderer::GetResetDisplayArea(void) const
        {
            return this->resetDisplayArea;
        }

        /*
            Returns the display area of the thirty second clickspot
        */
        RECT CountdownRenderer::GetThirtySecondDisplayArea(void) const
        {
            return this->thirtySecondDisplayArea;
        }

        /*
            Returns the display area of the three minute clickspot
        */
        RECT CountdownRenderer::GetThreeMinuteDisplayArea(void) const
        {
            return this->threeMinuteDisplayArea;
        }

        /*
            Returns the display area of the timer box
        */
        RECT CountdownRenderer::GetTimeDisplayArea(void) const
        {
            return this->timeDisplayArea;
        }

        /*
            Returns the display are of the two minute clickspot
        */
        RECT CountdownRenderer::GetTwoMinuteDisplayArea(void) const
        {
            return this->twoMinuteDisplayArea;
        }

        /*
            Returns the correct time string, on the number of seconds remaining.
        */
        std::wstring CountdownRenderer::GetCurrentTimeString(int secondsRemaining)
        {
            // Work out the numbers
            int minutes = secondsRemaining / 60;
            int seconds = secondsRemaining % 60;

            std::wstringstream wss;

            // Create string
            if (seconds < 10) {
                wss << minutes << ":0" << seconds << '\0';
            } else {
                wss << minutes << ":" << seconds << '\0';
            }

            return wss.str();
        }

        /*
            Returns the correct colour with which to display the time remaining.
        */
        const Gdiplus::Brush & CountdownRenderer::GetTimeColour(int secondsRemaining)
        {

            // Colour the text depending on how long is left, if we get 0, that means timer is up.
            if (secondsRemaining == 0) {
                return *this->brushes.whiteBrush;
            } else if (secondsRemaining > 30) {
                return *this->brushes.greenBrush;
            } else if (secondsRemaining > 10) {
                return *this->brushes.yellowBrush;
            }

            return *this->brushes.redBrush;
        }

        /*
            Returns true if the module should be rendered, false otherwise.
        */
        bool CountdownRenderer::IsVisible(void) const
        {
            return this->visible;
        }

        /*
            If someone clicks on the timer, handle it.
        */
        void CountdownRenderer::LeftClick(
            int objectId,
            std::string objectDescription,
            EuroscopeRadarLoopbackInterface & radarScreen
        ) {
            if (objectId == this->closeClickspotId) {
                this->visible = false;
                return;
            }

            this->FunctionButtonClick(objectDescription, radarScreen);
        }

        /*
            Move the position of the countdown module.
        */
        void CountdownRenderer::Move(RECT position)
        {
            this->ShiftAllElements(position.left, position.top);
        }

        /*
            Renders the module to the screen.
        */
        void CountdownRenderer::Render(GdiGraphicsInterface & graphics, EuroscopeRadarLoopbackInterface & radarScreen)
        {
            this->RenderTimeDisplay(graphics, radarScreen);
            this->RenderButtons(graphics, radarScreen);
        }

        /*
            Render all the buttons to the screen.
        */
        void CountdownRenderer::RenderButtons(
            GdiGraphicsInterface & graphics,
            EuroscopeRadarLoopbackInterface & radarScreen
        ) {
            // The close clickspot
            graphics.FillRect(this->closeClickspotDisplayArea, *this->brushes.euroscopeBackgroundBrush);
            graphics.DrawRect(this->closeClickspotDisplayArea, *this->brushes.blackPen);
            graphics.DrawString(L"X", this->closeClickspotDisplayArea, *this->brushes.whiteBrush);
            radarScreen.RegisterScreenObject(this->closeClickspotId, "", this->closeClickspotDisplayArea, false);

            // The thirty second button
            graphics.FillRect(this->thirtySecondDisplayArea, *this->brushes.euroscopeBackgroundBrush);
            graphics.DrawRect(this->thirtySecondDisplayArea, *this->brushes.blackPen);
            graphics.DrawString(L"0.5", this->thirtySecondDisplayArea, *this->brushes.whiteBrush);
            radarScreen.RegisterScreenObject(this->functionsClickspotId, "0.5", this->thirtySecondDisplayArea, false);

            // The 1 minute button
            graphics.FillRect(this->oneMinuteDisplayArea, *this->brushes.euroscopeBackgroundBrush);
            graphics.DrawRect(this->oneMinuteDisplayArea, *this->brushes.blackPen);
            graphics.DrawString(L"1", this->oneMinuteDisplayArea, *this->brushes.whiteBrush);
            radarScreen.RegisterScreenObject(this->functionsClickspotId, "1", this->oneMinuteDisplayArea, false);

            // The ninety second button
            graphics.FillRect(this->ninetysecondDisplayArea, *this->brushes.euroscopeBackgroundBrush);
            graphics.DrawRect(this->ninetysecondDisplayArea, *this->brushes.blackPen);
            graphics.DrawString(L"1.5", this->ninetysecondDisplayArea, *this->brushes.whiteBrush);
            radarScreen.RegisterScreenObject(this->functionsClickspotId, "1.5", this->ninetysecondDisplayArea, false);

            // The 2 minute button
            graphics.FillRect(this->twoMinuteDisplayArea, *this->brushes.euroscopeBackgroundBrush);
            graphics.DrawRect(this->twoMinuteDisplayArea, *this->brushes.blackPen);
            graphics.DrawString(L"2", this->twoMinuteDisplayArea, *this->brushes.whiteBrush);
            radarScreen.RegisterScreenObject(this->functionsClickspotId, "2", this->twoMinuteDisplayArea, false);

            // The 3 minute button
            graphics.FillRect(this->threeMinuteDisplayArea, *this->brushes.euroscopeBackgroundBrush);
            graphics.DrawRect(this->threeMinuteDisplayArea, *this->brushes.blackPen);
            graphics.DrawString(L"3", this->threeMinuteDisplayArea, *this->brushes.whiteBrush);
            radarScreen.RegisterScreenObject(this->functionsClickspotId, "3", this->threeMinuteDisplayArea, false);

            // The reset button.
            graphics.FillRect(this->resetDisplayArea, *this->brushes.euroscopeBackgroundBrush);
            graphics.DrawRect(this->resetDisplayArea, *this->brushes.blackPen);
            graphics.DrawString(L"R", this->resetDisplayArea, *this->brushes.whiteBrush);
            radarScreen.RegisterScreenObject(this->functionsClickspotId, "R", this->resetDisplayArea, false);
        }

        /*
            Renders the time display.
        */
        void CountdownRenderer::RenderTimeDisplay(
            GdiGraphicsInterface & graphics,
            EuroscopeRadarLoopbackInterface & radarScreen
        ) {
            // The time display
            graphics.FillRect(this->timeDisplayArea, *this->brushes.euroscopeBackgroundBrush);
            graphics.DrawRect(this->timeDisplayArea, *this->brushes.blackPen);
            radarScreen.RegisterScreenObject(this->timeDisplayClickspotId, "", this->timeDisplayArea, true);

            // Get the seconds remaining from the Countdown class and use that to draw the time to the screen.
            int secondsRemaining = this->countdownModule.GetSecondsRemaining();
            graphics.DrawString(
                this->GetCurrentTimeString(secondsRemaining),
                this->timeDisplayArea,
                this->GetTimeColour(secondsRemaining)
            );
        }

        /*
            Called when something is right clicked.

            We don't have anything to do here for now.
        */
        void CountdownRenderer::RightClick(
            int objectId,
            std::string objectDescription,
            EuroscopeRadarLoopbackInterface & radarScreen
        ){

        }

        /*
            Sets whether or not the countdown timer should be visible on the screen.
        */
        void CountdownRenderer::SetVisible(bool visible)
        {
            this->visible = visible;
        }

        /*
            Moves all the elements of the display.
        */
        void CountdownRenderer::ShiftAllElements(int topLeftX, int topLeftY)
        {

            // Top left - the display area
            this->timeDisplayArea.left = topLeftX;
            this->timeDisplayArea.top = topLeftY;
            this->timeDisplayArea.bottom = this->timeDisplayArea.top + this->rowHeight;
            this->timeDisplayArea.right = this->timeDisplayArea.left + this->timeDisplayWidth;

            // The function buttons
            this->closeClickspotDisplayArea = {
                this->timeDisplayArea.right,
                this->timeDisplayArea.top,
                this->timeDisplayArea.right + this->buttonWidth,
                this->timeDisplayArea.bottom
            };

            this->thirtySecondDisplayArea = {
                this->timeDisplayArea.left,
                this->timeDisplayArea.bottom,
                this->timeDisplayArea.left + this->buttonWidth,
                this->timeDisplayArea.bottom + this->rowHeight
            };

            this->oneMinuteDisplayArea = {
                this->thirtySecondDisplayArea.right,
                this->thirtySecondDisplayArea.top,
                this->thirtySecondDisplayArea.right + this->buttonWidth,
                this->thirtySecondDisplayArea.bottom
            };

            this->ninetysecondDisplayArea = {
                this->oneMinuteDisplayArea.right,
                this->oneMinuteDisplayArea.top,
                this->oneMinuteDisplayArea.right + this->buttonWidth,
                this->oneMinuteDisplayArea.bottom
            };

            this->twoMinuteDisplayArea = {
                this->ninetysecondDisplayArea.right,
                this->ninetysecondDisplayArea.top,
                this->ninetysecondDisplayArea.right + this->buttonWidth,
                this->ninetysecondDisplayArea.bottom
            };

            this->threeMinuteDisplayArea = {
                this->twoMinuteDisplayArea.right,
                this->twoMinuteDisplayArea.top,
                this->twoMinuteDisplayArea.right + this->buttonWidth,
                this->twoMinuteDisplayArea.bottom
            };

            this->resetDisplayArea = {
                this->threeMinuteDisplayArea.right,
                this->threeMinuteDisplayArea.top,
                this->threeMinuteDisplayArea.right + this->buttonWidth,
                this->threeMinuteDisplayArea.bottom
            };
        }
    }  // namespace Countdown
}  // namespace UKControllerPlugin
