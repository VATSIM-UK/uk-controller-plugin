#include "aircraft/CallsignSelectionProviderInterface.h"

using UKControllerPlugin::Aircraft::CallsignSelectionProviderInterface;

namespace UKControllerPluginTest::Aircraft {
    class MockCallsignSelectionProvider : public CallsignSelectionProviderInterface
    {
        public:
        MOCK_METHOD(std::set<std::string>, GetCallsigns, (), (override));
        MOCK_METHOD(void, CallsignSelected, (const std::string&), (override));
    };
} // namespace UKControllerPluginTest::Aircraft
