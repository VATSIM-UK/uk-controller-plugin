#include "ApproachSequence.h"
#include "ApproachSequencedAircraft.h"
#include "ApproachSequencer.h"
#include "ApproachSequencerDisplay.h"
#include "ApproachSequencerDisplayOptions.h"
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

using UKControllerPlugin::Components::CollapsibleWindowTitleBar;

namespace UKControllerPlugin::Approach {

    ApproachSequencerDisplay::ApproachSequencerDisplay(
        ApproachSequencer& sequencer,
        ApproachSpacingCalculator& spacingCalculator,
        std::shared_ptr<ApproachSequencerDisplayOptions> displayOptions,
        std::shared_ptr<List::PopupListInterface> airfieldSelector,
        std::shared_ptr<List::PopupListInterface> callsignSelector,
        std::shared_ptr<List::PopupListInterface> targetSelector,
        Euroscope::EuroscopePluginLoopbackInterface& plugin,
        int screenObjectId)
        : sequencer(sequencer), spacingCalculator(spacingCalculator), displayOptions(std::move(displayOptions)),
          airfieldSelector(std::move(airfieldSelector)), callsignSelector(std::move(callsignSelector)),
          targetSelector(std::move(targetSelector)), plugin(plugin), screenObjectId(screenObjectId),
          titleBar(CollapsibleWindowTitleBar::Create(
              L"Approach Sequencer",
              titleBarArea,
              [this]() -> bool { return this->displayOptions->ContentCollapsed(); },
              screenObjectId)),
          airfieldClickspot(Components::ClickableArea::Create(
              this->airfieldTextArea, screenObjectId, AIRFIELD_SELECTOR_CLICKSPOT, false)),
          addClickspot(
              Components::ClickableArea::Create(this->addButton, screenObjectId, ADD_AIRCRAFT_CLICKSPOT, false)),
          backgroundBrush(std::make_shared<Gdiplus::SolidBrush>(BACKGROUND_COLOUR)),
          textBrush(std::make_shared<Gdiplus::SolidBrush>(TEXT_COLOUR)),
          dividingPen(std::make_shared<Gdiplus::Pen>(TEXT_COLOUR))
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
                    this->titleBar->Draw(graphics, radarScreen);
                    return;
                }

                this->RenderBackground(graphics);
                this->titleBar->Draw(graphics, radarScreen);
                this->RenderAirfield(graphics, radarScreen);
                this->RenderAddButton(graphics, radarScreen);
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

        if (objectDescription.substr(0, 14) == "approachTarget") {
            auto flightplan = plugin.GetFlightplanForCallsign(objectDescription.substr(14));
            if (!flightplan) {
                return;
            }

            plugin.SetEuroscopeSelectedFlightplan(flightplan);
            targetSelector->Trigger(mousePos);
            return;
        }
    }

    void ApproachSequencerDisplay::RenderAirfield(
        Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen)
    {
        graphics.DrawString(
            L"Airfield:",
            airfieldStaticArea,
            *textBrush,
            Graphics::StringFormatManager::Instance().GetLeftAlign(),
            Graphics::FontManager::Instance().GetDefault());

        graphics.DrawString(
            HelperFunctions::ConvertToWideString(
                displayOptions->Airfield().empty() ? "--" : displayOptions->Airfield()),
            airfieldTextArea,
            *textBrush,
            Graphics::StringFormatManager::Instance().GetLeftAlign(),
            Graphics::FontManager::Instance().GetDefault());
        this->airfieldClickspot->Apply(graphics, radarScreen);
    }

    void ApproachSequencerDisplay::RenderDivider(Windows::GdiGraphicsInterface& graphics)
    {
        graphics.DrawLine(*dividingPen, dividerLeft, dividerRight);
    }

    void ApproachSequencerDisplay::RenderHeaders(Windows::GdiGraphicsInterface& graphics)
    {
        graphics.DrawString(L"#", numberHeader, *textBrush);
        graphics.DrawString(L"Callsign", callsignHeader, *textBrush);
        graphics.DrawString(L"Target", targetHeader, *textBrush);
        graphics.DrawString(L"Actual", actualHeader, *textBrush);
        graphics.DrawString(L"Actions", actionsHeader, *textBrush);
    }

    void ApproachSequencerDisplay::RenderAddButton(
        Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen)
    {
        graphics.DrawRect(addButton, *dividingPen);
        graphics.DrawString(L"Add", addButton, *textBrush);
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
        int sequenceNumber = 1;

        while (aircraftToProcess != nullptr) {
            graphics.DrawString(std::to_wstring(sequenceNumber), numberRect, *textBrush);
            graphics.DrawString(
                HelperFunctions::ConvertToWideString(aircraftToProcess->Callsign()), callsignRect, *textBrush);

            // The target distance / wake
            if (aircraftToProcess->Mode() == ApproachSequencingMode::WakeTurbulence) {
                graphics.DrawString(L"Wake", targetRect, *textBrush);
            } else {
                char distanceString[25];                                                  // NOLINT
                sprintf_s(distanceString, "%.1f", aircraftToProcess->ExpectedDistance()); // NOLINT
                graphics.DrawString(HelperFunctions::ConvertToWideString(distanceString), targetRect, *textBrush);
            }
            Components::ClickableArea::Create(
                targetRect, screenObjectId, "approachTarget" + aircraftToProcess->Callsign(), false)
                ->Apply(graphics, radarScreen);

            double requiredSpacing = spacingCalculator.Calculate(displayOptions->Airfield(), *aircraftToProcess);
            if (requiredSpacing == spacingCalculator.NoSpacing()) {
                graphics.DrawString(L"--", actualRect, *textBrush);
            } else {
                char distanceString[25];                            // NOLINT
                sprintf_s(distanceString, "%.1f", requiredSpacing); // NOLINT
                graphics.DrawString(HelperFunctions::ConvertToWideString(distanceString), actualRect, *textBrush);
            }

            auto upButton = Components::Button::Create(
                upButtonRect,
                screenObjectId,
                "moveUp" + aircraftToProcess->Callsign(),
                Components::UpArrow(TEXT_COLOUR));
            upButton->Draw(graphics, radarScreen);

            auto downButton = Components::Button::Create(
                downButtonRect,
                screenObjectId,
                "moveDown" + aircraftToProcess->Callsign(),
                Components::DownArrow(TEXT_COLOUR));
            downButton->Draw(graphics, radarScreen);

            auto deleteButton = Components::Button::Create(
                deleteButtonRect,
                screenObjectId,
                "deleteButton" + aircraftToProcess->Callsign(),
                Components::DeleteButton(TEXT_COLOUR));
            deleteButton->Draw(graphics, radarScreen);

            sequenceNumber++;
            aircraftToProcess = aircraftToProcess->Next();
            numberRect.Y = numberRect.GetBottom();
            callsignRect.Y = callsignRect.GetBottom();
            targetRect.Y = targetRect.GetBottom();
            actualRect.Y = actualRect.GetBottom();
            upButtonRect.Y = upButtonRect.GetBottom();
            downButtonRect.Y = downButtonRect.GetBottom();
            deleteButtonRect.Y = deleteButtonRect.GetBottom();
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
        graphics.FillRect(contentArea, *backgroundBrush);
    }
} // namespace UKControllerPlugin::Approach
