#include "pch/pch.h"
#include "graphics/GdiGraphicsWrapper.h"

namespace UKControllerPlugin {
    namespace Windows {


        GdiGraphicsWrapper::GdiGraphicsWrapper(void)
        {
            this->stringFormat = std::make_unique<Gdiplus::StringFormat>(
                Gdiplus::StringFormatFlags::StringFormatFlagsNoClip
            );
            this->stringFormat->SetAlignment(Gdiplus::StringAlignment::StringAlignmentCenter);
            this->stringFormat->SetLineAlignment(Gdiplus::StringAlignment::StringAlignmentCenter);
            this->font = CreateFont(
                13,
                '\0',
                '\0',
                '\0',
                400,
                '\0',
                '\0',
                '\0',
                '\x1',
                '\0',
                '\0',
                '\0',
                '\0',
                L"EuroScope"
            );
        }

        GdiGraphicsWrapper::GdiGraphicsWrapper(std::unique_ptr<Gdiplus::Graphics> graphics): graphics(
            std::move(graphics)) {}


        GdiGraphicsWrapper::~GdiGraphicsWrapper(void)
        {
            DeleteObject(this->font);
        }

        /*
            Draws the outline of a rectangle to the screen.
        */
        void GdiGraphicsWrapper::DrawRect(const Gdiplus::RectF& area, const Gdiplus::Pen& pen)
        {
            api->DrawRectangle(&pen, area);
        }

        /*
            Draws the outline of a rectangle to the screen.
        */
        void GdiGraphicsWrapper::DrawRect(const Gdiplus::Rect& area, const Gdiplus::Pen& pen)
        {
            api->DrawRectangle(&pen, area);
        }

        /*
            Draws the outline of a rectangle to the screen.
        */
        void GdiGraphicsWrapper::DrawRect(const RECT& area, const Gdiplus::Pen& pen)
        {
            Gdiplus::Rect newArea = {area.left, area.top, area.right - area.left, area.bottom - area.top};
            api->DrawRectangle(&pen, newArea);
        }

        /*
            Draws the outline of a circle to the screen.
        */
        void GdiGraphicsWrapper::DrawCircle(const Gdiplus::RectF& area, const Gdiplus::Pen& pen)
        {
            api->DrawEllipse(&pen, area);
        }

        /*
            Draws the outline of a circle to the screen.
        */
        void GdiGraphicsWrapper::DrawCircle(const Gdiplus::Rect& area, const Gdiplus::Pen& pen)
        {
            api->DrawEllipse(&pen, area);
        }

        void GdiGraphicsWrapper::DrawDiamond(const Gdiplus::RectF& area, const Gdiplus::Pen& pen)
        {
            // Points, starting at the centre left and going anticlockwise
            Gdiplus::PointF points[4] = {
                Gdiplus::PointF(area.X, area.Y + (area.Height / 2)),
                Gdiplus::PointF(area.X + (area.Width / 2), area.Y + area.Height),
                Gdiplus::PointF(area.X + area.Width, area.Y + (area.Height / 2)),
                Gdiplus::PointF(area.X + (area.Width / 2), area.Y)
            };
            api->DrawPolygon(&pen, points, 4);
        }

        /*
            Draw a line between two points
        */
        void GdiGraphicsWrapper::DrawLine(
            const Gdiplus::Pen& pen,
            const Gdiplus::Point& start,
            const Gdiplus::Point& end
        )
        {
            this->api->DrawLine(&pen, start, end);
        }

        void GdiGraphicsWrapper::DrawLine(
            const Gdiplus::Pen& pen,
            const Gdiplus::PointF& start,
            const Gdiplus::PointF& end
        )
        {
            this->api->DrawLine(&pen, start, end);
        }

        void GdiGraphicsWrapper::DrawPath(const Gdiplus::GraphicsPath& path, const Gdiplus::Pen& pen)
        {
            this->api->DrawPath(&pen, &path);
        }

        /*
            Draw a string to the screen in a bounding box.
        */
        void GdiGraphicsWrapper::DrawString(
            std::wstring text,
            const Gdiplus::RectF& area,
            const Gdiplus::Brush& brush
        )
        {
            api->DrawString(
                text.c_str(),
                text.size(),
                this->euroscopeFont.get(),
                area,
                this->stringFormat.get(),
                &brush
            );
        }

        /*
            Draw a string to the screen in a bounding box.
        */
        void GdiGraphicsWrapper::DrawString(std::wstring text, const Gdiplus::Rect& area, const Gdiplus::Brush& brush)
        {
            Gdiplus::RectF areaFloat = {
                static_cast<Gdiplus::REAL>(area.X),
                static_cast<Gdiplus::REAL>(area.Y),
                static_cast<Gdiplus::REAL>(area.Width),
                static_cast<Gdiplus::REAL>(area.Height)
            };

            api->DrawString(
                text.c_str(),
                text.size(),
                this->euroscopeFont.get(),
                areaFloat,
                this->stringFormat.get(),
                &brush
            );
        }

