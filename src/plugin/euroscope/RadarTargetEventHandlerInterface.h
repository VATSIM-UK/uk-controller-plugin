#pragma once
#include "euroscope/EuroScopeCRadarTargetInterface.h"

namespace UKControllerPlugin {
    namespace Euroscope {

        /*
            An interface for handling events related to radar targets.
        */
        class RadarTargetEventHandlerInterface
        {
            public:
                virtual ~RadarTargetEventHandlerInterface(void) {}
                virtual void RadarTargetPositionUpdateEvent(
                    UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface & radarTarget
                ) = 0;
        };
    }  // namespace Euroscope
}  // namespace UKControllerPlugin
