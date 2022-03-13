#include "ApproachSequence.h"
#include "ApproachSequencedAircraft.h"
#include "ApproachSequencer.h"
#include "ApproachSequencerDisplay.h"
#include "ApproachSequencerDisplayOptions.h"
#include "components/ClickableArea.h"
#include "components/CollapsibleWindowTitleBar.h"
#include "graphics/FontManager.h"
#include "graphics/GdiGraphicsInterface.h"
#include "graphics/StringFormatManager.h"
#include "helper/HelperFunctions.h"
#include "list/PopupListInterface.h"

using UKControllerPlugin::Components::CollapsibleWindowTitleBar;

namespace UKControllerPlugin::Approach {

    ApproachSequencerDisplay::ApproachSequencerDisplay(
        ApproachSequencer& sequencer,
        std::shared_ptr<ApproachSequencerDisplayOptions> displayOptions,
        std::shared_ptr<List::PopupListInterface> airfieldSelector,
        std::shared_ptr<List::PopupListInterface> callsignSelector,
        int screenObjectId)
        : sequencer(sequencer), displayOptions(std::move(displayOptions)),
          airfieldSelector(std::move(airfieldSelector)), callsignSelector(std::move(callsignSelector)),
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
        }

        if (objectDescription == "closeButton") {
            this->displayOptions->SetVisible(false);
        }

        if (objectDescription == AIRFIELD_SELECTOR_CLICKSPOT) {
            this->airfieldSelector->Trigger(mousePos);
        }

        if (objectDescription == ADD_AIRCRAFT_CLICKSPOT) {
            this->callsignSelector->Trigger(mousePos);
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
        Gdiplus::Rect actionsRect{
            actionsHeader.X, actionsHeader.GetBottom() + INSETS, actionsHeader.Width, actionsHeader.Height};
        int sequenceNumber = 1;

        while (aircraftToProcess != nullptr) {
            graphics.DrawString(std::to_wstring(sequenceNumber), numberRect, *textBrush);
            graphics.DrawString(
                HelperFunctions::ConvertToWideString(aircraftToProcess->Callsign()), callsignRect, *textBrush);
            graphics.DrawString(L"Wake", targetRect, *textBrush);
            graphics.DrawString(L"Wke", actualRect, *textBrush);
            graphics.DrawString(L"Act", actionsRect, *textBrush);

            sequenceNumber++;
            aircraftToProcess = aircraftToProcess->Next();
            numberRect.Y = numberRect.GetBottom();
            callsignRect.Y = callsignRect.GetBottom();
            targetRect.Y = targetRect.GetBottom();
            actualRect.Y = actualRect.GetBottom();
            actionsRect.Y = actionsRect.GetBottom();
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
