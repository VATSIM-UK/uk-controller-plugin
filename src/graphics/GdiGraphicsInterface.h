#pragma once

namespace Gdiplus {
    class RectF;
}  // namespace Gdiplus

namespace UKControllerPlugin {
    namespace Windows {

        /*
            Interface for Windows' GDI Graphics API. Used by classes that want to draw to the
            radar screen.
        */
        class GdiGraphicsInterface
        {
            public:
                virtual void DrawRect(const Gdiplus::RectF & area, const Gdiplus::Pen & pen) = 0;
                virtual void DrawRect(const Gdiplus::Rect & area, const Gdiplus::Pen & pen) = 0;
                virtual void DrawRect(const RECT & area, const Gdiplus::Pen & pen) = 0;
                virtual void DrawCircle(const Gdiplus::RectF & area, const Gdiplus::Pen & pen) = 0;
                virtual void DrawCircle(const Gdiplus::Rect & area, const Gdiplus::Pen & pen) = 0;
                virtual void DrawDiamond(const Gdiplus::RectF & area, const Gdiplus::Pen & pen) = 0;
                virtual void DrawLine(
                    const Gdiplus::Pen & pen,
                    const Gdiplus::Point & start,
                    const Gdiplus::Point & end
                ) = 0;
                virtual void DrawPath(const Gdiplus::GraphicsPath & path, const Gdiplus::Pen & pen) = 0;
                virtual void DrawString(
                    std::wstring text,
                    const Gdiplus::RectF & area,
                    const Gdiplus::Brush & brush
                ) = 0;
                virtual void DrawString(
                    std::wstring text,
                    const Gdiplus::Rect & area,
                    const Gdiplus::Brush & brush
                ) = 0;
                virtual void DrawString(std::wstring text, const RECT & area, const Gdiplus::Brush & brush) = 0;
                virtual void FillRect(const Gdiplus::RectF & area, const Gdiplus::Brush & brush) = 0;
                virtual void FillRect(const Gdiplus::Rect & area, const Gdiplus::Brush & brush) = 0;
                virtual void FillRect(const RECT & area, const Gdiplus::Brush & brush) = 0;
                virtual void SetAntialias(bool setting) = 0;
                virtual void SetDeviceHandle(HDC & handle) = 0;
        };
    }  // namespace Windows
}  // namespace UKControllerPlugin
