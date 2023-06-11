#include "controller/ControllerPosition.h"
#include "handoff/HandoffFrequencyUpdatedMessage.h"
#include "handoff/ResolvedHandoff.h"
#include "handoff/SendHandoffFrequencyUpdatedIntegrationMessage.h"

namespace UKControllerPluginTest::Handoff {
    class SendHandoffFrequencyUpdatedIntegrationMessageTest : public testing::Test
    {
        public:
        SendHandoffFrequencyUpdatedIntegrationMessageTest() : sender(mockOutboundEvent)
        {
        }

        testing::NiceMock<Integration::MockOutboundIntegrationEventHandler> mockOutboundEvent;
        UKControllerPlugin::Handoff::SendHandoffFrequencyUpdatedIntegrationMessage sender;
    };

    TEST_F(SendHandoffFrequencyUpdatedIntegrationMessageTest, ItSendsMessage)
    {
        auto controllerPosition = std::make_shared<UKControllerPlugin::Controller::ControllerPosition>(
            2, "EGFF_APP", 125.850, std::vector<std::string>{"EGGD", "EGFF"}, true, true);
        auto resolvedHandoff = std::make_shared<UKControllerPlugin::Handoff::ResolvedHandoff>(
            "BAW123", controllerPosition, nullptr, nullptr);

        auto expectedMessage =
            std::make_shared<UKControllerPlugin::Handoff::HandoffFrequencyUpdatedMessage>("BAW123", "125.850");
        EXPECT_CALL(this->mockOutboundEvent, SendEvent(MatchMessageInterface(expectedMessage))).Times(1);

        sender.OnEvent({resolvedHandoff});
    }
} // namespace UKControllerPluginTest::Handoff
