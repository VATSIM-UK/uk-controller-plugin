#include "controller/ControllerPosition.h"
#include "prenote/PrenoteMessage.h"
#include "prenote/SendPrenoteAcknowledgedChatAreaMessage.h"

using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Prenote::PrenoteMessage;
using UKControllerPlugin::Prenote::SendPrenoteAcknowledgedChatAreaMessage;

namespace UKControllerPluginTest::Prenote {
    class SendPrenoteAcknowledgedChatAreaMessageTest : public testing::Test
    {
        public:
        SendPrenoteAcknowledgedChatAreaMessageTest()
            : mockPrenoteRelevance(std::make_shared<testing::NiceMock<MockPrenoteUserRelevanceChecker>>()),
              sendMessage(mockPrenoteRelevance, plugin)
        {
            sendingPosition = std::make_shared<ControllerPosition>(
                1, "EGKK_TWR", 124.225, std::vector<std::string>{"EGKK"}, true, false);
            receivingPosition = std::make_shared<ControllerPosition>(
                2, "EGKK_F_APP", 124.225, std::vector<std::string>{"EGKK"}, true, false);
        }

        std::shared_ptr<ControllerPosition> sendingPosition;
        std::shared_ptr<ControllerPosition> receivingPosition;
        std::shared_ptr<testing::NiceMock<MockPrenoteUserRelevanceChecker>> mockPrenoteRelevance;
        testing::NiceMock<Euroscope::MockEuroscopePluginLoopbackInterface> plugin;
        SendPrenoteAcknowledgedChatAreaMessage sendMessage;
    };

    TEST_F(SendPrenoteAcknowledgedChatAreaMessageTest, ItSendsChatAreaMessageOnPrenoteMessageAcknowledged)
    {
        const PrenoteMessage message(
            1,
            "BAW123",
            "EGKK",
            "TEST1A",
            "EGLL",
            sendingPosition,
            receivingPosition,
            std::chrono::system_clock::now());
        EXPECT_CALL(
            plugin,
            ChatAreaMessage(
                "UKCP_COORDINATION",
                "UKCP",
                "Prenote message for BAW123 has been acknowledged by EGKK_F_APP.",
                true,
                true,
                true,
                true,
                true))
            .Times(1);
        EXPECT_CALL(*mockPrenoteRelevance, IsRelevant(testing::Ref(message))).Times(1).WillOnce(testing::Return(true));
        sendMessage.MessageAcknowledged(message);
    }

    TEST_F(SendPrenoteAcknowledgedChatAreaMessageTest, ItDoesntSendMessageIfNotRelevantToController)
    {
        const PrenoteMessage message(
            1,
            "BAW123",
            "EGKK",
            "TEST1A",
            "EGLL",
            sendingPosition,
            receivingPosition,
            std::chrono::system_clock::now());
        EXPECT_CALL(plugin, ChatAreaMessage).Times(0);
        EXPECT_CALL(*mockPrenoteRelevance, IsRelevant(testing::Ref(message))).Times(1).WillOnce(testing::Return(false));
        sendMessage.MessageAcknowledged(message);
    }
} // namespace UKControllerPluginTest::Prenote
