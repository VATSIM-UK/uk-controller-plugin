#include "pch/pch.h"
#include "countdown/CountdownRenderer.h"
#include "euroscope/UserSetting.h"
#include "graphics/GdiGraphicsInterface.h"
#include "graphics/GdiplusBrushes.h"
#include "euroscope/EuroscopeRadarLoopbackInterface.h"
#include "countdown/CountdownTimer.h"
#include "countdown/TimerConfigurationManager.h"

using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface;
using UKControllerPlugin::Windows::GdiGraphicsInterface;
using UKControllerPlugin::Windows::GdiplusBrushes;
namespace UKControllerPlugin {
    namespace Countdown {

        CountdownRenderer::CountdownRenderer(
            CountdownTimer & countdownModule,
            const TimerConfigurationManager & configManager,
            int functionsClickspotId,
            int timeDisplayClickspotId,
            int closeClickspotId,
            int toogleCallbackFunctionId,
            const GdiplusBrushes & brushes
        )
            : countdownModule(countdownModule), functionsClickspotId(functionsClickspotId),
            timeDisplayClickspotId(timeDisplayClickspotId), closeClickspotId(closeClickspotId),
            toggleCallbackFunctionId(toogleCallbackFunctionId), brushes(brushes), configManager(configManager)
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
        void CountdownRenderer::Configure(int functionId, std::string subject, RECT screenObjectArea)
        {
            this->visible = !this->visible;
        }

