#pragma once
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroScopeCFlightPlanWrapper.h"

namespace UKControllerPlugin::Euroscope {

    /*
        Wrapper around the EuroScope CFlightPlan class.
    */
    class EuroScopeCFlightPlanWrapper : public UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface
    {
        public:
        explicit EuroScopeCFlightPlanWrapper(EuroScopePlugIn::CFlightPlan originalData);
        void AnnotateFlightStrip(int index, std::string data) const override;
        std::string GetAnnotation(int index) const override;
        std::string GetAircraftType() const override;
        std::string GetAssignedSquawk() const override;
        std::string GetCallsign() const override;
        int GetClearedAltitude() const override;
        int GetCruiseLevel() const override;
        std::string GetDestination() const override;
        double GetDistanceFromOrigin() const override;
        double GetDistanceToDestination() const override;
        std::string GetExpectedDepartureTime() const override;
        virtual UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface& GetExtractedRoute() const override;
        [[nodiscard]] std::shared_ptr<Flightplan::ParsedFlightplan> GetParsedFlightplan() const override;
        std::string GetFlightRules() const override;
        std::string GetGroundState() const override;
        std::string GetIcaoWakeCategory() const override;
        std::string GetOrigin() const override;
        std::string GetRawRouteString() const override;
        std::string GetSidName() const override;
        std::string GetDepartureRunway() const override;
        bool HasAssignedSquawk() const override;
        bool HasControllerClearedAltitude() const override;
        bool HasControllerAssignedHeading() const override;
        bool HasSid() const override;
        bool IsSimulated() const override;
        bool IsTracked() const override;
        bool IsTrackedByUser() const override;
        bool IsValid() const override;
        bool IsVfr() const override;
        void SetClearedAltitude(int cleared) override;
        void SetHeading(int heading) override;
        void SetSquawk(std::string squawk) override;
        EuroScopePlugIn::CFlightPlan GetEuroScopeObject() const override;
        [[nodiscard]] auto GetRemarks() const -> std::string override;

        private:
        // The original data that we're wrapping.
        EuroScopePlugIn::CFlightPlan originalData;

        // The value ES passes as cleared altitude when none is set.
        const int euroScopeNoControllerClearedAltitude = 0;

        // The callsign that will be returned by GetTrackingControllerCallsign if untracked.
        const std::string notTrackedControllerCallsign = "";

        // Extracted route
        mutable std::shared_ptr<EuroscopeExtractedRouteInterface> extractedRoute;

        // Parsed flightplan
        mutable std::shared_ptr<Flightplan::ParsedFlightplan> parsedFlightplan;
    };
} // namespace UKControllerPlugin::Euroscope
