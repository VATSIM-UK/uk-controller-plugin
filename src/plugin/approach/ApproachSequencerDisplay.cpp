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
        std::shared_ptr<ApproachSequencerDisplayOptions> displayOptions,
        std::shared_ptr<List::PopupListInterface> airfieldSelector,
        std::shared_ptr<List::PopupListInterface> callsignSelector,
        int screenObjectId)
        : displayOptions(std::move(displayOptions)), airfieldSelector(std::move(airfieldSelector)),
          callsignSelector(std::move(callsignSelector)),
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

                graphics.FillRect(contentArea, *backgroundBrush);
                this->titleBar->Draw(graphics, radarScreen);
                this->RenderAirfield(graphics, radarScreen);
                this->RenderAddButton(graphics, radarScreen);
                this->RenderDivider(graphics);
                this->RenderHeaders(graphics);
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
        graphics.DrawString(L"Display", displayHeader, *textBrush);
        graphics.DrawString(L"Move", moveHeader, *textBrush);
    }

    void ApproachSequencerDisplay::RenderAddButton(
        Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen)
    {
        graphics.DrawRect(addButton, *dividingPen);
        graphics.DrawString(L"Add", addButton, *textBrush);
        addClickspot->Apply(graphics, radarScreen);
    }
} // namespace UKControllerPlugin::Approach
