#pragma once
#include "euroscope/EuroscopeExtractedRouteInterface.h"

namespace UKControllerPlugin::Flightplan {
    class ParsedFlightplan;
} // namespace UKControllerPlugin::Flightplan

namespace UKControllerPlugin::Euroscope {

    /*
        Interface for interacting with the wrapper around the EuroScope CFlightPlan class.
    */
    class EuroScopeCFlightPlanInterface
    {
        public:
        virtual ~EuroScopeCFlightPlanInterface() = default;
        virtual void AnnotateFlightStrip(int index, std::string data) const = 0;
        [[nodiscard]] virtual std::string GetAnnotation(int index) const = 0;
        [[nodiscard]] virtual std::string GetCallsign() const = 0;
        [[nodiscard]] virtual int GetClearedAltitude() const = 0;
        [[nodiscard]] virtual int GetCruiseLevel() const = 0;
        [[nodiscard]] virtual std::string GetDestination() const = 0;
        [[nodiscard]] virtual double GetDistanceFromOrigin() const = 0;
        [[nodiscard]] virtual double GetDistanceToDestination() const = 0;
        [[nodiscard]] virtual std::string GetExpectedDepartureTime() const = 0;
        [[nodiscard]] virtual UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface&
        GetExtractedRoute() const = 0;
        [[nodiscard]] virtual std::shared_ptr<Flightplan::ParsedFlightplan>
        GetParsedFlightplan() = 0;
        [[nodiscard]] virtual std::string GetFlightRules() const = 0;
        [[nodiscard]] virtual std::string GetGroundState() const = 0;
        [[nodiscard]] virtual std::string GetOrigin() const = 0;
        [[nodiscard]] virtual std::string GetRawRouteString() const = 0;
        [[nodiscard]] virtual std::string GetSidName() const = 0;
        [[nodiscard]] virtual std::string GetAssignedSquawk() const = 0;
        [[nodiscard]] virtual std::string GetAircraftType() const = 0;
        [[nodiscard]] virtual std::string GetIcaoWakeCategory() const = 0;
        [[nodiscard]] virtual bool HasAssignedSquawk() const = 0;
        [[nodiscard]] virtual bool HasControllerClearedAltitude() const = 0;
        [[nodiscard]] virtual bool HasControllerAssignedHeading() const = 0;
        [[nodiscard]] virtual bool HasSid() const = 0;
        virtual void SetClearedAltitude(int cleared) = 0;
        virtual void SetHeading(int heading) = 0;
        virtual void SetSquawk(std::string squawk) = 0;
        [[nodiscard]] virtual bool IsSimulated() const = 0;
        [[nodiscard]] virtual bool IsTracked() const = 0;
        [[nodiscard]] virtual bool IsTrackedByUser() const = 0;
        [[nodiscard]] virtual bool IsValid() const = 0;
        [[nodiscard]] virtual bool IsVfr() const = 0;
        [[nodiscard]] virtual EuroScopePlugIn::CFlightPlan GetEuroScopeObject() const = 0;
        [[nodiscard]] virtual auto GetRemarks() const -> std::string = 0;
        [[nodiscard]] virtual auto GetDepartureRunway() const -> std::string = 0;
    };
} // namespace UKControllerPlugin::Euroscope
