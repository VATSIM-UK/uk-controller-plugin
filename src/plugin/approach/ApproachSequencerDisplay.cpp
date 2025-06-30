#include "AirfieldApproachOptions.h"
#include "ApproachSequence.h"
#include "ApproachSequencedAircraft.h"
#include "ApproachSequencer.h"
#include "ApproachSequencerDisplay.h"
#include "ApproachSequencerDisplayOptions.h"
#include "ApproachSequencerOptions.h"
#include "ApproachSpacingCalculator.h"
#include "components/Button.h"
#include "components/ClickableArea.h"
#include "components/CollapsibleWindowTitleBar.h"
#include "components/StandardButtons.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "graphics/FontManager.h"
#include "graphics/GdiGraphicsInterface.h"
#include "graphics/StringFormatManager.h"
#include "helper/HelperFunctions.h"
#include "list/PopupListInterface.h"
#include "number/NumberFormat.h"
#include "graphics/GdiplusBrushes.h"

using UKControllerPlugin::Components::CollapsibleWindowTitleBar;
using UKControllerPlugin::Number::To1Dp;
using UKControllerPlugin::Number::To1DpWide;
using UKControllerPlugin::Windows::GdiplusBrushes;

namespace UKControllerPlugin::Approach {

    ApproachSequencerDisplay::ApproachSequencerDisplay(
        ApproachSequencer& sequencer,
        ApproachSpacingCalculator& spacingCalculator,
        ApproachSequencerOptions& options,
        std::shared_ptr<ApproachSequencerDisplayOptions> displayOptions,
        std::shared_ptr<List::PopupListInterface> airfieldSelector,
        std::shared_ptr<List::PopupListInterface> callsignSelector,
        std::shared_ptr<List::PopupListInterface> targetSelector,
        std::shared_ptr<List::PopupListInterface> airfieldTargetSelector,
        std::shared_ptr<List::PopupListInterface> airfieldSeparationSelector,
        Euroscope::EuroscopePluginLoopbackInterface& plugin,
        const GdiplusBrushes& brushes,
        int screenObjectId)
        : sequencer(sequencer), spacingCalculator(spacingCalculator), options(options),
          displayOptions(std::move(displayOptions)), airfieldSelector(std::move(airfieldSelector)),
          callsignSelector(std::move(callsignSelector)), targetSelector(std::move(targetSelector)),
          airfieldTargetSelector(std::move(airfieldTargetSelector)),
          airfieldSeparationSelector(std::move(airfieldSeparationSelector)), plugin(plugin),
          brushes(brushes), screenObjectId(screenObjectId), titleBar(CollapsibleWindowTitleBar::Create(
                                              L"Approach Sequencer",
                                              titleBarArea,
                                              [this]() -> bool { return this->displayOptions->ContentCollapsed(); },
                                              screenObjectId)),
          airfieldClickspot(Components::ClickableArea::Create(
              this->airfieldTextArea, screenObjectId, AIRFIELD_SELECTOR_CLICKSPOT, false)),
          addClickspot(
              Components::ClickableArea::Create(this->addButton, screenObjectId, ADD_AIRCRAFT_CLICKSPOT, false)),
          airfieldTargetClickspot(Components::ClickableArea::Create(
              this->airfieldTargetTextArea, screenObjectId, AIRFIELD_TARGET_CLICKSPOT, false)),
          airfieldSeparationClickspot(Components::ClickableArea::Create(
              this->airfieldSeparationTextArea, screenObjectId, AIRFIELD_SEPARATION_CLICKSPOT, false))
    {
    }

    auto ApproachSequencerDisplay::IsVisible() const -> bool
    {
        return displayOptions->IsVisible();
    }

    void ApproachSequencerDisplay::Move(RECT position, std::string objectDescription)
    {
        displayOptions->Position({position.left, position.top});
    }

    void ApproachSequencerDisplay::Render(
        Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen)
    {
        graphics.Translated(
            displayOptions->Position().x, displayOptions->Position().y, [this, &graphics, &radarScreen]() {
                if (this->displayOptions->ContentCollapsed()) {
                    this->titleBar->DrawTheme(graphics, radarScreen, brushes);
                    return;
                }

                this->RenderBackground(graphics);
                this->titleBar->DrawTheme(graphics, radarScreen, brushes);
                this->RenderAirfield(graphics, radarScreen);
                this->RenderAddButton(graphics, radarScreen);
                this->RenderAirfieldTarget(graphics, radarScreen);
                this->RenderAirfieldSeparation(graphics, radarScreen);
                this->RenderDivider(graphics);
                this->RenderHeaders(graphics);
                this->RenderContent(graphics, radarScreen);
            });
    }

