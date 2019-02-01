#pragma once
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroScopeCFlightPlanWrapper.h"

namespace UKControllerPlugin {
    namespace Euroscope {

        /*
            Wrapper around the EuroScope CFlightPlan class.
        */
        class EuroScopeCFlightPlanWrapper : public UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface
        {
            public:
                explicit EuroScopeCFlightPlanWrapper(EuroScopePlugIn::CFlightPlan originalData);
                std::string GetAircraftType(void) const override;
                std::string GetAssignedSquawk(void) const;
                const std::string GetCallsign(void) const;
                const int GetClearedAltitude(void) const;
                virtual const int GetCruiseLevel(void) const;
                const std::string GetDestination(void) const;
                const double GetDistanceFromOrigin(void) const;
                std::string GetExpectedDepartureTime(void) const override;
                virtual UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface GetExtractedRoute(void) const;
                std::string GetFlightRules(void) const;
                std::string GetGroundState(void) const;
                std::string GetIcaoWakeCategory(void) const override;
                const std::string GetOrigin(void) const;
                std::string GetRawRouteString(void) const override;
                const std::string GetSidName(void) const;
                bool HasAssignedSquawk(void) const;
                const bool HasControllerClearedAltitude(void) const;
                bool HasSid(void) const;
                bool IsSimulated(void) const;
                const bool IsTracked(void) const;
                const bool IsTrackedByUser(void) const;
                bool IsValid(void) const;
                bool IsVfr(void) const override;
                void SetClearedAltitude(int cleared);
                void SetSquawk(std::string squawk);

                // The callsign that will be returned by GetTrackingControllerCallsign if untracked.
                const std::string notTrackedControllerCallsign = "";

            private:
                // The original data that we're wrapping.
                EuroScopePlugIn::CFlightPlan originalData;

                // The value ES passes as cleared altitude when none is set.
                const int euroScopeNoControllerClearedAltitude = 0;
            };
    }  // namespace Euroscope
}  // namespace UKControllerPlugin
