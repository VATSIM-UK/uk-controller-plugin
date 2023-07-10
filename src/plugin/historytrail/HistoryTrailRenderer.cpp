#include "AircraftHistoryTrail.h"
#include "HistoryTrailData.h"
#include "HistoryTrailRenderer.h"
#include "HistoryTrailRepository.h"
#include "dialog/DialogManager.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "euroscope/EuroscopeRadarLoopbackInterface.h"
#include "euroscope/UserSetting.h"
#include "graphics/GdiGraphicsInterface.h"

using UKControllerPlugin::Dialog::DialogManager;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;
using UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface;
using UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface;
using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::HistoryTrail::HistoryTrailData;
using UKControllerPlugin::HistoryTrail::HistoryTrailRepository;
using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPlugin::Windows::GdiGraphicsInterface;

namespace UKControllerPlugin::HistoryTrail {

    HistoryTrailRenderer::HistoryTrailRenderer(
        const HistoryTrailRepository& trails,
        EuroscopePluginLoopbackInterface& plugin,
        const DialogManager& dialogManager,
        int toggleCallbackFunctionId)
        : toggleCallbackFunctionId(toggleCallbackFunctionId), dialogManager(dialogManager), plugin(plugin),
          trails(trails)
    {
        this->pen = std::make_unique<Gdiplus::Pen>(Gdiplus::Color(255, 255, 255, 255));
        this->brush = std::make_unique<Gdiplus::SolidBrush>(Gdiplus::Color(255, 255, 255, 255));
    }

