#pragma once
#include "pch/pch.h"
#include "euroscope/EuroscopeSectorFileElementInterface.h""

namespace UKControllerPluginTest {
    namespace Euroscope {

        class MockSectorFileElementInterface :
            public UKControllerPlugin::Euroscope::EuroscopeSectorFileElementInterface
        {
            public:
                MOCK_CONST_METHOD0(Airport, std::string(void));
                MOCK_CONST_METHOD0(Name, std::string(void));
                MOCK_METHOD0(Runway1ActiveForDepartures, bool(void));
                MOCK_METHOD0(Runway1ActiveForArrivals, bool(void));
                MOCK_METHOD0(Runway2ActiveForDepartures, bool(void));
                MOCK_METHOD0(Runway2ActiveForArrivals, bool(void));
                MOCK_METHOD0(Runway1Heading, int(void));
                MOCK_METHOD0(Runway2Heading, int(void));
                MOCK_CONST_METHOD0(Runway1Identifier, std::string(void));
                MOCK_CONST_METHOD0(Runway2Identifier, std::string(void));
        };
    }  // namespace Euroscope
}  // namespace UKControllerPluginTest
