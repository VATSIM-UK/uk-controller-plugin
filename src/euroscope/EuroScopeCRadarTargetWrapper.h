#pragma once
#include "euroscope/EuroScopeCRadarTargetInterface.h"

namespace UKControllerPlugin {
    namespace Euroscope {

        class EuroScopeCRadarTargetWrapper : public UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface
        {
            public:
                explicit EuroScopeCRadarTargetWrapper(EuroScopePlugIn::CRadarTarget & originalData);
                const std::string GetCallsign(void) const;
                int GetFlightLevel(void) const;
                const EuroScopePlugIn::CPosition GetPosition(void) const;
                const int GetGroundSpeed(void) const;
            private:
                // The original object from EuroScope
                EuroScopePlugIn::CRadarTarget originalData;
        };
    }  // namespace Euroscope
}  // namespace UKControllerPlugin