    /*
        Load settings from the ASR.
    */
    void HistoryTrailRenderer::AsrLoadedEvent(UserSetting& userSetting)
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
            userSetting.GetColourEntry(this->trailColourUserSettingKey, RGB(255, 130, 20)));
        this->alphaPerDot = 255 / this->historyTrailLength;
        this->minimumDisplayAltitude =
            userSetting.GetIntegerEntry(this->minAltitudeFilterUserSettingKey, this->defaultMinAltitude);
        this->maximumDisplayAltitude =
            userSetting.GetIntegerEntry(this->maxAltitudeFilterUserSettingKey, this->defaultMaxAltitude);
        this->filledDots = userSetting.GetBooleanEntry(this->dotFillUserSettingKey, false);
        this->rotatedDots = userSetting.GetBooleanEntry(this->dotRotateUserSettingKey, false);
        this->reducePerDot = (this->historyTrailDotSizeFloat / this->historyTrailLength) / 2;

        // Load the dot drawing function
        this->drawDot = this->GetDoDotFunction();
    }

    /*
        Save settings to the ASR.
    */
    void HistoryTrailRenderer::AsrClosingEvent(UserSetting& userSetting)
    {
        userSetting.Save(this->visibleUserSettingKey, this->visibleUserSettingDescription, this->visible);
        userSetting.Save(this->trailTypeUserSettingKey, this->trailTypeUserSettingDescription, this->historyTrailType);
        userSetting.Save(this->dotSizeUserSettingKey, this->dotSizeUserSettingDescription, this->historyTrailDotSize);
        userSetting.Save(this->degradingUserSettingKey, this->degradingUserSettingDescription, this->degradingTrails);
        userSetting.Save(this->fadingUserSettingKey, this->fadingUserSettingDescription, this->fadingTrails);
        userSetting.Save(
            this->trailLengthUserSettingKey, this->trailLengthUserSettingDescription, this->historyTrailLength);
        userSetting.Save(
            this->trailColourUserSettingKey, this->trailColourUserSettingDescription, this->startColour->ToCOLORREF());
        userSetting.Save(this->antialiasUserSettingKey, this->antialiasUserSettingDescription, this->antialiasedTrails);
        userSetting.Save(
            this->minAltitudeFilterUserSettingKey,
            this->minAltitudeFilterUserSettingDescription,
            this->minimumDisplayAltitude);
        userSetting.Save(
            this->maxAltitudeFilterUserSettingKey,
            this->maxAltitudeFilterUserSettingDescription,
            this->maximumDisplayAltitude);
        userSetting.Save(this->dotFillUserSettingKey, this->dotFillUserSettingDescription, this->filledDots);
        userSetting.Save(this->dotRotateUserSettingKey, this->dotRotateUserSettingDescription, this->rotatedDots);
    }

    /*
        Called when the configuration menu item is selected. Create dialog.
    */
    void HistoryTrailRenderer::Configure(int functionId, std::string subject, RECT screenObjectArea)
    {
        COLORREF newColour =
            RGB(this->startColour->GetRed(), this->startColour->GetGreen(), this->startColour->GetBlue());

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
        data.filledDots = &this->filledDots;
        data.rotate = &this->rotatedDots;

        this->dialogManager.OpenDialog(IDD_HISTORY_TRAIL, reinterpret_cast<LPARAM>(&data));

        this->historyTrailDotSizeFloat = static_cast<float>(this->historyTrailDotSize);
        this->startColour->SetFromCOLORREF(newColour);
        this->alphaPerDot = 255 / this->historyTrailLength;
        this->reducePerDot = (this->historyTrailDotSizeFloat / this->historyTrailLength) / 2;

        // Change the rendering function
        this->drawDot = this->GetDoDotFunction();
    }

    /*
        Returns the alpha decrease per dot.
    */
    auto HistoryTrailRenderer::GetAlphaPerDot() const -> int
    {
        return this->alphaPerDot;
    }

    /*
        Returns antialiasing mode.
    */
    auto HistoryTrailRenderer::GetAntiAliasedTrails() const -> bool
    {
        return this->antialiasedTrails;
    }

    /*
        Returns the configuration menu item.
    */
    auto HistoryTrailRenderer::GetConfigurationMenuItem() const -> PopupMenuItem
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
    auto HistoryTrailRenderer::GetDegradingTrails() const -> bool
    {
        return this->degradingTrails;
    }

    /*
        Return whether to fade trails.
    */
    auto HistoryTrailRenderer::GetFadingTrails() const -> bool
    {
        return this->fadingTrails;
    }

    /*
        Returns the length of trail to be rendered.
    */
    auto HistoryTrailRenderer::GetHistoryTrailLength() const -> int
    {
        return this->historyTrailLength;
    }

    /*
        Returns the trail type.
    */
    auto HistoryTrailRenderer::GetHistoryTrailType() const -> int
    {
        return this->historyTrailType;
    }

    /*
        Returns the dot size.
    */
    auto HistoryTrailRenderer::GetHistoryTrailDotSize() const -> int
    {
        return this->historyTrailDotSize;
    }

    /*
        Get the maximum display altitude
    */
    auto HistoryTrailRenderer::GetMaximumAltitudeFilter() const -> int
    {
        return this->maximumDisplayAltitude;
    }

    /*
        Get the minimum display altitude
    */
    auto HistoryTrailRenderer::GetMinimumAltitudeFilter() const -> int
    {
        return this->minimumDisplayAltitude;
    }

    auto HistoryTrailRenderer::GetFilledDots() const -> bool
    {
        return this->filledDots;
    }

    auto HistoryTrailRenderer::GetRotatedDots() const -> bool
    {
        return this->rotatedDots;
    }

    auto HistoryTrailRenderer::GetTrailColour() const -> Gdiplus::Color&
    {
        return *this->startColour;
    }

    /*
        Returns whether or not history trails should be rendered.
    */
    auto HistoryTrailRenderer::IsVisible() const -> bool
    {
        return this->visible;
    }

    /*
        These don't get moved.
    */
    void HistoryTrailRenderer::Move(RECT position, std::string objectDescription)
    {
    }

    /*
        Processes Euroscope dot commands.
    */
    auto HistoryTrailRenderer::ProcessCommand(std::string command) -> bool
    {
        if (command == this->dotCommand) {
            this->Configure(0, command, {});
            return true;
        }

        return false;
    }

    void HistoryTrailRenderer::Render(GdiGraphicsInterface& graphics, EuroscopeRadarLoopbackInterface& radarScreen)
    {
        Gdiplus::Color currentColourArgb = *this->startColour;
        this->pen->SetColor(currentColourArgb);
        this->brush->SetColor(currentColourArgb);

        // Anti aliasing
        graphics.SetAntialias(this->antialiasedTrails);

        // The dot we are to make.
        Gdiplus::RectF dot;

        int roundNumber;
        // Loop through the history trails.

        std::shared_ptr<EuroScopeCRadarTargetInterface> radarTarget;
        for (const auto& aircraft : trails.trailData) {
            // Check the radar target exists
            const auto& callsign = aircraft->GetCallsign();
            radarTarget = this->plugin.GetRadarTargetForCallsign(callsign);
            if (!radarTarget) {
                continue;
            }

            const auto& trail = aircraft->GetTrail();
            // If there's one or fewer dots, they're not going fast enough or are off the screen, don't display the
            // trail.
            if (trail.size() < 2 || radarScreen.GetGroundspeedForCallsign(callsign) < this->minimumSpeed ||
                radarScreen.PositionOffScreen(aircraft->GetTrail().rbegin()->position) ||
                radarTarget->GetFlightLevel() < this->minimumDisplayAltitude ||
                radarTarget->GetFlightLevel() > this->maximumDisplayAltitude) {
                continue;
            }

            // Round number used to govern fade and degrade, reset the colour
            roundNumber = 0;
            currentColourArgb = *this->startColour;
            this->pen->SetColor(currentColourArgb);
            this->brush->SetColor(currentColourArgb);

            // Reset the dot height and width
            dot.Width = this->historyTrailDotSizeFloat;
            dot.Height = this->historyTrailDotSizeFloat;

            // Loop through the points and display. The points are in reverse order, so we need to start at the end.
            for (auto position = ++trail.rbegin(); position != trail.rend(); ++position) {
                // Translate to screen location
                POINT dotCoordinates = radarScreen.ConvertCoordinateToScreenPoint(position->position);
                graphics.Translated(
                    static_cast<Gdiplus::REAL>(dotCoordinates.x),
                    static_cast<Gdiplus::REAL>(dotCoordinates.y),
                    [&graphics, this, &dot, &roundNumber, &position] {
                        // Adjust the dot size and position as required
                        if (this->degradingTrails) {
                            dot.X = -(this->historyTrailDotSizeFloat / 2) + (roundNumber * reducePerDot);
                            dot.Y = -(this->historyTrailDotSizeFloat / 2) + (roundNumber * reducePerDot);
                            dot.Width = dot.Width - reducePerDot;
                            dot.Height = dot.Height - reducePerDot;
                        } else {
                            dot.X = -(this->historyTrailDotSizeFloat / 2);
                            dot.Y = -(this->historyTrailDotSizeFloat / 2);
                        }

                        if (this->rotatedDots) {
                            graphics.Rotated(static_cast<Gdiplus::REAL>(position->heading), [&graphics, &dot, this]() {
                                this->drawDot(graphics, dot);
                            });
                        } else {
                            this->drawDot(graphics, dot);
                        }
                    });

                // If the trails are set to fade, reduce the alpha value for the next run
                if (this->fadingTrails) {
                    currentColourArgb = Gdiplus::Color(
                        255 - (roundNumber * this->alphaPerDot),
                        currentColourArgb.GetRed(),
                        currentColourArgb.GetGreen(),
                        currentColourArgb.GetBlue());
                    this->pen->SetColor(currentColourArgb);
                    this->brush->SetColor(currentColourArgb);
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
        History Trails cannot have their position reset. Do nothing.
    */
    void HistoryTrailRenderer::ResetPosition()
    {
    }

    auto HistoryTrailRenderer::GetDoDotFunction() const
        -> std::function<void(GdiGraphicsInterface&, const Gdiplus::RectF&)>
    {
        return this->filledDots && this->historyTrailType != this->trailTypeLine ? this->GetFillDotFunction()
                                                                                 : this->GetDrawDotFunction();
    }

    auto HistoryTrailRenderer::GetDrawDotFunction() const
        -> std::function<void(Windows::GdiGraphicsInterface&, const Gdiplus::RectF&)>
    {
        // Diamonds
        if (this->historyTrailType == this->trailTypeDiamond) {
            return [this](Windows::GdiGraphicsInterface& graphics, const Gdiplus::RectF& area) {
                graphics.DrawDiamond(area, *this->pen);
            };
        }

        // Circles
        if (this->historyTrailType == this->trailTypeCircle) {
            return [this](Windows::GdiGraphicsInterface& graphics, const Gdiplus::RectF& area) {
                graphics.DrawCircle(area, *this->pen);
            };
        }

        // Lines
        if (this->historyTrailType == this->trailTypeLine) {
            return [this](Windows::GdiGraphicsInterface& graphics, const Gdiplus::RectF& area) {
                graphics.DrawLine(
                    *this->pen,
                    Gdiplus::PointF{area.GetLeft(), area.GetBottom()},
                    Gdiplus::PointF{area.GetRight(), area.GetTop()});
            };
        }

        // Rectangles
        return [this](Windows::GdiGraphicsInterface& graphics, const Gdiplus::RectF& area) {
            graphics.DrawRect(area, *this->pen);
        };
    }

    auto HistoryTrailRenderer::GetFillDotFunction() const
        -> std::function<void(Windows::GdiGraphicsInterface&, const Gdiplus::RectF&)>
    {
        // Diamonds
        if (this->historyTrailType == this->trailTypeDiamond) {
            return [this](Windows::GdiGraphicsInterface& graphics, const Gdiplus::RectF& area) {
                graphics.FillDiamond(area, *this->brush);
            };
        }

        // Circles
        if (this->historyTrailType == this->trailTypeCircle) {
            return [this](Windows::GdiGraphicsInterface& graphics, const Gdiplus::RectF& area) {
                graphics.FillCircle(area, *this->brush);
            };
        }

        // Rects
        return [this](Windows::GdiGraphicsInterface& graphics, const Gdiplus::RectF& area) {
            graphics.FillRect(area, *this->brush);
        };
    }
} // namespace UKControllerPlugin::HistoryTrail
