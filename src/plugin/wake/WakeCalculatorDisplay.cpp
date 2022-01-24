#include "WakeCategory.h"
#include "WakeCalculatorDisplay.h"
#include "WakeCategoryMapperInterface.h"
#include "WakeCalculatorOptions.h"
#include "WakeIntervalFormatter.h"
#include "components/ClickableArea.h"
#include "components/TitleBar.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "euroscope/EuroscopeRadarLoopbackInterface.h"
#include "graphics/FontManager.h"
#include "graphics/GdiGraphicsInterface.h"
#include "graphics/StringFormatManager.h"
#include "helper/HelperFunctions.h"
#include "list/PopupListInterface.h"

namespace UKControllerPlugin::Wake {

    WakeCalculatorDisplay::WakeCalculatorDisplay(
        std::shared_ptr<WakeCalculatorOptions> options,
        std::shared_ptr<List::PopupListInterface> leadCallsignSelector,
        std::shared_ptr<List::PopupListInterface> followCallsignSelector,
        std::shared_ptr<List::PopupListInterface> wakeSchemeSelector,
        Euroscope::EuroscopePluginLoopbackInterface& plugin,
        int screenObjectId)
        : options(std::move(options)), leadCallsignSelector(std::move(leadCallsignSelector)),
          followCallsignSelector(std::move(followCallsignSelector)), wakeSchemeSelector(std::move(wakeSchemeSelector)),
          plugin(plugin), screenObjectId(screenObjectId),
          titleBar(Components::TitleBar::Create(L"Wake Turbulence Calculator", TitleBarArea())
                       ->WithDrag(this->screenObjectId)
                       ->WithDefaultBackgroundBrush()
                       ->WithDefaultTextBrush()),
          backgroundBrush(std::make_shared<Gdiplus::SolidBrush>(BACKGROUND_COLOUR)),
          textBrush(std::make_shared<Gdiplus::SolidBrush>(TEXT_COLOUR)),
          resultBrush(std::make_shared<Gdiplus::SolidBrush>(RESULT_COLOUR)),
          dividingLinePen(std::make_shared<Gdiplus::Pen>(TEXT_COLOUR)),
          leadClickspot(Components::ClickableArea::Create(leadTextArea, screenObjectId, "leadcallsign", false)),
          followingClickspot(
              Components::ClickableArea::Create(followingTextArea, screenObjectId, "followcallsign", false)),
          schemeClickspot(Components::ClickableArea::Create(schemeTextArea, screenObjectId, "scheme", false)),
          intermediateClickspot(
              Components::ClickableArea::Create(intermediateTextArea, screenObjectId, "intermediate", false))
    {
        this->Move({DEFAULT_WINDOW_POSITION.x, DEFAULT_WINDOW_POSITION.y, 0, 0}, "");
    }

    bool WakeCalculatorDisplay::IsVisible() const
    {
        return true;
    }

    void WakeCalculatorDisplay::LeftClick(
        Euroscope::EuroscopeRadarLoopbackInterface& radarScreen,
        int objectId,
        const std::string& objectDescription,
        POINT mousePos,
        RECT itemArea)
    {
        if (objectDescription == "leadcallsign") {
            leadCallsignSelector->Trigger(mousePos);
            return;
        }

        if (objectDescription == "followcallsign") {
            followCallsignSelector->Trigger(mousePos);
            return;
        }

        if (objectDescription == "scheme") {
            wakeSchemeSelector->Trigger(mousePos);
            return;
        }

        if (objectDescription == "intermediate") {
            options->Intermediate(!options->Intermediate());
            return;
        }
    }

