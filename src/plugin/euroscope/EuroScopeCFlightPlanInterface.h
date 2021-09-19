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
            virtual ~EuroScopeCFlightPlanInterface(void)
            {
            }
            virtual void AnnotateFlightStrip(int index, std::string data) const = 0;
            virtual std::string GetAnnotation(int index) const = 0;
            virtual const std::string GetCallsign(void) const = 0;
            virtual int GetClearedAltitude(void) const = 0;
            virtual int GetCruiseLevel(void) const = 0;
            virtual const std::string GetDestination(void) const = 0;
            virtual double GetDistanceFromOrigin(void) const = 0;
            virtual std::string GetExpectedDepartureTime(void) const = 0;
            virtual UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface GetExtractedRoute(void) const = 0;
            virtual std::string GetFlightRules(void) const = 0;
            virtual std::string GetGroundState(void) const = 0;
            virtual const std::string GetOrigin(void) const = 0;
            virtual std::string GetRawRouteString(void) const = 0;
            virtual const std::string GetSidName(void) const = 0;
            virtual std::string GetAssignedSquawk(void) const = 0;
            virtual std::string GetAircraftType(void) const = 0;
            virtual std::string GetIcaoWakeCategory(void) const = 0;
            virtual bool HasAssignedSquawk(void) const = 0;
            virtual bool HasControllerClearedAltitude(void) const = 0;
            virtual bool HasControllerAssignedHeading(void) const = 0;
            virtual bool HasSid(void) const = 0;
            virtual void SetClearedAltitude(int cleared) = 0;
            virtual void SetHeading(int heading) = 0;
            virtual void SetSquawk(std::string squawk) = 0;
            virtual bool IsSimulated(void) const = 0;
            virtual bool IsTracked(void) const = 0;
            virtual bool IsTrackedByUser(void) const = 0;
            virtual bool IsValid(void) const = 0;
            virtual bool IsVfr(void) const = 0;
            virtual EuroScopePlugIn::CFlightPlan GetEuroScopeObject() const = 0;
        };
    } // namespace Euroscope
} // namespace UKControllerPlugin
