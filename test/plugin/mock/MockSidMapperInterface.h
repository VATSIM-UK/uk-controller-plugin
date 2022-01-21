#pragma once
#include "sid/SidMapperInterface.h"

using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Sid::SidMapperInterface;
using UKControllerPlugin::Sid::StandardInstrumentDeparture;

namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroScopeCFlightPlanInterface;
    } // namespace Euroscope
    namespace Sid {
        class StandardInstrumentDeparture;
    } // namespace Sid
} // namespace UKControllerPlugin

namespace UKControllerPluginTest::Sid {
    class MockSidMapperInterface : public SidMapperInterface
    {
        public:
        MOCK_METHOD(
            std::shared_ptr<StandardInstrumentDeparture>,
            MapFlightplanToSid,
            (const EuroScopeCFlightPlanInterface&),
            (const, override));
    };
} // namespace UKControllerPluginTest::Sid
