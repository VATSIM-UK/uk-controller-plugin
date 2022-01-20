#include "metar/MetarEventHandlerInterface.h"

using UKControllerPlugin::Metar::MetarEventHandlerInterface;
using UKControllerPlugin::Metar::ParsedMetar;

namespace UKControllerPluginTest::Metar {
    class MockMetarEventHandler : public MetarEventHandlerInterface
    {
        public:
        MOCK_METHOD(void, MetarUpdated, (const ParsedMetar&), (override));
    };
} // namespace UKControllerPluginTest::Metar
