#pragma once
#include "graphics/GdiGraphicsInterface.h"

// Forward declare
namespace Gdiplus {
    class Graphics;
} // namespace Gdiplus
// END

namespace UKControllerPlugin {
    namespace Windows {

        /*
            Wrapper around the Windows GDI API.
        */
        class GdiGraphicsWrapper : public GdiGraphicsInterface
        {
            public:
            GdiGraphicsWrapper();
            void DrawRect(const Gdiplus::RectF& area, const Gdiplus::Pen& pen) override;
            void DrawRect(const Gdiplus::Rect& area, const Gdiplus::Pen& pen) override;
            void DrawRect(const RECT& area, const Gdiplus::Pen& pen) override;
            void DrawCircle(const Gdiplus::RectF& area, const Gdiplus::Pen& pen) override;
            void DrawCircle(const Gdiplus::Rect& area, const Gdiplus::Pen& pen) override;
            void DrawDiamond(const Gdiplus::RectF& area, const Gdiplus::Pen& pen) override;
            void DrawLine(const Gdiplus::Pen& pen, const Gdiplus::Point& start, const Gdiplus::Point& end) override;
            void DrawLine(const Gdiplus::Pen& pen, const Gdiplus::PointF& start, const Gdiplus::PointF& end) override;
            void DrawPath(const Gdiplus::GraphicsPath& path, const Gdiplus::Pen& pen) override;
            void DrawString(std::wstring text, const Gdiplus::RectF& area, const Gdiplus::Brush& brush) override;
            void DrawString(std::wstring text, const Gdiplus::Rect& area, const Gdiplus::Brush& brush) override;
            void DrawString(std::wstring text, const RECT& area, const Gdiplus::Brush& brush) override;
            void FillRect(const Gdiplus::RectF& area, const Gdiplus::Brush& brush) override;
            void FillRect(const Gdiplus::Rect& area, const Gdiplus::Brush& brush) override;
            void FillRect(const RECT& area, const Gdiplus::Brush& brush) override;
            void SetAntialias(bool setting) override;
            void SetDeviceHandle(HDC& handle) override;
            void Clipped(Gdiplus::Region& clipRegion, std::function<void()> drawFunction) override;
            void Translated(Gdiplus::REAL x, Gdiplus::REAL y, std::function<void()> drawFunction) override;
            void Scaled(Gdiplus::REAL x, Gdiplus::REAL y, std::function<void()> drawFunction) override;
            std::shared_ptr<Gdiplus::Matrix> GetTransform() override;
            Gdiplus::RectF GetClipBounds() override;
            void Rotated(Gdiplus::REAL angle, std::function<void()> drawFunction) override;
            void FillPolygon(Gdiplus::Point* points, const Gdiplus::Brush& brush, int numPoints) override;
            void FillCircle(const Gdiplus::RectF& area, const Gdiplus::Brush& brush) override;
            void FillDiamond(const Gdiplus::RectF& area, const Gdiplus::Brush& brush) override;

            private:
            std::unique_ptr<Gdiplus::Graphics> api;

            // The Gdiplus font
            const Gdiplus::Font& euroscopeFont;

            // String format
            std::unique_ptr<Gdiplus::StringFormat> stringFormat;
        };
    } // namespace Windows
} // namespace UKControllerPlugin