        /*
            Draw a string to the screen in a bounding box.
        */
        void GdiGraphicsWrapper::DrawString(std::wstring text, const RECT& area, const Gdiplus::Brush& brush)
        {
            Gdiplus::RectF areaFloat = {
                static_cast<Gdiplus::REAL>(area.left),
                static_cast<Gdiplus::REAL>(area.top),
                static_cast<Gdiplus::REAL>(area.right - area.left),
                static_cast<Gdiplus::REAL>(area.bottom - area.top)
            };

            api->DrawString(
                text.c_str(),
                text.size(),
                this->euroscopeFont.get(),
                areaFloat,
                this->stringFormat.get(),
                &brush
            );
        }

        /*
            Fill a rectangle.
        */
        void GdiGraphicsWrapper::FillRect(const Gdiplus::RectF& area, const Gdiplus::Brush& brush)
        {
            api->FillRectangle(&brush, area);
        }

        /*
            Fill a rectangle.
        */
        void GdiGraphicsWrapper::FillRect(const Gdiplus::Rect& area, const Gdiplus::Brush& brush)
        {
            api->FillRectangle(&brush, area);
        }

        /*
            Fill a rectangle.
        */
        void GdiGraphicsWrapper::FillRect(const RECT& area, const Gdiplus::Brush& brush)
        {
            Gdiplus::Rect newArea = {area.left, area.top, area.right - area.left, area.bottom - area.top};
            api->FillRectangle(&brush, newArea);
        }

        /*
            Set the antialiasing mode.
        */
        void GdiGraphicsWrapper::SetAntialias(bool setting)
        {
            this->api->SetSmoothingMode((setting) ? Gdiplus::SmoothingModeAntiAlias : Gdiplus::SmoothingModeNone);
        }

        /*
            Sets the device handle.
        */
        void GdiGraphicsWrapper::SetDeviceHandle(HDC& handle)
        {
            this->api.reset(new Gdiplus::Graphics(handle));
            this->euroscopeFont.reset(new Gdiplus::Font(handle, font));
        }

        void GdiGraphicsWrapper::Clipped(Gdiplus::Region& clipRegion, std::function<void()> drawFunction)
        {
            const auto container = this->api->BeginContainer();
            this->api->SetClip(&clipRegion);
            drawFunction();
            this->api->EndContainer(container);
        }

        void GdiGraphicsWrapper::Translated(Gdiplus::REAL x, Gdiplus::REAL y, std::function<void()> drawFunction)
        {
            const auto container = this->api->BeginContainer();
            this->api->TranslateTransform(x, y);
            drawFunction();
            this->api->EndContainer(container);
        }

        void GdiGraphicsWrapper::Scaled(Gdiplus::REAL x, Gdiplus::REAL y, std::function<void()> drawFunction)
        {
            const auto container = this->api->BeginContainer();
            this->api->ScaleTransform(x, y);
            drawFunction();
            this->api->EndContainer(container);
        }

        std::shared_ptr<Gdiplus::Matrix> GdiGraphicsWrapper::GetTransform()
        {
            Gdiplus::Matrix transform;
            this->api->GetTransform(&transform);
            return std::shared_ptr<Gdiplus::Matrix>(transform.Clone());
        }

        Gdiplus::RectF GdiGraphicsWrapper::GetClipBounds()
        {
            Gdiplus::RectF bounds;
            this->api->GetClipBounds(&bounds);
            return std::move(bounds);
        }

        void GdiGraphicsWrapper::Rotated(Gdiplus::REAL angle, std::function<void()> drawFunction)
        {
            const auto container = this->api->BeginContainer();
            this->api->RotateTransform(angle);
            drawFunction();
            this->api->EndContainer(container);
        }

        void GdiGraphicsWrapper::FillPolygon(Gdiplus::Point* points, const Gdiplus::Brush& brush, int numPoints)
        {
            this->api->FillPolygon(&brush, points, numPoints);
        }

        void GdiGraphicsWrapper::FillCircle(const Gdiplus::RectF& area, const Gdiplus::Brush& brush)
        {
            this->api->FillEllipse(&brush, area);
        }

        void GdiGraphicsWrapper::FillDiamond(const Gdiplus::RectF& area, const Gdiplus::Brush& brush)
        {
            Gdiplus::PointF points[4] = {
                Gdiplus::PointF(area.X, area.Y + (area.Height / 2)),
                Gdiplus::PointF(area.X + (area.Width / 2), area.Y + area.Height),
                Gdiplus::PointF(area.X + area.Width, area.Y + (area.Height / 2)),
                Gdiplus::PointF(area.X + (area.Width / 2), area.Y)
            };
            api->FillPolygon(&brush, points, 4);
        }
    } // namespace Windows
} // namespace UKControllerPlugin
