#include "CountdownRenderer.h"
#include "CountdownTimer.h"
#include "TimerConfigurationManager.h"
#include "euroscope/EuroscopeRadarLoopbackInterface.h"
#include "graphics/GdiGraphicsInterface.h"
#include "graphics/GdiplusBrushes.h"

using UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface;
using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPlugin::Windows::GdiGraphicsInterface;
using UKControllerPlugin::Windows::GdiplusBrushes;
namespace UKControllerPlugin::Countdown {

    CountdownRenderer::CountdownRenderer(
        CountdownTimer& countdownModule,
        const TimerConfigurationManager& configManager,
        int functionsClickspotId,
        int timeDisplayClickspotId,
        int closeClickspotId,
        int toogleCallbackFunctionId,
        const GdiplusBrushes& brushes)
        : countdownModule(countdownModule), brushes(brushes), configManager(configManager),
          functionsClickspotId(functionsClickspotId), timeDisplayClickspotId(timeDisplayClickspotId),
          closeClickspotId(closeClickspotId), toggleCallbackFunctionId(toogleCallbackFunctionId)
    {
    }

    /*
        Called when the ASR is first loaded, configures the renderer with the per-ASR settings.
    */
    void CountdownRenderer::AsrLoadedEvent(UserSetting& userSetting)
    {
        // Visibility
        this->visible = userSetting.GetBooleanEntry(this->userSettingVisible, true);

        // Position
        this->ShiftAllElements(
            userSetting.GetIntegerEntry(this->userSettingPosX, DEFAULT_POSITION),
            userSetting.GetIntegerEntry(this->userSettingPosY, DEFAULT_POSITION));
    }

    /*
        Called when the ASR is closed, saves rendering options to the ASR for next time.
    */
    void CountdownRenderer::AsrClosingEvent(UserSetting& userSetting)
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
    void
    CountdownRenderer::FunctionButtonClick(const std::string& function, EuroscopeRadarLoopbackInterface& radarScreen)
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