    void WakeCalculatorDisplay::Move(RECT position, std::string objectDescription)
    {
        this->windowPosition = {position.left, position.top};
        this->contentArea = WindowContentArea();
        this->titleBar->WithPosition(TitleBarArea());

        // Scheme
        this->schemeStaticArea = {TEXT_INSET, TitleBarArea().GetBottom() + TEXT_INSET, 60, 20};
        this->schemeTextArea = {
            this->schemeStaticArea.GetRight() + TEXT_INSET, TitleBarArea().GetBottom() + TEXT_INSET, 75, 20};
        this->schemeClickspot->WithPosition(this->schemeTextArea);

        // Intermediate
        this->intermediateStaticArea = {schemeTextArea.GetRight() + TEXT_INSET, schemeStaticArea.GetTop(), 105, 20};
        this->intermediateTextArea = {
            intermediateStaticArea.GetRight() + TEXT_INSET, schemeStaticArea.GetTop(), 30, 20};
        this->intermediateClickspot->WithPosition(this->intermediateTextArea);

        // Lead
        this->leadStaticArea = {TEXT_INSET, schemeStaticArea.GetBottom() + TEXT_INSET, 45, 20};
        this->leadTextArea = {
            leadStaticArea.GetRight() + TEXT_INSET, schemeStaticArea.GetBottom() + TEXT_INSET, 110, 20};
        this->leadClickspot->WithPosition(this->leadTextArea);

        // Following
        this->followingStaticArea = {leadTextArea.GetRight() + TEXT_INSET, leadStaticArea.GetTop(), 60, 20};
        this->followingTextArea = {followingStaticArea.GetRight() + TEXT_INSET, leadStaticArea.GetTop(), 110, 20};
        this->followingClickspot->WithPosition(this->followingTextArea);

        // Dividing line
        this->dividingLineStart = Gdiplus::Point(0, followingTextArea.GetBottom() + TEXT_INSET);
        this->dividingLineEnd = Gdiplus::Point(WINDOW_WIDTH, followingTextArea.GetBottom() + TEXT_INSET);

        // Category comparison
        this->comparisonTextArea = {TEXT_INSET, dividingLineEnd.Y + TEXT_INSET, WINDOW_WIDTH - (2 * TEXT_INSET), 20};

        // Calculation result
        this->calculationResultArea = {TEXT_INSET, comparisonTextArea.GetBottom(), WINDOW_WIDTH - (2 * TEXT_INSET), 60};
    }

    void WakeCalculatorDisplay::Render(
        Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen)
    {
        graphics.Translated(windowPosition.x, windowPosition.y, [&graphics, &radarScreen, this]() {
            graphics.FillRect(this->contentArea, *backgroundBrush);
            this->RenderScheme(graphics, radarScreen);
            this->RenderIntermediate(graphics, radarScreen);
            this->RenderLead(graphics, radarScreen);
            this->RenderFollowing(graphics, radarScreen);
            this->RenderDividingLine(graphics);
            this->RenderSeparationRequirement(graphics);
            titleBar->Draw(graphics, radarScreen);
        });
    }

    void WakeCalculatorDisplay::ResetPosition()
    {
        this->Move({DEFAULT_WINDOW_POSITION.x, DEFAULT_WINDOW_POSITION.y, 0, 0}, "");
        RadarRenderableInterface::ResetPosition();
    }

    void WakeCalculatorDisplay::AsrLoadedEvent(Euroscope::UserSetting& userSetting)
    {
    }

    void WakeCalculatorDisplay::AsrClosingEvent(Euroscope::UserSetting& userSetting)
    {
    }

    auto WakeCalculatorDisplay::TitleBarArea() -> Gdiplus::Rect
    {
        return {0, 0, WINDOW_WIDTH, TITLE_BAR_HEIGHT};
    }

    auto WakeCalculatorDisplay::WindowContentArea() -> Gdiplus::Rect
    {
        return {0, TITLE_BAR_HEIGHT, WINDOW_WIDTH, CONTENT_HEIGHT};
    }

    void WakeCalculatorDisplay::RenderScheme(
        Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen)
    {
        graphics.DrawString(
            L"Scheme:",
            schemeStaticArea,
            *textBrush,
            Graphics::StringFormatManager::Instance().GetLeftAlign(),
            Graphics::FontManager::Instance().GetDefault());
        graphics.DrawString(
            HelperFunctions::ConvertToWideString(options->Scheme()),
            schemeTextArea,
            *textBrush,
            Graphics::StringFormatManager::Instance().GetLeftAlign(),
            Graphics::FontManager::Instance().GetDefault());
        this->schemeClickspot->Apply(graphics, radarScreen);
    }

