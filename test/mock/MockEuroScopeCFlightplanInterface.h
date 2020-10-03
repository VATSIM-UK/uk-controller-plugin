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
                MOCK_CONST_METHOD0(GetCallsign, const std::string(void));
                MOCK_CONST_METHOD0(GetClearedAltitude, const int(void));
                MOCK_CONST_METHOD0(GetCruiseLevel, const int(void));
                MOCK_CONST_METHOD0(GetDestination, const std::string (void));
                MOCK_CONST_METHOD0(GetDistanceFromOrigin, const double(void));
                MOCK_CONST_METHOD0(GetExpectedDepartureTime, std::string(void));
                MOCK_CONST_METHOD0(
                    GetExtractedRoute,
                    UKControllerPlugin::Euroscope::EuroscopeExtractedRouteInterface(void)
                );
                MOCK_CONST_METHOD0(GetFlightRules, std::string(void));
                MOCK_CONST_METHOD0(GetGroundState, std::string(void));
                MOCK_CONST_METHOD0(GetIcaoWakeCategory, std::string(void));
                MOCK_CONST_METHOD0(GetOrigin, const std::string(void));
                MOCK_CONST_METHOD0(GetRawRouteString, std::string(void));
                MOCK_CONST_METHOD0(GetSidName, const std::string(void));
                MOCK_CONST_METHOD0(GetAssignedSquawk, std::string(void));
                MOCK_CONST_METHOD0(HasControllerClearedAltitude, const bool(void));
                MOCK_CONST_METHOD0(HasAssignedSquawk, bool(void));
                MOCK_CONST_METHOD0(HasSid, bool(void));
                MOCK_CONST_METHOD0(IsSimulated, bool(void));
                MOCK_CONST_METHOD0(IsTracked, const bool(void));
                MOCK_CONST_METHOD0(IsTrackedByUser, const bool(void));
                MOCK_CONST_METHOD0(IsValid, bool(void));
                MOCK_CONST_METHOD0(IsVfr, bool(void));
                MOCK_METHOD1(SetClearedAltitude, void(int));
                MOCK_METHOD1(SetSquawk, void(std::string));
        };
    }  // namespace Euroscope
}  // namespace UKControllerPluginTest
