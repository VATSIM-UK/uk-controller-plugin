#include "pch/stdafx.h"
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


        GdiGraphicsWrapper::~GdiGraphicsWrapper(void)
        {
            DeleteObject(this->font);
        }

        /*
            Draws the outline of a rectangle to the screen.
        */
        void GdiGraphicsWrapper::DrawRect(const Gdiplus::RectF & area, const Gdiplus::Pen & pen)
        {
            api->DrawRectangle(&pen, area);
        }

        /*
            Draws the outline of a rectangle to the screen.
        */
        void GdiGraphicsWrapper::DrawRect(const Gdiplus::Rect & area, const Gdiplus::Pen & pen)
        {
            api->DrawRectangle(&pen, area);
        }

        /*
            Draws the outline of a rectangle to the screen.
        */
        void GdiGraphicsWrapper::DrawRect(const RECT & area, const Gdiplus::Pen & pen)
        {
            Gdiplus::Rect newArea = { area.left, area.top, area.right - area.left, area.bottom - area.top };
            api->DrawRectangle(&pen, newArea);
        }

        /*
            Draws the outline of a circle to the screen.
        */
        void GdiGraphicsWrapper::DrawCircle(const Gdiplus::RectF & area, const Gdiplus::Pen & pen)
        {
            api->DrawEllipse(&pen, area);
        }

        /*
            Draws the outline of a circle to the screen.
        */
        void GdiGraphicsWrapper::DrawCircle(const Gdiplus::Rect & area, const Gdiplus::Pen & pen)
        {
            api->DrawEllipse(&pen, area);
        }

        void GdiGraphicsWrapper::DrawDiamond(const Gdiplus::RectF & area, const Gdiplus::Pen & pen)
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
            Draw a string to the screen in a bounding box.
        */
        void GdiGraphicsWrapper::DrawString(
            std::wstring text,
            const Gdiplus::RectF & area,
            const Gdiplus::Brush & brush
        ) {
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
        void GdiGraphicsWrapper::DrawString(std::wstring text, const Gdiplus::Rect & area, const Gdiplus::Brush & brush)
        {
            Gdiplus::RectF areaFloat = {
                Gdiplus::REAL(area.X),
                Gdiplus::REAL(area.Y),
                Gdiplus::REAL(area.Width),
                Gdiplus::REAL(area.Height)
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
        void GdiGraphicsWrapper::DrawString(std::wstring text, const RECT & area, const Gdiplus::Brush & brush)
        {
            Gdiplus::RectF areaFloat = {
                Gdiplus::REAL(area.left),
                Gdiplus::REAL(area.top),
                Gdiplus::REAL(area.right - area.left),
                Gdiplus::REAL(area.bottom - area.top)
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
        void GdiGraphicsWrapper::FillRect(const Gdiplus::RectF & area, const Gdiplus::Brush & brush)
        {
            api->FillRectangle(&brush, area);
        }

        /*
            Fill a rectangle.
        */
        void GdiGraphicsWrapper::FillRect(const Gdiplus::Rect & area, const Gdiplus::Brush & brush)
        {
            api->FillRectangle(&brush, area);
        }

        /*
            Fill a rectangle.
        */
        void GdiGraphicsWrapper::FillRect(const RECT & area, const Gdiplus::Brush & brush)
        {
            Gdiplus::Rect newArea = { area.left, area.top, area.right - area.left, area.bottom - area.top };
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
        void GdiGraphicsWrapper::SetDeviceHandle(HDC & handle)
        {
            this->api.reset(new Gdiplus::Graphics(handle));
            this->euroscopeFont.reset(new Gdiplus::Font(handle, font));
        }
    }  // namespace Windows
}  // namespace UKControllerPlugin