    void WakeCalculatorDisplay::RenderLead(
        Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen)
    {
        const auto lead = options->LeadAircraft();
        graphics.DrawString(
            L"Lead:",
            leadStaticArea,
            *textBrush,
            Graphics::StringFormatManager::Instance().GetLeftAlign(),
            Graphics::FontManager::Instance().GetDefault());
        graphics.DrawString(
            HelperFunctions::ConvertToWideString(lead.empty() ? "--" : lead),
            leadTextArea,
            *textBrush,
            Graphics::StringFormatManager::Instance().GetLeftAlign(),
            Graphics::FontManager::Instance().GetDefault());
        this->leadClickspot->Apply(graphics, radarScreen);
    }

    void WakeCalculatorDisplay::RenderFollowing(
        Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen)
    {
        const auto following = options->FollowingAircraft();
        graphics.DrawString(
            L"Follow:",
            followingStaticArea,
            *textBrush,
            Graphics::StringFormatManager::Instance().GetLeftAlign(),
            Graphics::FontManager::Instance().GetDefault());
        graphics.DrawString(
            HelperFunctions::ConvertToWideString(following.empty() ? "--" : following),
            followingTextArea,
            *textBrush,
            Graphics::StringFormatManager::Instance().GetLeftAlign(),
            Graphics::FontManager::Instance().GetDefault());
        this->followingClickspot->Apply(graphics, radarScreen);
    }

    void WakeCalculatorDisplay::RenderIntermediate(
        Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen)
    {
        graphics.DrawString(
            L"Intermediate:",
            intermediateStaticArea,
            *textBrush,
            Graphics::StringFormatManager::Instance().GetLeftAlign(),
            Graphics::FontManager::Instance().GetDefault());
        graphics.DrawString(
            options->Intermediate() ? L"Yes" : L"No",
            intermediateTextArea,
            *textBrush,
            Graphics::StringFormatManager::Instance().GetLeftAlign(),
            Graphics::FontManager::Instance().GetDefault());
        this->intermediateClickspot->Apply(graphics, radarScreen);
    }

    void WakeCalculatorDisplay::RenderDividingLine(Windows::GdiGraphicsInterface& graphics)
    {
        graphics.DrawLine(*dividingLinePen, dividingLineStart, dividingLineEnd);
    }

    void WakeCalculatorDisplay::RenderSeparationRequirement(Windows::GdiGraphicsInterface& graphics)
    {
        // Check for the mapper and required flightplans
        const auto mapper = options->SchemeMapper();
        const auto leadFlightplan = plugin.GetFlightplanForCallsign(options->LeadAircraft());
        const auto followingFlightplan = plugin.GetFlightplanForCallsign(options->FollowingAircraft());
        if (options->SchemeMapper() == nullptr || leadFlightplan == nullptr || followingFlightplan == nullptr) {
            return;
        }

        // Check for the categories;
        const auto leadCategory = mapper->MapForFlightplan(*leadFlightplan);
        const auto followingCategory = mapper->MapForFlightplan(*followingFlightplan);
        if (leadCategory == nullptr || followingCategory == nullptr) {
            return;
        }

        // Summarise the WTC comparisons
        const std::wstring categoryComparison = HelperFunctions::ConvertToWideString(leadCategory->Code()) +
                                                L" followed by " +
                                                HelperFunctions::ConvertToWideString(followingCategory->Code()) +
                                                (options->Intermediate() ? L" (intermediate)" : L"");

        graphics.DrawString(categoryComparison, comparisonTextArea, *textBrush);

        // Check for intervals and display if present
        const auto departureInterval = leadCategory->DepartureInterval(*followingCategory, options->Intermediate());
        if (departureInterval == nullptr) {
            graphics.DrawString(
                L"--",
                calculationResultArea,
                *resultBrush,
                Graphics::StringFormatManager::Instance().GetCentreAlign(),
                Graphics::FontManager::Instance().Get(16));
            return;
        }

        graphics.DrawString(
            FormatInterval(*departureInterval),
            calculationResultArea,
            *resultBrush,
            Graphics::StringFormatManager::Instance().GetCentreAlign(),
            Graphics::FontManager::Instance().Get(16));
    }
} // namespace UKControllerPlugin::Wake
