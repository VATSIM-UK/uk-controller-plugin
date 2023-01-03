#pragma once
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroscopeExtractedRouteInterface.h"

namespace UKControllerPluginTest {
    namespace Euroscope {

        class MockEuroScopeCFlightPlanInterface : public UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface
        {
            public:
            MOCK_CONST_METHOD2(AnnotateFlightStrip, void(int, std::string));
            MOCK_CONST_METHOD1(GetAnnotation, std::string(int));
            MOCK_CONST_METHOD0(GetAircraftType, std::string(void));
            MOCK_CONST_METHOD0(GetCallsign, std::string(void));
            MOCK_CONST_METHOD0(GetClearedAltitude, int(void));
            MOCK_CONST_METHOD0(GetCruiseLevel, int(void));
            MOCK_CONST_METHOD0(GetDestination, std::string(void));
            MOCK_CONST_METHOD0(GetDistanceFromOrigin, double(void));
            MOCK_CONST_METHOD0(GetDistanceToDestination, double(void));
            MOCK_CONST_METHOD0(GetExpectedDepartureTime, std::string(void));
            MOCK_CONST_METHOD0(
                GetExtractedRoute, UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface&(void));
            MOCK_CONST_METHOD0(
                GetParsedFlightplan, std::shared_ptr<UKControllerPlugin::Flightplan::ParsedFlightplan>(void));
            MOCK_CONST_METHOD0(GetFlightRules, std::string(void));
            MOCK_CONST_METHOD0(GetGroundState, std::string(void));
            MOCK_CONST_METHOD0(GetIcaoWakeCategory, std::string(void));
            MOCK_CONST_METHOD0(GetOrigin, std::string(void));
            MOCK_CONST_METHOD0(GetRawRouteString, std::string(void));
            MOCK_CONST_METHOD0(GetSidName, std::string(void));
            MOCK_CONST_METHOD0(GetAssignedSquawk, std::string(void));
            MOCK_CONST_METHOD0(GetRemarks, std::string());
            MOCK_CONST_METHOD0(GetDepartureRunway, std::string());
            MOCK_CONST_METHOD0(HasControllerClearedAltitude, bool(void));
            MOCK_CONST_METHOD0(HasControllerAssignedHeading, bool(void));
            MOCK_CONST_METHOD0(HasAssignedSquawk, bool(void));
            MOCK_CONST_METHOD0(HasSid, bool(void));
            MOCK_CONST_METHOD0(IsSimulated, bool(void));
            MOCK_CONST_METHOD0(IsTracked, bool(void));
            MOCK_CONST_METHOD0(IsTrackedByUser, bool(void));
            MOCK_CONST_METHOD0(IsValid, bool(void));
            MOCK_CONST_METHOD0(IsVfr, bool(void));
            MOCK_METHOD1(SetClearedAltitude, void(int));
            MOCK_METHOD1(SetHeading, void(int));
            MOCK_METHOD1(SetSquawk, void(std::string));
            MOCK_CONST_METHOD0(GetEuroScopeObject, EuroScopePlugIn::CFlightPlan(void));
        };
    } // namespace Euroscope
} // namespace UKControllerPluginTest