        /*
            Handle the case where one of the function buttons are clicked.
        */
        void CountdownRenderer::FunctionButtonClick(std::string function, EuroscopeRadarLoopbackInterface & radarScreen)
        {
            // Reset timer
            if (function == "R") {
                this->countdownModule.ResetTimer(false);
                return;
            }

            // Timer is clicked, get time
            std::regex timerRegex("timer([0-9])Toggle");
            std::smatch timerMatches;
            std::regex_search(function, timerMatches, timerRegex);

            if (timerMatches.size() == 0) {
                LogWarning("Invalid timer toggle clicked");
                return;
            }

            TimerConfiguration config = this->configManager.GetTimer(std::stoi(timerMatches[1]));

            if (config == this->configManager.invalidTimer) {
                LogWarning("No valid timer config found");
                return;
            }

            this->countdownModule.StartTimer(config.timerDuration);
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

        RECT CountdownRenderer::GetTimerButtonArea(unsigned int configId) const
        {
            return this->timerButtonAreas.count(configId) ? this->timerButtonAreas.at(configId) : RECT {};
        }

        /*
            Returns the display area of the reset clickspot
        */
        RECT CountdownRenderer::GetResetDisplayArea(void) const
        {
            return this->resetDisplayArea;
        }

        /*
            Return the display area of the close clickspot
        */
        RECT CountdownRenderer::GetCloseClickspotDisplayArea(void) const
        {
            return this->closeClickspotDisplayArea;
        }

        /*
            Returns the display area of the timer box
        */
        RECT CountdownRenderer::GetTimeDisplayArea(void) const
        {
            return this->timeDisplayArea;
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
            EuroscopeRadarLoopbackInterface& radarScreen,
            int objectId,
            std::string objectDescription,
            POINT mousePos,
            RECT itemArea
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
        void CountdownRenderer::Move(RECT position, std::string objectDescription)
        {
            this->ShiftAllElements(position.left, position.top);
        }

        /*
            Renders the module to the screen.
        */
        void CountdownRenderer::Render(GdiGraphicsInterface & graphics, EuroscopeRadarLoopbackInterface & radarScreen)
        {
            // Update the current display information from config if out of date
            if (this->lastConfigVersion < this->configManager.GetConfigVersion()) {
                this->ShiftAllElements(this->timeDisplayArea.left, this->timeDisplayArea.top);
                this->lastConfigVersion = this->configManager.GetConfigVersion();
            }

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
            // Render the buttons
            int renderedButtons = 0;
            for (
                TimerConfigurationManager::const_iterator it = this->configManager.cbegin();
                it != this->configManager.cend();
                ++it
            ) {
                // Dont account for timer if not enabled
                if (!it->timerEnabled) {
                    continue;
                }

                graphics.FillRect(this->timerButtonAreas[it->timerId], *this->brushes.euroscopeBackgroundBrush);
                graphics.DrawRect(this->timerButtonAreas[it->timerId], *this->brushes.blackPen);
                graphics.DrawString(
                    std::to_wstring(it->timerDuration),
                    this->timerButtonAreas[it->timerId],
                    *this->brushes.whiteBrush
                );
                radarScreen.RegisterScreenObject(
                    this->functionsClickspotId,
                    "timer" + std::to_string(it->timerId) + "Toggle",
                    this->timerButtonAreas[it->timerId],
                    false
                );

                renderedButtons++;
            }

            // Fill in gaps where not enough buttons
            if (renderedButtons < 2) {
                RECT spaceToFill = {
                    this->timeDisplayArea.left + (this->buttonWidth * renderedButtons),
                    this->timeDisplayArea.bottom,
                    this->timeDisplayArea.right,
                    this->timeDisplayArea.bottom + this->rowHeight
                };
                graphics.FillRect(spaceToFill, *this->brushes.euroscopeBackgroundBrush);
                graphics.DrawRect(spaceToFill, *this->brushes.blackPen);
            }

            // The close clickspot
            graphics.FillRect(this->closeClickspotDisplayArea, *this->brushes.euroscopeBackgroundBrush);
            graphics.DrawRect(this->closeClickspotDisplayArea, *this->brushes.blackPen);
            graphics.DrawString(L"X", this->closeClickspotDisplayArea, *this->brushes.whiteBrush);
            radarScreen.RegisterScreenObject(this->closeClickspotId, "", this->closeClickspotDisplayArea, false);

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
            Reset the positon of the display
        */
        void CountdownRenderer::ResetPosition(void)
        {
            this->Move({ 100, 100, 100, 100 }, "");
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

            // If not enough timer buttons are enabled, force the time window width to the size of 2 buttons
            // else, make it the length of the number of enabled buttons
            if (this->configManager.CountEnabledTimers() < 2) {
                this->timeDisplayArea.right = this->timeDisplayArea.left + (2 * this->buttonWidth);
            } else {
                this->timeDisplayArea.right = this->timeDisplayArea.left +
                    (this->buttonWidth * (this->configManager.CountEnabledTimers()));
            }

            // Iterate the config and work out display areas for the visible timer buttons
            int numberOfVisibleButtons = 0;
            for (
                TimerConfigurationManager::const_iterator it = this->configManager.cbegin();
                it != this->configManager.cend();
                ++it
            ) {
                // Dont account for timer if not enabled
                if (!it->timerEnabled) {
                    continue;
                }

                this->timerButtonAreas[it->timerId] = {
                    this->timeDisplayArea.left + (numberOfVisibleButtons * this->buttonWidth),
                    this->timeDisplayArea.bottom,
                    this->timeDisplayArea.left + (numberOfVisibleButtons * this->buttonWidth) + this->buttonWidth,
                    this->timeDisplayArea.bottom + this->rowHeight
                };
                numberOfVisibleButtons++;
            }

            // Close button
            this->closeClickspotDisplayArea = {
                this->timeDisplayArea.right,
                this->timeDisplayArea.top,
                this->timeDisplayArea.right + this->buttonWidth,
                this->timeDisplayArea.bottom
            };

            // Reset button
            this->resetDisplayArea = {
                this->timeDisplayArea.right,
                this->timeDisplayArea.bottom,
                this->timeDisplayArea.right + this->buttonWidth,
                this->timeDisplayArea.bottom + this->rowHeight
            };
        }
    }  // namespace Countdown
}  // namespace UKControllerPlugin