    void ApproachSequencerDisplay::ResetPosition()
    {
        this->Move({200, 200, 1, 2}, "");
    }

    void ApproachSequencerDisplay::LeftClick(
        Euroscope::EuroscopeRadarLoopbackInterface& radarScreen,
        int objectId,
        const std::string& objectDescription,
        POINT mousePos,
        RECT itemArea)
    {
        if (objectDescription == "collapseButton") {
            this->displayOptions->ToggleCollapsed();
            return;
        }

        if (objectDescription == "closeButton") {
            this->displayOptions->SetVisible(false);
            return;
        }

        if (objectDescription == AIRFIELD_SELECTOR_CLICKSPOT) {
            this->airfieldSelector->Trigger(mousePos);
            return;
        }

        if (objectDescription == ADD_AIRCRAFT_CLICKSPOT) {
            this->callsignSelector->Trigger(mousePos);
            return;
        }

        if (objectDescription.substr(0, 6) == "moveUp") {
            sequencer.MoveAircraftUp(displayOptions->Airfield(), objectDescription.substr(6));
            return;
        }

        if (objectDescription.substr(0, 8) == "moveDown") {
            sequencer.MoveAircraftDown(displayOptions->Airfield(), objectDescription.substr(8));
            return;
        }

        if (objectDescription.substr(0, 12) == "deleteButton") {
            sequencer.RemoveAircraftFromSequences(objectDescription.substr(12));
            return;
        }

        if (objectDescription.substr(0, 10) == "toggleDraw") {
            const std::string callsign = objectDescription.substr(10);
            sequencer.GetForAirfield(sequencer.AirfieldForAircraft(callsign)).Get(callsign)->ToggleDraw();
            return;
        }

        if (objectDescription.substr(0, 14) == "approachTarget") {
            auto flightplan = plugin.GetFlightplanForCallsign(objectDescription.substr(14));
            if (!flightplan) {
                return;
            }

            plugin.SetEuroscopeSelectedFlightplan(flightplan);
            targetSelector->Trigger(mousePos);
            return;
        }

        if (objectDescription == AIRFIELD_TARGET_CLICKSPOT) {
            airfieldTargetSelector->Trigger(mousePos);
            return;
        }

        if (objectDescription == AIRFIELD_SEPARATION_CLICKSPOT) {
            airfieldSeparationSelector->Trigger(mousePos);
            return;
        }
    }

    void ApproachSequencerDisplay::RenderAirfield(
        Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen)
    {
        graphics.DrawString(
            L"Airfield:",
            airfieldStaticArea,
            Gdiplus::SolidBrush(this->brushes.text),
            Graphics::StringFormatManager::Instance().GetLeftAlign(),
            Graphics::FontManager::Instance().GetDefault());

        graphics.DrawString(
            HelperFunctions::ConvertToWideString(
                displayOptions->Airfield().empty() ? "--" : displayOptions->Airfield()),
            airfieldTextArea,
            Gdiplus::SolidBrush(this->brushes.text),
            Graphics::StringFormatManager::Instance().GetLeftAlign(),
            Graphics::FontManager::Instance().GetDefault());
        this->airfieldClickspot->Apply(graphics, radarScreen);
    }

    void ApproachSequencerDisplay::RenderDivider(Windows::GdiGraphicsInterface& graphics)
    {
        graphics.DrawLine(Gdiplus::Pen(this->brushes.text), dividerLeft, dividerRight);
    }

    void ApproachSequencerDisplay::RenderHeaders(Windows::GdiGraphicsInterface& graphics)
    {
        graphics.DrawString(L"#", numberHeader, Gdiplus::SolidBrush(this->brushes.text));
        graphics.DrawString(L"Callsign", callsignHeader, Gdiplus::SolidBrush(this->brushes.text));
        graphics.DrawString(L"Target", targetHeader, Gdiplus::SolidBrush(this->brushes.text));
        graphics.DrawString(L"Actual", actualHeader, Gdiplus::SolidBrush(this->brushes.text));
        graphics.DrawString(L"Actions", actionsHeader, Gdiplus::SolidBrush(this->brushes.text));
    }

