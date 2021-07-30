#pragma once
#include "graphics/GdiGraphicsInterface.h"
#include <gdiplustypes.h>

namespace UKControllerPluginTest {
    namespace Windows {

        class MockGraphicsInterface : public UKControllerPlugin::Windows::GdiGraphicsInterface
        {
            public:
                ~MockGraphicsInterface() override = default;
                MOCK_METHOD3(DrawLine, void(const Gdiplus::Pen &, const Gdiplus::Point &, const Gdiplus::Point &));
                MOCK_METHOD2(DrawPath, void(const Gdiplus::GraphicsPath &, const Gdiplus::Pen &));
                MOCK_METHOD2(DrawRectRectF, void(const Gdiplus::RectF &, const Gdiplus::Pen &));
                MOCK_METHOD2(DrawRectRect, void(const Gdiplus::Rect &, const Gdiplus::Pen &));
                MOCK_METHOD2(DrawRectRegularRect, void(const RECT &, const Gdiplus::Pen &));
                MOCK_METHOD2(DrawCircle, void(const Gdiplus::RectF &, const Gdiplus::Pen &));
                MOCK_METHOD2(DrawCircle, void(const Gdiplus::Rect &, const Gdiplus::Pen &));
                MOCK_METHOD2(DrawDiamond, void(const Gdiplus::RectF &, const Gdiplus::Pen &));
                MOCK_METHOD3(DrawStringRectF, void(std::wstring, const Gdiplus::RectF &, const Gdiplus::Brush &));
                MOCK_METHOD3(DrawStringRect, void(std::wstring, const Gdiplus::Rect &, const Gdiplus::Brush &));
                MOCK_METHOD3(DrawStringRegularRect, void(std::wstring, const RECT &, const Gdiplus::Brush &));
                MOCK_METHOD2(DrawCircle, void(const Gdiplus::RectF &, const Gdiplus::Brush &));
                MOCK_METHOD2(FillRectRect, void(const Gdiplus::Rect &, const Gdiplus::Brush &));
                MOCK_METHOD2(FillRectRectF, void(const Gdiplus::RectF &, const Gdiplus::Brush &));
                MOCK_METHOD2(FillRectRegularRect, void(const RECT &, const Gdiplus::Brush &));
                MOCK_METHOD1(SetAntialias, void(bool));
                MOCK_METHOD1(SetDeviceHandle, void(HDC &));

                void Clipped(Gdiplus::Region&, std::function<void()> func) override
                {
                    func();
                }

                void Translated(Gdiplus::REAL, Gdiplus::REAL, std::function<void()> func) override
                {
                    func();
                }

                void Scaled(Gdiplus::REAL, Gdiplus::REAL, std::function<void()> func) override
                {
                    func();
                }

                void Rotated(Gdiplus::REAL, std::function<void()> func) override
                {
                    func();
                }

                MOCK_METHOD(std::shared_ptr<Gdiplus::Matrix>, GetTransform, (), (override));
                MOCK_METHOD(Gdiplus::RectF, GetClipBounds, (), (override));
                MOCK_METHOD(void, FillPolygon, (Gdiplus::Point*, const Gdiplus::Brush&, int), (override));

                void DrawRect(const Gdiplus::RectF& area, const Gdiplus::Pen& pen) override
                {
                    this->DrawRectRectF(area, pen);
                }

                void DrawRect(const Gdiplus::Rect& area, const Gdiplus::Pen& pen) override
                {
                    this->DrawRectRect(area, pen);
                }

                void DrawRect(const RECT& area, const Gdiplus::Pen& pen) override
                {
                    this->DrawRectRegularRect(area, pen);
                }

                void DrawString(std::wstring text, const Gdiplus::RectF& area, const Gdiplus::Brush& brush) override
                {
                    this->DrawStringRectF(text, area, brush);
                }

                void DrawString(std::wstring text, const Gdiplus::Rect& area, const Gdiplus::Brush& brush) override
                {
                    this->DrawStringRect(text, area, brush);
                }

                void DrawString(std::wstring text, const RECT& area, const Gdiplus::Brush& brush) override
                {
                    this->DrawStringRegularRect(text, area, brush);
                }

                void FillRect(const Gdiplus::RectF& area, const Gdiplus::Brush& brush) override
                {
                    this->FillRectRectF(area, brush);
                }

                void FillRect(const Gdiplus::Rect& area, const Gdiplus::Brush& brush) override
                {
                    this->FillRectRect(area, brush);
                }

                void FillRect(const RECT& area, const Gdiplus::Brush& brush) override
                {
                    this->FillRectRegularRect(area, brush);
                }

                MOCK_METHOD(
                    void,
                    DrawLine, (const Gdiplus::Pen&, const Gdiplus::PointF&, const Gdiplus::PointF&),
                    (override)
                );
                MOCK_METHOD(void, FillCircle, (const Gdiplus::RectF&, const Gdiplus::Brush&), (override));
                MOCK_METHOD(void, FillDiamond, (const Gdiplus::RectF&, const Gdiplus::Brush&), (override));
        };
    }  // namespace Windows
}  // namespace UKControllerPluginTest
