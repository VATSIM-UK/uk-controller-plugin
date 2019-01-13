#pragma once
#include "euroscope/EuroscopeExtractedRouteInterface.h"

namespace UKControllerPlugin {
    namespace Euroscope {

        /*
            Interface for interacting with the wrapper around the EuroScope CFlightPlan class.
        */
        class EuroScopeCFlightPlanInterface
        {
            public:
                virtual ~EuroScopeCFlightPlanInterface(void) {}
                virtual const std::string GetCallsign(void) const = 0;
                virtual const int GetClearedAltitude(void) const = 0;
                virtual const int GetCruiseLevel(void) const = 0;
                virtual const std::string GetDestination(void) const = 0;
                virtual const double GetDistanceFromOrigin(void) const = 0;
                virtual std::string GetExpectedDepartureTime(void) const = 0;
                virtual UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface GetExtractedRoute(
                    void
                ) const = 0;
                virtual std::string GetFlightRules(void) const = 0;
                virtual std::string GetGroundState(void) const = 0;
                virtual const std::string GetOrigin(void) const = 0;
                virtual const std::string GetSidName(void) const = 0;
                virtual std::string GetAssignedSquawk(void) const = 0;
                virtual std::string GetAircraftType(void) const = 0;
                virtual std::string GetIcaoWakeCategory(void) const = 0;
                virtual bool HasAssignedSquawk(void) const = 0;
                virtual const bool HasControllerClearedAltitude(void) const = 0;
                virtual bool HasSid(void) const = 0;
                virtual void SetClearedAltitude(int cleared) = 0;
                virtual void SetSquawk(std::string squawk) = 0;
                virtual bool IsSimulated(void) const = 0;
                virtual const bool IsTracked(void) const = 0;
                virtual const bool IsTrackedByUser(void) const = 0;
                virtual bool IsValid(void) const = 0;
        };
    }  // namespace Euroscope
}  // namespace UKControllerPlugin
