#include "aircraft/CallsignSelectionListInterface.h"

using UKControllerPlugin::Aircraft::CallsignSelectionListInterface;

namespace UKControllerPluginTest::Aircraft {
    class MockCallsignSelectionList : public CallsignSelectionListInterface
    {
        public:
        MOCK_METHOD(void, TriggerList, (const POINT&), (override));
    };
} // namespace UKControllerPluginTest::Aircraft
