#include "MockGraphicsInterface.h"

namespace UKControllerPluginTest::Windows {
    MockGraphicsInterface::MockGraphicsInterface() = default;
    MockGraphicsInterface::~MockGraphicsInterface() = default;

    void MockGraphicsInterface::Clipped(Gdiplus::Region&, std::function<void()> func)
    {
        func();
    }

    void MockGraphicsInterface::Translated(Gdiplus::REAL, Gdiplus::REAL, std::function<void()> func)
    {
        func();
    }

    void MockGraphicsInterface::Scaled(Gdiplus::REAL, Gdiplus::REAL, std::function<void()> func)
    {
        func();
    }

    void MockGraphicsInterface::Rotated(Gdiplus::REAL, std::function<void()> func)
    {
        func();
    }

    void MockGraphicsInterface::DrawRect(const Gdiplus::RectF& area, const Gdiplus::Pen& pen)
    {
        this->DrawRectRectF(area, pen);
    }

    void MockGraphicsInterface::DrawRect(const Gdiplus::Rect& area, const Gdiplus::Pen& pen)
    {
        this->DrawRectRect(area, pen);
    }

    void MockGraphicsInterface::DrawRect(const RECT& area, const Gdiplus::Pen& pen)
    {
        this->DrawRectRegularRect(area, pen);
    }

    void MockGraphicsInterface::DrawString(std::wstring text, const Gdiplus::RectF& area, const Gdiplus::Brush& brush)
    {
        this->DrawStringRectF(text, area, brush);
    }

    void MockGraphicsInterface::DrawString(std::wstring text, const Gdiplus::Rect& area, const Gdiplus::Brush& brush)
    {
        this->DrawStringRect(text, area, brush);
    }

    void MockGraphicsInterface::DrawString(std::wstring text, const RECT& area, const Gdiplus::Brush& brush)
    {
        this->DrawStringRegularRect(text, area, brush);
    }

    void MockGraphicsInterface::DrawString(
        const std::wstring& text,
        const Gdiplus::Rect& area,
        const Gdiplus::Brush& brush,
        const Gdiplus::StringFormat& format,
        const Gdiplus::Font& font)
    {
        this->DrawStringRectFiveArgs(text, area, brush, format, font);
    }

    void MockGraphicsInterface::FillRect(const Gdiplus::RectF& area, const Gdiplus::Brush& brush)
    {
        this->FillRectRectF(area, brush);
    }

    void MockGraphicsInterface::FillRect(const Gdiplus::Rect& area, const Gdiplus::Brush& brush)
    {
        this->FillRectRect(area, brush);
    }

    void MockGraphicsInterface::FillRect(const RECT& area, const Gdiplus::Brush& brush)
    {
        this->FillRectRegularRect(area, brush);
    }
} // namespace UKControllerPluginTest::Windows
