#include "pch/stdafx.h"
#include "historytrail/HistoryTrailRenderer.h"
#include "euroscope/EuroscopeRadarLoopbackInterface.h"
#include "graphics/GdiGraphicsInterface.h"
#include "euroscope/UserSetting.h"
#include "historytrail/AircraftHistoryTrail.h"
#include "historytrail/HistoryTrailRepository.h"
#include "historytrail/HistoryTrailData.h"
#include "windows/WinApiInterface.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"

using UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface;
using UKControllerPlugin::Windows::GdiGraphicsInterface;
using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPlugin::HistoryTrail::HistoryTrailRepository;
using UKControllerPlugin::HistoryTrail::HistoryTrailData;
using UKControllerPlugin::Windows::WinApiInterface;
using UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;

namespace UKControllerPlugin {
    namespace HistoryTrail {

        HistoryTrailRenderer::HistoryTrailRenderer(
            const HistoryTrailRepository & trails,
            EuroscopePluginLoopbackInterface & plugin,
            WinApiInterface & winApi,
            int toggleCallbackFunctionId
        )
            : trails(trails), winApi(winApi), toggleCallbackFunctionId(toggleCallbackFunctionId), plugin(plugin)
        {
            this->pen = std::make_unique<Gdiplus::Pen>(Gdiplus::Color(255, 255, 255, 255));
        }

        /*
            Load settings from the ASR.
        */
        void HistoryTrailRenderer::AsrLoadedEvent(UserSetting & userSetting)
        {
            this->visible = userSetting.GetBooleanEntry(this->visibleUserSettingKey, true);
            this->historyTrailType = userSetting.GetIntegerEntry(this->trailTypeUserSettingKey, this->trailTypeDiamond);
            this->historyTrailDotSize = userSetting.GetIntegerEntry(this->dotSizeUserSettingKey, 4);
            this->historyTrailDotSizeFloat = static_cast<float>(this->historyTrailDotSize);
            this->degradingTrails = userSetting.GetBooleanEntry(this->degradingUserSettingKey, true);
            this->fadingTrails = userSetting.GetBooleanEntry(this->fadingUserSettingKey, true);
            this->historyTrailLength = userSetting.GetIntegerEntry(this->trailLengthUserSettingKey, 15);
            this->antialiasedTrails = userSetting.GetBooleanEntry(this->antialiasUserSettingKey, true);
            this->startColour = std::make_unique<Gdiplus::Color>(255, 255, 130, 20);
            this->startColour->SetFromCOLORREF(
                userSetting.GetColourEntry(this->trailColourUserSettingKey, RGB(255, 130, 20))
            );
            this->alphaPerDot = 255 / this->historyTrailLength;
            this->minimumDisplayAltitude = userSetting.GetIntegerEntry(
                this->minAltitudeFilterUserSettingKey,
                this->defaultMinAltitude
            );
            this->maximumDisplayAltitude = userSetting.GetIntegerEntry(
                this->maxAltitudeFilterUserSettingKey,
                this->defaultMaxAltitude
            );
        }

        /*
            Save settings to the ASR.
        */
        void HistoryTrailRenderer::AsrClosingEvent(UserSetting & userSetting)
        {
            userSetting.Save(this->visibleUserSettingKey, this->visibleUserSettingDescription, this->visible);
            userSetting.Save(
                this->trailTypeUserSettingKey,
                this->trailTypeUserSettingDescription,
                this->historyTrailType
            );
            userSetting.Save(
                this->dotSizeUserSettingKey,
                this->dotSizeUserSettingDescription,
                this->historyTrailDotSize
            );
            userSetting.Save(
                this->degradingUserSettingKey,
                this->degradingUserSettingDescription,
                this->degradingTrails
            );
            userSetting.Save(this->fadingUserSettingKey, this->fadingUserSettingDescription, this->fadingTrails);
            userSetting.Save(
                this->trailLengthUserSettingKey,
                this->trailLengthUserSettingDescription,
                this->historyTrailLength
            );
            userSetting.Save(
                this->trailColourUserSettingKey,
                this->trailColourUserSettingDescription,
                this->startColour->ToCOLORREF()
            );
            userSetting.Save(
                this->antialiasUserSettingKey,
                this->antialiasUserSettingDescription,
                this->antialiasedTrails
            );
            userSetting.Save(
                this->minAltitudeFilterUserSettingKey,
                this->minAltitudeFilterUserSettingDescription,
                this->minimumDisplayAltitude
            );
            userSetting.Save(
                this->maxAltitudeFilterUserSettingKey,
                this->maxAltitudeFilterUserSettingDescription,
                this->maximumDisplayAltitude
            );
        }

