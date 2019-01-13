#pragma once
#include "pch/pch.h"
#include "euroscope/EuroScopeCControllerInterface.h"

namespace UKControllerPluginTest {
    namespace Euroscope {

        class MockEuroScopeCControllerInterface : public UKControllerPlugin::Euroscope::EuroScopeCControllerInterface
        {
            public:
                MOCK_CONST_METHOD0(GetCallsign, const std::string(void));
                MOCK_CONST_METHOD0(GetFrequency, const double(void));
                MOCK_CONST_METHOD0(GetControllerName, const std::string(void));
                MOCK_CONST_METHOD0(HasActiveFrequency, const bool(void));
                MOCK_CONST_METHOD0(IsCurrentUser, const bool(void));
        };
    }  // namespace Euroscope
}  // namespace UKControllerPluginTest
