#pragma once
#include "euroscope/EuroScopePlugIn.h"

namespace UKControllerPlugin {
    namespace Euroscope {

        class EuroScopeCRadarTargetInterface
        {
            public:
            virtual ~EuroScopeCRadarTargetInterface(void)
            {
            }
            virtual const std::string GetCallsign(void) const = 0;
            virtual int GetFlightLevel(void) const = 0;
            virtual const EuroScopePlugIn::CPosition GetPosition(void) const = 0;
            virtual int GetGroundSpeed(void) const = 0;
            virtual int GetVerticalSpeed(void) const = 0;
            virtual double GetHeading() const = 0;
            virtual EuroScopePlugIn::CRadarTarget& GetEuroScopeObject() const = 0;
        };
    } // namespace Euroscope
} // namespace UKControllerPlugin
