#pragma once
#include "euroscope/AsrEventHandlerInterface.h"
#include "euroscope/UserSetting.h"

namespace UKControllerPluginTest {
    namespace RadarScreen {

        class MockAsrEventHandlerInterface : public UKControllerPlugin::Euroscope::AsrEventHandlerInterface
        {
            public:
            MOCK_METHOD1(AsrLoadedEvent, void(UKControllerPlugin::Euroscope::UserSetting&));
            MOCK_METHOD1(AsrClosingEvent, void(UKControllerPlugin::Euroscope::UserSetting&));
        };
    } // namespace RadarScreen
} // namespace UKControllerPluginTest
