#include "pch/pch.h"
#include "euroscope/EuroScopeCFlightPlanWrapper.h"
#include "squawk/SquawkValidator.h"

using UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface;
using UKControllerPlugin::Squawk::SquawkValidator;

namespace UKControllerPlugin {
    namespace Euroscope {

        EuroScopeCFlightPlanWrapper::EuroScopeCFlightPlanWrapper(EuroScopePlugIn::CFlightPlan originalData)
        {
            this->originalData = originalData;
        }

        /*
            Note indexes begin from 0
        */
        void EuroScopeCFlightPlanWrapper::AnnotateFlightStrip(int index, std::string data) const
        {
            this->originalData.GetControllerAssignedData().SetFlightStripAnnotation(index, data.c_str());
        }

        std::string EuroScopeCFlightPlanWrapper::GetAnnotation(int index) const
        {
            return this->originalData.GetControllerAssignedData().GetFlightStripAnnotation(index);
        }

        std::string EuroScopeCFlightPlanWrapper::GetAircraftType(void) const
        {
            return this->originalData.GetFlightPlanData().GetAircraftFPType();
        }

        const std::string EuroScopeCFlightPlanWrapper::GetCallsign(void) const
        {
            return this->originalData.GetCallsign();
        }

        const int EuroScopeCFlightPlanWrapper::GetClearedAltitude(void) const
        {
            return this->originalData.GetControllerAssignedData().GetClearedAltitude();
        }

        const int EuroScopeCFlightPlanWrapper::GetCruiseLevel(void) const
        {
            return this->originalData.GetFinalAltitude();
        }

        const std::string EuroScopeCFlightPlanWrapper::GetDestination(void) const
        {
            return this->originalData.GetFlightPlanData().GetDestination();
        }

        const double EuroScopeCFlightPlanWrapper::GetDistanceFromOrigin(void) const
        {
            return this->originalData.GetDistanceFromOrigin();
        }

        std::string EuroScopeCFlightPlanWrapper::GetExpectedDepartureTime(void) const
        {
            return this->originalData.GetFlightPlanData().GetEstimatedDepartureTime();
        }

        EuroscopeExtractedRouteInterface EuroScopeCFlightPlanWrapper::GetExtractedRoute(void) const
        {
            return EuroscopeExtractedRouteInterface(this->originalData.GetExtractedRoute());
        }

        std::string EuroScopeCFlightPlanWrapper::GetFlightRules(void) const
        {
            return this->originalData.GetFlightPlanData().GetPlanType();
        }

        std::string EuroScopeCFlightPlanWrapper::GetGroundState(void) const
        {
            return this->originalData.GetGroundState();
        }

        std::string EuroScopeCFlightPlanWrapper::GetIcaoWakeCategory(void) const
        {
            return std::string(1, this->originalData.GetFlightPlanData().GetAircraftWtc());
        }

        std::string EuroScopeCFlightPlanWrapper::GetAssignedSquawk(void) const
        {
            return this->originalData.GetControllerAssignedData().GetSquawk();
        }

        const std::string EuroScopeCFlightPlanWrapper::GetOrigin(void) const
        {
            return this->originalData.GetFlightPlanData().GetOrigin();
        }

        std::string EuroScopeCFlightPlanWrapper::GetRawRouteString(void) const
        {
            return this->originalData.GetFlightPlanData().GetRoute();
        }

        const std::string EuroScopeCFlightPlanWrapper::GetSidName(void) const
        {
            return this->originalData.GetFlightPlanData().GetSidName();
        }

        bool EuroScopeCFlightPlanWrapper::HasAssignedSquawk(void) const
        {
            std::string squawk = this->originalData.GetControllerAssignedData().GetSquawk();
            return SquawkValidator::ValidSquawk(squawk) &&
                squawk != "0200" &&
                squawk != "2200" &&
                squawk != "1200" &&
                squawk != "2000" &&
                squawk != "0000";
        }

        const bool EuroScopeCFlightPlanWrapper::HasControllerClearedAltitude(void) const
        {
            return this->originalData.GetControllerAssignedData().GetClearedAltitude() !=
                this->euroScopeNoControllerClearedAltitude;
        }

        const bool EuroScopeCFlightPlanWrapper::HasControllerAssignedHeading() const
        {
            return this->originalData.GetControllerAssignedData().GetAssignedHeading() != 0;
        }

        bool EuroScopeCFlightPlanWrapper::HasSid(void) const
        {
            return this->originalData.GetFlightPlanData().GetSidName() != "";
        }

        /*
            Sets the cleared altitude.
        */
        void EuroScopeCFlightPlanWrapper::SetClearedAltitude(int cleared)
        {
            this->originalData.GetControllerAssignedData().SetClearedAltitude(cleared);
        }

        void EuroScopeCFlightPlanWrapper::SetHeading(int heading)
        {
            this->originalData.GetControllerAssignedData().SetAssignedHeading(heading);
        }

        void EuroScopeCFlightPlanWrapper::SetSquawk(std::string squawk)
        {
            this->originalData.GetControllerAssignedData().SetSquawk(squawk.c_str());
        }

        EuroScopePlugIn::CFlightPlan EuroScopeCFlightPlanWrapper::GetEuroScopeObject() const
        {
            return this->originalData;
        }

        /*
            Returns true if its actually real and not Euroscope pretending.
        */
        bool EuroScopeCFlightPlanWrapper::IsSimulated(void) const
        {
            return this->originalData.GetSimulated();
        }

        /*
            Returns true if the aircraft is being tracked by anyone at all.
        */
        const bool EuroScopeCFlightPlanWrapper::IsTracked(void) const
        {
            return this->notTrackedControllerCallsign.compare(this->originalData.GetTrackingControllerCallsign()) != 0;
        }

        /*
            Returns true if the current controller tracking the flightplan is the user.
        */
        const bool EuroScopeCFlightPlanWrapper::IsTrackedByUser(void) const
        {
            return this->originalData.GetTrackingControllerIsMe();
        }

        /*
            Returns true if Euroscope deems that the flightplan is valid.
        */
        bool EuroScopeCFlightPlanWrapper::IsValid(void) const
        {
            return this->originalData.IsValid();
        }

        /*
            Returns true if the flightplan is VFR
        */
        bool EuroScopeCFlightPlanWrapper::IsVfr(void) const
        {
            return strcmp(this->originalData.GetFlightPlanData().GetPlanType(), "V") == 0;
        }
    }  // namespace Euroscope
}  // namespace UKControllerPlugin
