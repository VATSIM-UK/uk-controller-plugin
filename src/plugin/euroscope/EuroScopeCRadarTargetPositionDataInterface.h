#pragma once

namespace UKControllerPlugin {
    namespace Euroscope {

        class EuroScopeCRadarTargetPositionDataInterface : public EuroScopePlugIn::CRadarTargetPositionData
        {
            public:
            virtual std::string GetCallsign(void) = 0;
        };
    } // namespace Euroscope
} // namespace UKControllerPlugin