        if (timerMatches.empty()) {
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
    auto CountdownRenderer::GetConfigurationMenuItem() const -> PopupMenuItem
    {
        PopupMenuItem returnVal;
        returnVal.firstValue = this->menuItemDescription;
        returnVal.secondValue = "";
        returnVal.callbackFunctionId = this->toggleCallbackFunctionId;
        returnVal.checked =
            this->visible ? EuroScopePlugIn::POPUP_ELEMENT_CHECKED : EuroScopePlugIn::POPUP_ELEMENT_UNCHECKED;
        returnVal.disabled = false;
        returnVal.fixedPosition = false;
        return returnVal;
    }

    auto CountdownRenderer::GetTimerButtonArea(unsigned int configId) const -> RECT
    {
        return this->timerButtonAreas.count(configId) == 1 ? this->timerButtonAreas.at(configId) : RECT{};
    }

    /*
        Returns the display area of the reset clickspot
    */
    auto CountdownRenderer::GetResetDisplayArea() const -> RECT
    {
        return this->resetDisplayArea;
    }

    /*
        Return the display area of the close clickspot
    */
    auto CountdownRenderer::GetCloseClickspotDisplayArea() const -> RECT
    {
        return this->closeClickspotDisplayArea;
    }

    /*
        Returns the display area of the timer box
    */
    auto CountdownRenderer::GetTimeDisplayArea() const -> RECT
    {
        return this->timeDisplayArea;
    }

    /*
        Returns the correct time string, on the number of seconds remaining.
    */
    auto CountdownRenderer::GetCurrentTimeString(int secondsRemaining) -> std::wstring
    {
        // Work out the numbers
        int minutes = secondsRemaining / SECONDS_IN_MINUTE;
        int seconds = secondsRemaining % SECONDS_IN_MINUTE;

        std::wstringstream wss;

        // Create string
        if (seconds < TEN_SECONDS) {
            wss << minutes << ":0" << seconds << '\0';
        } else {
            wss << minutes << ":" << seconds << '\0';
        }

        return wss.str();
    }

    /*
        Returns the correct colour with which to display the time remaining.
    */
    auto CountdownRenderer::GetTimeColour(int secondsRemaining) -> const Gdiplus::Brush&
    {
        // Colour the text depending on how long is left, if we get 0, that means timer is up.
        if (secondsRemaining == 0) {
            static Gdiplus::SolidBrush textBrush(this->brushes.text);
            return textBrush;
        }

        if (secondsRemaining > WARNING_SECONDS) {
            static Gdiplus::SolidBrush timerGreen(this->brushes.timerGreen);
            return timerGreen;
        }

        if (secondsRemaining > CRITICAL_SECONDS) {
            static Gdiplus::SolidBrush timerYellow(this->brushes.timerYellow);
            return timerYellow;
        }

        static Gdiplus::SolidBrush timerRed(this->brushes.timerRed);
        return timerRed;
    }

    /*
        Returns true if the module should be rendered, false otherwise.
    */
    auto CountdownRenderer::IsVisible() const -> bool
    {
        return this->visible;
    }

    /*
        If someone clicks on the timer, handle it.
    */
    void CountdownRenderer::LeftClick(
        EuroscopeRadarLoopbackInterface& radarScreen,
        int objectId,
        const std::string& objectDescription,
        POINT mousePos,
        RECT itemArea)
    {
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
    void CountdownRenderer::Render(GdiGraphicsInterface& graphics, EuroscopeRadarLoopbackInterface& radarScreen)
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
    void CountdownRenderer::RenderButtons(GdiGraphicsInterface& graphics, EuroscopeRadarLoopbackInterface& radarScreen)
    {
        // Render the buttons
        int renderedButtons = 0;
        for (auto it = this->configManager.cbegin(); it != this->configManager.cend(); ++it) {
            // Dont account for timer if not enabled
            if (!it->timerEnabled) {
                continue;
            }

            graphics.FillRect(this->timerButtonAreas[it->timerId], Gdiplus::SolidBrush(this->brushes.background));
            graphics.DrawRect(this->timerButtonAreas[it->timerId], Gdiplus::Pen(this->brushes.border));
            graphics.DrawString(
                std::to_wstring(it->timerDuration), this->timerButtonAreas[it->timerId], Gdiplus::SolidBrush(this->brushes.text));
            radarScreen.RegisterScreenObject(
                this->functionsClickspotId,
                "timer" + std::to_string(it->timerId) + "Toggle",
                this->timerButtonAreas[it->timerId],
                false);

            renderedButtons++;
        }

        // Fill in gaps where not enough buttons
        if (renderedButtons < 2) {
            RECT spaceToFill = {
                this->timeDisplayArea.left + (this->buttonWidth * renderedButtons),
                this->timeDisplayArea.bottom,
                this->timeDisplayArea.right,
                this->timeDisplayArea.bottom + this->rowHeight};
            graphics.FillRect(spaceToFill, Gdiplus::SolidBrush(this->brushes.background));
            graphics.DrawRect(spaceToFill, Gdiplus::Pen(this->brushes.border));
        }

        // The close clickspot
        graphics.FillRect(this->closeClickspotDisplayArea, Gdiplus::SolidBrush(this->brushes.background));
        graphics.DrawRect(this->closeClickspotDisplayArea, Gdiplus::Pen(this->brushes.border));
        graphics.DrawString(L"X", this->closeClickspotDisplayArea, Gdiplus::SolidBrush(this->brushes.text));
        radarScreen.RegisterScreenObject(this->closeClickspotId, "", this->closeClickspotDisplayArea, false);

        // The reset button.
        graphics.FillRect(this->resetDisplayArea, Gdiplus::SolidBrush(this->brushes.background));
        graphics.DrawRect(this->resetDisplayArea, Gdiplus::Pen(this->brushes.border));
        graphics.DrawString(L"R", this->resetDisplayArea, Gdiplus::SolidBrush(this->brushes.text));
        radarScreen.RegisterScreenObject(this->functionsClickspotId, "R", this->resetDisplayArea, false);
    }

    /*
        Renders the time display.
    */
    void
    CountdownRenderer::RenderTimeDisplay(GdiGraphicsInterface& graphics, EuroscopeRadarLoopbackInterface& radarScreen)
    {
        // The time display
        graphics.FillRect(this->timeDisplayArea, Gdiplus::SolidBrush(this->brushes.background));
        graphics.DrawRect(this->timeDisplayArea, Gdiplus::Pen(this->brushes.border));
        radarScreen.RegisterScreenObject(this->timeDisplayClickspotId, "", this->timeDisplayArea, true);

        // Get the seconds remaining from the Countdown class and use that to draw the time to the screen.
        int secondsRemaining = this->countdownModule.GetSecondsRemaining();
        graphics.DrawString(
            this->GetCurrentTimeString(secondsRemaining), this->timeDisplayArea, this->GetTimeColour(secondsRemaining));
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
    void CountdownRenderer::ResetPosition()
    {
        this->Move({DEFAULT_POSITION, DEFAULT_POSITION, DEFAULT_POSITION, DEFAULT_POSITION}, "");
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
            this->timeDisplayArea.right =
                this->timeDisplayArea.left +
                (this->buttonWidth * (static_cast<LONG>(this->configManager.CountEnabledTimers())));
        }

        // Iterate the config and work out display areas for the visible timer buttons
        int numberOfVisibleButtons = 0;
        for (auto it = this->configManager.cbegin(); it != this->configManager.cend(); ++it) {
            // Dont account for timer if not enabled
            if (!it->timerEnabled) {
                continue;
            }

            this->timerButtonAreas[it->timerId] = {
                this->timeDisplayArea.left + (numberOfVisibleButtons * this->buttonWidth),
                this->timeDisplayArea.bottom,
                this->timeDisplayArea.left + (numberOfVisibleButtons * this->buttonWidth) + this->buttonWidth,
                this->timeDisplayArea.bottom + this->rowHeight};
            numberOfVisibleButtons++;
        }

        // Close button
        this->closeClickspotDisplayArea = {
            this->timeDisplayArea.right,
            this->timeDisplayArea.top,
            this->timeDisplayArea.right + this->buttonWidth,
            this->timeDisplayArea.bottom};

        // Reset button
        this->resetDisplayArea = {
            this->timeDisplayArea.right,
            this->timeDisplayArea.bottom,
            this->timeDisplayArea.right + this->buttonWidth,
            this->timeDisplayArea.bottom + this->rowHeight};
    }
} // namespace UKControllerPlugin::Countdown
