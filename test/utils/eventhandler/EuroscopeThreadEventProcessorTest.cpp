#include "eventhandler/EuroscopeThreadEventProcessor.h"

namespace UKControllerPluginUtilsTest::EventHandler {
    class EuroscopeThreadEventProcessorTest : public testing::Test
    {
        public:
        UKControllerPluginUtils::EventHandler::EuroscopeThreadEventProcessor processor;
    };

    TEST_F(EuroscopeThreadEventProcessorTest, ProcessEvent)
    {
        int runCount = 0;
        auto handler1 = [&runCount]() { runCount++; };
        auto handler2 = [&runCount]() { runCount++; };
        auto handler3 = [&runCount]() { runCount++; };
        this->processor.OnEvent(handler1);
        this->processor.OnEvent(handler2);
        this->processor.OnEvent(handler3);

        this->processor.Drain();

        ASSERT_EQ(runCount, 3);
    }
} // namespace UKControllerPluginUtilsTest::EventHandler
