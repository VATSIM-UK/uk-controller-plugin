#pragma once
#include "bootstrap/BootstrapProviderInterface.h"

using UKControllerPlugin::Bootstrap::BootstrapProviderInterface;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Euroscope::AsrEventHandlerCollection;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection;
using UKControllerPlugin::RadarScreen::MenuToggleableDisplayFactory;
using UKControllerPlugin::RadarScreen::RadarRenderableCollection;

namespace UKControllerPluginTest::Bootstrap {
    class MockBootstrapProvider : public BootstrapProviderInterface
    {
        public:
        MockBootstrapProvider();
        virtual ~MockBootstrapProvider();
        MOCK_METHOD(void, BootstrapPlugin, (PersistenceContainer&), (override));
        MOCK_METHOD(
            void,
            BootstrapRadarScreen,
            (const PersistenceContainer&,
             RadarRenderableCollection&,
             ConfigurableDisplayCollection&,
             AsrEventHandlerCollection&,
             const MenuToggleableDisplayFactory&),
            (override));
    };
} // namespace UKControllerPluginTest::Bootstrap
