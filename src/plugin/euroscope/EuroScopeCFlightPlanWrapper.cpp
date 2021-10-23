#include "EuroScopeCFlightPlanWrapper.h"
#include "squawk/SquawkValidator.h"

using UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface;
using UKControllerPlugin::Squawk::SquawkValidator;

namespace UKControllerPlugin::Euroscope {

    EuroScopeCFlightPlanWrapper::EuroScopeCFlightPlanWrapper(EuroScopePlugIn::CFlightPlan originalData)
        : originalData(originalData)
    {
    }

    /*
        Note indexes begin from 0
    */
    void EuroScopeCFlightPlanWrapper::AnnotateFlightStrip(int index, std::string data) const
    {
        this->originalData.GetControllerAssignedData().SetFlightStripAnnotation(index, data.c_str());
    }

    auto EuroScopeCFlightPlanWrapper::GetAnnotation(int index) const -> std::string
    {
        return this->originalData.GetControllerAssignedData().GetFlightStripAnnotation(index);
    }

    auto EuroScopeCFlightPlanWrapper::GetAircraftType() const -> std::string
    {
        return this->originalData.GetFlightPlanData().GetAircraftFPType();
    }

    auto EuroScopeCFlightPlanWrapper::GetCallsign() const -> std::string
    {
        return this->originalData.GetCallsign();
    }

    auto EuroScopeCFlightPlanWrapper::GetClearedAltitude() const -> int
    {
        return this->originalData.GetControllerAssignedData().GetClearedAltitude();
    }

    auto EuroScopeCFlightPlanWrapper::GetCruiseLevel() const -> int
    {
        return this->originalData.GetFinalAltitude();
    }

    auto EuroScopeCFlightPlanWrapper::GetDestination() const -> std::string
    {
        return this->originalData.GetFlightPlanData().GetDestination();
    }

    auto EuroScopeCFlightPlanWrapper::GetDistanceFromOrigin() const -> double
    {
        return this->originalData.GetDistanceFromOrigin();
    }

    auto EuroScopeCFlightPlanWrapper::GetExpectedDepartureTime() const -> std::string
    {
        return this->originalData.GetFlightPlanData().GetEstimatedDepartureTime();
    }

    auto EuroScopeCFlightPlanWrapper::GetExtractedRoute() const -> EuroscopeExtractedRouteInterface
    {
        return EuroscopeExtractedRouteInterface(this->originalData.GetExtractedRoute());
    }

    auto EuroScopeCFlightPlanWrapper::GetFlightRules() const -> std::string
    {
        return this->originalData.GetFlightPlanData().GetPlanType();
    }

    auto EuroScopeCFlightPlanWrapper::GetGroundState() const -> std::string
    {
        return this->originalData.GetGroundState();
    }

    auto EuroScopeCFlightPlanWrapper::GetIcaoWakeCategory() const -> std::string
    {
        return {this->originalData.GetFlightPlanData().GetAircraftWtc()};
    }

    auto EuroScopeCFlightPlanWrapper::GetAssignedSquawk() const -> std::string
    {
        return this->originalData.GetControllerAssignedData().GetSquawk();
    }

    auto EuroScopeCFlightPlanWrapper::GetOrigin() const -> std::string
    {
        return this->originalData.GetFlightPlanData().GetOrigin();
    }

    auto EuroScopeCFlightPlanWrapper::GetRawRouteString() const -> std::string
    {
        return this->originalData.GetFlightPlanData().GetRoute();
    }

    auto EuroScopeCFlightPlanWrapper::GetSidName() const -> std::string
    {
        return this->originalData.GetFlightPlanData().GetSidName();
    }

    auto EuroScopeCFlightPlanWrapper::HasAssignedSquawk() const -> bool
    {
        std::string squawk = this->originalData.GetControllerAssignedData().GetSquawk();
        return SquawkValidator::ValidSquawk(squawk) && squawk != "0200" && squawk != "2200" && squawk != "1200" &&
               squawk != "2000" && squawk != "0000";
    }

    auto EuroScopeCFlightPlanWrapper::HasControllerClearedAltitude() const -> bool
    {
        return this->originalData.GetControllerAssignedData().GetClearedAltitude() !=
               this->euroScopeNoControllerClearedAltitude;
    }

    auto EuroScopeCFlightPlanWrapper::HasControllerAssignedHeading() const -> bool
    {
        return this->originalData.GetControllerAssignedData().GetAssignedHeading() != 0;
    }

    auto EuroScopeCFlightPlanWrapper::HasSid() const -> bool
    {
        return !std::string(this->originalData.GetFlightPlanData().GetSidName()).empty();
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

    auto EuroScopeCFlightPlanWrapper::GetEuroScopeObject() const -> EuroScopePlugIn::CFlightPlan
    {
        return this->originalData;
    }

    /*
        Returns true if its actually real and not Euroscope pretending.
    */
    auto EuroScopeCFlightPlanWrapper::IsSimulated() const -> bool
    {
        return this->originalData.GetSimulated();
    }

    /*
        Returns true if the aircraft is being tracked by anyone at all.
    */
    auto EuroScopeCFlightPlanWrapper::IsTracked() const -> bool
    {
        return this->notTrackedControllerCallsign != this->originalData.GetTrackingControllerCallsign();
    }

    /*
        Returns true if the current controller tracking the flightplan is the user.
    */
    auto EuroScopeCFlightPlanWrapper::IsTrackedByUser() const -> bool
    {
        return this->originalData.GetTrackingControllerIsMe();
    }

    /*
        Returns true if Euroscope deems that the flightplan is valid.
    */
    auto EuroScopeCFlightPlanWrapper::IsValid() const -> bool
    {
        return this->originalData.IsValid();
    }

    /*
        Returns true if the flightplan is VFR
    */
    auto EuroScopeCFlightPlanWrapper::IsVfr() const -> bool
    {
        return strcmp(this->originalData.GetFlightPlanData().GetPlanType(), "V") == 0;
    }

    auto EuroScopeCFlightPlanWrapper::GetDistanceToDestination() const -> double
    {
        return this->originalData.GetDistanceToDestination();
    }
} // namespace UKControllerPlugin::Euroscope
