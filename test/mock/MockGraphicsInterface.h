#pragma once
#include "graphics/GdiGraphicsInterface.h"
#include <gdiplustypes.h>

namespace UKControllerPluginTest {
    namespace Windows {

        class MockGraphicsInterface : public UKControllerPlugin::Windows::GdiGraphicsInterface
        {
            public:

                MOCK_METHOD3(DrawLine, void(const Gdiplus::Pen &, const Gdiplus::Point &, const Gdiplus::Point &));
                MOCK_METHOD2(DrawPath, void(const Gdiplus::GraphicsPath &, const Gdiplus::Pen &));
                MOCK_METHOD2(DrawRect, void(const Gdiplus::RectF &, const Gdiplus::Pen &));
                MOCK_METHOD2(DrawRect, void(const Gdiplus::Rect &, const Gdiplus::Pen &));
                MOCK_METHOD2(DrawRect, void(const RECT &, const Gdiplus::Pen &));
                MOCK_METHOD2(DrawCircle, void(const Gdiplus::RectF &, const Gdiplus::Pen &));
                MOCK_METHOD2(DrawCircle, void(const Gdiplus::Rect &, const Gdiplus::Pen &));
                MOCK_METHOD2(DrawDiamond, void(const Gdiplus::RectF &, const Gdiplus::Pen &));
                MOCK_METHOD3(DrawString, void(std::wstring, const Gdiplus::RectF &, const Gdiplus::Brush &));
                MOCK_METHOD3(DrawString, void(std::wstring, const Gdiplus::Rect &, const Gdiplus::Brush &));
                MOCK_METHOD3(DrawString, void(std::wstring, const RECT &, const Gdiplus::Brush &));
                MOCK_METHOD2(DrawCircle, void(const Gdiplus::RectF &, const Gdiplus::Brush &));
                MOCK_METHOD2(FillRect, void(const Gdiplus::Rect &, const Gdiplus::Brush &));
                MOCK_METHOD2(FillRect, void(const Gdiplus::RectF &, const Gdiplus::Brush &));
                MOCK_METHOD2(FillRect, void(const RECT &, const Gdiplus::Brush &));
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
        };
    }  // namespace Windows
}  // namespace UKControllerPluginTest