        /*
            Called when the configuration menu item is selected. Create dialog.
        */
        void HistoryTrailRenderer::Configure(std::string subject)
        {
            COLORREF newColour = RGB(
                this->startColour->GetRed(),
                this->startColour->GetGreen(),
                this->startColour->GetBlue()
            );

            HistoryTrailData data;
            data.fade = &this->fadingTrails;
            data.degrade = &this->degradingTrails;
            data.antiAlias = &this->antialiasedTrails;
            data.type = &this->historyTrailType;
            data.length = &this->historyTrailLength;
            data.colour = &newColour;
            data.dotSize = &this->historyTrailDotSize;
            data.maxAltitude = &this->maximumDisplayAltitude;
            data.minAltitude = &this->minimumDisplayAltitude;

            this->winApi.OpenDialog(this->configurationDialogId, NULL, (LPARAM) &data);

            this->historyTrailDotSizeFloat = static_cast<float>(this->historyTrailDotSize);
            this->startColour->SetFromCOLORREF(newColour);
            this->alphaPerDot = 255 / this->historyTrailLength;
        }

        /*
            Returns the alpha decrease per dot.
        */
        int HistoryTrailRenderer::GetAlphaPerDot(void) const
        {
            return this->alphaPerDot;
        }

        /*
            Returns antialiasing mode.
        */
        bool HistoryTrailRenderer::GetAntiAliasedTrails(void) const
        {
            return this->antialiasedTrails;
        }

        /*
            Draws a single dot to the screen.
        */
        void HistoryTrailRenderer::DrawDot(
            GdiGraphicsInterface & graphics,
            Gdiplus::Pen & pen,
            const Gdiplus::RectF & area
        ) {
            if (this->historyTrailType == this->trailTypeDiamond) {
                graphics.DrawDiamond(area, pen);
            } else if (this->historyTrailType == this->trailTypeCircle) {
                graphics.DrawCircle(area, pen);
            } else {
                graphics.DrawRect(area, pen);
            }
        }

        /*
            Returns the configuration menu item.
        */
        PopupMenuItem HistoryTrailRenderer::GetConfigurationMenuItem(void) const
        {
            PopupMenuItem returnVal;
            returnVal.firstValue = this->menuItemDescription;
            returnVal.secondValue = "";
            returnVal.callbackFunctionId = this->toggleCallbackFunctionId;
            returnVal.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
            returnVal.disabled = false;
            returnVal.fixedPosition = false;
            return returnVal;
        }

        /*
            Return whether to degrade trails.
        */
        bool HistoryTrailRenderer::GetDegradingTrails(void) const
        {
            return this->degradingTrails;
        }

        /*
            Return whether to fade trails.
        */
        bool HistoryTrailRenderer::GetFadingTrails(void) const
        {
            return this->fadingTrails;
        }

        /*
            Returns the length of trail to be rendered.
        */
        int HistoryTrailRenderer::GetHistoryTrailLength(void) const
        {
            return this->historyTrailLength;
        }

        /*
            Returns the trail type.
        */
        int HistoryTrailRenderer::GetHistoryTrailType(void) const
        {
            return this->historyTrailType;
        }

        /*
            Returns the dot size.
        */
        int HistoryTrailRenderer::GetHistoryTrailDotSize(void) const
        {
            return this->historyTrailDotSize;
        }

        /*
            Get the maximum display altitude
        */
        int HistoryTrailRenderer::GetMaximumAltitudeFilter(void) const
        {
            return this->maximumDisplayAltitude;
        }

        /*
            Get the minimum display altitude
        */
        int HistoryTrailRenderer::GetMinimumAltitudeFilter(void) const
        {
            return this->minimumDisplayAltitude;
        }

        Gdiplus::Color & HistoryTrailRenderer::GetTrailColour(void) const
        {
            return *this->startColour;
        }

        /*
            Returns whether or not history trails should be rendered.
        */
        bool HistoryTrailRenderer::IsVisible(void) const
        {
            return this->visible;
        }

        /*
            They aren't clickable.
        */
        void HistoryTrailRenderer::LeftClick(
            int objectId,
            std::string objectDescription,
            EuroscopeRadarLoopbackInterface & radarScreen
        ) {

        }

