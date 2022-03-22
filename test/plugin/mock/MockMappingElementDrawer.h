#pragma once
#include "mapping/MappingElementDrawer.h"

using UKControllerPlugin::Graphics::GdiGraphicsInterface;
using UKControllerPlugin::Mapping::MappingElementDrawer;

namespace UKControllerPluginTest::Mapping {
    class MockMappingElementDrawer : public MappingElementDrawer
    {
        public:
        MOCK_METHOD(void, Draw, (GdiGraphicsInterface&, const Gdiplus::Rect&), (override));
    };
} // namespace UKControllerPluginTest::Mapping
