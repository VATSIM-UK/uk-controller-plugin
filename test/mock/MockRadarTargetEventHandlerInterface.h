#pragma once
#include "euroscope/RadarTargetEventHandlerInterface.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"

namespace UKControllerPluginTest {
    namespace EventHandler {

        class MockRadarTargetEventHandlerInterface :
            public UKControllerPlugin::Euroscope::RadarTargetEventHandlerInterface
        {
            public:
                MOCK_METHOD1(
                    RadarTargetPositionUpdateEvent,
                    void(UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface & radarTarget)
                );
      };
    }  // namespace EventHandler
}  // namespace UKControllerPluginTest