    void ApproachSequencerDisplay::RenderAddButton(
        Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen)
    {
        graphics.DrawRect(addButton, Gdiplus::Pen(this->brushes.text));
        graphics.DrawString(L"Add Aircraft", addButton, Gdiplus::SolidBrush(this->brushes.text));
        addClickspot->Apply(graphics, radarScreen);
    }

    void ApproachSequencerDisplay::RenderContent(
        Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen)
    {
        if (displayOptions->Airfield().empty()) {
            return;
        }

        auto aircraftToProcess = sequencer.GetForAirfield(displayOptions->Airfield()).First();
        Gdiplus::Rect numberRect{
            numberHeader.X, numberHeader.GetBottom() + INSETS, numberHeader.Width, numberHeader.Height};
        Gdiplus::Rect callsignRect{
            callsignHeader.X, callsignHeader.GetBottom() + INSETS, callsignHeader.Width, callsignHeader.Height};
        Gdiplus::Rect targetRect{
            targetHeader.X, targetHeader.GetBottom() + INSETS, targetHeader.Width, targetHeader.Height};
        Gdiplus::Rect actualRect{
            actualHeader.X, actualHeader.GetBottom() + INSETS, actualHeader.Width, actualHeader.Height};
        Gdiplus::Rect upButtonRect{
            actionsHeader.X + 30,
            actionsHeader.GetBottom() + INSETS,
            actionsHeader.Height - INSETS,
            actionsHeader.Height - INSETS};
        Gdiplus::Rect downButtonRect{
            upButtonRect.GetRight() + INSETS,
            actionsHeader.GetBottom() + INSETS,
            actionsHeader.Height - INSETS,
            actionsHeader.Height - INSETS};
        Gdiplus::Rect deleteButtonRect{
            downButtonRect.GetRight() + INSETS,
            actionsHeader.GetBottom() + INSETS,
            actionsHeader.Height - INSETS,
            actionsHeader.Height - INSETS};
        Gdiplus::Rect toggleButtonRect{
            deleteButtonRect.GetRight() + INSETS,
            actionsHeader.GetBottom() + INSETS,
            actionsHeader.Height - INSETS,
            actionsHeader.Height - INSETS};
        int sequenceNumber = 1;

        while (aircraftToProcess != nullptr) {
            graphics.DrawString(std::to_wstring(sequenceNumber), numberRect, Gdiplus::SolidBrush(this->brushes.text));
            graphics.DrawString(
                HelperFunctions::ConvertToWideString(aircraftToProcess->Callsign()), callsignRect, Gdiplus::SolidBrush(this->brushes.text));

            // The target distance / wake
            if (aircraftToProcess->Mode() == ApproachSequencingMode::WakeTurbulence) {
                graphics.DrawString(L"Wake", targetRect, Gdiplus::SolidBrush(this->brushes.text));
            } else {
                graphics.DrawString(To1DpWide(aircraftToProcess->ExpectedDistance()), targetRect, Gdiplus::SolidBrush(this->brushes.text));
            }
            Components::ClickableArea::Create(
                targetRect, screenObjectId, "approachTarget" + aircraftToProcess->Callsign(), false)
                ->Apply(graphics, radarScreen);

            double requiredSpacing = spacingCalculator.Calculate(displayOptions->Airfield(), *aircraftToProcess);
            if (requiredSpacing == spacingCalculator.NoSpacing()) {
                graphics.DrawString(L"--", actualRect, Gdiplus::SolidBrush(this->brushes.text));
            } else {
                graphics.DrawString(To1DpWide(requiredSpacing), actualRect, Gdiplus::SolidBrush(this->brushes.text));
            }

            auto upButton = Components::Button::Create(
                upButtonRect,
                screenObjectId,
                "moveUp" + aircraftToProcess->Callsign(),
                Components::UpArrow(this->brushes.text));
            upButton->Draw(graphics, radarScreen);

            auto downButton = Components::Button::Create(
                downButtonRect,
                screenObjectId,
                "moveDown" + aircraftToProcess->Callsign(),
                Components::DownArrow(this->brushes.text));
            downButton->Draw(graphics, radarScreen);

            auto deleteButton = Components::Button::Create(
                deleteButtonRect,
                screenObjectId,
                "deleteButton" + aircraftToProcess->Callsign(),
                Components::DeleteButton(this->brushes.text));
            deleteButton->Draw(graphics, radarScreen);

            auto toggleButton = Components::Button::Create(
                toggleButtonRect,
                screenObjectId,
                "toggleDraw" + aircraftToProcess->Callsign(),
                [&aircraftToProcess, &radarScreen, this](
                    Windows::GdiGraphicsInterface& graphics, const Gdiplus::Rect& area) {
                    Gdiplus::Rect drawArea = {0, 0, area.Width, area.Height};
                    graphics.FillCircle(drawArea, Gdiplus::SolidBrush(this->brushes.text));
                    if (!aircraftToProcess->ShouldDraw()) {
                        Components::Button::Create(
                            drawArea,
                            screenObjectId,
                            "toggleDraw" + aircraftToProcess->Callsign(),
                            Components::DeleteButton(this->brushes.background))
                            ->Draw(graphics, radarScreen);
                    }
                });
            toggleButton->Draw(graphics, radarScreen);

            sequenceNumber++;
            aircraftToProcess = aircraftToProcess->Next();
            numberRect.Y = numberRect.GetBottom();
            callsignRect.Y = callsignRect.GetBottom();
            targetRect.Y = targetRect.GetBottom();
            actualRect.Y = actualRect.GetBottom();
            upButtonRect.Y = upButtonRect.GetBottom();
            downButtonRect.Y = downButtonRect.GetBottom();
            deleteButtonRect.Y = deleteButtonRect.GetBottom();
            toggleButtonRect.Y = toggleButtonRect.GetBottom();
        }
    }

