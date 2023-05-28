#include "eventhandler/ProcessEventBusEventsOnTimer.h"
#include "eventhandler/DrainableEuroscopeThreadEventSink.h"

namespace UKControllerPluginTest::EventHandler {

    class MockDrainableSink : public UKControllerPluginUtils::EventHandler::DrainableEuroscopeThreadEventSink
    {
        public:
        MOCK_METHOD(void, Drain, (), (override));
    };

    class ProcessEventBusEventsOnTimerTest : public testing::Test
    {
        public:
        ProcessEventBusEventsOnTimerTest() : mockSink(std::make_shared<MockDrainableSink>()), process(mockSink)
        {
        }

        std::shared_ptr<MockDrainableSink> mockSink;
        UKControllerPlugin::EventHandler::ProcessEventBusEventsOnTimer process;
    };

    TEST_F(ProcessEventBusEventsOnTimerTest, DrainsSinkOnTimer)
    {
        EXPECT_CALL(*mockSink, Drain()).Times(1);
        process.TimedEventTrigger();
    }
} // namespace UKControllerPluginTest::EventHandler
