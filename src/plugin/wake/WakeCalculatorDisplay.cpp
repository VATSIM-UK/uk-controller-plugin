#include "WakeCalculatorDisplay.h"
#include "components/TitleBar.h"
#include "graphics/FontManager.h"
#include "graphics/GdiGraphicsInterface.h"
#include "graphics/StringFormatManager.h"

namespace UKControllerPlugin::Wake {

    WakeCalculatorDisplay::WakeCalculatorDisplay(int screenObjectId)
        : screenObjectId(screenObjectId),
          titleBar(Components::TitleBar::Create(L"Wake Turbulence Calculator", TitleBarArea())
                       ->WithDrag(this->screenObjectId)
                       ->WithDefaultBackgroundBrush()
                       ->WithDefaultTextBrush()),
          backgroundBrush(std::make_shared<Gdiplus::SolidBrush>(BACKGROUND_COLOUR)),
          textBrush(std::make_shared<Gdiplus::SolidBrush>(TEXT_COLOUR)),
          resultBrush(std::make_shared<Gdiplus::SolidBrush>(RESULT_COLOUR)),
          dividingLinePen(std::make_shared<Gdiplus::Pen>(TEXT_COLOUR))
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
        RadarRenderableInterface::LeftClick(radarScreen, objectId, objectDescription, mousePos, itemArea);
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

        // Intermediate
        this->intermediateStaticArea = {schemeTextArea.GetRight() + TEXT_INSET, schemeStaticArea.GetTop(), 105, 20};
        this->intermediateTextArea = {
            intermediateStaticArea.GetRight() + TEXT_INSET, schemeStaticArea.GetTop(), 30, 20};

        // Lead
        this->leadStaticArea = {TEXT_INSET, schemeStaticArea.GetBottom() + TEXT_INSET, 45, 20};
        this->leadTextArea = {
            leadStaticArea.GetRight() + TEXT_INSET, schemeStaticArea.GetBottom() + TEXT_INSET, 110, 20};

        // Following
        this->followingStaticArea = {leadTextArea.GetRight() + TEXT_INSET, leadStaticArea.GetTop(), 60, 20};
        this->followingTextArea = {followingStaticArea.GetRight() + TEXT_INSET, leadStaticArea.GetTop(), 110, 20};

        // Dividing line
        this->dividingLineStart = Gdiplus::Point(0, followingTextArea.GetBottom() + TEXT_INSET);
        this->dividingLineEnd = Gdiplus::Point(WINDOW_WIDTH, followingTextArea.GetBottom() + TEXT_INSET);

        // Category comparison
        this->comparisonTextArea = {TEXT_INSET, dividingLineEnd.Y + TEXT_INSET, WINDOW_WIDTH - (2 * TEXT_INSET), 20};

        // Calculation result
        this->calculationResultArea = {
            TEXT_INSET, comparisonTextArea.GetBottom(), WINDOW_WIDTH - (2 * TEXT_INSET), 60};
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
            this->RenderComparison(graphics);
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
            L"RECAT-EU",
            schemeTextArea,
            *textBrush,
            Graphics::StringFormatManager::Instance().GetLeftAlign(),
            Graphics::FontManager::Instance().GetDefault());
    }

    void WakeCalculatorDisplay::RenderLead(
        Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen)
    {
        graphics.DrawString(
            L"Lead:",
            leadStaticArea,
            *textBrush,
            Graphics::StringFormatManager::Instance().GetLeftAlign(),
            Graphics::FontManager::Instance().GetDefault());
        graphics.DrawString(
            L"EZY123456789",
            leadTextArea,
            *textBrush,
            Graphics::StringFormatManager::Instance().GetLeftAlign(),
            Graphics::FontManager::Instance().GetDefault());
    }

    void WakeCalculatorDisplay::RenderFollowing(
        Windows::GdiGraphicsInterface& graphics, Euroscope::EuroscopeRadarLoopbackInterface& radarScreen)
    {
        graphics.DrawString(
            L"Follow:",
            followingStaticArea,
            *textBrush,
            Graphics::StringFormatManager::Instance().GetLeftAlign(),
            Graphics::FontManager::Instance().GetDefault());
        graphics.DrawString(
            L"EZY123456789",
            followingTextArea,
            *textBrush,
            Graphics::StringFormatManager::Instance().GetLeftAlign(),
            Graphics::FontManager::Instance().GetDefault());
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
            L"Yes",
            intermediateTextArea,
            *textBrush,
            Graphics::StringFormatManager::Instance().GetLeftAlign(),
            Graphics::FontManager::Instance().GetDefault());
    }
    void WakeCalculatorDisplay::RenderDividingLine(Windows::GdiGraphicsInterface& graphics)
    {
        graphics.DrawLine(*dividingLinePen, dividingLineStart, dividingLineEnd);
    }

    void WakeCalculatorDisplay::RenderComparison(Windows::GdiGraphicsInterface& graphics)
    {
        graphics.DrawString(L"UM followed by LM (intermediate)", comparisonTextArea, *textBrush);
    }

    void WakeCalculatorDisplay::RenderSeparationRequirement(Windows::GdiGraphicsInterface& graphics)
    {
        graphics.DrawString(
            L"3 mins",
            calculationResultArea,
            *resultBrush,
            Graphics::StringFormatManager::Instance().GetCentreAlign(),
            Graphics::FontManager::Instance().Get(16));
    }
} // namespace UKControllerPlugin::Wake