    void ApproachSequencerDisplay::RenderBackground(Windows::GdiGraphicsInterface& graphics)
    {
        auto numberOfCallsigns = displayOptions->Airfield().empty()
                                     ? 0
                                     : sequencer.GetForAirfield(displayOptions->Airfield()).Callsigns().size();
        const Gdiplus::Rect contentArea{
            0,
            TITLE_BAR_HEIGHT,
            WINDOW_WIDTH,
            static_cast<INT>(callsignHeader.GetBottom() + INSETS + (numberOfCallsigns * callsignHeader.Height))};
        graphics.FillRect(contentArea, Gdiplus::SolidBrush(this->brushes.background));
    }

    void ApproachSequencerDisplay::RenderAirfieldTarget(
        Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen)
    {
        graphics.DrawString(
            L"Target:",
            airfieldTargetStatic,
            Gdiplus::SolidBrush(this->brushes.text),
            Graphics::StringFormatManager::Instance().GetLeftAlign(),
            Graphics::FontManager::Instance().GetDefault());

        // The target distance / wake
        std::wstring targetString = L"--";
        if (!displayOptions->Airfield().empty()) {
            targetString = options.Get(displayOptions->Airfield()).defaultMode == ApproachSequencingMode::WakeTurbulence
                               ? L"Wake"
                               : To1DpWide(options.Get(displayOptions->Airfield()).targetDistance);
        }

        graphics.DrawString(
            targetString,
            airfieldTargetTextArea,
            Gdiplus::SolidBrush(this->brushes.text),
            Graphics::StringFormatManager::Instance().GetLeftAlign(),
            Graphics::FontManager::Instance().GetDefault());
        this->airfieldTargetClickspot->Apply(graphics, radarScreen);
    }

    void ApproachSequencerDisplay::RenderAirfieldSeparation(
        Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen)
    {
        graphics.DrawString(
            L"Separation:",
            airfieldSeparationStatic,
            Gdiplus::SolidBrush(this->brushes.text),
            Graphics::StringFormatManager::Instance().GetLeftAlign(),
            Graphics::FontManager::Instance().GetDefault());

        graphics.DrawString(
            displayOptions->Airfield().empty()
                ? L"--"
                : To1DpWide(options.Get(displayOptions->Airfield()).minimumSeparationRequirement),
            airfieldSeparationTextArea,
            Gdiplus::SolidBrush(this->brushes.text),
            Graphics::StringFormatManager::Instance().GetLeftAlign(),
            Graphics::FontManager::Instance().GetDefault());
        this->airfieldSeparationClickspot->Apply(graphics, radarScreen);
    }
} // namespace UKControllerPlugin::Approach
