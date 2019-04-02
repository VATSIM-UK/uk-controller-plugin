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
        };
    }  // namespace Windows
}  // namespace UKControllerPluginTest
