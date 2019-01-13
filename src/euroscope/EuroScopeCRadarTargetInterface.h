#pragma once

namespace UKControllerPlugin {
    namespace Euroscope {

        class EuroScopeCRadarTargetInterface
        {
            public:
                virtual ~EuroScopeCRadarTargetInterface(void) {}
                virtual const std::string GetCallsign(void) const = 0;
                virtual int GetFlightLevel(void) const = 0;
                virtual const EuroScopePlugIn::CPosition GetPosition(void) const = 0;
                virtual const int GetGroundSpeed(void) const = 0;
        };
    }  // namespace Euroscope
}  // namespace UKControllerPlugin