        /*
            These don't get moved.
        */
        void HistoryTrailRenderer::Move(RECT position)
        {

        }

        /*
            Processes Euroscope dot commands.
        */
        bool HistoryTrailRenderer::ProcessCommand(std::string command)
        {
            if (command == this->dotCommand) {
                this->Configure(command);
                return true;
            }

            return false;
        }

        void HistoryTrailRenderer::Render(
            GdiGraphicsInterface & graphics,
            EuroscopeRadarLoopbackInterface & radarScreen
        ) {
            Gdiplus::Color currentColourArgb = *this->startColour;
            this->pen->SetColor(currentColourArgb);

            // Anti aliasing
            graphics.SetAntialias((this->antialiasedTrails) ? true : false);

            // The dot we are to make.
            Gdiplus::RectF dot;

            // The amount that we'll reduce the history dot by on all sides each time.
            Gdiplus::REAL reducePerDot = (this->historyTrailDotSizeFloat / this->historyTrailLength) / 2;

            int roundNumber;
            // Loop through the history trails.
            for (
                HistoryTrailRepository::const_iterator aircraft = this->trails.cbegin();
                aircraft != this->trails.cend();
                ++aircraft
            ) {

                try {
                    std::shared_ptr<EuroScopeCRadarTargetInterface> radarTarget =
                        this->plugin.GetRadarTargetForCallsign(aircraft->second->GetCallsign());

                    // If they're not going fast enough or are off the screen, don't display the trail.
                    if (radarScreen.GetGroundspeedForCallsign(aircraft->second->GetCallsign()) < this->minimumSpeed ||
                        radarScreen.PositionOffScreen(*aircraft->second->GetTrail().begin()) ||
                        radarTarget->GetFlightLevel() < this->minimumDisplayAltitude ||
                        radarTarget->GetFlightLevel() > this->maximumDisplayAltitude
                    ) {
                        continue;
                    }
                }
                catch (std::invalid_argument) {
                    // No radar target, continue.
                    continue;
                }

                // Round number used to govern fade and degrade, reset the colour
                roundNumber = 0;
                currentColourArgb = *this->startColour;

                // Reset the dot height and width
                dot.Width = this->historyTrailDotSizeFloat;
                dot.Height = this->historyTrailDotSizeFloat;

                // Loop through the points and display.
                for (
                    std::deque<EuroScopePlugIn::CPosition>::const_iterator position =
                        aircraft->second->GetTrail().begin();
                    position != aircraft->second->GetTrail().end();
                    ++position
                ) {

                    POINT dotCoordinates = radarScreen.ConvertCoordinateToScreenPoint(*position);
                    // Adjust the dot size and position as required
                    if (this->degradingTrails) {
                        dot.X = dotCoordinates.x - (this->historyTrailDotSizeFloat / 2) + (roundNumber * reducePerDot);
                        dot.Y = dotCoordinates.y - (this->historyTrailDotSizeFloat / 2) + (roundNumber * reducePerDot);
                        dot.Width = dot.Width - reducePerDot;
                        dot.Height = dot.Height - reducePerDot;
                    } else {
                        dot.X = dotCoordinates.x - (this->historyTrailDotSizeFloat / 2);
                        dot.Y = dotCoordinates.y - (this->historyTrailDotSizeFloat / 2);
                    }

                    // Draw the dot
                    this->DrawDot(
                        graphics,
                        *this->pen,
                        dot
                    );

                    // If the trails are set to fade, reduce the alpha value for the next run
                    if (this->fadingTrails) {
                        currentColourArgb = Gdiplus::Color(
                            255 - (roundNumber * this->alphaPerDot),
                            currentColourArgb.GetRed(),
                            currentColourArgb.GetGreen(),
                            currentColourArgb.GetBlue()
                        );
                        this->pen->SetColor(currentColourArgb);
                    }

                    // If we've done enough dots, we stop.
                    if (roundNumber == this->historyTrailLength) {
                        break;
                    }

                    roundNumber++;
                }
            }
        }

        /*
            They aren't clickable.
        */
        void HistoryTrailRenderer::RightClick(
            int objectId,
            std::string objectDescription,
            EuroscopeRadarLoopbackInterface & radarScreen
        ) {
        }
    }  // namespace HistoryTrail
}  // namespace UKControllerPlugin
