#include "ApproachSequencerDisplay.h"
#include "ApproachSequencerDisplayOptions.h"
#include "components/CollapsibleWindowTitleBar.h"
#include "graphics/FontManager.h"
#include "graphics/GdiGraphicsInterface.h"
#include "graphics/StringFormatManager.h"
#include "helper/HelperFunctions.h"

using UKControllerPlugin::Components::CollapsibleWindowTitleBar;

namespace UKControllerPlugin::Approach {

    ApproachSequencerDisplay::ApproachSequencerDisplay(
        std::shared_ptr<ApproachSequencerDisplayOptions> displayOptions, int screenObjectId)
        : displayOptions(std::move(displayOptions)), titleBar(CollapsibleWindowTitleBar::Create(
                                              L"Approach Sequencer",
                                              titleBarArea,
                                              [this]() -> bool { return this->contentCollapsed; },
                                              screenObjectId)),
          backgroundBrush(std::make_shared<Gdiplus::SolidBrush>(BACKGROUND_COLOUR)),
          textBrush(std::make_shared<Gdiplus::SolidBrush>(TEXT_COLOUR))
    {
        this->Move({windowPosition.x, windowPosition.y, 0, 0}, "");
    }

    auto ApproachSequencerDisplay::IsVisible() const -> bool
    {
        return this->isVisible;
    }

    void ApproachSequencerDisplay::Move(RECT position, std::string objectDescription)
    {
        this->windowPosition = {position.left, position.top};
    }

    void ApproachSequencerDisplay::Render(
        Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen)
    {
        graphics.Translated(windowPosition.x, windowPosition.y, [this, &graphics, &radarScreen]() {
            if (this->contentCollapsed) {
                this->titleBar->Draw(graphics, radarScreen);
                return;
            }

            graphics.FillRect(contentArea, *backgroundBrush);
            this->titleBar->Draw(graphics, radarScreen);
            this->RenderAirfield(graphics);
        });
    }

    void ApproachSequencerDisplay::ResetPosition()
    {
        RadarRenderableInterface::ResetPosition();
    }

    void ApproachSequencerDisplay::LeftClick(
        Euroscope::EuroscopeRadarLoopbackInterface& radarScreen,
        int objectId,
        const std::string& objectDescription,
        POINT mousePos,
        RECT itemArea)
    {
        if (objectDescription == "collapseButton") {
            this->contentCollapsed = !this->contentCollapsed;
        }

        if (objectDescription == "closeButton") {
            this->isVisible = false;
        }
    }

    auto ApproachSequencerDisplay::ContentCollapsed() const -> bool
    {
        return contentCollapsed;
    }

    void ApproachSequencerDisplay::RenderAirfield(Windows::GdiGraphicsInterface& graphics)
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
    }
} // namespace UKControllerPlugin::Approach
