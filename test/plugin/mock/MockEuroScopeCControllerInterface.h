#pragma once
#include "euroscope/EuroScopeCControllerInterface.h"

namespace UKControllerPluginTest {
    namespace Euroscope {

        class MockEuroScopeCControllerInterface : public UKControllerPlugin::Euroscope::EuroScopeCControllerInterface
        {
            public:
            MOCK_CONST_METHOD0(IsVatsimRecognisedController, bool(void));
            MOCK_CONST_METHOD0(GetCallsign, const std::string(void));
            MOCK_CONST_METHOD0(GetFrequency, double(void));
            MOCK_CONST_METHOD0(GetControllerName, const std::string(void));
            MOCK_CONST_METHOD0(HasActiveFrequency, bool(void));
            MOCK_CONST_METHOD0(IsCurrentUser, bool(void));
        };
    } // namespace Euroscope
} // namespace UKControllerPluginTest
