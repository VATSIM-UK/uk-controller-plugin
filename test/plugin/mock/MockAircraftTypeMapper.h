#pragma once
#include "aircraft/AircraftTypeMapperInterface.h"

using UKControllerPlugin::Aircraft::AircraftType;
using UKControllerPlugin::Aircraft::AircraftTypeMapperInterface;
using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;

namespace UKControllerPluginTest::Aircraft {
    class MockAircraftTypeMapper : public AircraftTypeMapperInterface
    {
        public:
        MOCK_METHOD(
            std::shared_ptr<AircraftType>, MapForFlightplan, (const EuroScopeCFlightPlanInterface&), (const, override));
    };
} // namespace UKControllerPluginTest::Aircraft
